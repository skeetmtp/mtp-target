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

#ifdef NL_OS_WINDOWS
#	include <windows.h>
#	include "../resource.h"
#	undef min
#	undef max
	extern HINSTANCE ghInstance;
#endif

#include <3d/mesh.h>
#include <3d/shape.h>
#include <3d/material.h>
#include <3d/register_3d.h>
	

#include "task_manager.h"
#include "3d_task.h"
#include "editor_task.h"
#include "time_task.h"
#include "config_file_task.h"
#include "resource_manager.h"
#include "entity_manager.h"
#include "gui.h"
	

//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLMISC;


//
// Variables
//

//
// Functions
//

class CInterfaceListener : public IEventListener
{
	virtual void	operator() ( const CEvent& event )
	{
		if ( ! _MaxWidthReached )
		{
			char c = (char)((CEventChar&)event).Char;
			if(CEditorTask::instance().enable())
			{
				if(c=='4' && C3DTask::instance().kbDown(KeyNUMPAD4))
					return;
				if(c=='6' && C3DTask::instance().kbDown(KeyNUMPAD6))
					return;
				if(c=='8' && C3DTask::instance().kbDown(KeyNUMPAD8))
					return;
				if(c=='5' && C3DTask::instance().kbDown(KeyNUMPAD5))
					return;
			}
			_Line += c;
		}
	}
	
public:
	CInterfaceListener() : _MaxWidthReached( false )
	{}
	
	virtual ~CInterfaceListener() {}
	
	const string&	line() const
	{
		return _Line;
	}
	
	void setLine(const string &str) { _Line = str; }
	
	void			setMaxWidthReached( bool b )
	{
		_MaxWidthReached = b;
	}
	
private:
	string			_Line;
	bool			_MaxWidthReached;
	string			_LastCommand;
};

static CInterfaceListener InterfaceListener;

