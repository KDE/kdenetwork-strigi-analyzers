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
#ifndef TORRENT_ANALYZER_DICT_H
#define TORRENT_ANALYZER_DICT_H

#include "bbase.h"

#include <QtCore/QHash>

class ByteStream;

// Some useful typedefs
typedef QHash<QByteArray, BBase::Ptr> BDictionary;
typedef QHashIterator<QByteArray, BBase::Ptr> BDictionaryIterator;

/**
 * Class to handle the BitTorrent b-encoded dictionary.  It is keyed
 * using QByteArray, and stores shared pointers to a class descended
 * from BBase, such as BInt, BString, BList, or even more BDicts.
 *
 * @author Michael Pyne <michael.pyne@kdemail.net>
 * @see BBase, BInt, BString, BList
 */
class BDict : public BBase
{
    public:

    typedef boost::shared_ptr<BDict> Ptr;

    /**
     * Construct a dictionary from a ByteStream.  The data and current
     * position of @p stream will be shared with all objects using it.
     * @p stream should already be positioned to the b-encoded dictionary.
     * After construction, @p stream will point to the byte after the
     * dictionary if successful.  If not successful, an exception will
     * be thrown (std::runtime_error).
     *
     * @param stream the ByteStream to read from
     * @see ByteStream
     */
    BDict (ByteStream &stream);

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
    virtual int count() const;

    /**
     * This function determines whether or not a value with a
     * certain key exists in the dictionary.  The key is case-sensitive.
     *
     * @param key the key to search for a value for
     * @return true, if there is a value for the @p key in the
     *         dictionary, false otherwise
     */
    virtual bool contains (const QByteArray &key);

    /**
     * Returns a pointer to the BBase descendant keyed by @p key.  You
     * can use the type_id() method to determine the type of the
     * object returned.
     *
     * @param key the key to search the dictionary for
     * @return null pointer if the value was not found, otherwise a pointer
     *         to the result.
     * @see BBase
     */
    virtual BBase::Ptr find (const QByteArray &key) const;

    /**
     * Convenience function to find and return a pointer to a value of
     * a specific type.
     *
     * @return null pointer if the key doesn't match a value, or if the value
     *         isn't the given type.  Otherwise, a pointer to the matching BInt is
     *         returned.
     */
    template<class T>
    boost::shared_ptr<T> findType (const QByteArray &key) const
    {
        return boost::dynamic_pointer_cast<T>(m_dict.value(key));
    }

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
    BDictionaryIterator iterator() const;

    private:

    BDictionary m_dict; /// The QHash that actually store the data
};

#endif /* TORRENT_ANALYZER_DICT_H */

// vim: set et sw=4 ts=4:
