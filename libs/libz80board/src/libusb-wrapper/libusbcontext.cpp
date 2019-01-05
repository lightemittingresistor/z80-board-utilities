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

#include "libusbcontext.h"
#include "usbdevice.h"

#include <stdexcept>

namespace z80board
{

LibUSBContext::LibUSBContext()
{
    libusb_context* lctx;
    if(libusb_init(&lctx) != 0)
    {
        throw std::runtime_error("Unable to initialise libUSB");
    }
    ctx = std::shared_ptr<libusb_context>(lctx, libusb_exit);
}

LibUSBContext::~LibUSBContext()
{
    
}

std::vector<USBDevice> LibUSBContext::getDevices()
{
    std::vector<USBDevice> res;
    libusb_device** lst;
    ssize_t count = libusb_get_device_list(ctx.get(), &lst);
    res.reserve(count);

    for(ssize_t i = 0; i < count; ++i)
    {
        res.push_back(USBDevice(ctx, lst[i]));
    }

    libusb_free_device_list(lst, 1);
    return res;
}

}
