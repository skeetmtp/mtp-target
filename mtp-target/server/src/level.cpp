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

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
};

#include <nel/misc/file.h>
#include <nel/misc/path.h>
#include <nel/misc/sha1.h>

#include "level.h"
#include "module.h"
#include "entity_manager.h"
#include "net_callbacks.h"
#include "network.h"
#include "lua_engine.h"
#include "../../common/lua_nel.h"
#include "../../common/lua_utility.h"


//
// Namespaces
//

using namespace std;
using namespace NLNET;
using namespace NLMISC;


//
// Types
//

//
// Declarations
//

//
// Variables
//

uint8 CLevel::NextStartingPoint = 0;


//
// Functions
//

CLevel::CLevel(const string &filename)
{
	nlinfo("Trying to load level '%s'", filename.c_str());

	Valid = false;
	
	FileName = filename;
	_luaInit();

}


void CLevel::_luaInit()
{
	CLuaEngine::instance().init(FileName);
	lua_State *_luaSession = CLuaEngine::instance().session();
	if(_luaSession)
		Valid = true;
	else
		return;
	
	string ServerLua;
	luaGetGlobalVariable(_luaSession, ServerLua);
	luaLoad(CLuaEngine::instance().session(),ServerLua);

	luaGetGlobalVariable(_luaSession, Name);
	//	nlinfo("level name '%s'", Name.c_str());
	
	luaGetGlobalVector(_luaSession, Cameras);

/*
	nlinfo("%d Camera", Cameras.size());
	for(uint i = 0; i < Cameras.size(); i++)
	{
		nlinfo("  > camera %g %g %g", Cameras[i].x, Cameras[i].y, Cameras[i].z);
	}
*/

	luaGetGlobalVector(_luaSession, StartPoints);

/*
	nlinfo("%d Starting points", StartPoints.size());
	for(uint i = 0; i < StartPoints.size(); i++)
	{
		nlinfo("  > starting point %d : %g %g %g", i, StartPoints[i].x, StartPoints[i].y, StartPoints[i].z);
	}
*/


	// Load modules
	lua_getglobal(_luaSession, "Modules");
	lua_pushnil(_luaSession);
	uint8 moduleId = 0;
	while(lua_next(_luaSession, -2) != 0)
	{
		// `key' is at index -2 and `value' at index -1

		// Get module position
		CLuaVector Position;
		lua_pushstring(_luaSession,"Position");
		lua_gettable(_luaSession, -2);
		luaGetVariable(_luaSession, Position);		
		nlinfo("pos %g %g %g", Position.x, Position.y, Position.z);
		lua_pop(_luaSession, 1);  // removes `value'; keeps `key' for next iteration

		// Get module rotation
		CLuaAngleAxis Rotation;
		lua_pushstring(_luaSession,"Rotation");
		lua_gettable(_luaSession, -2);
		luaGetVariable(_luaSession, Rotation);		
		nlinfo("rot %g %g %g %g", Rotation.Axis.x , Rotation.Axis.y, Rotation.Axis.z, Rotation.Angle);
		lua_pop(_luaSession, 1);  // removes `value'; keeps `key' for next iteration
		
		// Get module name
		string Name;
		lua_pushstring(_luaSession,"Name");
		lua_gettable(_luaSession, -2);
		luaGetVariable(_luaSession, Name);		
		nlinfo("name %s", Name.c_str());
		lua_pop(_luaSession, 1);  // removes `value'; keeps `key' for next iteration

		Modules.push_back(new CModule(Name, Position, Rotation,moduleId));
		moduleId++;
	}
	lua_pop(_luaSession, 1);  // removes `key'
	
	for(uint i = 0; i < Modules.size(); i++)
		Modules[i]->initBeforeStartLevel();

}

CLevel::~CLevel()
{
	save();
	CLuaEngine::instance().release();
	
	for(uint i = 0; i < Modules.size(); i++)
		delete Modules[i];
	
	Modules.clear();
}

void CLevel::update()
{
	vector<CModule *>::iterator it;

	for(it=Modules.begin();it!=Modules.end();it++)
		(*it)->update();
}

void CLevel::nextStartingPoint(CVector &pos, uint8 &id)
{
	id = (NextStartingPoint++)%StartPoints.size();
	pos = StartPoints[id];
}

void CLevel::display(CLog *log) const
{
	log->displayNL("LevelName = %s", name().c_str());

	log->displayNL("The level contains %d modules:", Modules.size());
	for(uint i = 0; i < Modules.size(); i++)
	{
		log->displayNL("Module %d", i);
		Modules[i]->display(log);
	}
}

