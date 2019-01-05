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

#include "widgets/BoardChooser.h"
#include "widgets/MainWindow.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <QScopedPointer>

#include <libz80board.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    z80_board_list_t lst = z80_board_list();
    size_t boardCount = z80_board_list_count(lst);
    z80_board_destroy_list(lst);

    if(boardCount == 0)
    {
        QMessageBox::critical(nullptr,
            "Z80 Board Utility",
            "No boards found!",
            QMessageBox::Ok);
        return 1;
    }

    QScopedPointer<BoardChooser> chooser;
    QScopedPointer<MainWindow> mainw;

    mainw.reset(new MainWindow(nullptr));

    if(boardCount > 1)
    {
        chooser.reset(new BoardChooser(nullptr, boardCount));
        chooser->show();
    }
    else
    {
        mainw->showMain(1);
    }

    return app.exec();
}