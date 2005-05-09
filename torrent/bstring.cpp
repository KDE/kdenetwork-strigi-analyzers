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
#include <qcstring.h>
#include <qiodevice.h>

#include <kdebug.h>

#include "bstring.h"
#include "bytetape.h"

BString::BString (QByteArray &dict, int start) :
    m_data(), m_valid(false)
{
    ByteTape tape (dict, start);
    init (tape);
}

BString::BString (ByteTape &tape)
    : m_data(), m_valid(false)
{
    init (tape);
}

// The reason we don't store stuff in a QString is because BitTorrent
// b-encoded strings may contain zeroes within the string, which makes
// a BString more of a buffer than a true string.
void BString::init (ByteTape &tape)
{
    QByteArray &dict(tape.data());

    if (dict.find(':', tape.pos()) == -1)
    {
        kdDebug(7034) << "Can't find : for string!" << endl;
        return;
    }

    // Copy the part from start to :, as it will be a number
    // That number is the number of characters to read
    int length = dict.find(':', tape.pos()) - tape.pos();
    char *ptr = dict.data();

    ptr += tape.pos();

    QByteArray buffer (length + 1);
    qmemmove (buffer.data(), ptr, length);
    buffer[length] = 0;

    QString numberString (buffer);
    bool a_isValid;
    ulong len = numberString.toULong (&a_isValid);

    if (!a_isValid)
    {
        kdDebug(7034) << "Invalid string length!" << endl;
        return;
    }

    // Now that we have the length, we need to advance the tape
    // past the colon
    tape += length; // Move to colon
    if (*tape != ':')
    {
        // Sanity check
        kdError(7034) << "SANITY CHECK FAILED. *tape != ':'!" << endl;
        return;
    }

    tape++; // Move past colon

    // Time to copy the data
    char *textBuffer = tape.at(tape.pos());
    if (!m_data.resize(len + 1))
        return;

    qmemmove (m_data.data(), textBuffer, len);
    m_data[len] = 0; // Null terminate for convienience

    tape += len;
    m_valid = true;
}

BString::~BString ()
{
}

bool BString::writeToDevice(QIODevice &device)
{
    if (!m_valid)
        return false;

    QString str = QString("%1:").
        arg(get_len());

    QCString utfString = str.utf8();

    /* Don't write null terminator */
    device.writeBlock (utfString.data(), utfString.size() - 1);

    // Output the actual data
    device.writeBlock (m_data.data(), m_data.size() - 1);

    // Done
    return true;
}

bool BString::setValue (const QString &str)
{
    m_data = str.utf8();
    return true;
}

// vim: set et ts=4 sw=4:
