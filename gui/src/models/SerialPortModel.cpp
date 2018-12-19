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

#include "SerialPortModel.h"

SerialPortModel::SerialPortModel(QObject *parent) :
    QAbstractListModel(parent),
    portList(z80_board_list_serial_ports(), z80_board_destroy_port_list)
{

}

SerialPortModel::~SerialPortModel()
{

}

int SerialPortModel::rowCount(const QModelIndex &parent) const
{
    // only a single dimension - if we have a parent, there's nothing in here!
    if(!parent.isValid())
        return z80_board_serial_port_list_count(portList.get());
    return 0;
}

QVariant SerialPortModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < z80_board_serial_port_list_count(portList.get()))
    {
        switch(role)
        {
        case Qt::DisplayRole:
            return QString::fromUtf8(z80_board_serial_port_list_get_desc(portList.get(), index.row()));
        case Qt::UserRole:
            return QString::fromUtf8(z80_board_serial_port_list_get_path(portList.get(), index.row()));
        }
    }
    return QVariant::Invalid;
}

void SerialPortModel::refresh()
{
    beginResetModel();
    portList.reset(z80_board_list_serial_ports());
    endResetModel();
}