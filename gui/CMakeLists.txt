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

# Some stuff required for Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

set(Z80_BOARD_UTILS_UIS 
	ui/connection.ui)

set(Z80_BOARD_UTILS_SOURCES
	src/z80-board-utility.cpp
	src/models/SerialPortModel.cpp src/models/SerialPortModel.h
	src/widgets/Connection.cpp src/widgets/Connection.h)

set(Z80_BOARD_UTILS_TS_SOURCES )

# make UIs
qt5_wrap_ui(Z80_BOARD_UTILS_UI_SOURCES ${Z80_BOARD_UTILS_UIS})
source_group("Header Files\\ui"${Z80_BOARD_UTILS_UI_SOURCES})
source_group("UI Files" ${Z80_BOARD_UTILS_UIS})

add_executable(z80-board-utility WIN32 
				${Z80_BOARD_UTILS_SOURCES}
				${Z80_BOARD_UTILS_UI_SOURCES}
				)
target_link_libraries(z80-board-utility Qt5::Widgets z80board)

# make translations NOTE: This will not automagically update the TS files
qt5_add_translation(Z80_BOARD_UTILS_QM_FILES ${Z80_BOARD_UTILS_TS_SOURCES})
add_custom_target(z80-board-utility_translations ALL DEPENDS ${Z80_BOARD_UTILS_QM_FILES})

# Collect the Qt stuff if needed
if(WIN32)
# see https://www.codeday.top/2017/10/11/47866.html for hints about this bit 

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

add_custom_command(TARGET z80-board-utility
                   POST_BUILD
                   COMMAND ${WINDEPLOYQT_EXECUTABLE} --no-compiler-runtime --no-opengl-sw $<TARGET_FILE:z80-board-utility>
				   )
endif()