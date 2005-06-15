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

#include "gui.h"
#include "3d_task.h"
#include "time_task.h"
#include "mtp_target.h"
#include "editor_task.h"
#include "task_manager.h"
#include "entity_manager.h"
#include "config_file_task.h"
#include "level_manager.h"
#include "resource_manager2.h"
	

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
			if(CEditorTask::getInstance().enable())
			{
				if(c=='4' && C3DTask::getInstance().kbDown(KeyNUMPAD4))
					return;
				if(c=='6' && C3DTask::getInstance().kbDown(KeyNUMPAD6))
					return;
				if(c=='8' && C3DTask::getInstance().kbDown(KeyNUMPAD8))
					return;
				if(c=='5' && C3DTask::getInstance().kbDown(KeyNUMPAD5))
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
	ScreenWidth = CConfigFileTask::getInstance().configFile().getVar("ScreenWidth").asInt();
	ScreenHeight = CConfigFileTask::getInstance().configFile().getVar("ScreenHeight").asInt();

	EnableExternalCamera = false;

	CConfigFile::CVar v;
	v = CConfigFileTask::getInstance().configFile().getVar("AmbientColor");
	nlassert(v.size()==4);
	AmbientColor.set(v.asInt(0),v.asInt(1),v.asInt(2),v.asInt(3));
	v = CConfigFileTask::getInstance().configFile().getVar("ClearColor");
	nlassert(v.size()==4);
	ClearColor.set(v.asInt(0),v.asInt(1),v.asInt(2),v.asInt(3));


	// Create a driver
	uint icon = 0;
#ifdef NL_OS_WINDOWS
	icon = (uint)LoadIcon(ghInstance,MAKEINTRESOURCE(IDI_ICON1));
#endif
	bool useD3D = CConfigFileTask::getInstance().configFile().getVar("OpenGL").asInt()==0;
#ifdef NL_INDEX_BUFFER_H //new 3d
	Driver = UDriver::createDriver(icon,useD3D);
#else
	Driver = UDriver::createDriver(icon);
#endif
	nlassert(Driver);


	bool displayOk = false;
	
	try
	{
		displayOk = Driver->setDisplay (UDriver::CMode(ScreenWidth, ScreenHeight, CConfigFileTask::getInstance().configFile().getVar("ScreenDepth").asInt(), CConfigFileTask::getInstance().configFile().getVar("Fullscreen").asInt()==0, CConfigFileTask::getInstance().configFile().getVar("ScreenFrequency").asInt()));
	}
	catch (EBadDisplay e) 
	{
		nlwarning ("Can't set display mode %d %d %d %d %d", ScreenWidth, ScreenHeight, CConfigFileTask::getInstance().configFile().getVar("ScreenDepth").asInt(), CConfigFileTask::getInstance().configFile().getVar("Fullscreen").asInt(), CConfigFileTask::getInstance().configFile().getVar("ScreenFrequency").asInt());
		nlwarning ("%s",e.what());
#ifdef NL_OS_WINDOWS
		MessageBox (NULL, toString("Please, update your video card drivers\n reason : %s",e.what()).c_str(), "Drivers", MB_OK);
#endif
		exit(1);
	}

	// Create the window with config file values
	if (!displayOk)
	{
		nlwarning ("Can't set display mode %d %d %d %d %d", ScreenWidth, ScreenHeight, CConfigFileTask::getInstance().configFile().getVar("ScreenDepth").asInt(), CConfigFileTask::getInstance().configFile().getVar("Fullscreen").asInt(), CConfigFileTask::getInstance().configFile().getVar("ScreenFrequency").asInt());

		std::vector<UDriver::CMode> modes;
		bool res = Driver->getModes(modes);
		std::vector<UDriver::CMode>::iterator it;
		nlinfo("available video mode : ");
		if(modes.size()==0)
			nlinfo("none available");
		
		for(it=modes.begin();it!=modes.end();it++)
		{
			UDriver::CMode m = *it;
			nlinfo("%d %d %d %dHz %s",m.Width,m.Height,m.Depth,m.Frequency,m.Windowed?"windowed":"fullscreen");
		}
		
		return;
	}

	Driver->EventServer.addListener (EventCharId, &InterfaceListener);
	//Driver->enableLowLevelKeyboard(true);

	Driver->setAmbientColor(AmbientColor);

	Driver->enableFog(false);
	Driver->setupFog(CConfigFileTask::getInstance().configFile().getVar("FogDistMin").asFloat(),CConfigFileTask::getInstance().configFile().getVar("FogDistMax").asFloat(),ClearColor);

	// Create a scene
	Scene = Driver->createScene(false);

	if (Scene == 0)
	{
		nlwarning ("Can't create nel scene");
		return;
	}

	//Scene->getCam()->setFrustum(0.26f, 0.2f, 0.1f, 40.0f);
	Scene->getCam().setPerspective(degToRad(CConfigFileTask::getInstance().configFile().getVar("Fov").asFloat()), 1.33f, 1.0f*GScale, 30000.0f*GScale);
	Scene->getCam().setTransformMode(UTransformable::DirectMatrix);

	MouseListener = new C3dMouseListener();
	MouseListener->init();
	//CaptureCursor = false;
	
	Scene->enableLightingSystem(true);
	Scene->setSunAmbient(AmbientColor);
	Scene->setSunDiffuse(CRGBA(255,255,255));
	Scene->setSunSpecular(CRGBA(255,255,255));
	Scene->setSunDirection(CVector(-1,0,-1));

	Scene->setPolygonBalancingMode(UScene::PolygonBalancingOn);
	Scene->setGroupLoadMaxPolygon("Fx", CConfigFileTask::getInstance().configFile().getVar("FxNbMaxPoly").asInt());


	LevelParticle = 0;
	//too much particles , no left to trace.ps
	if(CConfigFileTask::getInstance().configFile().getVar("DisplayParticle").asInt() == 1)
	{
		string res;
		res = CResourceManager::getInstance().get("snow.ps");
		LevelParticle.cast(C3DTask::getInstance().scene().createInstance(res));
		if (!LevelParticle.empty())
		{
			LevelParticle.setTransformMode(UTransformable::RotQuat);
			LevelParticle.hide();
			LevelParticle.setOrderingLayer(2);
		}
	}
	
#ifdef NL_OS_WINDOWS
	HWND hWnd = (HWND )Driver->getDisplay();
	SetWindowText(hWnd,"Mtp Target");
#endif
}

void C3DTask::update()
{
	if(!Driver->isActive())
		CTaskManager::getInstance().exit();
		//exit(0);
	Scene->animate(CTimeTask::getInstance().time());
	Driver->EventServer.pump();

	if(C3DTask::getInstance().kbPressed(KeyF1))
	{
		CTaskManager::getInstance().switchBench();
	}
}

void C3DTask::takeScreenShot()
{
	CBitmap btm;
	C3DTask::getInstance().driver().getBuffer (btm);
	//		btm.flipV();
	string filename = "";
	if(CMtpTarget::getInstance().sessionFileName().size())
		filename = CFile::findNewFile(CFile::getFilenameWithoutExtension(CMtpTarget::getInstance().sessionFileName())+"_.jpg");
	else
		filename = CFile::findNewFile ("screenshot.jpg");
	COFile fs (filename);
	btm.writeJPG(fs);
	nlinfo("Screenshot '%s' saved", filename.c_str());	
}

void C3DTask::render()
{
	CViewport vp;
	CScissor s;
	
	Driver->enableFog(true);
	Scene->render();

	if(C3DTask::getInstance().kbDown(KeyMENU) && C3DTask::getInstance().kbPressed(KeyF2))
		takeScreenShot();
	
	if(EnableExternalCamera && CLevelManager::getInstance().levelPresent() && CLevelManager::getInstance().currentLevel().ExternalCameras.size() > 0)
	{
		CMatrix oldmat = C3DTask::getInstance().scene().getCam().getMatrix();
		
		vp.init(0.69f,0.55f,0.3f,0.3f);
		s.init(0.69f,0.55f,0.3f,0.3f);

		LevelParticle.hide();
		CMatrix m;
		m.identity();
		m.setPos(CLevelManager::getInstance().currentLevel().ExternalCameras[0].first);
		m.setRot(CLevelManager::getInstance().currentLevel().ExternalCameras[0].second);
		C3DTask::getInstance().scene().getCam().setMatrix(m);
		Scene->setViewport(vp);
		Driver->setViewport(vp);
		Driver->setScissor(s);
		Driver->clearBuffers();
		Scene->render();
		LevelParticle.show();

		vp.init(0,0,1,1);
		s.init(0,0,1,1);
		Scene->setViewport(vp);
		Driver->setViewport(vp);
		Driver->setScissor(s);
		C3DTask::getInstance().scene().getCam().setMatrix(oldmat);
	}
	
	C3DTask::getInstance().driver().enableFog(false);
	
	CEntityManager::getInstance().renderNames();
}

void C3DTask::release()
{
	nlassert(Driver);
	Driver->release();
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
	CGuiObjectManager::getInstance().mouseListener().captureCursor(b);
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

NL3D::UMaterial C3DTask::createMaterial() const
{
	nlassert(Driver); 
	UMaterial m = driver().createMaterial();
	m.initUnlit();
	return m;
}
