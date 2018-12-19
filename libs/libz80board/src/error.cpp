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

#include "error_internal.h"

namespace z80board
{


Error* getErrorFromExternal(z80_board_error_t e)
{
    return static_cast<Error*>(e);
}

z80_board_error_t createExternalError(Error e)
{
    return new Error(std::move(e));
}

z80_board_error_t createExternalError(std::string errorText)
{
    return new Error(std::move(errorText));
}

}


void z80_board_error_destroy(z80_board_error_t error)
{
    delete z80board::getErrorFromExternal(error);
}

const char* z80_board_error_gettext(z80_board_error_t error)
{
    return z80board::getErrorFromExternal(error)->errorText.c_str();
}