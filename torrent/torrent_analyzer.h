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
#ifndef TORRENT_ANALYZER_H
#define TORRENT_ANALYZER_H

#include <strigi/streamthroughanalyzer.h>

class TorrentThroughAnalyzerFactory;

class TorrentThroughAnalyzer : public Strigi::StreamThroughAnalyzer
{
public:
    TorrentThroughAnalyzer(const TorrentThroughAnalyzerFactory *f);

    virtual const char *name() const { return "TorrentThroughAnalyzer"; }
    virtual bool isReadyWithStream() { return true; }

    virtual Strigi::InputStream *connectInputStream(Strigi::InputStream *input);

    virtual void setIndexable(Strigi::AnalysisResult *result) {
        m_analysisResult = result;
    }

private:
    const TorrentThroughAnalyzerFactory *m_factory;
    Strigi::AnalysisResult *m_analysisResult;
};

#endif
