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
#include "bint.h"
#include "bytestream.h"

#include <QtCore/QString>
#include <QtCore/QIODevice>

#include <stdexcept>
#include <string>

// A bencoded int is (approximately) as follows:
// i(\d)+e
BInt::BInt (ByteStream &stream)
    : m_value(0)
{
    if (*stream != 'i')
        return;

    ++stream; // Move to start of digits
    QByteArray digits;

    // Loop until we encounter the 'e'
    while (*stream != 'e') {
        digits.append(*stream);
        ++stream;
    }

    // Eat the 'e'
    ++stream;

    bool a_isValid; // We want to make sure the string is a valid number

    m_value = digits.toLongLong(&a_isValid);

    if(!a_isValid)
        throw std::runtime_error("Invalid int read");
}

BInt::~BInt()
{
}

bool BInt::writeToDevice (QIODevice &device)
{
    if(!device.putChar('i'))
        return false;

    QByteArray value(QByteArray::number(m_value));

    if(value.size() != device.write(value.constData(), value.size()))
        return false;

    if(!device.putChar('e'))
        return false;

    return true;
}

// vim: set et ts=4 sw=4:
