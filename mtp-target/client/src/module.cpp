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

#include <deque>
#include <string>

#include <3d/mesh.h>
#include <3d/shape.h>
#include <3d/material.h>
#include <3d/register_3d.h>

#include <nel/misc/quat.h>
#include <nel/misc/common.h>

#include <nel/3d/u_instance_material.h>

#include "3d/water_height_map.h"
#include "3d/water_pool_manager.h"

#include "global.h"
#include "entity.h"
#include "network_task.h"
#include "mtp_target.h"
#include "resource_manager.h"
#include "level_manager.h"

#include "stdpch.h"

#include <deque>

#include <nel/misc/quat.h>
#include <nel/misc/common.h>

#include <nel/3d/u_instance_material.h>

#include "3d/water_height_map.h"
#include "3d/water_pool_manager.h"

#include "module.h"
#include "global.h"
#include "3d_task.h"
#include "time_task.h"
#include "mtp_target.h"
#include "font_manager.h"
#include "config_file_task.h"
#include "../../common/load_mesh.h"

using namespace std;
using namespace NLMISC;
using namespace NL3D;





CModule::CModule():CModuleCommon()
{
	mat = C3DTask::instance().driver().createMaterial();
}


CModule::~CModule()
{
	if(Mesh)
	{
		C3DTask::instance().scene().deleteInstance(Mesh);
		Mesh = NULL;	
	}
}




void CModule::init(const string &name,uint8 id, CVector position, CAngleAxis rotation)
{
	CModuleCommon::init(name,id,position,rotation);

	ShapeName = CResourceManager::instance().get(Name+".shape");
	NbFaces = loadMesh(ShapeName, Vertices, Normals, Indices);
	
	
	Mesh = C3DTask::instance().scene().createInstance (ShapeName);
	if (Mesh == 0)
	{
		nlwarning ("Can't load '%s.shape'", Name.c_str());
	}
	Mesh->setTransformMode(UTransformable::RotQuat);
	Mesh->setRotQuat(CQuat(rotation));
	Mesh->setPos(position);
	CMatrix mmatrix = mesh()->getMatrix();
	
}





void CModule::renderSelection()
{
	mat->setColor(CRGBA(255,255,255,200));
	mat->setZWrite(true);
	mat->setZFunc(UMaterial::always);
	mat->setBlend(true);
	mat->setBlendFunc(UMaterial::srcalpha,UMaterial::invsrcalpha);
	
	CMatrix matrix = mesh()->getMatrix();

	float grow = 0.001f; 
	uint i;
	for(i = 0; i<NbFaces; i++)
	{
		CTriangle tri;
		tri.V0 = matrix * (Vertices[Indices[i*3+0]] + grow * Normals[Indices[i*3+0]]);
		tri.V1 = matrix * (Vertices[Indices[i*3+1]] + grow * Normals[Indices[i*3+1]]);
		tri.V2 = matrix * (Vertices[Indices[i*3+2]] + grow * Normals[Indices[i*3+2]]);

#if 0
		C3DTask::instance().driver().drawTriangle(tri,*mat);
#else
		CLine line1(tri.V0,tri.V1);
		CLine line2(tri.V1,tri.V2);
		CLine line3(tri.V2,tri.V0);
		
		C3DTask::instance().driver().drawLine(line1,*mat);
		C3DTask::instance().driver().drawLine(line2,*mat);
		C3DTask::instance().driver().drawLine(line3,*mat);
#endif
		
	}
	
}

void CModule::update(NLMISC::CVector pos,NLMISC::CVector rot)
{
	//TODO rot
	position(pos);
}