void C3DTask::init()
{
	ScreenWidth = CConfigFileTask::instance().configFile().getVar("ScreenWidth").asInt();
	ScreenHeight = CConfigFileTask::instance().configFile().getVar("ScreenHeight").asInt();

	CConfigFile::CVar v;
	v = CConfigFileTask::instance().configFile().getVar("AmbientColor");
	nlassert(v.size()==4);
	AmbientColor.set(v.asInt(0),v.asInt(1),v.asInt(2),v.asInt(3));
	v = CConfigFileTask::instance().configFile().getVar("ClearColor");
	nlassert(v.size()==4);
	ClearColor.set(v.asInt(0),v.asInt(1),v.asInt(2),v.asInt(3));


	// Create a driver
	uint icon = 0;
#ifdef NL_OS_WINDOWS
	icon = (uint)LoadIcon(ghInstance,MAKEINTRESOURCE(IDI_ICON1));
#endif
	bool useD3D = CConfigFileTask::instance().configFile().getVar("OpenGL").asInt()==0;
#ifdef NL_INDEX_BUFFER_H //new 3d
	Driver = UDriver::createDriver(icon,useD3D);
#else
	Driver = UDriver::createDriver(icon);
#endif
	nlassert(Driver);

	// Create the window with config file values
	if (!Driver->setDisplay (UDriver::CMode(ScreenWidth, ScreenHeight,
		CConfigFileTask::instance().configFile().getVar("ScreenDepth").asInt(),
		CConfigFileTask::instance().configFile().getVar("Fullscreen").asInt()==0,
		CConfigFileTask::instance().configFile().getVar("ScreenFrequency").asInt())))
	{
		nlwarning ("Can't set display mode %d %d %d %d %d", ScreenWidth, ScreenHeight,
			CConfigFileTask::instance().configFile().getVar("ScreenDepth").asInt(),
			CConfigFileTask::instance().configFile().getVar("Fullscreen").asInt(),
			CConfigFileTask::instance().configFile().getVar("ScreenFrequency").asInt());
		return;
	}

	// Set the cache size for the font manager (in bytes)
	Driver->setFontManagerMaxMemory (2000000);

	// Create a Text context for later text rendering
	TextContext = Driver->createTextContext (CPath::lookup("n019003l.pfb"));

	if (TextContext == 0)
	{
		nlwarning ("Can't create text context");
		return;
	}

	TextContext->setKeep800x600Ratio(false);

	Driver->EventServer.addListener (EventCharId, &InterfaceListener);

	Driver->setAmbientColor(AmbientColor);

	Driver->enableFog(false);
	Driver->setupFog(CConfigFileTask::instance().configFile().getVar("FogDistMin").asFloat(),CConfigFileTask::instance().configFile().getVar("FogDistMax").asFloat(),ClearColor);

	// Create a scene
	Scene = Driver->createScene(false);

	if (Scene == 0)
	{
		nlwarning ("Can't create nel scene");
		return;
	}

	//Scene->getCam()->setFrustum(0.26f, 0.2f, 0.1f, 40.0f);
	Scene->getCam()->setPerspective(degToRad(CConfigFileTask::instance().configFile().getVar("Fov").asFloat()), 1.33f, 1.0f*GScale, 30000.0f*GScale);
	Scene->getCam()->setTransformMode(UTransformable::DirectMatrix);

	MouseListener = new C3dMouseListener();
	MouseListener->init();
	//CaptureCursor = false;
	
	Scene->enableLightingSystem(true);
	Scene->setSunAmbient(AmbientColor);
	Scene->setSunDiffuse(CRGBA(255,255,255));
	Scene->setSunSpecular(CRGBA(255,255,255));
	Scene->setSunDirection(CVector(-1,0,-1));

	Scene->setPolygonBalancingMode(UScene::PolygonBalancingOn);
	Scene->setGroupLoadMaxPolygon("Fx", CConfigFileTask::instance().configFile().getVar("FxNbMaxPoly").asInt());


	LevelParticle = NULL;
	//too much particles , no left to trace.ps
	/*
	if(CConfigFileTask::instance().configFile().getVar("DisplayParticle").asInt() == 1)
	{
		string res;
		res = CResourceManager::instance().get("snow.ps");
		LevelParticle = Scene->createInstance(res);
		if (LevelParticle != 0)
		{
			LevelParticle->setTransformMode(UTransformable::RotQuat);
			LevelParticle->hide();
			LevelParticle->setOrderingLayer(2);
		}
	}
	*/


#ifdef NL_OS_WINDOWS
	HWND hWnd = (HWND )Driver->getDisplay();
	SetWindowText(hWnd,"Mtp Target");
#endif
}

void C3DTask::update()
{
	if(!Driver->isActive())
		CTaskManager::instance().exit();
		//exit(0);
	Scene->animate(CTimeTask::instance().time());
	Driver->EventServer.pump();

	if(C3DTask::instance().kbPressed(KeyF1))
	{
		CHTimer::display();
		CHTimer::displayHierarchicalByExecutionPathSorted(InfoLog, CHTimer::TotalTime, true, 64);
	}
}

void C3DTask::render()
{
//	nlinfo("C3DTask::render():begin render");
	C3DTask::instance().driver().enableFog(true);
	Scene->render();
	C3DTask::instance().driver().enableFog(false);
	CEntityManager::instance().renderNames();
//	nlinfo("C3DTask::render():end render");
}

void C3DTask::release()
{
}

bool C3DTask::kbPressed(NLMISC::TKey key) const
{
	return Driver->AsyncListener.isKeyPushed(key);
}

bool C3DTask::kbDown(NLMISC::TKey key) const
{
	return Driver->AsyncListener.isKeyDown(key);
}

std::string C3DTask::kbGetString() const
{
	string str = InterfaceListener.line();
	InterfaceListener.setLine("");
	return str;
}

void C3DTask::clear()
{
	Driver->clearBuffers (ClearColor);
}

void C3DTask::clearColor(CRGBA color)
{
	ClearColor = color;
}
void C3DTask::captureCursor(bool b)
{
	CGuiObjectManager::instance().mouseListener().captureCursor(b);
	/*
	CaptureCursor = b;
	if(b)
	{
		driver().setCapture(true);
		driver().showCursor(false);
		mouseListener().addToServer(driver().EventServer);
	}
	else
	{
		driver().setCapture(false);
		driver().showCursor(true);
		mouseListener().removeFromServer(driver().EventServer);
	}
	*/
}

