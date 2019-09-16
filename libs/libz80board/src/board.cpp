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

#include "board.h"

namespace z80board
{

// USB Request Numbers
static const uint8_t USB_REQ_GET_STATUS = 1;
static const uint8_t USB_REQ_TAKE_CONTROL = 2;
static const uint8_t USB_REQ_POINTER = 3;
static const uint8_t USB_REQ_TRANSFER_DATA = 4;
static const uint8_t USB_REQ_RESET = 5;
static const uint8_t USB_REQ_JTAG = 6;

Board::Board(USBDevice dev) :
    dev(std::move(dev))
{
    this->dev.open();
}

Board::~Board()
{

}

void Board::setAddress(uint16_t addr)
{
    dev.sendVendor(USB_REQ_POINTER, 0, 0, (unsigned char*)&addr, 2);
}

uint16_t Board::getAddress()
{
    uint16_t address;
    dev.receiveVendor(USB_REQ_POINTER, 0, 0, (unsigned char*)&address, 2);

    return address;
}

z80_board_status Board::getStatus()
{
    unsigned char data[5];
    dev.receiveVendor(USB_REQ_GET_STATUS, 0, 0, data, 5);

    z80_board_status ret;
    ret.hwVersionMajor = data[0];
    ret.hwVersionMinor = data[1];
    ret.swVersionMajor = data[2];
    ret.swVersionMinor = data[3];
    ret.isBusMaster = data[4] == 1;

    return ret;
}

void Board::reset()
{
    dev.receiveVendor(USB_REQ_RESET, 0, 0, nullptr, 0);
}

void Board::read(unsigned char* data, uint16_t length)
{
    dev.sendVendor(USB_REQ_TAKE_CONTROL, 1, 0, nullptr, 0);
    uint16_t remaining = length;
    while(remaining > 0)
    {
        int currentLength = 2048;
        if(currentLength > remaining)
        {
            currentLength = remaining;
        }
        dev.receiveVendor(USB_REQ_TRANSFER_DATA, currentLength, 0, data, currentLength);
        data += currentLength;
        remaining -= currentLength;
    }
    dev.sendVendor(USB_REQ_TAKE_CONTROL, 0, 0, nullptr, 0);
}

void Board::write(unsigned char* data, uint16_t length)
{
    dev.sendVendor(USB_REQ_TAKE_CONTROL, 1, 0, nullptr, 0);
    static const int step = 1;
    
    for(uint16_t i = 0; i < length; i += step)
    {
        int remaining = length-i;
        if(remaining > step)
        {
            remaining = step;
        }
        dev.sendVendor(USB_REQ_TRANSFER_DATA, remaining, 0, data+i, remaining);
    }
    dev.sendVendor(USB_REQ_TAKE_CONTROL, 0, 0, nullptr, 0);
}

void Board::setJtagMode(bool enabled)
{
    dev.sendVendor(USB_REQ_JTAG, enabled ? 1 : 0, 0, nullptr, 0);
}

}
