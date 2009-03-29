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
#include "torrent_analyzer_factory.h"
#include "torrent_analyzer.h"

#include <strigi/analysisresult.h>

const std::string TorrentThroughAnalyzerFactory::announceFieldName
("http://freedesktop.org/standards/xesam/1.0/core#RemoteResource");
const std::string TorrentThroughAnalyzerFactory::creationDateFieldName
("http://freedesktop.org/standards/xesam/1.0/core#contentCreated");
const std::string TorrentThroughAnalyzerFactory::lengthFieldName
(Strigi::FieldRegister::sizeFieldName);
const std::string TorrentThroughAnalyzerFactory::numFilesFieldName
(Strigi::FieldRegister::integerType);
const std::string TorrentThroughAnalyzerFactory::nameFieldName
("http://freedesktop.org/standards/xesam/1.0/core#name");
const std::string TorrentThroughAnalyzerFactory::pieceLengthFieldName
(Strigi::FieldRegister::sizeFieldName);
const std::string TorrentThroughAnalyzerFactory::commentFieldName
("http://freedesktop.org/standards/xesam/1.0/core#comment");

void TorrentThroughAnalyzerFactory::registerFields(Strigi::FieldRegister &fields)
{
    announce     = fields.registerField(announceFieldName);
    creationDate = fields.registerField(creationDateFieldName);
    length       = fields.registerField(lengthFieldName);
    numFiles     = fields.registerField(numFilesFieldName);
    nameField    = fields.registerField(nameFieldName);
    pieceLength  = fields.registerField(pieceLengthFieldName);
    comment      = fields.registerField(commentFieldName);
}

Strigi::StreamThroughAnalyzer *TorrentThroughAnalyzerFactory::newInstance() const
{
    return new TorrentThroughAnalyzer(this);
}
