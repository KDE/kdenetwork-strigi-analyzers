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
#include <qstringlist.h>
#include <qiodevice.h>

#include <kdebug.h>

#include "bbase.h"
#include "bdict.h"
#include "bstring.h"
#include "bint.h"
#include "blist.h"

BDict::BDict (QByteArray &dict, int start)
    : m_map(), m_valid(false)
{
    ByteTape tape(dict, start);

    init (tape);
}

BDict::BDict (ByteTape &tape)
    : m_map(), m_valid (false)
{
    init (tape);
}

void BDict::init (ByteTape &tape)
{
    if (*tape != 'd')
    {
        kdDebug(7034) << "This isn't a dictionary!" << endl;
        return; // This isn't a dictionary
    }

    tape++;

    // We need to loop and read in a string, then read in some data
    while (*tape != 'e')
    {
        BBase *temp_item = 0;

        // Read in string
        KSharedPtr<BString> str (new BString (tape));

        // Ensure str will be automatically deleted
        if (!str || !str->isValid())
        {
            kdDebug(7034) << (str ? "Invalid string" : "Unable to read String!") << endl;
            return;
        }

        // Read in data
        switch (*tape)
        {
            case 'l':
                temp_item = new BList (tape);
            break;

            case 'i':
                temp_item = new BInt (tape);
            break;

            case 'd':
                temp_item = new BDict (tape);
            break;

            default:
                // Hopefully this is a string
                temp_item = new BString (tape);
        }

        if (!temp_item || !temp_item->isValid())
        {
            kdDebug(7034) << (temp_item ? "Invalid item!"
                      : "Unable to create keyed data!") << endl;
            return;
        }

        m_map.insert(str->get_string(), temp_item);
    }

    // Move past the 'e'
    tape++;

    // Let the map delete the items
    m_map.setAutoDelete (true);

    // Oh yeah, we're valid now, too. :-)
    m_valid = true;
}

BDict::~BDict ()
{
    // QDict will take care of deleting each entry that
    // it holds.
}

BInt *BDict::findInt (const char *key)
{
    BBase *base = find(key);

    if (base && base->type_id() == bInt)
        return dynamic_cast<BInt*>(base);

    return 0;
}

BList *BDict::findList (const char *key)
{
    BBase *base = find(key);

    if (base && base->type_id() == bList)
        return dynamic_cast<BList*>(base);

    return 0;
}

BDict *BDict::findDict (const char *key)
{
    BBase *base = find(key);

    if (base && base->type_id() == bDict)
        return dynamic_cast<BDict*>(base);

    return 0;
}

BString *BDict::findStr (const char *key)
{
    BBase *base = find(key);

    if (base && base->type_id() == bString)
        return dynamic_cast<BString*>(base);

    return 0;
}

bool BDict::writeToDevice(QIODevice &device)
{
    if (!isValid())
        return false;

    const char *d_str = "d";
    const char *e_str = "e";
    Q_LONG written = 0, result = 0;

    written = device.writeBlock (d_str, 1);
    while (written < 1)
    {
        if (written < 0 || result < 0)
            return false;

        result = device.writeBlock (d_str, 1);
        written += result;
    }

    // Strings are supposed to be written in the dictionary such that
    // the keys are in sorted order.  QDictIterator doesn't support an
    // ordering, so we have to get a list of all the keys, sort it, and
    // then go by the list.

    BBaseHashIterator iter (m_map);
    QStringList key_list;

    for ( ; iter.current(); ++iter)
        key_list.append(iter.currentKey());

    key_list.sort();

    QStringList::Iterator key_iter;
    for (key_iter = key_list.begin(); key_iter != key_list.end(); ++key_iter)
    {
        QCString utfString = (*key_iter).utf8();
        QString str = QString("%1:").arg(utfString.size() - 1);

        QCString lenString = str.utf8();

        // Write out length of key
        device.writeBlock(lenString.data(), lenString.size() - 1);

        // Write out actual key
        device.writeBlock(utfString.data(), utfString.size() - 1);

        // Write out the key's data
        BBase *base = m_map.find(*key_iter);
        if (!base->writeToDevice (device))
            return false;
    }

    written = device.writeBlock (e_str, 1);
    while ((uint) written < 1)
    {
        if (written < 0 || result < 0)
            return false;

        result = device.writeBlock (e_str, 1);
        written += result;
    }

    return true;
}

// vim: set et sw=4 ts=4:
