/*
 * Copyright © 2003, 2004, 2009 Michael Pyne <michael.pyne@kdemail.net>
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; see the file COPYING.
 * If not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include "bstring.h"
#include "bytestream.h"

#include <QtCore/QString>
#include <QtCore/QIODevice>

#include <kdebug.h>

#include <stdexcept>
#include <string>

// The reason we don't store stuff in a QString is because BitTorrent
// b-encoded strings may contain zeroes within the string, which makes
// a BString more of a buffer than a true string.
BString::BString (ByteStream &stream)
    : m_data()
{
    // A BString is \d+:.{n}, where n is whatever \d+ converted to.
    // So, read in the number part first.
    QByteArray numberData;

    while(*stream != ':') {
        numberData.append(*stream);
        ++stream;
    }

    ++stream; // Read past ':'

    bool validNumber = false;
    quint32 length = numberData.toUInt(&validNumber);

    if(!validNumber)
        throw std::runtime_error("Invalid number in string data");

    for(quint32 i = 0; i < length; ++i) {
        m_data.append(*stream);
        ++stream;
    }
}

BString::~BString ()
{
}

QString BString::toString() const
{
    return QString::fromUtf8(m_data.constData());
}

bool BString::writeToDevice(QIODevice &device)
{
    const QByteArray lengthStr(QByteArray::number(m_data.length()));

    if(lengthStr.size() != device.write(lengthStr.data(), lengthStr.size()))
        return false;

    if(!device.putChar(':'))
        return false;

    if(m_data.size() != device.write(m_data.constData(), m_data.size()))
        return false;

    return true;
}

bool BString::setValue (const QString &str)
{
    m_data = str.toUtf8();
    return true;
}

// vim: set et ts=4 sw=4:
