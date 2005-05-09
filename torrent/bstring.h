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
#ifndef _BSTRING_H
#define _BSTRING_H

#include <qstring.h>
#include <qcstring.h>

#include "bytetape.h"
#include "bbase.h"

/**
 * A class to handle the b-encoded strings used by BitTorrent.
 * It implements BBase, and although the class is referred to
 * as a string, it can hold arbitrary data, since b-encoded strings
 * are stored with a length, instead of using a terminator character.
 *
 * @author Michael Pyne <mpyne@grammarian.homelinux.net>
 * @see BBase
 */
class BString : public BBase
{
    public:

    /**
     * Construct a BString from @p dict.
     *
     * @param dict the buffer to read from
     * @param start the position in the buffer to start
     *        reading from
     */
    BString (QByteArray &dict, int start = 0);

    /**
     * Construct a BString from @p tape.  Any changes made to
     * @p tape, such as its current position and data, will be
     * shared with the object that called this constructor.  @p tape
     * should already be positioned at the position to read from.
     * If construction was successful, @p tape will point to the
     * byte after the string data.  If construction was unsuccessful,
     * the position of the tape is undefined.
     *
     * @param tape the ByteTape to read from.
     */
    BString (ByteTape &tape);
    
    /**
     * Destroys the BString, and deallocates all memory that had
     * been used.
     */
    virtual ~BString ();

    /**
     * Returns a QString representation of the data in the
     * BString.  It is the responsibility of the caller to ensure
     * that the data is convertible to a QString.  More specifically,
     * the data should not contain any embedded NULLs.
     *
     * @return QString containing the data from this BString.
     */
    QString get_string() const { return QString::fromUtf8(m_data.data()); }

    /**
     * Returns the amount of data held by the string.  It would be
     * perhaps more appropriate to call this size(), since this is
     * a buffer, not a true text string.
     *
     * @return the size of the string, not including the NULL
     *         terminator.
     */
    const int get_len() const { return m_data.size() - 1; }

    /**
     * Returns the type of this class.
     *
     * @return bString.  This value is only returned by this class.
     */
    virtual classID type_id() const { return bString; }

    /**
     * This function should be called to determine whether the
     * string was successfully created, since no exceptions
     * are thrown.
     *
     * @return true if this is a valid string, false otherwise
     */
    virtual bool isValid() const { return m_valid; }

    /**
     * Outputs the b-encoded representation of the object to the given
     * QIODevice.
     * @param device the QIODevice to write to
     * @return true on a successful write, false otherwise
     */
    virtual bool writeToDevice (QIODevice &device);
    
    /**
     * Changes the value of the string to the given QString.
     *
     * @param str the QString containing the new value
     * @return true if the value was successfully changed,
     *         false otherwise.
     */
    bool setValue (const QString &str);
    
    private:

    /**
     * This function handles the actual initialization of the object upon
     * construction, and set the m_valid flag if successful.
     *
     * @param tape the ByteTape to read from
     */
    void init (ByteTape &tape);

    QByteArray m_data;
    bool m_valid;
};

#endif /* _BSTRING_H */

// vim: set et ts=4 sw=4:
