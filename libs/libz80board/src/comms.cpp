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

#include <libz80board.h>
#include "SerialPort.h"
#include "error_internal.h"

z80_board_t z80_board_open(const char* serialPort, z80_board_error_t* error_out)
{
    if(error_out)
    {
        *error_out = z80board::createExternalError("Not Implemented");
    }
	return nullptr;
}

void z80_board_close(z80_board_t board)
{

}

static inline std::vector<std::pair<std::string, std::string>>* getList(z80_board_serial_port_list_t lst)
{
    return reinterpret_cast<std::vector<std::pair<std::string, std::string>>*>(lst);
}

z80_board_serial_port_list_t z80_board_list_serial_ports()
{
    std::vector<std::pair<std::string, std::string>> lst = z80board::listSerialPorts();

    return new std::vector<std::pair<std::string, std::string>>(std::move(lst));
}

void z80_board_destroy_port_list(z80_board_serial_port_list_t lst)
{
    delete getList(lst);
}

size_t z80_board_serial_port_list_count(z80_board_serial_port_list_t lst)
{
    return getList(lst)->size();
}

const char* z80_board_serial_port_list_get_path(z80_board_serial_port_list_t lst, int n)
{
    return getList(lst)->at(n).first.c_str();
}

const char* z80_board_serial_port_list_get_desc(z80_board_serial_port_list_t lst, int n)
{
    return getList(lst)->at(n).second.c_str();
}
