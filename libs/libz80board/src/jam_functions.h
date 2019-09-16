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

#ifndef Z80_BOARD_JAM_FUNCS_H
#define Z80_BOARD_JAM_FUNCS_H

#include <cstddef>

class Board;

struct JamContext
{
    size_t jam_ptr;
    size_t jam_length;
    unsigned char* jam_buffer;
    Board* board;

    JamContext() : jam_ptr(0), jam_length(0), jam_buffer(nullptr), board(nullptr) {}
};

void jam_activate_context(JamContext* ctx);
inline void jam_deactivate_context() { jam_activate_context(nullptr); }

#endif
