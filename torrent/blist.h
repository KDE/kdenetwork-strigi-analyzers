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
#ifndef _BLIST_H
#define _BLIST_H

#include <qvaluelist.h>
#include <qcstring.h>
#include "bbase.h"
#include "bytetape.h"

typedef QValueList<BBase *> BBaseVector;
typedef QValueList<BBase *>::iterator BBaseVectorIterator;

// Predeclare the following classes
class BDict;
class BString;
class BInt;  

/**
 * Class to construct a list of BBase objects from a b-encoded
 * list.
 *
 * @author Michael Pyne <mpyne@grammarian.homelinux.net>
 * @see BBase
 */
class BList : public BBase
{
    public:
    
    /**
     * Construct a BList from @p dict.
     *
     * @param dict the buffer to read from
     * @param start the position in the buffer to start
     *        reading from
     */
    BList (QByteArray &dict, unsigned int start = 0);

    /**
     * Construct a BList from @p tape.  Any changes made to
     * @p tape, such as its current position and data, will be
     * shared with the object that called this constructor.  @p tape
     * should already be positioned at the position to read from.
     * If construction was successful, @p tape will point to the
     * byte after the list data.  If construction was unsuccessful,
     * the position of the tape is undefined.
     *
     * @param tape the ByteTape to read from.
     */
    BList (ByteTape &tape);
    
    /**
     * Destroys the list, and deletes all of the items that had
     * been contained within.
     */
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
    virtual unsigned int count() const { return m_array.count(); }

    /**
     * This function should be called to determine whether the
     * list was successfully created, since no exceptions
     * are thrown.
     *
     * @return true if this is a valid list, false otherwise
     */
    virtual bool isValid() const { return m_valid; }
    
    /**
     * This function returns a pointer to the appropriate
     * item in the list.
     * 
     * @param i index of the item to return
     * @return pointer to the appropriate BBase, or 0 if
     *         the index is out-of-bounds
     */
    inline BBase * index (unsigned int i);
    
    /**
     * Convience function to return a pointer to the appropriate
     * item in the list, already casted.
     *
     * @param i index of the item to return
     * @return pointer to the appropriate BBase, downcasted to
     *         BList.  If the element is <b>not</b> a BList, 0
     *         will be returned instead, even if it was a valid
     *         BBase.
     */
    BList *   indexList (unsigned int i);
    
    /**
     * Convience function to return a pointer to the appropriate
     * item in the list, already casted.
     *
     * @param i index of the item to return
     * @return pointer to the appropriate BBase, downcasted to
     *         BInt.  If the element is <b>not</b> a BInt, 0
     *         will be returned instead, even if it was a valid
     *         BBase.
     */
    BInt *    indexInt (unsigned int i);  
    
    /**
     * Convience function to return a pointer to the appropriate
     * item in the list, already casted.
     *
     * @param i index of the item to return
     * @return pointer to the appropriate BBase, downcasted to
     *         BDict.  If the element is <b>not</b> a BDict, 0
     *         will be returned instead, even if it was a valid
     *         BBase.
     */
    BDict *   indexDict (unsigned int i);
    
    /**
     * Convience function to return a pointer to the appropriate
     * item in the list, already casted.
     *
     * @param i index of the item to return
     * @return pointer to the appropriate BBase, downcasted to
     *         BString.  If the element is <b>not</b> a BString, 0
     *         will be returned instead, even if it was a valid
     *         BBase.
     */
    BString * indexStr (unsigned int i);
    
    /**
     * Returns an iterator to the first element in the list.
     * There is no particular sorting associated with the list
     * at this time.
     *
     * @return iterator pointing to the beginning of the list
     * @see QValueList
     */
    BBaseVectorIterator begin(void) { return m_array.begin(); }

    /**
     * Returns an iterator pointing one element past the end of
     * the list.  Although this element belongs to the list,
     * you should never dereference this iterator.  Instead, treat
     * it as a boundary condition to avoid.
     *
     * @return iterator pointing one element past the end of the list
     * @see QValueList
     */
    BBaseVectorIterator end(void) { return m_array.end(); }
    
    /**
     * Outputs the b-encoded representation of the object to the given
     * QIODevice.
     * @param device the QIODevice to write to
     * @return true on a successful write, false otherwise
     */
    virtual bool writeToDevice (QIODevice &device);

    private:
    
    /**
     * This function handles the actual initialization of the object upon
     * construction, and set the m_valid flag if successful.
     *
     * @param tape the ByteTape to read from
     */
    void init(ByteTape &tape);

    bool m_valid;
    BBaseVector m_array;
};

#endif /* _BLIST_H */

// vim: set et sw=4 ts=4:
