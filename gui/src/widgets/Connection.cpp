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

#include "Connection.h"
#include "../models/SerialPortModel.h"

#include <QtGlobal>

Connection::Connection(QWidget* parent) :
	QWidget(parent)
{
    // basic UI setup
    ui.setupUi(this);
    connect(ui.btnGo, &QPushButton::pressed,
        this, &Connection::onGoButtonPressed);
    connect(ui.cbPort, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,&Connection::onSerialDropdownChanged);

    // Setup serial port model
    SerialPortModel* portModel = new SerialPortModel(ui.cbPort);
    connect(ui.btnSerialReset, &QPushButton::pressed, portModel, &SerialPortModel::refresh);
    ui.cbPort->setModel(portModel);

    // setup properties
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
}

Connection::~Connection()
{

}

void Connection::onGoButtonPressed()
{
    QVariant portVariant = ui.cbPort->currentData(Qt::UserRole);
    QString port = portVariant.toString();
    if(!port.isEmpty())
    {
        emit serialConnectionRequested(port);
    }
}

void Connection::onSerialDropdownChanged(int index)
{
    ui.btnGo->setEnabled(index != -1);
}
