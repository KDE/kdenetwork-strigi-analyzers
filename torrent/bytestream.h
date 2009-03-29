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
#ifndef TORRENT_ANALYZER_BYTESTREAM_H
#define TORRENT_ANALYZER_BYTESTREAM_H

#include <QtGlobal>

#include <strigi/streambase.h>

#include <stdexcept>

class end_of_stream : public std::runtime_error
{
public:
    end_of_stream();
};

/**
 * A very simple class to read characters one by one from a
 * Strigi::InputStream, for use in decoding b-encoded data.
 */
class ByteStream
{
public:
    ByteStream(Strigi::InputStream *in);

    /**
     * Reads the current character.  If you have not already
     * called operator ++ at least once then there is not data,
     * and an exception will be thrown instead.
     */
    char operator*() const;

    /**
     * Prefix increment operator, advances to the next character.
     */
    void operator++();

    bool atEnd() const { return m_atEnd; }

private:
    void refillBuffer();

    Strigi::InputStream *m_input;
    qint32 m_bufSize;
    char *m_buffer, *m_curPos;
    bool m_atEnd;
};

#endif

// vim: set et sw=4 ts=4:
