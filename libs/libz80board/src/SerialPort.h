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

#ifndef Z80_BOARD_SERIALPORT_H
#define Z80_BOARD_SERIALPORT_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace z80board
{

class SerialPort
{
public:
	virtual ~SerialPort();

	/**
	 * Close the port, preventing any further use
	 */
	virtual void close()=0;

    /**
    * Check if the port is open
    */
    virtual bool isOpen() const = 0;

    /**
    * Close and reopen the port
    */
    virtual void reopen() = 0;

};

/**
 * Open a port by name
 */

std::unique_ptr<SerialPort> openSerialPort(const std::string& portName);

/**
 * Get a list of ports, as a pair of (path,name) entries
 */
std::vector<std::pair<std::string,std::string>> listSerialPorts();

}

#endif
