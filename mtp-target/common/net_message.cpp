/* Copyright, 2003 Melting Pot
 *
 * This file is part of MTP Target.
 * MTP Target is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * MTP Target is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with MTP Target; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */


//
// Includes
//

#include "stdpch.h"

#include "net_message.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;


//
// Functions
//

CNetMessage::CNetMessage(TType type, bool inputStream) : CMemStream(inputStream)
{
	setHeader(type);
}

void CNetMessage::setHeader(TType type)
{
	uint16 size = 0;
	serial(size);

	nlassert(type < 256);
	Type = type;
	uint8 t = (uint8)Type;
	serial(t);
}


bool CNetMessage::send(NLNET::CTcpSock *sock)
{
	nlassert(sock);

	// put the size in the begin of the message, size is without the header size
	uint32 len = length();
	nlassert(len-3<(1<<16));
	uint16 size = (uint16)len-3;
	poke(size, 0);

//	nlinfo("real send %"NL_I64"d", CTime::getLocalTime());
//	nlinfo("Send a message type %hu size %hu", (uint16)buffer()[1], (uint16)buffer()[0]);

	NLNET::CSock::TSockResult res = sock->send(buffer(), len, false);

	if(res != NLNET::CSock::Ok)
	{
		nlwarning("Send failed: %s (code %u)", NLNET::CSock::errorString(NLNET::CSock::getLastError()).c_str(), NLNET::CSock::getLastError());
		//sock->close();
		//sock->disconnect();
		return false;
	}
	return true;
}

NLNET::CSock::TSockResult CNetMessage::receive(NLNET::CTcpSock *sock)
{
	nlassert (sock);

	uint16 buffer[1];
	uint32 len = 2;

	// receive the size

	NLNET::CSock::TSockResult res = sock->receive((uint8*)buffer, len, false);

	if(res != NLNET::CSock::Ok)
		return res;

//	nldebug("received size %hu", (uint16)buffer[0]);

	// receive the message

	len = buffer[0] + 1;

	uint8 *buf = bufferToFill(len);

	res = sock->receive(buf, len, false);

	if(res != NLNET::CSock::Ok)
		return res;

	uint8 t = 0;
	serial(t);
	Type = (TType)t;

//	nlinfo("real send %"NL_I64"d", CTime::getLocalTime());
//	nldebug("received message %hu", (uint16)Type);

	return NLNET::CSock::Ok;
}
