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

#include <nel/misc/path.h>
#include <nel/misc/file.h>

#include <nel/net/service.h>

#include <3d/mesh.h>
#include <3d/shape.h>
#include <3d/material.h>
#include <3d/register_3d.h>

#include <nel/misc/smart_ptr.h> 

/*
#include "bot.h"
#include "main.h"
#include "entity.h"
#include "physics.h"
#include "variables.h"
#include "entity_manager.h"
*/

//
// Namespaces
//

using namespace std;
using namespace NL3D;
using namespace NLNET;
using namespace NLMISC;


//
// Functions
//


uint32 loadMesh(const std::string &meshFileName, std::vector<NLMISC::CVector> &vertices, std::vector<NLMISC::CVector> &normals, std::vector<int> &indices, bool applyPreTransform)
{
	uint32 nbFaces = 0;

	vertices.clear();
	indices.clear();

	//return 0;

	NL3D::registerSerial3d();

	if(CPath::lookup(meshFileName, false).empty())
	{
		nlwarning("Mesh '%s' is not found, can't get colission", meshFileName.c_str());
		return 0;
	}

	CMesh *m = NULL;
	{
		CShapeStream ss;
		NLMISC::CIFile i(CPath::lookup(meshFileName, false).c_str());
		i.serial(ss);
		i.close();
		
		
		//CSmartPtr<IShape> is = ss.getShapePointer();
		m = (CMesh*)ss.getShapePointer();
	}
	const CMeshGeom &mg = m->getMeshGeom();

	CMatrix tmat;
	tmat.identity();

	if(applyPreTransform)
	{
		CAnimatedValueBlock avBlock;

		//dynamic_cast<const CAnimatedValueVector &>(m->getDefaultPos()->getValue()).Value;
		CVector gpos = dynamic_cast<const CAnimatedValueVector &>(m->getDefaultPos()->eval(m->getDefaultPos()->getBeginTime(),avBlock)).Value;
		CQuat grot = dynamic_cast<const CAnimatedValueBlendable<CQuat> &>(m->getDefaultRotQuat()->eval(m->getDefaultRotQuat()->getBeginTime(),avBlock)).Value;
		CVector gscale = dynamic_cast<const CAnimatedValueVector &>(m->getDefaultScale()->eval(m->getDefaultScale()->getBeginTime(),avBlock)).Value;
/*
		CVector gpos = dynamic_cast<const CAnimatedValueVector &>(m->getDefaultPos()->getValue()).Value;
		CQuat grot = dynamic_cast<const CAnimatedValueBlendable<CQuat> &>(m->getDefaultRotQuat()->getValue()).Value;
		CVector gscale = dynamic_cast<const CAnimatedValueVector &>(m->getDefaultScale()->getValue()).Value;
*/		
		tmat.setPos(gpos);
		tmat.setRot(grot);
		tmat.scale(gscale);
	}
	

#ifdef NL_INDEX_BUFFER_H //new 3d

	uint nbmb = mg.getNbMatrixBlock();
	for(uint i = 0; i < nbmb; i++)
	{
		uint nbrp = mg.getNbRdrPass(i);
		for(uint j = 0; j < nbrp; j++)
		{
			const CIndexBuffer &ib = mg.getRdrPassPrimitiveBlock(i, j);
			CIndexBufferRead iba;
			ib.lock(iba);
			const uint32 *ibptr = iba.getPtr();
			uint nbi = ib.getNumIndexes();
			for(uint k = 0; k < nbi; k++)
			{
				indices.push_back(ibptr[k]);
				if((k%3)==2)
					nbFaces++;
				//				nlinfo("%d %d %d %d", k, ptr[k+0], ptr[k+1], ptr[k+2]);
			}
		}
	}
	

	{
		const CVertexBuffer &vb = mg.getVertexBuffer();
		CVertexBufferRead vba;
		vb.lock (vba);
		uint32 nbv = vb.getNumVertices();
		for(uint i = 0; i < nbv; i++)
		{
			const void *vv = vba.getVertexCoordPointer(i);
			CVector v = *(CVector*)vv;
			const void *nn = vba.getNormalCoordPointer(i);
			CVector n = *(CVector*)nn;
			//		uint j;
			//		for(j = 0; j < vertices.c_str(); j++)
			//		{
			vertices.push_back(tmat * v);
			normals.push_back(n);
			//		}
			//		if(j)
		}
	}

#else //old 3d
	uint nbmb = mg.getNbMatrixBlock();
	for(uint i = 0; i < nbmb; i++)
	{
		uint nbrp = mg.getNbRdrPass(i);
		for(uint j = 0; j < nbrp; j++)
		{
			const CPrimitiveBlock &pb = mg.getRdrPassPrimitiveBlock(i, j);
			const uint32 *ptr = pb.getTriPointer();
			uint nbt = pb.getNumTri();
			for(uint k = 0; k < nbt*3; k+=3)
			{
				nbFaces++;
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
		const void *vv = vb.getVertexCoordPointer(i);
		CVector v = *(CVector*)vv;
		const void *nn = vb.getNormalCoordPointer(i);
		CVector n = *(CVector*)nn;
		//		uint j;
		//		for(j = 0; j < vertices.c_str(); j++)
		//		{
		vertices.push_back(tmat * v);
		normals.push_back(n);
		//		}
		//		if(j)
	}
#endif

	delete m;
	return nbFaces;
}
