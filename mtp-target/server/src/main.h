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

#ifndef MTPT_MAIN
#define MTPT_MAIN


//
// Includes
//

#include <nel/net/callback_server.h>
#include <nel/net/udp_sock.h>


//
// Variables
//

extern uint32 UpdatePacketSize;

extern bool ForceFinish;

extern NLMISC::TTime LastPingSent;	// When we send all information of client (ping)

extern const float GSCALE;


#endif
