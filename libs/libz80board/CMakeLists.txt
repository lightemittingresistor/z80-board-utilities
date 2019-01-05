###############################################################################
# z80-board-utilities CMake file
#
# Copyright (c) 2018 Michael Cullen
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
###############################################################################

set(Z80BOARD_SOURCES
    include/libz80board.h
    include/libz80board/base.h
    include/libz80board/comms.h
    include/libz80board/error.h
	src/comms.cpp
    src/error.cpp src/error_internal.h
    )

set(Z80BOARD_LIBS )

if(WIN32)
	set(Z80BOARD_PLATFORM_SOURCES
			src/win32/stringutils.cpp src/win32/stringutils.h)
	list(APPEND Z80BOARD_LIBS Setupapi.lib)
endif()

add_definitions(-DBUILD_LIB_Z80BOARD)

add_library(z80board SHARED 
				${Z80BOARD_SOURCES} ${Z80BOARD_PLATFORM_SOURCES})
target_link_libraries(z80board ${Z80BOARD_LIBS})
target_include_directories(z80board PUBLIC include)

source_group("common" FILES ${Z80BOARD_SOURCES})
source_group("platform" FILES ${Z80BOARD_PLATFORM_SOURCES})

add_subdirectory(include)