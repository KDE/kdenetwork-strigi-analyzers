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
#ifndef _BYTETAPE_H
#define _BYTETAPE_H

#include <ksharedptr.h>

#include <qcstring.h>

class ByteTapeShared : public KShared
{
    public:

    unsigned int pos;
};

/**
 * Class to simulate a seekable byte stream.  Very similar to QByteArray,
 * but the difference is that this class "knows" what a current position
 * is.  Also, the copy constructor will share the byte stream of the
 * ByteTape being copied.  This means that any copies made of an object of
 * this class will share BOTH the data and the current position.  This is
 * by design.
 *
 * @author Michael Pyne <mpyne@grammarian.homelinux.net>
 * @see QByteArray
 */
class ByteTape
{
    public:
    
    /**
     * Constructs a new ByteTape object from @p array.  The
     * current position will be set to @p pos.
     *
     * @param array a data buffer to use for reading and writing.
     *        The tape will be fixed to the size of this buffer.
     * @param pos the initial position of the tape head.  It must be
     *        a position within the buffer contained in @p array.
     */
    ByteTape (QByteArray &array, int pos = 0);

    /**
     * Creates a ByteTape as a copy of @p tape.  The newly created
     * object will share both data and the current position with @p tape.
     * This is done using reference counts.
     *
     * @param tape the ByteTape to copy
     */
    ByteTape (const ByteTape &tape);

    /**
     * Increments the current position by @p i.  It is the responsibility
     * of the function caller to ensure that the new position is in bounds.
     * The position will be capped to remain in bounds regardless.
     *
     * @param i the amount to increment the current position by
     * @return t reference to the object incremented
     */
    ByteTape & operator += (const unsigned int i);

    /**
     * Decrements the current position by @p i.  It is the responsibility
     * of the function caller to ensure that the new position is in bounds.
     * Unlike a real Turing machine, attempting to decrement past the
     * start will be capped to the beginning instead of crashing the
     * computer.
     *
     * @param i the amount to decrement the current position by
     * @return a reference to the object decremented
     */
    ByteTape & operator -= (const unsigned int i);

    /**
     * Increments the current position by 1.  This is a postfix
     * operator (i++).  The current position will be capped to the end
     * of the buffer.
     *
     * @return the object before the increment operation
     */
    ByteTape operator ++ (int);

    /**
     * Increments the current position by 1.  This is a prefix
     * operator (++i).  The current position will be capped to the end
     * of the buffer.
     *
     * @return a reference to the object incremented
     */
    ByteTape & operator ++ ();
    
    /**
     * Decrements the current position by 1.  This is a postfix
     * operator (i--).  The current position will be capped to the start
     * of the buffer.
     *
     * @return the object before the decrement operation
     */
    ByteTape operator -- (int);

    /**
     * Decrements the current position by 1.  This is a prefix
     * operator (--i).  The current position will be capped to the start
     * of the buffer.
     *
     * @return a reference to the object decremented
     */
    ByteTape & operator -- ();
    
    /**
     * Returns the byte within the array indexed by @p i.  It is
     * the responsibility of the caller to ensure that @p i is in bounds.
     * Although out-of-range errors will be detected, no exceptions will
     * be thrown.  Since a reference is not returned, you won't be able
     * to assign to the result.  Example: tape[i] = 'a' will not work.
     *
     * The reason a reference isn't returned is because no exceptions are
     * thrown by this class, and we can't return a reference to an out-of-
     * bounds character.
     *
     * @param i the index of the byte to return
     * @return the byte at the given index.  0 may be returned on error,
     *         but does not necessarily indicate an error.
     */
    char operator [] (const unsigned int i);

    /**
     * Returns the byte at the tape's current position.  You can assign
     * to the reference returned.
     *
     * @return a reference to the byte at the tape head's current position
     */
    char &operator * ();

    /**
     * Returns the position in memory of data at the given index, @p i.
     * Unlike operator [], this function returns a pointer, so it can be
     * used to access memory.
     *
     * @param i index of the byte to lookup.
     * @return 0 if @p i is out of range, else the address of memory
     *         at that index
     */
    char *at(const unsigned int i);

    /**
     * Returns the current position of the tape head.
     *
     * @return the tape head's current position
     */
    unsigned int pos() const { return m_shared->pos; }

    /**
     * Sets the current position of the tape head to @p pos.  If the
     * position given is out-of-bounds, it will be capped to be within
     * the array.
     *
     * @param pos the new position of the tape head
     * @return whether the set operation was successful
     */
    bool setPos(unsigned int pos);

    /**
     * Returns a reference to the QByteArray used to hold all the data.
     *
     * @return the QByteArray used to hold the data
     * @see QByteArray
     */
    QByteArray &data() { return m_array; }

    private:
    QByteArray &m_array; 
    KSharedPtr<ByteTapeShared> m_shared;
};

#endif /* _BYTETAPE_H */

// vim: set et ts=4 sw=4:
