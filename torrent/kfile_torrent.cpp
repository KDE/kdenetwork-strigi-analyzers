/*
 * Copyright (c) 2003, 2004 2004 Michael Pyne <michael.pyne@kdemail.net>
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
#include <qdatetime.h>
#include <qfile.h>
#include <qregexp.h>
#include <kmessagebox.h>
#include <kgenericfactory.h>
#include <qstringlist.h>

#include "kfile_torrent.h"
#include "bdict.h"
#include "blist.h"
#include "bint.h"
#include "bstring.h"

typedef KGenericFactory<KTorrentPlugin> TorrentFactory;
K_EXPORT_COMPONENT_FACTORY(kfile_torrent, TorrentFactory("kfile_torrent"))

QStringList filesList (BList *list);
uint filesLength (BList *list);

KTorrentPlugin::KTorrentPlugin (QObject *parent, const char *name,
                                const QStringList &args)
    : KFilePlugin (parent, name, args), m_failed(true), m_dict(0)
{
    KFileMimeTypeInfo *info = addMimeTypeInfo ("application/x-bittorrent");
    if (!info)
    {
        kdError() << "Error creating application/x-bittorrent mime type info!\n";
        return;
    }

    KFileMimeTypeInfo::GroupInfo* group =
        addGroupInfo (info, "TorrentInfo", i18n("Torrent Information"));
    if (!group)
    {
        kdError() << "Error creating TorrentInfo group!\n";
        return;
    }
    setAttributes (group, KFileMimeTypeInfo::Modifiable);
    
    KFileMimeTypeInfo::ItemInfo *item = 0;
    
    item = addItemInfo(group, "name", i18n("Name"), QVariant::String);
    if (!item)
    {
        kdError() << "Error adding Name to group!\n";
        return;
    }
    setHint (item, KFileMimeTypeInfo::Name);
    setAttributes (item, KFileMimeTypeInfo::Modifiable);
    
    item = addItemInfo(group, "length", i18n("Torrent Length"), QVariant::Int);
    if (!item)
    {
        kdError() << "Error adding Length to group!\n";
        return;
    }
    setHint (item, KFileMimeTypeInfo::Length);
    setUnit (item, KFileMimeTypeInfo::Bytes);
    
    item = addItemInfo(group, "announce", i18n("Tracker URL"), QVariant::String);
    if (!item)
    {
        kdError() << "Error adding Announce to group!\n";
        return;
    }
    
    item = addItemInfo(group, "creation date", i18n("Date Created"), QVariant::DateTime);
    if (!item)
    {
        kdError() << "Error adding DateCreated to group!\n";
        return;
    }
    
    item = addItemInfo(group, "NumFiles", i18n("Number of Files"), QVariant::Int);
    if (!item)
    {
        kdError() << "Error adding NumFiles to group!\n";
        return;
    }
    
    item = addItemInfo(group, "piece length", i18n("File Piece Length"), QVariant::Int);
    if (!item)
    {
        kdError() << "Error adding PieceLength to group!\n";
        return;
    }
    setUnit (item, KFileMimeTypeInfo::Bytes);

    item = addItemInfo(group, "comment", i18n("Comment"), QVariant::String);
    if (!item)
    {
        kdError() << "Error adding Comment to group!\n";
        return;
    }
    setAttributes (item, KFileMimeTypeInfo::MultiLine | KFileMimeTypeInfo::Modifiable);

    m_failed = false;
}

bool KTorrentPlugin::readInfo (KFileMetaInfo &info, uint)
{
    /* Since we don't throw during the ctor, check here whether we actually
     * are valid are not.  If not, die.
     */
    if (m_failed)
    {
        kdError() << "Construction of KTorrentPlugin failed for " << info.path() << endl;
        kdError() << "Aborting meta-info read.\n";
        return false;
    }
    
    QFile file (info.path());
    if (!file.open(IO_ReadOnly))
    {
        kdError() << "Unable to open given file!\n";
        return false;
    }

    // We need to read in the entire file to parse the dictionary structure.
    QByteArray buf = file.readAll();
    file.close();

    m_dict = new BDict(buf);
    
    if (!m_dict)
    {
        kdError() << "Error creating dictionary from open file: " << info.path() << endl;
        return false;
    }

    if (!m_dict->isValid())
    {
        kdDebug() << "Invalid torrent file: " << info.path() << endl;
        return false;
    }
    
    KFileMetaInfoGroup group = appendGroup(info, "TorrentInfo");

    // The remainder of this function will consist of a lot of redundancy checks.
    // If a torrent has a key, but it is of the wrong type, then it isn't a valid
    // torrent, and so we should just die.
    
    if (m_dict->contains("announce"))
    {
        BString *str = m_dict->findStr ("announce");
        if (!str)
            return false;
        appendItem (group, "announce", QString(str->get_string()));
    }
    
    if (m_dict->contains("creation date"))
    {
        BInt *the_data = m_dict->findInt ("creation date");
        QDateTime my_date;
        
        if (!the_data)
            return false;

        uint the_time = the_data->get_value();
        
        /* Hopefully the_time is UTC, because that's what QDateTime does. */
        my_date.setTime_t (the_time);
        appendItem (group, "creation date", my_date);
    }

    if (m_dict->contains ("info"))
    {
        BDict *info_dict = m_dict->findDict("info");
        int num_files = 1, length = 0;
        
        if (!info_dict)
            return false;
            
        if (!info_dict->contains("length"))
        {
            /* Has more than one file.  The list of files is contained in a
             * list called, appropriately enough, 'files'
             */
            BList *info_list = info_dict->findList("files");
            if (!info_list)
                return false;
            
            num_files = info_list->count();
            length = filesLength (info_list);
        }
        else
        {
            /* Only one file, let's put its length */
            BInt *blength = info_dict->findInt("length");

            if (!blength)
                return false;
                
            length = blength->get_value();
        }

        appendItem (group, "NumFiles", num_files);
        appendItem (group, "length", length);

        if (info_dict->contains("name"))
        {
            BString *str = info_dict->findStr("name");
            if (!str)
                return false;
                
            QString real_str (str->get_string());

            if (num_files > 1 && !real_str.endsWith("/"))
                real_str.append('/');

            appendItem (group, "name", real_str);
        }

        if (info_dict->contains("piece length"))
        {
            BInt *piece_length = info_dict->findInt("piece length");
            if (!piece_length)
                return false;
                
            appendItem (group, "piece length", piece_length->get_value());
        }
    } // if m_dict (info)

    if (m_dict->contains("comment"))
    {
        BString *comment = m_dict->findStr("comment");
        if (!comment)
            return false;
            
        appendItem (group, "comment", comment->get_string());
    }

    return true;
}

/* Returns a QStringList containing file names within the list.  The list
 * should be the one contained within the info dictionary of the torrent,
 * keyed by 'files'
 */
QStringList filesList (BList *list)
{
    BBaseVectorIterator list_iter = list->begin();
    QStringList str_list;

    for ( ; list_iter != list->end(); ++list_iter)
    {
        /* Each item in this list is a dictionary, composed as follows:
         * length -> BInt (size of file)
         * path -> BList (list of strings)
         * The list of strings is used to construct directory paths.  The
         * last element of the list is the file name.
         */

        BDict *list_dict = dynamic_cast<BDict*>(*list_iter);
        BList *list_path = dynamic_cast<BList*>(list_dict->find("path"));
        QString str;

        BBaseVectorIterator str_iter = list_path->begin();
        BString *temp_str = dynamic_cast<BString*>(*str_iter);
        
        if (str_iter != list_path->end())
            str.append (temp_str->get_string());

        /* Construct QString consisting of path and file name */
        for (++str_iter; str_iter != list_path->end(); ++str_iter)
        {
            str.append ('/'); /* Add path separator */
            temp_str = dynamic_cast<BString*>(*str_iter);
            str.append (temp_str->get_string());
        }

        str_list += str;
    }

    return str_list;
}

/* This function determines the total length of a torrent stream.
 * The list provided should be the same one provided for filesList.
 */
uint filesLength (BList *list)
{
    BBaseVectorIterator list_iter = list->begin();
    int length = 0;

    for ( ; list_iter != list->end(); ++list_iter)
    {
        /* Each item in this list is a dictionary, composed as follows:
         * length -> BInt (size of file)
         * path -> BList (list of strings)
         */

        BDict *list_dict = dynamic_cast<BDict*>(*list_iter);
        BInt *bfile_len = dynamic_cast<BInt*>(list_dict->find("length"));

        length += bfile_len->get_value();
    }

    return length;
}

bool KTorrentPlugin::writeInfo(const KFileMetaInfo &info) const
{
    if (m_failed || !m_dict)
        return false;
    
    // The m_dict is ready, all we have to do is open a file, and
    // let 'er go.
    QStringList list = info.groups();
    QStringList::Iterator it = list.begin();
    
    for (; it != list.end(); ++it)
    {
        kdDebug() << "Group: " << *it << endl;
        
        QStringList list2 = info[*it].keys();
        QStringList::Iterator it2 = list2.begin();
        
        for (; it2 != list2.end(); ++it2)
        {
            kdDebug() << "\tKey: " << *it2 << endl;
            QString key = *it2;
            
            if (info[*it][key].isModified())
            {
                // Re-enter the entry in the dictionary.
                kdDebug() << "\t\tIs Modified." << endl;
                
                if (key == "comment")
                {
                    BString *b_str = m_dict->findStr("comment");
                    b_str->setValue (info[*it][key].value().toString());
                }
                else if (key == "name")
                {
                    BDict *info_dict = m_dict->findDict ("info");
                    BString *name_str = info_dict->findStr ("name");
                    QString the_name = info[*it][key].value().toString();
                    
                    // Remove trailing slashes
                    the_name.replace (QRegExp("/*$"), "");
                    
                    name_str->setValue (the_name);
                }
            }
        }
    }
        
    QFile output (info.path());
    
    if (!output.open(IO_WriteOnly | IO_Truncate))
        return false;
    
    return m_dict->writeToDevice(output);
}

#include "kfile_torrent.moc"

// vim: set ts=4 sw=4 et: