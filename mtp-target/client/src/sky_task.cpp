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

#include <nel/3d/u_scene.h>
#include <nel/3d/u_camera.h>
#include <nel/3d/u_driver.h>
#include <nel/3d/u_texture.h>
#include <nel/3d/u_instance.h>
#include <nel/3d/u_cloud_scape.h>
#include <nel/3d/u_text_context.h>

#include "3d/nelu.h"
#include "3d/scene.h"
#include "time_task.h"
#include "3d/scene_user.h"
#include "3d/water_model.h"
#include "3d/water_shape.h"
#include "3d/texture_file.h"
#include "3d/texture_blend.h"
#include "3d/transform_shape.h"
#include "3d/water_height_map.h"
#include "3d/water_pool_manager.h"

#include "3d_task.h"
#include "sky_task.h"
#include "time_task.h"
#include "game_task.h"
#include "chat_task.h"
#include "score_task.h"
#include "network_task.h"
#include "font_manager.h"
#include "task_manager.h"
#include "entity_manager.h"
#include "background_task.h"
#include "resource_manager.h"
#include "config_file_task.h"


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


CSkyTask::CSkyTask():ITask()
{
	ShapeName = "sky.shape";
	EnvMap0Name = "water_env.tga";
	EnvMap1Name = "water_env.tga";
	HeightMap0Name = "water_disp.tga";
	HeightMap1Name = "water_disp2.tga";

	nelSkyScene   = NULL;
	nelSkyMesh    = NULL;
	nelCloudScape = NULL;
	WaterShape    = NULL;
	WaterModel    = NULL;
	WaterMesh     = NULL;	
}

void cbVar (CConfigFile::CVar &var)
{
	if (CSkyTask::instance().WaterShape == 0)
	{
		nlwarning ("Can't set water parameter");
		return;
	}
	
	if (var.Name == "Map1Scale")
		CSkyTask::instance().WaterShape->setHeightMapScale(0, NLMISC::CVector2f(var.asFloat(0), var.asFloat(1)));
	else if (var.Name == "Map2Scale")
		CSkyTask::instance().WaterShape->setHeightMapScale(1, NLMISC::CVector2f(var.asFloat(0), var.asFloat(1)));
	else if (var.Name == "Map1Speed")
		CSkyTask::instance().WaterShape->setHeightMapSpeed(0, NLMISC::CVector2f(var.asFloat(0), var.asFloat(1)));
	else if (var.Name == "Map2Speed")
		CSkyTask::instance().WaterShape->setHeightMapSpeed(1, NLMISC::CVector2f(var.asFloat(0), var.asFloat(1)));
	else if (var.Name == "WaveHeightFactor")
		CSkyTask::instance().WaterShape->setWaveHeightFactor(var.asFloat());
	else if (var.Name == "WavesParams")
	{
		CWaterHeightMap &whm = GetWaterPoolManager().getPoolByID(0);
		whm.setWaves (var.asFloat(0), var.asFloat(1), (uint)var.asFloat(2), var.asFloat(3) == 1.0f);
	}
	else if (var.Name == "WavesPropagation")
	{
		CWaterHeightMap &whm = GetWaterPoolManager().getPoolByID(0);
		whm.setPropagationTime(var.asFloat());
	}
	else
		nlstop;
}


void CSkyTask::init()
{
	nelSkyScene = C3DTask::instance().driver().createScene(false);

	nelSkyScene->getCam ()->setPerspective(degToRad(CConfigFileTask::instance().configFile().getVar("Fov").asFloat()), 1.33f, 0.15f, 3000.0f);
	nelSkyScene->getCam ()->setTransformMode (UTransformable::DirectMatrix);

	string res = CResourceManager::instance().get(shapeName());
	nelSkyMesh = nelSkyScene->createInstance(res);
	if (nelSkyMesh)
	{
		nelSkyMesh->setTransformMode (UTransformable::DirectMatrix);
		nelSkyMesh->setMatrix(CMatrix::Identity);
	}

////
	if(CConfigFileTask::instance().configFile().getVar("DisplayWater").asInt() == 2)
	{
		string res;
		static std::string shapeName("water_quad.shape");
		CSceneUser *su = dynamic_cast<CSceneUser *>(&C3DTask::instance().scene());
		CScene		&scene = su->getScene ();

		// load textures
		res = CResourceManager::instance().get(envMap0Name());
		ITexture *envMap1   = new CTextureFile(res);
		res = CResourceManager::instance().get(envMap1Name());
		ITexture *envMap2   = new CTextureFile(res);
		
		res = CResourceManager::instance().get(heightMap0Name());
		ITexture *heightMap  = new CTextureFile(res);
		res = CResourceManager::instance().get(heightMap1Name());
		ITexture *heightMap2 = new CTextureFile(res);
		
		WaterShape = new CWaterShape;
		WaterShape->setEnvMap(0, envMap1);
		WaterShape->setEnvMap(1, envMap2);
		
		WaterShape->setHeightMap(0, heightMap);
		WaterShape->setHeightMap(1, heightMap2);

		WaterShape->setWaterPoolID(0);

		CWaterHeightMap &whm = GetWaterPoolManager().getPoolByID(0);
//		float res = whm.getUnitSize();
//		res /= 10.0f;
//		whm.setUnitSize(res);
		whm.enableWaves ();

		float res2 = whm.getPropagationTime ();

		CConfigFileTask::instance().configFile().setCallback("Map1Scale", cbVar);
		cbVar (CConfigFileTask::instance().configFile().getVar("Map1Scale"));
		CConfigFileTask::instance().configFile().setCallback("Map2Scale", cbVar);
		cbVar (CConfigFileTask::instance().configFile().getVar("Map2Scale"));
		CConfigFileTask::instance().configFile().setCallback("Map1Speed", cbVar);
		cbVar (CConfigFileTask::instance().configFile().getVar("Map1Speed"));
		CConfigFileTask::instance().configFile().setCallback("Map2Speed", cbVar);
		cbVar (CConfigFileTask::instance().configFile().getVar("Map2Speed"));
		CConfigFileTask::instance().configFile().setCallback("WaveHeightFactor", cbVar);
		cbVar (CConfigFileTask::instance().configFile().getVar("WaveHeightFactor"));
		CConfigFileTask::instance().configFile().setCallback("WavesParams", cbVar);
		cbVar (CConfigFileTask::instance().configFile().getVar("WavesParams"));
		CConfigFileTask::instance().configFile().setCallback("WavesPropagation", cbVar);
		cbVar (CConfigFileTask::instance().configFile().getVar("WavesPropagation"));

//		WaterShape->setHeightMapScale(0, NLMISC::CVector2f(0.08f/2, 0.072f/2));
//		WaterShape->setHeightMapScale(1, NLMISC::CVector2f(0.1f/2, 0.091f/2));
//		WaterShape->setHeightMapSpeed(0, NLMISC::CVector2f(0.06f, 0.051f));
//		WaterShape->setHeightMapSpeed(1, NLMISC::CVector2f(-0.0623f, 0.085f));
//		WaterShape->setWaveHeightFactor(3);

		const float wqSize = 10000.0f;
		CVector2f corners[] =	{ CVector2f(-wqSize, wqSize), CVector2f(wqSize, wqSize),
								  CVector2f(wqSize, -wqSize), CVector2f(-wqSize, -wqSize)
								};
		CPolygon2D waterPoly;
		waterPoly.Vertices.resize(4);
		std::copy(corners, corners + 4, waterPoly.Vertices.begin());
		WaterShape->setShape(waterPoly);

		scene.getShapeBank()->add(shapeName, WaterShape);

		WaterModel = NLMISC::safe_cast<CWaterModel *>(scene.createInstance(shapeName));
		WaterModel->setPos(0.0f,0.0f,1.0f*GScale);

		//	CWaterHeightMap &whm = GetWaterPoolManager().getPoolByID(0);
		//	whm.enableWaves ();
		//	whm.setWaves (3.0f, 1.0f, 2.0f, false);
	}
	else if(CConfigFileTask::instance().configFile().getVar("DisplayWater").asInt() == 1)
	{
		string res;
		res = CResourceManager::instance().get("water_light.shape");
		WaterMesh = C3DTask::instance().scene().createInstance(res);
		WaterMesh->setPos(0.0f,0.0f,1.0f*GScale);
	}

	if(CConfigFileTask::instance().configFile().getVar("DisplayClouds").asInt() == 1)
	{
		nelCloudScape = nelSkyScene->createCloudScape();
		SCloudScapeSetup css;	
		css.NbCloud = 50;
		css.CloudSpeed = 8.0f;
		css.WindSpeed = 1.5f;

		css.Ambient = NLMISC::CRGBA (64, 72, 132, 255);
		css.Diffuse = NLMISC::CRGBA (167, 177, 236, 255);

		//css.Ambient = NLMISC::CRGBA (120,150,155,255);
		//css.Diffuse = NLMISC::CRGBA (220,250,255,255);
		nelCloudScape->setNbCloudToUpdateIn80ms (1);
		nelCloudScape->setQuality (160.0);
		//nelCloudScape->setDebugQuad (true);
		nelCloudScape->init (&css);	
	}
}

