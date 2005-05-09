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

#include <kdebug.h>

#include "bytetape.h"

ByteTape::ByteTape (QByteArray &array, int pos)
    : m_array(array), m_shared(new ByteTapeShared)
{
    m_shared->pos = pos;
}

ByteTape::ByteTape (const ByteTape &tape)
    : m_array(tape.m_array), m_shared(tape.m_shared)
{
}

ByteTape& ByteTape::operator += (const unsigned int i)
{
    m_shared->pos += i;
    if (m_array.size() <= m_shared->pos)
    {
        kdDebug(7034) << "Whoops.  Advanced too far." << endl;
        m_shared->pos = m_array.size() - 1;
    }
    
    return *this;
}

ByteTape& ByteTape::operator -= (const unsigned int i)
{
    if (i > m_shared->pos)
    {
        kdDebug(7034) << "Whoops, tried to back up too far." << endl;
        m_shared->pos = 0;
    }
    else
        m_shared->pos -= i;
    
    return *this;
}

char ByteTape::operator [] (const unsigned int i)
{
    if (i < m_array.size())
        return m_array[i];
    else
    {
        kdWarning() << "Can't dereference tape at " << i
                    << ", size is " << m_array.size() << endl;
        return 0;
    }
}

char &ByteTape::operator * ()
{
    return m_array[m_shared->pos];
}

// Postfix increment
ByteTape ByteTape::operator ++ (int)
{
    // Can't use copy ctor, as we'll be copying shared data, which defeats
    // the semantics of the postfix operator.
    ByteTape temp(m_array, m_shared->pos);
    m_shared->pos ++;

    if (m_shared->pos >= m_array.size())
    {
        m_shared->pos = m_array.size() - 1;
        kdDebug(7034) << "Tape already at end!" << endl;
        kdDebug(7034) << "Tape size is " << m_array.size() << endl;
    }
    
    return temp;
}

// Prefix increment
ByteTape & ByteTape::operator ++()
{
    m_shared->pos ++;
    if (m_shared->pos >= m_array.size())
    {
        m_shared->pos = m_array.size() - 1;
        kdDebug(7034) << "Tape already at end!" << endl;
        kdDebug(7034) << "Tape size is " << m_array.size() << endl;
    }
    
    return *this;
}

// Postfix decrement
ByteTape ByteTape::operator -- (int)
{
    // Can't use copy ctor, as we'll be copying shared data, which defeats
    // the semantics of the postfix operator.
    ByteTape temp(m_array, m_shared->pos);

    if (m_shared->pos != 0)
        m_shared->pos --;
    else
        kdDebug(7034) << "Tape already at beginning!" << endl;
    
    return temp;
}

// Prefix decrement
ByteTape & ByteTape::operator -- ()
{
    if (m_shared->pos != 0)
        m_shared->pos --;
    else
        kdDebug(7034) << "Tape already at beginning!" << endl;
    
    return *this;
}

bool ByteTape::setPos (unsigned int pos)
{
    if (pos >= m_array.size())
    {
        kdDebug(7034) << "Can't set tape to " << pos << endl;
        return false;
    }

    m_shared->pos = pos;
    return true;
}

char* ByteTape::at (const unsigned int i)
{
    if (i >= m_array.size())
    {
        kdDebug(7034) << "Access to tape at " << i << " out-of-range." << endl;
        return 0;
    }
        
    return m_array.data() + i;
}

// vim: set et ts=4 sw=4:
