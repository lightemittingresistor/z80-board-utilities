/*
 * z80-board-utilities
 * 
 * Copyright (c) 2018 Michael Cullen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <libz80board/jtag.h>
#include "jam_functions.h"
#include "error_internal.h"
#include "board.h"

#include <cstdio>
#include <memory>
#include <iostream>

extern "C"
{
#include "jam_player/jamexprt.h"
}

void z80_board_jtag(z80_board_t board, const char* jamfile, z80_board_error_t* error_out)
{
    FILE* jamFile = fopen(jamfile, "rb");
    if(!jamFile)
    {
        maybeError(error_out, "Could not open jam file for reading");
        return;
    }

    fseek(jamFile, 0, SEEK_END);
    size_t jamFileSize = ftell(jamFile);
    fseek(jamFile, 0, SEEK_SET);

    auto ctx = std::make_unique<JamContext>();

    ctx->jam_buffer = static_cast<char*>(malloc(jamFileSize));
    ctx->jam_length = jamFileSize;
    ctx->board = static_cast<z80board::Board*>(board);

    fread(ctx->jam_buffer, ctx->jam_length, 1, jamFile);

    jam_activate_context(ctx.get());

    unsigned short expectedCrc;
    unsigned short actualCrc;
    JAM_RETURN_TYPE crcRet = jam_check_crc(ctx->jam_buffer , ctx->jam_length, &expectedCrc, &actualCrc);

    switch(crcRet)
    {
        case JAMC_CRC_ERROR:
        {
            maybeError(error_out, "CRC Mismatch");
            //TEMP
            std::cerr << "CRC Error: Expected " << expectedCrc << " got " << actualCrc << std::endl;
        } break;
        case JAMC_UNEXPECTED_END:
        {
            maybeError(error_out, "CRC Missing (damaged file?)");
            //TEMP
            std::cerr << "CRC Missing" << std::endl;
        } break;
        case JAMC_SUCCESS:
            break;
        default:
        {
            maybeError(error_out, "Unknown CRC Error");
            //TEMP
            std::cerr << "Unknown CRC Error: " << crcRet << std::endl;
        }
    };

    ctx->board->setJtagMode(true);

    if(crcRet == JAMC_SUCCESS)
    {
        int error_line;
        int exit_code;
        int format_version;
        JAM_RETURN_TYPE execCode = jam_execute(ctx->jam_buffer, ctx->jam_length, nullptr, 0, "program", nullptr, 1, &error_line, &exit_code, &format_version);

        if(exit_code != 0)
        {
            std::cerr << "Exit code was " << exit_code << std::endl;
            std::cerr << "Return code was " << execCode << std::endl;
            std::cerr << "Line was " << error_line << std::endl;
        }
    }

    ctx->board->setJtagMode(false);

    jam_deactivate_context();

    free(ctx->jam_buffer);

    fclose(jamFile);
}
