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

#include <libz80board.h>

#include <iostream>
#include <sstream>

int main(int argc, char** argv)
{
    // parse arguments
    bool help = false;
    bool count = false;
    bool status = false;
    bool reset = false;
    bool dump = false;
    bool write = false;
    bool peek = false;
    int boardNum = 1;
    int peekAddress = 0;
    char filename[1024] = "dump.bin";

    for(int i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "--help")
        {
            help = true;
        }
        else if(std::string(argv[i]) == "--count")
        {
            count = true;
        }
        else if(std::string(argv[i]) == "--board")
        {
            ++i;
            if(i == argc)
            {
                help = true;
                break;
            }
            std::stringstream ss(argv[i]);
            ss >> boardNum;
        }
        else if(std::string(argv[i]) == "--status")
        {
            status = true;
        }
        else if(std::string(argv[i]) == "--reset")
        {
            reset = true;
        }
        else if(std::string(argv[i]) == "--dump")
        {
            dump = true;
        }
        else if(std::string(argv[i]) == "--write")
        {
            write = true;
        }
        else if(std::string(argv[i]) == "--file")
        {
            if((i+1) == argc)
            {
                std::cerr << "Please specify a filename!" << std::endl;
                help = true;
            }
            else
            {
                strncpy(filename, argv[++i], 1023);
            }
        }
        else if(std::string(argv[i]) == "--peek")
        {
            if((i+1) == argc)
            {
                std::cerr << "Please specify an address!" << std::endl;
                help = true;
            }
            else
            {
                peek = true;
                std::string scratch(argv[++i]);
                std::stringstream ss(scratch);

                ss >> std::hex >> peekAddress;
            }
        }
    }

    if(!help && !count && !status && !reset && !dump && !write && !peek)
    {
        help = true;
    }

    if(help)
    {
        std::cout   << "Usage: " << argv[0] << " [--help|--count] [--board X]" << std::endl
                    << " --help\t\tShows this help message" << std::endl
                    << " --count\t\tCount the number of connected boards" << std::endl
                    << " --board X\t\tuse board X (1..n)" << std::endl
                    << " --status\t\tget the board status" << std::endl
                    << " --reset\t\treset the z80" << std::endl
                    << " --dump\t\tdump the eeprom to a file" << std::endl
                    << " --write\t\tload the eeprom from a file" << std::endl
                    << " --file <filename>\t\tsets the filename for dump and load (defaults to dump.bin)" << std::endl
                    << " --peek <address>\t\tread a single hex address" << std::endl;

        return 1;
    }

    z80_board_list_t lst = z80_board_list();

    if(count)
    {
        std::cout << "Found " << z80_board_list_count(lst) << " boards" << std::endl;
    }
    // all commands below here need a board
    else if(z80_board_list_count(lst) == 0)
    {
        std::cerr << "No boards present" << std::endl;
    }
    else if(z80_board_list_count(lst) < boardNum)
    {
        std::cerr << "Board " << boardNum << " does not exist" << std::endl;
    }
    else if(boardNum < 1)
    {
        std::cerr << "Board " << boardNum << " is invalid" << std::endl;
    }
    else
    {
        // open the board
        z80_board_error_t err;
        z80_board_t board = z80_board_open(lst, boardNum, &err);
        if(!board)
        {
            std::cerr << "Error opening board " << boardNum << ": " << z80_board_error_gettext(err) << std::endl;
            z80_board_error_destroy(err);
            z80_board_destroy_list(lst);
            return 1;
        }

        if(status)
        {
            z80_board_status s = z80_board_get_status(board);
            std::cout << "Z80 Board Status: " << std::endl;
            std::cout << "\tHardware Version: " << (int)s.hwVersionMajor << "." << (int)s.hwVersionMinor << std::endl;
            std::cout << "\tSoftware Version: " << (int)s.swVersionMajor << "." << (int)s.swVersionMinor << std::endl;
            std::cout << "\tCurrently " << (s.isBusMaster ? "is" : "is not") << " bus master" << std::endl;
        }

        if(reset)
        {
            z80_board_reset(board);
        }

        if(dump)
        {
            #define TRANSFER_SIZE 8192
            unsigned char buffer[TRANSFER_SIZE];
            z80_board_set_pointer(board, 0);
            z80_board_read(board, buffer, TRANSFER_SIZE);
            FILE* dumpf = fopen(filename, "wb");
            fwrite(buffer, TRANSFER_SIZE, 1, dumpf);
            fclose(dumpf);
        }

        if(write)
        {
            unsigned char buffer[8192];
            FILE* loadf = fopen(filename, "rb");
            int count = fread(buffer, 1, 8192, loadf);
            z80_board_set_pointer(board, 0x0);
            std::cout << "Writing " << count << " bytes to device" << std::endl;
            z80_board_write(board, buffer, count);
            fclose(loadf);
        }

        if(peek)
        {
            z80_board_set_pointer(board, peekAddress);
            uint8_t byte;
            z80_board_read(board, &byte, 1);
            std::cout << "Data at 0x" << std::hex << peekAddress << " is 0x" << int(byte) << std::endl;
        }

        z80_board_close(board);
    }

    z80_board_destroy_list(lst);
    
    return 0;
}