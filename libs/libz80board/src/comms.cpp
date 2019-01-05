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

#include "libusb-wrapper/libusbcontext.h"
#include "libusb-wrapper/usbdevice.h"

#include <libz80board.h>

#include "error_internal.h"
#include "comms_internal.h"

namespace expected
{
static const uint16_t vendorID = 0x16c0;
static const uint16_t productID = 0x05df;
static const char* manufacturerName = "lightemittingresistor.com";
static const char* productName = "z80 board";
}

void maybeError(z80_board_error_t* error_out, std::string text)
{
    if(error_out)
    {
        *error_out = z80board::createExternalError(std::move(text));
    }
}

z80_board_t z80_board_open(z80_board_list_t list, int id, z80_board_error_t* error_out)
{
    maybeError(error_out, "Not Implemented");
	return nullptr;
}

void z80_board_close(z80_board_t board)
{

}

LIBZ80BOARD_EXPORT z80_board_list_t z80_board_list()
{
    z80board::LibUSBContext ctx;
    auto ret = std::make_unique<z80board::BoardList>();
    auto devices = ctx.getDevices();

    for(auto it = devices.begin(); it != devices.end(); ++it)
    {
        if(it->vendorId == expected::vendorID && it->productId == expected::productID 
            && it->manufacturer == expected::manufacturerName
            && it->product == expected::productName)
        {
            ret->push_back(*it);
        }
    }

    return ret.release();;
}

LIBZ80BOARD_EXPORT void z80_board_destroy_list(z80_board_list_t list)
{
    z80board::BoardList* real = static_cast<z80board::BoardList*>(list);
    delete real;
}

size_t z80_board_list_count(z80_board_list_t list)
{
    z80board::BoardList* real = static_cast<z80board::BoardList*>(list);
    return real->size();
}
