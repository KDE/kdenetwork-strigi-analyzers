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
#ifndef _BDICT_H
#define _BDICT_H

#include <qdict.h>
#include <qcstring.h> // QByteArray
#include "bytetape.h"
#include "bbase.h"

// Some useful typedefs
typedef QDict<BBase> BBaseHash;
typedef QDictIterator<BBase> BBaseHashIterator;

// Forward declarations
class BInt;
class BList;
class BString;

/**
 * Class to handle the BitTorrent b-encoded dictionary.  It is keyed
 * using const char *strings, and stores pointers to a class descended
 * from BBase, such as BInt, BString, BList, or even more BDicts.
 * 
 * @author Michael Pyne <mpyne@grammarian.homelinux.net>
 * @see BBase, BInt, BString, BList
 */
class BDict : public BBase
{
    public:

    /**
     * Construct a dictionary based on the b-encoded data contained in
     * @p dict.  You need to pass a value for @p start if the b-encoded
     * dictionary doesn't start at the beginning of @p dict.
     *
     * @param dict the buffer containing the b-encoded dictionary
     * @param start the position of the data within the buffer
     */
    BDict (QByteArray &dict, int start = 0);

    /**
     * Construct a dictionary from a ByteTape.  The data and current
     * position of @p tape will be shared with all objects using it.
     * @p tape should already be positioned to the b-encoded dictionary.
     * After construction, @p tape will point to the byte after the
     * dictionary if successful.  If not successful, @p tape will have
     * an undefined position.
     *
     * @param tape the ByteTape to read from
     * @see ByteTape
     */
    BDict (ByteTape &tape);
    
    /**
     * Destroys the object and frees all memory allocated to it.
     */
    virtual ~BDict();
    
    /**
     * Returns the type of this class.
     *
     * @return bDict.  This value is only returned by this class.
     */
    virtual classID type_id() const { return bDict; }
    
    /**
     * Returns the number of keyed values contained within this
     * dictionary.
     *
     * @return the number of items in this dictionary
     */
    virtual int count() const { return m_map.count(); }

    /**
     * This function should be called to determine whether the
     * dictionary was successfully created, since no exceptions
     * are thrown.
     *
     * @return true if this is a valid (possibly empty!) dictionary,
     *         false otherwise
     */
    virtual bool isValid() const { return m_valid; }

    /**
     * This function determines whether or not a value with a
     * certain key exists in the dictionary.  The key is case-sensitive.
     *
     * @param key the key to search for a value for
     * @return true, if there is a value for the @p key in the
     *         dictionary, false otherwise
     */
    virtual bool contains (const char *key) { return m_map.find(key) != 0; }

    /**
     * Returns a pointer to the BBase descendant keyed by @p key.  You
     * can use the type_id() method to determine the type of the
     * object returned.
     *
     * @param key the key to search the dictionary for
     * @return a pointer to the matching object, or 0 if no object
     *         matches the key
     * @see BBase
     */
    virtual BBase *find (const char *key) { return m_map.find(key); }

    /**
     * Convienience function to find and return a BInt keyed by @p key.
     *
     * @param key the key to find a value for
     * @return 0 if the key doesn't match a value, or if the value isn't
     *         a BInt.  Otherwise, a pointer to the matching BInt is
     *         returned.
     * @see BInt
     */
    BInt* findInt (const char *key);
    
    /**
     * Convienience function to find and return a BList keyed by @p key.
     *
     * @param key the key to find a value for
     * @return 0 if the key doesn't match a value, or if the value isn't
     *         a BList.  Otherwise, a pointer to the matching BList is
     *         returned.
     * @see BList
     */
    BList* findList (const char *key);
    
    /**
     * Convienience function to find and return a BDict keyed by @p key.
     *
     * @param key the key to find a value for
     * @return 0 if the key doesn't match a value, or if the value isn't
     *         a BDict.  Otherwise, a pointer to the matching BDict is
     *         returned.
     * @see BDict
     */
    BDict* findDict (const char *key);
    
    /**
     * Convienience function to find and return a BString keyed by @p key.
     *
     * @param key the key to find a value for
     * @return 0 if the key doesn't match a value, or if the value isn't
     *         a BString.  Otherwise, a pointer to the matching BString is
     *         returned.
     * @see BString
     */
    BString* findStr (const char *key);
    
    /**
     * Outputs the b-encoded representation of the object to the given
     * QIODevice.
     * @param device the QIODevice to write to
     * @return true on a successful write, false otherwise
     */
    virtual bool writeToDevice (QIODevice &device);

    /**
     * Returns a QDictIterator<BBase> that you can use to iterate through
     * the items in the dictionary.
     *
     * @return QDictIterator<BBase>, which can be used to iterate through
     * the items in the dictionary.
     */
    BBaseHashIterator iterator() const
    {
        return BBaseHashIterator(m_map);
    }

    private:

    /**
     * This function handles the actual initialization of the object upon
     * construction, and set the m_valid flag if successful.
     *
     * @param tape the ByteTape to read from
     */
    void init (ByteTape &tape);

    BBaseHash m_map; /// The QDict that actually store the data
    bool m_valid;    /// Store initialization status
};

#endif /* _BDICT_H */

// vim: set et sw=4 ts=4:
