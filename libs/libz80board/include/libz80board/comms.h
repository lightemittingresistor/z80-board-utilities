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

#ifndef Z80_BOARD_COMMS_H
#define Z80_BOARD_COMMS_H

#include "base.h"
#include "error.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// opaque handle to board list
typedef void* z80_board_list_t;
LIBZ80BOARD_EXPORT z80_board_list_t z80_board_list();
LIBZ80BOARD_EXPORT void z80_board_destroy_list(z80_board_list_t list);

LIBZ80BOARD_EXPORT size_t z80_board_list_count(z80_board_list_t list);

/* opaque handle to board object */
typedef void* z80_board_t;

// Open a z80 board index, optionally returning errors via error_out
LIBZ80BOARD_EXPORT z80_board_t z80_board_open(z80_board_list_t list, int id, z80_board_error_t* error_out);
LIBZ80BOARD_EXPORT void z80_board_close(z80_board_t board);

#ifdef __cplusplus
}
#endif

#endif
