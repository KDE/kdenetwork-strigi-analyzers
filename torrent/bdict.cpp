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
#include "bdict.h"
#include "bytestream.h"
#include "bstring.h"
#include "bint.h"
#include "blist.h"

#include <QtCore/QIODevice>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QtAlgorithms>

#include <stdexcept>
#include <string>

BDict::BDict (ByteStream &stream)
    : m_dict()
{
    if (*stream != 'd')
    {
        throw std::runtime_error("Trying to read dictionary, but this isn't a dictionary");
    }

    ++stream;

    // We need to loop and read in a string, then read in some data
    while (*stream != 'e')
    {
        // Read in string
        BString::Ptr str(new BString(stream));

        BBase::Ptr temp_item;

        // Read in data
        switch (*stream)
        {
            case 'l':
                temp_item = BBase::Ptr(new BList (stream));
            break;

            case 'i':
                temp_item = BBase::Ptr(new BInt (stream));
            break;

            case 'd':
                temp_item = BBase::Ptr(new BDict (stream));
            break;

            default:
                // Hopefully this is a string
                temp_item = BBase::Ptr(new BString (stream));
        }

        m_dict.insert(str->raw_data(), temp_item);
    }

    // Move past the 'e'
    ++stream;
}

BDict::~BDict ()
{
}

int BDict::count() const
{
    return m_dict.count();
}

BBase::Ptr BDict::find (const QByteArray &key) const
{
    return m_dict.value(key);
}

bool BDict::contains (const QByteArray &key)
{
    return m_dict.contains(key);
}

BDictionaryIterator BDict::iterator() const
{
    return BDictionaryIterator(m_dict);
}

bool BDict::writeToDevice(QIODevice &device)
{
    if(!device.putChar('d'))
        return false;

    // Strings are supposed to be written in the dictionary such that
    // the keys are in sorted order.  QHashIterator doesn't support an
    // ordering, so we have to get a list of all the keys, sort it, and
    // then go by the list.

    QList<QByteArray> keys = m_dict.keys();
    qSort(keys);

    foreach (const QByteArray &key, keys) {
        const QByteArray lenString(QByteArray::number(key.length()));

        // Write out length of key
        if(lenString.size() != device.write(lenString.constData(), lenString.size()))
            return false;

        // Write out actual key
        if(key.size() != device.write(key.constData(), key.size()))
            return false;

        // Write out the key's data
        BBase::Ptr base(m_dict.value(key));
        if (!base || !base->writeToDevice (device))
            return false;
    }

    if(!device.putChar('e'))
        return false;

    return true;
}

// vim: set et sw=4 ts=4:
