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

#ifndef Z80_BOARD_USBDEVICE_H
#define Z80_BOARD_USBDEVICE_H

#include <libusb-1.0/libusb.h>

#include <string>
#include <memory>

namespace z80board
{

struct USBDevice
{
    uint8_t busNumber;
    uint8_t portNumber;
    uint8_t deviceAddress;

    uint16_t vendorId;
    uint16_t productId;

    std::string manufacturer;
    std::string product;

    USBDevice(std::shared_ptr<libusb_context> ctx, libusb_device* dev);

    USBDevice(const USBDevice& other);
    USBDevice(USBDevice&& other);
    USBDevice& operator=(const USBDevice& other);
    USBDevice& operator=(USBDevice&& other);

    void open();
    void close();

private:
    std::shared_ptr<libusb_context> ctx;
    std::unique_ptr<libusb_device, void(*)(libusb_device*)> dev;
    std::unique_ptr<libusb_device_handle, void(*)(libusb_device_handle*)> hndl;
};

}

#endif
