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
#ifndef _BBASE_H
#define _BBASE_H

#include <ksharedptr.h>

class QIODevice;

/**
 * Abstract base class for the b-encoded types.  Re-implemented
 * by BInt, BList, BDict, and BString.  Derive from this class in
 * order to make a new type for the b-encoding suite.  Classes
 * derived from this one should not throw exceptions.  Instead,
 * they should implement isValid(), and allow calling modules to
 * check error status that way.
 *
 * @author Michael Pyne <mpyne@grammarian.homelinux.net>
 * @see BInt, BList, BDict, BString, KSharedPtr
 */

// Derive from KShared to enable use of KSharedPtr.
class BBase : public KShared
{
    public:

    /**
     * Identifies the particular class that has been instantiated.  All
     * subclasses of this class should have an identifier here.
     */
    enum classID { 
        bBase,   /**< This class is BBase.  No subclass should return this. */
        bString, /**< This class is a BString. */
        bInt,    /**< This class is a BInt. */
        bList,   /**< This class is a BList. */
        bDict    /**< This class is a BDict. (Dictionary/Map) */
    };

    /**
     * Returns the type identification of the object.  It will
     * be a value in the classID enum.  A subclass of this class
     * must implement this function.
     *
     * @return type identifier of the class
     * @see classID
     */
    virtual classID type_id() const = 0;

    /**
     * Destructor for the class.  This function must be reimplemented
     * in subclasses.
     */
    virtual ~BBase () { ; }

    /**
     * Returns the validity status of the object.  Newly constructed
     * objects are invalid unless the initialization sequence completed
     * successfully.
     *
     * @return the validity status of the object
     */
    virtual bool isValid() const = 0;
    
    /**
     * Outputs the b-encoded representation of the object to the given
     * QIODevice.
     * @param device the QIODevice to write to
     * @return true on a successful write, false otherwise
     */
    virtual bool writeToDevice (QIODevice &device) = 0;
};

#endif /* _BBASE_H */

// vim: set et sw=4 ts=4:
