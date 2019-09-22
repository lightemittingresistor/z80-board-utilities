/*
 * z80-board-utilities
 * 
 * Copyright (c) 2019 Michael Cullen
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

extern "C"
{
#include "jamexprt.h"
}

#include "../jam_functions.h"
#include "../board.h"

#include <unistd.h>
#include <iostream>

static JamContext* currentContext = nullptr;

void jam_activate_context(JamContext* ctx)
{
    currentContext = ctx;
}

int jam_getc(void)
{
    if(currentContext->jam_ptr < currentContext->jam_length)
    {
        return currentContext->jam_buffer[currentContext->jam_ptr++];
    }
    return -1;
}

int jam_seek(long offset)
{
    if(offset >= 0 && offset < currentContext->jam_length)
    {
        currentContext->jam_ptr = offset;
    }
    else
    {
        return -1;
    }

    return 0;
}

int jam_jtag_io(int tms, int tdi, int read_tdo)
{
    //std::cout << "jam_jtag_io(tms=" << tms << ", tdi=" << tdi << ", read_tdo=" << read_tdo << ")" << std::endl;

    int tdo = currentContext->board->jam_jtag_io(tms, tdi, read_tdo) ? 1 : 0;

    if(read_tdo)
    {
        //std::cout << "TDO is " << tdo << std::endl;
    }

    return tdo;// return tdo
}

void jam_message(char *message_text)
{
    std::cout << message_text << std::endl;
}

void jam_export_integer(char *key, long value)
{
    std::cout << "Export: " << key << ": " << value << std::endl;
}

void jam_delay(long microseconds)
{
    usleep(microseconds);
}

void *jam_malloc(unsigned int size)
{
    return malloc(size);
}

void jam_free(void *ptr)
{
    free(ptr);
}

int jam_vector_io
(
    int signal_count,
    int *dir_vect,
    int *data_vect,
    int *capture_vect
)
{
    std::cout << "unimplemented vector io call" << std::endl;
    return 0;
}

int jam_vector_map
(
    int signal_count,
    char **signals
)
{
    std::cout << "unimplemented vector map call" << std::endl;
    return 0;
}

int jam_set_frequency(long hertz)
{
    std::cout << "would set frequency to " << hertz << std::endl;
    return 0;
}