/** \file connection_ws.h
 * <File description>
 *
 * $Id$
 */

/* Copyright, 2001 Nevrax Ltd.
 *
 * This file is part of NEVRAX NeL Network Services.
 * NEVRAX NeL Network Services is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * NEVRAX NeL Network Services is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with NEVRAX NeL Network Services; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#ifndef NL_CONNECTION_WS_H
#define NL_CONNECTION_WS_H

#include "nel/misc/types_nl.h"
#include "nel/net/callback_server.h"

//extern NLNET::CCallbackServer *WSServer;

void connectionWSInit ();
void connectionWSUpdate ();
void connectionWSRelease ();

#endif // NL_CONNECTION_WS_H

/* End of connection_ws.h */
