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

#include "editable_element.h"
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



CEditableElement::CEditableElement()
{
	mat = C3DTask::instance().driver().createMaterial();
}


CEditableElement::~CEditableElement()
{
	C3DTask::instance().scene().deleteInstance(Mesh);
	
}


/*
void CEditableElement::init(const string &name,uint8 id, CVector position, CAngleAxis rotation)
{
//	CEditableElementCommon::init(name,id,position,rotation);


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
	
	
	string res = CResourceManager::instance().get(ShapeName);
	Mesh = C3DTask::instance().scene().createInstance (res);
	if (!Mesh)
	{
		nlwarning ("Can't load 'col_box.shape'");
	}
	
	Mesh->setTransformMode(UTransformable::RotQuat);
	Mesh->setRotQuat(CQuat(rotation));
	Mesh->setPos(position);
	
}
*/






//   /E--F
//  A---B|
//  | H |G
//  D---C



void CEditableElement::renderSelection()
{
	mat->setColor(CRGBA(255,255,255,255));
	mat->setZWrite(true);
	mat->setZFunc(UMaterial::always);
	CAABBox bbox;
	Mesh->getShapeAABBox(bbox);
	CAABBox tbbox = CAABBox::transformAABBox(Mesh->getMatrix(),bbox);
	
	CVector boxMin = tbbox.getMin();
	CVector boxMax = tbbox.getMax();

	CQuad quad;
	CVector a = boxMin;
	CVector b = a;
	b.x = boxMax.x;
	CVector c = a;
	c.x = boxMax.x;
	c.y = boxMax.y;
	CVector d = a;
	d.y = boxMax.y;
	
	CVector e = a;
	e.z = boxMax.z;
	CVector f = b;
	f.z = boxMax.z;
	CVector g = c;
	g.z = boxMax.z;
	CVector h = d;
	h.z = boxMax.z;
	
	CLine l;

	l.V0 = a;
	l.V1 = b;
	C3DTask::instance().driver().drawLine(l,*mat);
	l.V0 = b;
	l.V1 = c;
	C3DTask::instance().driver().drawLine(l,*mat);
	l.V0 = c;
	l.V1 = d;
	C3DTask::instance().driver().drawLine(l,*mat);
	l.V0 = d;
	l.V1 = a;
	C3DTask::instance().driver().drawLine(l,*mat);
	
	l.V0 = e;
	l.V1 = f;
	C3DTask::instance().driver().drawLine(l,*mat);
	l.V0 = f;
	l.V1 = g;
	C3DTask::instance().driver().drawLine(l,*mat);
	l.V0 = g;
	l.V1 = h;
	C3DTask::instance().driver().drawLine(l,*mat);
	l.V0 = h;
	l.V1 = e;
	C3DTask::instance().driver().drawLine(l,*mat);
	
	l.V0 = a;
	l.V1 = e;
	C3DTask::instance().driver().drawLine(l,*mat);
	l.V0 = b;
	l.V1 = f;
	C3DTask::instance().driver().drawLine(l,*mat);
	l.V0 = c;
	l.V1 = g;
	C3DTask::instance().driver().drawLine(l,*mat);
	l.V0 = d;
	l.V1 = h;
	C3DTask::instance().driver().drawLine(l,*mat);
	
	
	/*
	quad.V0 = a;
	quad.V1 = b;
	quad.V2 = c;
	quad.V3 = d;

	C3DTask::instance().driver().drawQuad(quad,*mat);
	*/			
}

void CEditableElement::show()
{
	Mesh->show();
}

void CEditableElement::hide()
{
	Mesh->hide();
}

/*
void CEditableElement::position(CVector pos)
{
	Position = pos;
	Mesh->setPos(pos);
	_changed = true;
	CLevelManager::instance().currentLevel().changed(true);
}
*/

UInstance *CEditableElement::mesh()
{
	return Mesh;
}

/*
void CEditableElement::update(NLMISC::CVector pos,NLMISC::CVector rot,uint32 selectedBy)
{
	position(pos);
}
*/