uint8 CLevel::getModuleCount() 
{
	nlassert(Modules.size()<255);
	return Modules.size();
}

CModule *CLevel::getModule(uint32 id)
{
	nlassert(id<getModuleCount());
	return Modules[id];
}


void CLevel::updateModule(uint32 id,CVector pos,CVector rot)
{
	getModule(id)->update(pos,rot);
}

bool isLuaSeparator(char c)
{
	return c==' ' || c=='\t' || c=='\n';
}

bool findLuaEndingBrace(string &luaStr,uint32 &pos)
{
	uint32 i = pos;
	sint32 braceStack = 0;
	for(;i<luaStr.size();i++)
	{
		if(luaStr[i]=='{')
			braceStack++;
		else if(luaStr[i]=='}')
			braceStack--;
		if(braceStack<0)
			break;
	}
	if(braceStack<0)
	{
		pos = i;
		return true;
	}
	return false;
}

bool findLuaNextWord(string &luaStr,string word,uint32 &pos)
{
	uint32 i = pos;
	while( i<luaStr.size() && isLuaSeparator(luaStr[i]) )
	{
		i++;
	}
	if(i>=luaStr.size()) return false;
	string next = luaStr.substr(i,word.size());
	if(next==word)
	{
		pos = i;
		return true;
	}
	return false;
}

bool findLuaWord(string &luaStr,string word,uint32 &pos)
{
	size_t res = luaStr.find(word,pos);
	if(res!=string::npos)
	{
		pos = res;
		return true;
	}
	return false;
}

bool findLuaArrayDeclaration(string &luaStr,string name,uint32 &start,uint32 &end)
{
	uint32 pos = 0;
	uint32 resStart;
	uint32 resEnd;
	bool nameFound = findLuaWord(luaStr,name,pos);
	pos += name.size();
	while(nameFound)
	{
		bool resEqual = findLuaNextWord(luaStr,"=",pos);
		pos++;
		if(resEqual)
		{
			bool resBrace = findLuaNextWord(luaStr,"{",pos);
			pos++;
			if(resBrace)
			{
				resStart = pos;
				bool resEndingBrace = findLuaEndingBrace(luaStr,pos);
				if(resEndingBrace)
				{
					resEnd = pos;
					break;
				}
			}
		}
		nameFound = findLuaWord(luaStr,name,pos);		
	}
	if(nameFound)
	{
		start = resStart;
		end = resEnd;
		return true;
	}
	return false;
}

void CLevel::save()
{
	if(!changed()) return;

	string fn = CPath::lookup(FileName, false);
	if(fn.empty())
	{
		nlwarning("Llevel::save(%s) file not found ", FileName.c_str());
		return;
	}
	
	/*
	CHashKey serverHashKey = getSHA1(fn);
	CNetMessage msgout(CNetMessage::RequestCRCKey);
	msgout.serial(FileName);
	msgout.serial(serverHashKey);
	CNetwork::instance().send(msgout);
	*/

	
	FILE *fp;
	fp = fopen(fn.c_str(),"rt");
	nlassert(fp!=NULL);
	string luaStr = "";
	char ch[256];
	while(!feof(fp))
	{
		uint32 res = fread(ch,1,255,fp);
		ch[res]='\0';
		luaStr += ch;
	}
	fclose(fp);

	uint32 moduleStartPos;
	uint32 moduleEndPos;
	bool found = findLuaArrayDeclaration(luaStr,"Modules",moduleStartPos,moduleEndPos);
	if(found)
	{
		string beforeModule = luaStr.substr(0,moduleStartPos);
		string moduleStr = luaStr.substr(moduleStartPos,moduleEndPos-moduleStartPos);
		string afterModule = luaStr.substr(moduleEndPos);

		fp = fopen(fn.c_str(),"wt");
		nlassert(fp!=NULL);
		fwrite(beforeModule.c_str(),1,beforeModule.size(),fp);
		uint32 i;
		fprintf(fp,"\n");
		for(i=0;i<Modules.size();i++)
		{
			fprintf(fp,"\t");
			Modules[i]->save(fp);
			if(i+1<Modules.size())
				fprintf(fp,",\n");
		}
		fprintf(fp,"\n");
		fwrite(afterModule.c_str(),1,afterModule.size(),fp);
		fclose(fp);
	}

}

bool CLevel::changed()
{
	for(uint32 i=0;i<Modules.size();i++)
	{
		if(Modules[i]->changed())
			return true;
	}
	return false;
}
