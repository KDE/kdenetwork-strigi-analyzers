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
#include <kdebug.h>
#include <qcstring.h>
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

	kdDebug() << "1. Tape at position "  << tape.pos() << endl;

	if (dict.find(':', tape.pos()) == -1)
	{
		kdDebug() << "Can't find : for string!\n";
		return;
	}

	// Copy the part from start to :, as it will be a number
	// That number is the number of characters to read
	int length = dict.find(':', tape.pos()) - tape.pos();
	kdDebug () << "String digit length is apparently " << length << endl;
	char *ptr = dict.data();

	kdDebug() << "2. Tape at position " << tape.pos() << endl;
	ptr += tape.pos();

    QByteArray buffer (length + 1);
    qmemmove (buffer.data(), ptr, length);
	buffer[length] = 0;

	kdDebug() << "Read " << buffer.data() << " into buffer.\n";

	QString numberString (buffer);
	bool a_isValid;
	ulong len = numberString.toULong (&a_isValid);

	if (!a_isValid)
	{
		kdDebug() << "Invalid string length!\n";
		return;
	}
	else
		kdDebug() << "String length is apparently " << len << endl;

	// Now that we have the length, we need to advance the tape
	// past the colon
	kdDebug() << "Trying to move tape " << length << " spaces.\n";
	tape += length; // Move to colon
	kdDebug() << "3. Tape is at " << tape.pos() << endl;
	if (*tape != ':')
	{
		// Sanity check
		kdDebug() << "SANITY CHECK FAILED. *tape != ':'!\n";
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
	kdDebug() << "String is " << get_string() << endl;
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

    kdDebug() << "Writing string " << str << endl;
    Q_LONG written = 0, result = 0;

    written = device.writeBlock (str.latin1(), str.length());
    while ((uint) written < str.length())
    {
        if (written < 0 || result < 0)
            return false;

        result = device.writeBlock (str.latin1() + written,
            str.length() - written);
        written += result;
    }

    // Output the actual data
    written = device.writeBlock (m_data.data(), m_data.count() - 1);
    while ((uint) written < m_data.count() - 1)
    {
        if (written < 0 || result < 0)
            return false;

        result = device.writeBlock (m_data.data() + written,
            m_data.count() - written - 1);
        written += result;
    }

    // Done
    return true;
}

bool BString::setValue (const QString &str)
{
    m_data.resize (str.length() + 1);
    qstrcpy (m_data.data(), str.latin1());

    return true;
}