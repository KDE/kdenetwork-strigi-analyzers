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
#ifndef TORRENT_ANALYZER_STRING_H
#define TORRENT_ANALYZER_STRING_H

#include "bbase.h"

#include <QtCore/QByteArray>

class ByteStream;
class QString;

/**
 * A class to handle the b-encoded strings used by BitTorrent.
 * It implements BBase, and although the class is referred to
 * as a string, it can hold arbitrary data, since b-encoded strings
 * are stored with a length, instead of using a terminator character.
 *
 * @author Michael Pyne <michael.pyne@kdemail.net>
 * @see BBase
 */
class BString : public BBase
{
    public:

    typedef boost::shared_ptr<BString> Ptr;

    /**
     * Construct a BString from @p stream.  Any changes made to
     * @p stream, such as its current position and data, will be
     * shared with the object that called this constructor.  @p stream
     * should already be positioned at the position to read from.
     * If construction was successful, @p stream will point to the
     * byte after the string data.  If construction was unsuccessful,
     * then an exception is thrown.
     *
     * @param stream the ByteStream to read from.
     */
    BString (ByteStream &stream);

    virtual ~BString ();

    /**
     * @return the character data as read in from the b-encoding.
     */
    QByteArray raw_data() const { return m_data; }

    /**
     * Returns a QString representation of the data in the
     * BString.  It is the responsibility of the caller to ensure
     * that the data is convertible to a QString.  More specifically,
     * the data should not contain any embedded NULLs.
     *
     * @return QString containing the data from this BString.
     */
    QString toString() const;

    /**
     * Returns the size in bytes of the string.  Note that no \0 terminator
     * is included in the size calculation (and indeed, b-encoded strings
     * may have arbitrary nulls.
     *
     * @return the size of the string, not including the NULL
     *         terminator.
     */
    int size() const { return m_data.size(); }

    /**
     * Returns the type of this class.
     *
     * @return bString.  This value is only returned by this class.
     */
    virtual classID type_id() const { return bString; }

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

    QByteArray m_data;
    bool m_valid;
};

#endif /* TORRENT_ANALYZER_STRING_H */

// vim: set et ts=4 sw=4:
