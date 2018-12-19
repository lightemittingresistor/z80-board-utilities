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

#include "../SerialPort.h"
#include "stringutils.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <SetupAPI.h>
#include <Ntddser.h>

namespace z80board
{

class Win32SerialPort : public SerialPort
{
public:
    Win32SerialPort(std::string portname);
    ~Win32SerialPort();

    void close() override;
    bool isOpen() const override;
    void reopen() override;

private:
    HANDLE portHandle;
    std::string portName;
};

Win32SerialPort::Win32SerialPort(std::string portname) :
    portHandle(INVALID_HANDLE_VALUE),
    portName(portname)
{
    reopen();
}

Win32SerialPort::~Win32SerialPort()
{
    close();
}

bool Win32SerialPort::isOpen() const
{
    return portHandle == INVALID_HANDLE_VALUE;
}

void Win32SerialPort::close()
{
    if(portHandle != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(portHandle);
        portHandle = INVALID_HANDLE_VALUE;
    }
}

void Win32SerialPort::reopen()
{
    close();

    portHandle = ::CreateFileW(convertString(portName).c_str(),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                0,
                                OPEN_EXISTING,
                                0,
                                0);
    if(portHandle == INVALID_HANDLE_VALUE)
    {
        return;
    }
        
    // now set the baud rate
    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);
    serialParams.BaudRate = CBR_57600;
    serialParams.ByteSize = 8;
    serialParams.StopBits = ONESTOPBIT;
    serialParams.Parity = NOPARITY;
    SetCommState(portHandle, &serialParams);

}

std::vector<std::pair<std::string,std::string>> listSerialPorts()
{
	std::vector<std::pair<std::string,std::string>> results;
	
	HDEVINFO hDevInfo = ::SetupDiGetClassDevsW(&GUID_CLASS_COMPORT, NULL, NULL, DIGCF_DEVICEINTERFACE);

	if(hDevInfo == INVALID_HANDLE_VALUE)
	{
		// TODO: Handle error
		return results;
	}

	// enumerate the device interfaces
	DWORD memberIndex = 0;

	while(1) // we will break out when needed
	{
        SP_DEVICE_INTERFACE_DATA devInterfaceData;
        devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
        if(!::SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &GUID_CLASS_COMPORT, memberIndex++, &devInterfaceData))
        {
            DWORD err = ::GetLastError();
            if(ERROR_NO_MORE_ITEMS)
            {
                break;
            }
            break;
        }
        
        DWORD requiredSize = 0;
        SP_DEVINFO_DATA devInfoData;
        devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        ::SetupDiGetDeviceInterfaceDetailW(hDevInfo, &devInterfaceData, NULL, 0, &requiredSize, &devInfoData);

        PSP_DEVICE_INTERFACE_DETAIL_DATA pDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
        pDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        if(::SetupDiGetDeviceInterfaceDetailW(hDevInfo, &devInterfaceData, pDetailData, requiredSize, &requiredSize, NULL))
        {
            // Ah we have a port
            
            // Get the friendly name
            DWORD requiredPropSize;
            ::SetupDiGetDeviceRegistryPropertyW(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, NULL, NULL, 0, &requiredPropSize);
            BYTE* buffer = (BYTE*)malloc(requiredPropSize);
            std::string friendlyName;

            if(::SetupDiGetDeviceRegistryPropertyW(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, NULL, buffer, requiredPropSize, nullptr))
            {
                friendlyName = convertString((LPCWSTR)buffer);
            }

            //Add whatever we got to the list of things we found
            results.emplace_back(std::make_pair(convertString(pDetailData->DevicePath),friendlyName));
            free(buffer);
        }
        free(pDetailData);
	}

	::SetupDiDestroyDeviceInfoList(hDevInfo);

	return results;
}

std::unique_ptr<SerialPort> openSerialPort(const std::string& portName)
{
    return std::make_unique<Win32SerialPort>(portName);
}

}