/*
 * Copyright (c) 2003, 2004 Michael Pyne <michael.pyne@kdemail.net>
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
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#include <qstring.h>
#include <qcstring.h>
#include <qiodevice.h>

#include "bytetape.h"
#include "bint.h"

// A bencoded int is (approximately) as follows:
// i(\d)+e
BInt::BInt (QByteArray &dict, int start)
    : m_value (0), m_valid(false)
{
    ByteTape tape (dict, start);
    init (tape);
}

BInt::BInt (ByteTape &tape)
    : m_value(0), m_valid(false)
{
    init(tape);
}

void BInt::init (ByteTape &tape)
{
    if (*tape != 'i')
        return;

    tape ++; // Move to start of digits

    QByteArray &dict (tape.data());
    if (dict.find('e', tape.pos()) == -1)
        return;

    // Copy the part from the start to the e.  The values in-between
    // should be digits, perhaps preceded by a negative sign.
    int length = dict.find('e', tape.pos()) - tape.pos();
    char *ptr = dict.data(); // Get start of buffer
    ptr += tape.pos(); // Advance to current position in tape

    // Allocate temporary data buffer
    QByteArray buffer(length + 1);

    qmemmove (buffer.data(), ptr, length);
    buffer[length] = 0; // Null-terminate

    QString numberString (buffer);
    bool a_isValid; // We want to make sure the string is a valid number

    m_value = numberString.toLongLong(&a_isValid);

    tape += length; // Move to 'e'
    tape ++;        // Move to next char

    m_valid = a_isValid; // Now we're good, if it was a number
}

BInt::~BInt()
{
    /* Nothing yet */
}

bool BInt::writeToDevice (QIODevice &device)
{
    if (!m_valid)
        return false;

    /* Write out i234e, and such */
    QString str = QString("i%1e").
        arg (m_value);

    Q_LONG written = 0, result = 0;
    written = device.writeBlock (str.latin1(), str.length());
    while ((uint) written < str.length())
    {
        if (written < 0 || result < 0)
            return false;

        result = device.writeBlock(str.latin1() + written,
                str.length() - written);
        written += result;
    }

    return true;
}

// vim: set et ts=4 sw=4:
