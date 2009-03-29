/*
 * Copyright © 2009 Michael Pyne <michael.pyne@kdemail.net>
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
#include "torrent_analyzer.h"
#include "torrent_analyzer_factory.h"
#include "bdict.h"
#include "bytestream.h"
#include "blist.h"
#include "bstring.h"
#include "bint.h"

#include <strigi/analyzerplugin.h>
#include <strigi/streamthroughanalyzer.h>
#include <strigi/analysisresult.h>
#include <strigi/streambase.h>

#include <stdexcept>
#include <string>

class TorrentFactory : public Strigi::AnalyzerFactoryFactory {
public:
    virtual std::list<Strigi::StreamThroughAnalyzerFactory*>
    streamThroughAnalyzerFactories() const {
        std::list<Strigi::StreamThroughAnalyzerFactory*> factories;
        factories.push_back(new TorrentThroughAnalyzerFactory);
        return factories;
    }
};

STRIGI_ANALYZER_FACTORY(TorrentFactory)

static qulonglong filesLength(const BList::Ptr &list)
{
    qulonglong length = 0;

    for(unsigned int i = 0; i < list->count(); ++i) {
        BDict::Ptr fileDict = list->indexType<BDict>(i);
        if(!fileDict)
            return 0;

        BInt::Ptr fileLength = fileDict->findType<BInt>("length");
        if(!fileLength)
            return 0;

        length += fileLength->get_value();
    }

    return length;
}

TorrentThroughAnalyzer::TorrentThroughAnalyzer(const TorrentThroughAnalyzerFactory *f)
  : m_factory(f)
{
}

Strigi::InputStream *TorrentThroughAnalyzer::connectInputStream(Strigi::InputStream *input)
{
    ByteStream stream(input);
    ++stream; // Read first character

    try {
        BDict::Ptr dict(new BDict(stream));
        input->reset(0); // Reposition to beginning

        BString::Ptr announce = dict->findType<BString>("announce");
        if(announce)
            m_analysisResult->addValue(m_factory->announce, announce->raw_data().constData());

        BInt::Ptr creationDate = dict->findType<BInt>("creation date");
        if(creationDate)
            m_analysisResult->addValue(m_factory->creationDate, (uint32_t) creationDate->get_value());

        BDict::Ptr info = dict->findType<BDict>("info");
        if(!info)
            return input;

        int numFiles = 1;
        qulonglong length = 0;

        if(info->contains("length")) {
            // Only 1 file
            BInt::Ptr bLength = info->findType<BInt>("length");
            if (!bLength)
                return input;
            length = bLength->get_value();
        }
        else {
            BList::Ptr fileList = info->findType<BList>("files");
            if(!fileList)
                return input;

            numFiles = fileList->count();
            length = filesLength(fileList);
        }

        m_analysisResult->addValue(m_factory->length, (uint32_t)length);
        m_analysisResult->addValue(m_factory->numFiles, (uint32_t)numFiles);

        BString::Ptr name = info->findType<BString>("name");
        if(name)
            m_analysisResult->addValue(m_factory->nameField, name->raw_data().constData());

        BInt::Ptr pieceLength = info->findType<BInt>("piece length");
        if(pieceLength)
            m_analysisResult->addValue(m_factory->pieceLength, (uint32_t)pieceLength->get_value());

        BString::Ptr comment = info->findType<BString>("comment");
        if(comment)
            m_analysisResult->addValue(m_factory->comment, comment->raw_data().constData());
    }
    // Don't allow exceptions to propagate out
    catch(...) {
        input->reset(0);
        return input;
    }

    input->reset(0);
    return input;
}
