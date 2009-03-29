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
#ifndef TORRENT_ANALYZER_BASE_H
#define TORRENT_ANALYZER_BASE_H

#include <boost/shared_ptr.hpp>

class QIODevice;

/**
 * Abstract base class for the b-encoded types.  Re-implemented
 * by BInt, BList, BDict, and BString.  Derive from this class in
 * order to make a new type for the b-encoding suite.  Classes
 * derived from this one should not throw exceptions.  Instead,
 * they should implement isValid(), and allow calling modules to
 * check error status that way.
 *
 * @author Michael Pyne <michael.pyne@kdemail.net>
 * @see BInt, BList, BDict, BString, QSharedData
 */
class BBase
{
    public:

    typedef boost::shared_ptr<BBase> Ptr;

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
     * Outputs the b-encoded representation of the object to the given
     * QIODevice.
     * @param device the QIODevice to write to
     * @return true on a successful write, false otherwise
     */
    virtual bool writeToDevice (QIODevice &device) = 0;
};

#endif /* TORRENT_ANALYZER_BASE_H */

// vim: set et sw=4 ts=4:
