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
	: m_array(array), m_pos(0), m_refcount (0)
{
	// This is a constructor for a QByteArray, so allocate new data.
	
	m_refcount = new uint (1); // If this fails, the user has other problems
	m_pos = new uint (pos);
}

ByteTape::ByteTape (const ByteTape &tape)
	: m_array(tape.m_array), m_pos(tape.m_pos), m_refcount(tape.m_refcount)
{
	// We use the same pointer for the reference count.  But we need
	// to increase it as well.  This poses multi-threading issues.  This
	// class is NOT thread safe as it stands.
	
	(*m_refcount)++;
}

ByteTape::~ByteTape()
{
	(*m_refcount)--;

	if (*m_refcount == 0)
	{
		delete m_pos;
		delete m_refcount;
	}
}

ByteTape& ByteTape::operator += (const int i)
{
	kdDebug() << "tape+= " << i << endl;
	
	kdDebug() << "m_pos = " << (*m_pos) << endl;
	(*m_pos) += i;
	kdDebug() << "m_pos = " << (*m_pos) << endl;
	if (m_array.size() <= (*m_pos))
	{
		kdDebug() << "Whoops.  Moved too far.\n";
		(*m_pos) = m_array.size() - 1;
	}
	
	return *this;
}

ByteTape& ByteTape::operator -= (const int i)
{
	if ((uint) i > *m_pos)
		*m_pos = 0;
	else
		(*m_pos) -= i;
	
	return *this;
}

char ByteTape::operator [] (const uint i)
{
	if (i < m_array.size())
		return m_array[i];
	else
	{
		kdDebug() << "Can't dereference tape at " << i << endl;
		return 0;
	}
}

char &ByteTape::operator * ()
{
	return m_array[*m_pos];
}

ByteTape ByteTape::operator ++ (int)
{
	// Postfix increment
	ByteTape temp(*this);
	(*m_pos) ++;
	if ((*m_pos) >= m_array.size())
	{
		(*m_pos) = m_array.size() - 1;
		kdDebug() << "Tape already at end!\n";
		kdDebug() << "Tape size is " << m_array.size() << endl;
	}
	
	return temp;
}

ByteTape & ByteTape::operator ++()
{
	(*m_pos) ++;
	if ((*m_pos) >= m_array.size())
	{
		(*m_pos) = m_array.size() - 1;
		kdDebug() << "Tape already at end!\n";
	}
	
	return *this;
}

ByteTape ByteTape::operator -- (int)
{
	ByteTape temp(*this);

	if (*m_pos != 0)
		(*m_pos) --;
	else
		kdDebug() << "Tape already at beginning!\n";
	
	return temp;
}

ByteTape & ByteTape::operator -- ()
{
	if (*m_pos != 0)
		(*m_pos) --;
	else
		kdDebug() << "Tape already at beginning!\n";
	
	return *this;
}

bool ByteTape::setPos (uint pos)
{
	if (pos >= m_array.size())
	{
		kdDebug() << "Can't set tape to " << pos << "\n";
		return false;
	}

	(*m_pos) = pos;
	return true;
}

char* ByteTape::at (const uint i)
{
	if (i >= m_array.size())
	{
		kdDebug() << "Access to tape at " << i << " out-of-range.\n";
		return 0;
	}
		
	return m_array.data() + i;
}
