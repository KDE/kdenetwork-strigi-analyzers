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
#ifndef _KFILE_TORRENT_H
#define _KFILE_TORRENT_H

#include <kfilemetainfo.h>

#include "bdict.h"

class QStringList;

/**
 * Class to provide meta info for BitTorrent files within Konqueror.
 * Handles the mime type application/x-bittorrent, files typically
 * have the extension .torrent
 *
 * @author Michael Pyne <mpyne@grammarian.homelinux.net>
 * @todo Handle editing the meta info as well
 * @bug The comment meta info should be Multi Line, but that only
 *      works when the comment can also be edited.
 */
class KTorrentPlugin : public KFilePlugin
{
    Q_OBJECT

    public:
    /**
     * Constructs the class, and prepares for reading info on a torrent.
     *
     * @param parent the parent of this object
     * @param name the name of this object (not user-readable)
     * @param args unused by this class
     */
    KTorrentPlugin (QObject *parent, const char *name, const QStringList &args);

    /**
     * Destructor that closes the dictionary holding the torrent information.
     */
    ~KTorrentPlugin () { delete m_dict; }

    /**
     * Reads information on a torrent file given by @p info.
     *
     * @param info information on the file to decode
     * @return true if the meta info was successfully detected and added,
     *         false otherwise.
     */
    virtual bool readInfo (KFileMetaInfo& info, unsigned int);

    /**
     * Writes information on a torrent file given by @p info.
     * BitTorrent are practically nothing but meta information.
     * Therefore, the entire file might be changed.
     *
     * @param info information on the file to encode
     * @return true if the meta info was successfully updated,
     *         false otherwise
     */
    virtual bool writeInfo (const KFileMetaInfo& info) const;

    private:
    bool m_failed;
    BDict *m_dict;
};

#endif /* _KFILE_TORRENT_H */

// vim: set et ts=4 sw=4:
