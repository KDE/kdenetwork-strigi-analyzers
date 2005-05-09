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
#ifndef _BINT_H
#define _BINT_H

#include <qcstring.h>
#include "bbase.h"
#include "bytetape.h"

/**
 * Class to represent a b-encoded integer.
 *
 * @author Michael Pyne <mpyne@grammarian.homelinux.net>
 * @see BBase
 */
class BInt : public BBase
{
    public:

    /**
     * Constructs a BInt by reading a b-encoded integer from @p dict.
     * You can start reading from a position other than the beginning
     * by passing that position to the constructor.
     *
     * @param dict the buffer to read from
     * @param start the position within the buffer of the beginning
     *        of the b-encoded integer.
     */
    BInt (QByteArray &dict, int start = 0);

    /**
     * Constructs a BInt by reading a b-encoded integer from @p tape.
     * 
     * @param tape the ByteTape to read from.  It should already be
     *        positioned at the beginning of the b-encoded integer data.
     *        After construction, @p tape will point to the byte after
     *        the b-encoded integer on success.  If construction was
     *        not successful, @p tape will have an undefined position.
     */
    BInt (ByteTape &tape);

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
    Q_LLONG get_value () const { return m_value; }

    /**
     * Returns the type of this class.
     *
     * @return bInt.  This value is only returned by this class.
     */
    virtual classID type_id() const { return bInt; }
    
    /**
     * This function should be called to determine whether the
     * integer was successfully created, since no exceptions
     * are thrown.
     *
     * @return true if this is a valid integer, false otherwise
     */
    virtual bool isValid() const { return m_valid; }
    
    /**
     * Outputs the b-encoded representation of the object to the given
     * QIODevice.
     * @param device the QIODevice to write to
     * @return true on a successful write, false otherwise
     */
    virtual bool writeToDevice (QIODevice &device);

    private:
    
    /**
     * Initialization function for the class, called to handle the
     * actual work of reading the b-encoded data from @p tape.
     *
     * @param tape the ByteTape to read from
     */
    void init(ByteTape &tape);

    Q_LLONG m_value;
    bool m_valid;
};

#endif /* _BINT_H */

// vim: set et ts=4 sw=4:
