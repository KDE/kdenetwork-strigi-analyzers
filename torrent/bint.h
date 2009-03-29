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
#ifndef TORRENT_ANALYZER_INT_H
#define TORRENT_ANALYZER_INT_H

#include "bbase.h"

#include <QtGlobal>

class ByteStream;

/**
 * Class to represent a b-encoded integer.
 *
 * @author Michael Pyne <michael.pyne@kdemail.net>
 * @see BBase
 */
class BInt : public BBase
{
    public:

    typedef boost::shared_ptr<BInt> Ptr;

    /**
     * Constructs a BInt by reading a b-encoded integer from @p stream.
     *
     * If an error is encountered decoding the stream an exception will
     * be raised.
     *
     * Note: The stream should still be on the initial 'i' character.
     *
     * @param stream the ByteStream to read from.
     */
    BInt (ByteStream &stream);

    /**
     * Destructor for this class.  No special action is taken.
     */
    virtual ~BInt ();

    /**
     * Returns the integer value of the data used to construct this
     * object.
     *
     * @return this object's integer value
     */
    qlonglong get_value () const { return m_value; }

    /**
     * Returns the type of this class.
     *
     * @return bInt.  This value is only returned by this class.
     */
    virtual classID type_id() const { return bInt; }

    /**
     * Outputs the b-encoded representation of the object to the given
     * QIODevice.
     * @param device the QIODevice to write to
     * @return true on a successful write, false otherwise
     */
    virtual bool writeToDevice (QIODevice &device);

    private:

    qlonglong m_value;
};

#endif /* TORRENT_ANALYZER_INT_H */

// vim: set et ts=4 sw=4:
