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

#ifndef Z80_BOARD_BOARD_H
#define Z80_BOARD_BOARD_H

#include "libusb-wrapper/usbdevice.h"
#include <libz80board/comms.h>

namespace z80board
{

class Board
{
public:
    Board(USBDevice dev);
    ~Board();

    void setAddress(uint16_t addr);
    uint16_t getAddress();
    void reset();

    z80_board_status getStatus();

    void read(unsigned char* data, uint16_t length);
    void write(unsigned char* data, uint16_t length);

    void setJtagMode(bool enabled);

private:
    USBDevice dev;
};

}

#endif
