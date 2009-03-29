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

#ifndef TORRENT_ANALYZER_FACTORY
#define TORRENT_ANALYZER_FACTORY

#include <strigi/streamthroughanalyzer.h>
#include <strigi/fieldtypes.h>

#include <string>

class TorrentThroughAnalyzerFactory : public Strigi::StreamThroughAnalyzerFactory
{
    friend class TorrentThroughAnalyzer;

    static const std::string announceFieldName;
    static const std::string creationDateFieldName;
    static const std::string lengthFieldName;
    static const std::string numFilesFieldName;
    static const std::string nameFieldName;
    static const std::string pieceLengthFieldName;
    static const std::string commentFieldName;

    const Strigi::RegisteredField *announce;
    const Strigi::RegisteredField *creationDate;
    const Strigi::RegisteredField *length;
    const Strigi::RegisteredField *numFiles;
    const Strigi::RegisteredField *nameField;
    const Strigi::RegisteredField *pieceLength;
    const Strigi::RegisteredField *comment;

    const char *name() const {
        return "TorrentThroughAnalyzer";
    }

    Strigi::StreamThroughAnalyzer *newInstance() const;
    void registerFields(Strigi::FieldRegister &fields);
};

#endif
