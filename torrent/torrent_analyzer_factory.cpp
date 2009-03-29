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
