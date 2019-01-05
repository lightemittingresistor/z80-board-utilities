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
    int boardNum = 1;

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
    }

    if(!help && !count)
    {
        //help = true;
    }

    if(help)
    {
        std::cout   << "Usage: " << argv[0] << " [--help|--count] [--board X]" << std::endl
                    << " --help\t\tShows this help message" << std::endl
                    << " --count\t\tCount the number of connected boards" << std::endl
                    << " --board X\t\tuse board X (1..n)" << std::endl;

        return 1;
    }

    z80_board_list_t lst = z80_board_list();

    if(count)
    {
        std::cout << "Found " << z80_board_list_count(lst) << " boards" << std::endl;
        z80_board_destroy_list(lst);
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

        z80_board_close(board);
    }

    z80_board_destroy_list(lst);
    
    return 0;
}