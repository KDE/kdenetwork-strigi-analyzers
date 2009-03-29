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

#include "bytestream.h"

#include <kdemacros.h>

#include <stdexcept>

end_of_stream::end_of_stream() : std::runtime_error("reached eos")
{
}

ByteStream::ByteStream(Strigi::InputStream *in)
  : m_input(in), m_bufSize(0), m_buffer(0), m_curPos(0), m_atEnd(true)
{
}

char ByteStream::operator*() const
{
    if(KDE_ISUNLIKELY(m_atEnd)) {
        throw end_of_stream();
    }

    if(KDE_ISUNLIKELY(0 == m_buffer)) {
        throw std::logic_error("operator*() called before operator++()");
    }

    return *m_curPos;
}

void ByteStream::operator++()
{
    m_curPos++;

    // If not setup or we're past end of buffer, refill
    if(KDE_ISUNLIKELY(0 == m_buffer || (m_curPos - m_buffer) >= m_bufSize)) {
        refillBuffer();
    }
}

void ByteStream::refillBuffer()
{
    // This roundabout pointer manipulation works around what I think
    // is a g++ 4.3 bug searching through template functions.
    const char *ptr;
    m_bufSize = m_input->read(ptr, 4096, 0);
    m_buffer = const_cast<char *>(ptr);

    if(m_bufSize < -1)
        throw std::runtime_error("Failed to refill buffer");

    if(m_bufSize == -1) {
        m_atEnd = true;
        m_bufSize = 0;
        m_buffer = 0;
        m_curPos = 0;

        return;
    }

    m_atEnd = false;
    m_curPos = m_buffer;
}

// vim: set et sw=4 ts=4:
