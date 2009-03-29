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
#include "blist.h"
#include "bytestream.h"
#include "bdict.h"
#include "bstring.h"
#include "bint.h"

#include <QtCore/QIODevice>

#include <stdexcept>
#include <string>

BList::BList (ByteStream &stream)
    : m_array()
{
    BBase::Ptr temp;

    if (*stream != 'l')
        return;

    ++stream;

    /* Repeat circling over the string until the list is over */
    while (*stream != 'e')
    {
        switch (*stream)
        {
            case 'd':
                temp = BBase::Ptr(new BDict (stream));
            break;

            case 'l': /* This element is a list */
                temp = BBase::Ptr(new BList (stream));
            break;

            case 'i': /* This element is an int */
                temp = BBase::Ptr(new BInt (stream));
            break;

            default: /* Maybe a string? */
                temp = BBase::Ptr(new BString (stream));
        }

        if (!temp)
            throw std::runtime_error("Error creating BList");

        m_array.append (temp);
    }

    // Only way out is to detect 'e', so we need to increment stream past that.
    ++stream;
}

BList::~BList()
{
}

unsigned int BList::count() const
{
    return m_array.count();
}

BBase::Ptr BList::index (unsigned int i) const
{
    if (i >= count())
        throw std::runtime_error("BList array access out of bounds");

    return m_array[i];
}

BBaseVectorIterator BList::iterator() const
{
    return BBaseVectorIterator(m_array);
}

bool BList::writeToDevice(QIODevice &device)
{
    if (!device.putChar('l'))
        return false;

    foreach(const BBase::Ptr &ptr, m_array) {
        if(!ptr->writeToDevice(device))
            return false;
    }

    if (!device.putChar('e'))
        return false;

    return true;
}

// vim: set et sw=4 ts=4:
