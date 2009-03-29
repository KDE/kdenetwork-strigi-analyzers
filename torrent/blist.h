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
#ifndef TORRENT_ANALYZER_LIST_H
#define TORRENT_ANALYZER_LIST_H

#include <QtCore/QList>

#include "bbase.h"

class ByteStream;

typedef QList<BBase::Ptr> BBaseVector;
typedef QListIterator<BBase::Ptr> BBaseVectorIterator;

/**
 * Class to construct a list of BBase objects from a b-encoded
 * list.
 *
 * @author Michael Pyne <michael.pyne@kdemail.net>
 * @see BBase
 */
class BList : public BBase
{
public:
    typedef boost::shared_ptr<BList> Ptr;

    /**
     * Construct a BList from @p stream.  Any changes made to
     * @p stream, such as its current position and data, will be
     * shared with the object that called this constructor.  @p stream
     * should already be positioned at the position to read from.
     * If construction was successful, @p stream will point to the
     * byte after the list data.  An exception is thrown if errors are
     * encountered while constructing.
     *
     * @param stream the ByteStream to read from.
     */
    BList (ByteStream &stream);

    virtual ~BList ();

    /**
     * Returns the type of this class.
     *
     * @return bList.  This value is only returned by this class.
     */
    virtual classID type_id() const { return bList; }

    /**
     * Returns the number of items contained within the list.
     *
     * @return number of items in the list
     */
    virtual unsigned int count() const;

    /**
     * This function returns a pointer to the appropriate
     * item in the list.
     *
     * @param i index of the item to return
     * @return pointer to the appropriate BBase.  An exception
     *         is raised if the index is out of bounds.
     */
    BBase::Ptr index (unsigned int i) const;

    /**
     * Convenience function to return a pointer to the appropriate
     * item in the list, already casted to the appropriate type.
     *
     * @param i index of the item to return
     * @return pointer to the appropriate BBase, downcasted to
     *         the given type.  If the element is not the given
     *         type a null pointer is returned.  If the index is
     *         out-of-range, an exception will be raised.
     */
    template<class T>
    boost::shared_ptr<T> indexType(unsigned int i) const
    {
        return boost::dynamic_pointer_cast<T>(index(i));
    }

    /**
     * Returns an iterator on the list (Java-style).
     * There is no particular sorting associated with the list
     * at this time.
     *
     * @return non-mutable iterator
     * @see QListIterator
     */
    BBaseVectorIterator iterator() const;

    /**
     * Outputs the b-encoded representation of the object to the given
     * QIODevice.
     * @param device the QIODevice to write to
     * @return true on a successful write, false otherwise
     */
    virtual bool writeToDevice (QIODevice &device);

private:
    BBaseVector m_array;
};

#endif /* TORRENT_ANALYZER_LIST_H */

// vim: set et sw=4 ts=4:
