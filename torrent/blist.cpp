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
#include <qiodevice.h>

#include "bytetape.h"
#include "blist.h"
#include "bdict.h"
#include "bstring.h"
#include "bint.h"

BList::BList (ByteTape &tape)
    : m_valid(false), m_array()
{
    init (tape);
}

BList::BList (QByteArray &dict, unsigned int start)
    : m_valid(false), m_array()
{
    ByteTape tape (dict, start);

    init (tape);
}

void BList::init (ByteTape &tape)
{
    BBase *temp;

    if (*tape != 'l')
        return;

    tape++;
    
    /* Repeat circling over the string until the list is over */
    while (*tape != 'e')
    {
        switch (*tape)
        {
            case 'd':
                temp = new BDict (tape);
            break;

            case 'l': /* This element is a list */
                temp = new BList (tape);
            break;

            case 'i': /* This element is an int */
                temp = new BInt (tape);
            break;
                
            default: /* Maybe a string? */
                temp = new BString (tape);
        }
        
        if (!temp || !temp->isValid())
            return;  // Invalid list element

        m_array.append (temp);
    }

    m_valid = true;

    // Only way out is to detect 'e', so we need to increment tape past that.
    tape++;
}

BList::~BList()
{
    BBaseVectorIterator iter;

    for (iter = begin(); iter != end(); ++iter)
        delete *iter;
}

BBase* BList::index (unsigned int i)
{
    if (i >= count())
        return 0;
    else
        return m_array[i];
}

BList * BList::indexList (unsigned int i)
{
    BBase *base = index(i);

    if (base && base->type_id() == bList)
        return dynamic_cast<BList*>(base);
    
    return 0;
}

BInt * BList::indexInt (unsigned int i)
{
    BBase *base = index(i);

    if (base && base->type_id() == bInt)
        return dynamic_cast<BInt*>(base);
    
    return 0;
}

BDict * BList::indexDict (unsigned int i)
{
    BBase *base = index(i);

    if (base && base->type_id() == bDict)
        return dynamic_cast<BDict*>(base);
    
    return 0;
}

BString * BList::indexStr (unsigned int i)
{
    BBase *base = index(i);

    if (base && base->type_id() == bString)
        return dynamic_cast<BString*>(base);
    
    return 0;
}

bool BList::writeToDevice(QIODevice &device)
{
    if (!m_valid)
        return false;

    const char *l_str = "l";
    const char *e_str = "e";
    Q_LONG written = 0, result = 0;
    
    written = device.writeBlock (l_str, 1);
    while (written < 1)
    {
        if (written < 0 || result < 0)
            return false;
            
        result = device.writeBlock (l_str, 1);
        written += result;
    }
    
    BBaseVectorIterator iter;
    for (iter = begin(); iter != end(); ++iter)
    {
        if (!((*iter)->writeToDevice (device)))
            return false;
    }
    
    written = device.writeBlock (e_str, 1);
    while (written < 1)
    {
        if (written < 0 || result < 0)
            return false;
            
        result = device.writeBlock (e_str, 1);
        written += result;
    }
    
    return true;
}

// vim: set et sw=4 ts=4:
