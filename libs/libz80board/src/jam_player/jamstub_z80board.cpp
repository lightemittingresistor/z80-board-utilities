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

extern "C"
{
#include "jamexprt.h"
}

#include <unistd.h>
#include <iostream>

int jam_getc(void)
{
    return -1;
}

int jam_seek(long offset)
{
    return -1;
}

int jam_jtag_io(int tms, int tdi, int read_tdo)
{
    return 0;// return tdo
}

void jam_message(char *message_text)
{
    std::cout << message_text << std::endl;
}

void jam_export_integer(char *key, long value)
{
    std::cout << "Export: " << key << ": " << value << std::endl;
}

void jam_delay(long microseconds)
{
    usleep(microseconds);
}

void *jam_malloc(unsigned int size)
{
    return malloc(size);
}

void jam_free(void *ptr)
{
    free(ptr);
}

int jam_vector_io
(
    int signal_count,
    long *dir_vect,
    long *data_vect,
    long *capture_vect
)
{
    std::cout << "unimplemented vector io call" << std::endl;
    return 0;
}

int jam_vector_map
(
    int signal_count,
    char **signals
)
{
    std::cout << "unimplemented vector map call" << std::endl;
    return 0;
}

int jam_set_frequency(long hertz)
{
    std::cout << "would set frequency to " << hertz << std::endl;
    return 0;
}