void CSkyTask::update()
{
	CMatrix skyCameraMatrix;
	skyCameraMatrix.identity();
	skyCameraMatrix = C3DTask::instance().scene().getCam()->getMatrix();
	skyCameraMatrix.setPos(CVector::Null);
	
	nelSkyScene->getCam()->setMatrix(skyCameraMatrix);
	
	nelSkyScene->animate (CTimeTask::instance().time());
	
	if (nelCloudScape)
		nelCloudScape->anim (CTimeTask::instance().deltaTime()); // WARNING this function work with screen
}

void CSkyTask::render()
{
	nelSkyScene->render ();
	
	// Must clear ZBuffer For incoming rendering.
	C3DTask::instance().driver().clearZBuffer();
	
	if (nelCloudScape)
		nelCloudScape->render ();
}

void CSkyTask::release()
{
	if(nelSkyMesh)
	{
		nelSkyScene->deleteInstance(nelSkyMesh);
		nelSkyMesh = NULL;
	}

	if(nelCloudScape)
	{
		nelSkyScene->deleteCloudScape(nelCloudScape);
		nelCloudScape = NULL;
	}

	if(nelSkyScene)
	{
		C3DTask::instance().driver().deleteScene(nelSkyScene);
		nelSkyScene = NULL;
	}

	
}


void CSkyTask::shapeName(std::string shapeName)
{
	if(shapeName.empty()) return;
	ShapeName = shapeName;
}

void CSkyTask::envMap0Name(std::string envMapName)
{
	if(envMapName.empty()) return;
	EnvMap0Name = envMapName;
}

void CSkyTask::envMap1Name(std::string envMapName)
{
	if(envMapName.empty()) return;
	EnvMap1Name = envMapName;
}

void CSkyTask::heightMap0Name(std::string heightMap)
{
	if(heightMap.empty()) return;
	HeightMap0Name = heightMap;
}

void CSkyTask::heightMap1Name(std::string heightMap)
{
	if(heightMap.empty()) return;
	HeightMap1Name = heightMap;
}



string CSkyTask::shapeName()
{
	return ShapeName;
}

string CSkyTask::envMap0Name()
{
	return EnvMap0Name;
}

string CSkyTask::envMap1Name()
{
	return EnvMap1Name;
}

string CSkyTask::heightMap0Name()
{
	return HeightMap0Name;
}

string CSkyTask::heightMap1Name()
{
	return HeightMap1Name;
}

