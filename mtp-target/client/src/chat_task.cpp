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

#include "3d_task.h"
#include "time_task.h"
#include "game_task.h"
#include "chat_task.h"
#include "intro_task.h"
#include "network_task.h"
#include "font_manager.h"
#include "task_manager.h"
#include "background_task.h"
#include "resource_manager.h"


//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;


//
// Variables
//

#define CHAT_LINES 3
#define CHAT_COLUMNS 160

static std::list<std::string> ChatText;
static std::string ChatInput = "";
static std::list<std::string>::reverse_iterator CurrentChatLine = ChatText.rbegin();


//
// Functions
//
	
void CChatTask::init()
{
}

void CChatTask::update()
{
	// Get user input
	string res = C3DTask::instance().kbGetString();
	for (const char *src = res.c_str(); *src != '\0';src++)
	{
		if (*src == 27)
			continue; // Escape
		
		if (*src == 8)
		{ // Backspace
			if (ChatInput.length() > 0)
			{
				ChatInput.pop_back();
			}
			continue;
		}
		
		if (*src == '\r')
		{
			if (ChatInput.length() > 0)
			{
				if (ChatInput[0] == '/')
					CNetworkTask::instance().command(ChatInput.substr(1));
				else
					CNetworkTask::instance().chat(ChatInput);
			}
			ChatInput = "";
			continue;
		}
		
		if (ChatInput.size() >= CHAT_COLUMNS - 10)
			continue;
		
		if (isprint(*src))
		{
			ChatInput += *src;
		}
	}
	if (C3DTask::instance().kbPressed(KeyPRIOR))
	{
		std::list<std::string>::reverse_iterator it = ChatText.rend();
		sint32 i;
		// can't go back up too much
		for (i = 0; i < CHAT_LINES && it != ChatText.rbegin(); i++, --it)
		{
			// nothing
		}
		// ok, go back up
		for (i = 0; i < CHAT_LINES && CurrentChatLine != it; i++, ++CurrentChatLine)
		{
			// nothing
		}
	}
	if (C3DTask::instance().kbPressed(KeyNEXT))
	{
		// go back down
		for (sint32 i = 0; i < CHAT_LINES && CurrentChatLine != ChatText.rbegin(); i++, --CurrentChatLine)
		{
		}
	}
}

void CChatTask::render()
{
	uint cl = CHAT_LINES;
//ace todo	if(large) cl *= 4;
	
	C3DTask::instance().driver().setFrustum(CFrustum(0, (float)C3DTask::instance().screenWidth(), 0, (float)C3DTask::instance().screenHeight(), -1, 1, false));
	C3DTask::instance().driver().clearZBuffer();
	
	// black background
	C3DTask::instance().driver().drawQuad(0.0f, (float)C3DTask::instance().screenHeight() - 16*(cl+1)-8, C3DTask::instance().screenWidth(), C3DTask::instance().screenHeight(), CRGBA (0, 0, 0, 64));
	
	// text
	std::list<std::string>::reverse_iterator it = CurrentChatLine;
	for (sint32 i = cl - 1; i >= 0 && it != ChatText.rend(); i--, ++it)
	{
		CFontManager::instance().littlePrintf(0.0f, (float)i, "%s", (*it).c_str());
	}
	CFontManager::instance().littlePrintf(0.0f, (float)cl, "> %s", ChatInput.c_str());
}

void CChatTask::release()
{
}

void CChatTask::addLine(const std::string &text)
{
	if (CurrentChatLine == ChatText.rbegin())
	{
		ChatText.push_back(text);
		CurrentChatLine = ChatText.rbegin();
	}
	else
	{
		ChatText.push_back(text);
	}
}
