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

using namespace std;
using namespace NLMISC;
using namespace NL3D;





CModule::CModule(const std::string &name,uint8 id, CVector position, CAngleAxis rotation):CEditableElement(name,id,position,rotation)
{

	string MeshName = CResourceManager::instance().get(Name+".shape");
	
	vertices.clear();
	indices.clear();
	
	NbFaces = 0;
	NLMISC::CIFile i(CPath::lookup(MeshName, false).c_str());
	CShapeStream ss;
	i.serial(ss);
	i.close();
	


	CMesh *m = (CMesh*)ss.getShapePointer();
	const CMeshGeom &mg = m->getMeshGeom();

	uint nbmb = mg.getNbMatrixBlock();
	for(uint i = 0; i < nbmb; i++)
	{
		uint nbrp = mg.getNbRdrPass(i);
		for(uint j = 0; j < nbrp; j++)
		{
			const CPrimitiveBlock &pb = mg.getRdrPassPrimitiveBlock(i, j);
			const uint32 *ptr = pb.getTriPointer();
			uint nbt = pb.getNumTri();
			NbFaces += nbt;
			for(uint k = 0; k < nbt*3; k+=3)
			{
				indices.push_back(ptr[k+0]);
				indices.push_back(ptr[k+1]);
				indices.push_back(ptr[k+2]);
				//				nlinfo("%d %d %d %d", k, ptr[k+0], ptr[k+1], ptr[k+2]);
			}
		}
	}
	
	const CVertexBuffer &vb = mg.getVertexBuffer();
	uint32 nbv = vb.getNumVertices();
	for(uint i = 0; i < nbv; i++)
	{
		const void *nn = vb.getNormalCoordPointer(i);
		CVector n = *(CVector*)nn;
		const void *vv = vb.getVertexCoordPointer(i);
		CVector v = *(CVector*)vv;
		//		uint j;
		//		for(j = 0; j < vertices.c_str(); j++)
		//		{
		vertices.push_back(v);
		normals.push_back(n);
		//		}
		//		if(j)
	}

	//C3DTask::instance().scene().deleteInstance(Mesh);
	
	Mesh = C3DTask::instance().scene().createInstance (MeshName);
	if (mesh == 0)
	{
		nlwarning ("Can't load '%s.shape'", Name.c_str());
	}
	Mesh->setTransformMode(UTransformable::RotQuat);
	Mesh->setRotQuat(CQuat(rotation));
	Mesh->setPos(position);

	_type = CEditableElement::Module;
}


CModule::~CModule()
{
}








bool CModule::intersect(NLMISC::CVector rayStart,NLMISC::CVector rayEnd,NLMISC::CVector &rayHit)
{
	//return true;
	CMatrix mat = mesh()->getMatrix();
	CMatrix imat = mat;
	imat.invert();
	//rayEnd = imat * rayEnd;
	//rayStart = imat * rayStart;
	
	uint32 i;
	for(i = 0; i<NbFaces; i++)
	{
		CTriangle tri;
		tri.V0 = mat * vertices[indices[i*3+0]];
		tri.V1 = mat * vertices[indices[i*3+1]];
		tri.V2 = mat * vertices[indices[i*3+2]];

		CPlane p;
		p.make(tri.V0,tri.V1,tri.V2);
		CVector hit;
		bool res = tri.intersect(rayStart,rayEnd,hit,p);
		if(res)
		{
			rayHit = hit;
			return true;
		}
	}


	return false;
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
		tri.V0 = matrix * (vertices[indices[i*3+0]] + grow * normals[indices[i*3+0]]);
		tri.V1 = matrix * (vertices[indices[i*3+1]] + grow * normals[indices[i*3+1]]);
		tri.V2 = matrix * (vertices[indices[i*3+2]] + grow * normals[indices[i*3+2]]);

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
