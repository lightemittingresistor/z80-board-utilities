
#include "usbdevice.h"

#include <iostream>

namespace z80board
{

void closeUSBDevice(libusb_device_handle* hdl)
{
    if(hdl)
    {
        libusb_close(hdl);
    }
}

USBDevice::USBDevice(std::shared_ptr<libusb_context> ctx, libusb_device* dev) :
    ctx(std::move(ctx)),
    dev(dev, libusb_unref_device),
    hndl(nullptr, closeUSBDevice)
{
    libusb_ref_device(dev);

    busNumber = libusb_get_bus_number(dev);
    portNumber = libusb_get_port_number(dev);
    deviceAddress = libusb_get_device_address(dev);

    struct libusb_device_descriptor desc;
    libusb_get_device_descriptor(dev, &desc);
    vendorId = desc.idVendor;
    productId = desc.idProduct;

    open();
    if(hndl)
    {
        unsigned char scratch[256] = { 0 };
        libusb_get_string_descriptor_ascii(hndl.get(), desc.iManufacturer, scratch, 256);
        manufacturer = (char*)scratch;
        libusb_get_string_descriptor_ascii(hndl.get(), desc.iProduct, scratch, 256);
        product = (char*)scratch;
        close();
    }
}

USBDevice::USBDevice(const USBDevice& other) :
    busNumber(other.busNumber),
    portNumber(other.portNumber),
    deviceAddress(other.deviceAddress),
    vendorId(other.vendorId),
    productId(other.productId),
    manufacturer(other.manufacturer),
    product(other.product),
    ctx(other.ctx),
    dev(other.dev.get(), libusb_unref_device),
    hndl(nullptr, closeUSBDevice)
{
    libusb_ref_device(dev.get());
}

USBDevice::USBDevice(USBDevice&& other) :
    busNumber(other.busNumber),
    portNumber(other.portNumber),
    deviceAddress(other.deviceAddress),
    vendorId(other.vendorId),
    productId(other.productId),
    manufacturer(std::move(other.manufacturer)),
    product(std::move(other.product)),
    ctx(std::move(other.ctx)),
    dev(std::move(other.dev)),
    hndl(std::move(other.hndl))
{
    
}

USBDevice& USBDevice::operator=(const USBDevice& other)
{
    hndl.reset();
    dev = std::unique_ptr<libusb_device, void(*)(libusb_device*)>(other.dev.get(), libusb_unref_device);
    libusb_ref_device(dev.get());
    ctx = other.ctx;

    busNumber = other.busNumber;
    portNumber = other.portNumber;
    deviceAddress = other.deviceAddress;
    vendorId = other.vendorId;
    productId = other.productId;
    manufacturer = other.manufacturer;
    product = other.product;

    return *this;
}

USBDevice& USBDevice::operator=(USBDevice&& other)
{
    hndl = std::move(other.hndl);
    dev = std::move(other.dev);
    ctx = std::move(other.ctx);

    busNumber = other.busNumber;
    portNumber = other.portNumber;
    deviceAddress = other.deviceAddress;
    vendorId = other.vendorId;
    productId = other.productId;
    manufacturer = std::move(other.manufacturer);
    product = std::move(other.product);

    return *this;
}

void USBDevice::open()
{
    if(hndl)
        return;
    libusb_device_handle* lhdl;
    if(libusb_open(dev.get(), &lhdl) == 0)
    {
        hndl = std::unique_ptr<libusb_device_handle, void(*)(libusb_device_handle*)>(lhdl, closeUSBDevice);
    }
}

void USBDevice::close()
{
    hndl.reset();
}

void USBDevice::sendVendor(uint8_t request,
                            uint16_t value,
                            uint16_t index,
                            unsigned char *data,
                            uint16_t length)
{
    if(!hndl)
        open();

    int res = libusb_control_transfer(hndl.get(), 
            LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR, // type
            request, // request
            value, // value
            index, // index
            data, //data
            length, // length
            1000 //timeout
            );

    if(res < 0)
    {
        std::cerr << "sendVendor: USB Error: " << libusb_error_name(res) << std::endl;
    }
}

void USBDevice::receiveVendor(uint8_t request,
                            uint16_t value,
                            uint16_t index,
                            unsigned char *data,
                            uint16_t length)
{
    if(!hndl)
        open();

    int res = libusb_control_transfer(hndl.get(), 
            LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR, // type
            request, // request
            value, // value
            index, // index
            data, //data
            length, // length
            5000 //timeout
            );

    if(res < 0)
    {
        std::cerr << "receiveVendor: USB Error: " << libusb_error_name(res) << std::endl;
    }
}

}