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
#include "font_manager.h"
#include "config_file_task.h"
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

string CharOrder = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.:-$@[]";
uint32 CharX[] =
{
	35,  62,  88, 111, 137, 163, 187, 212,
	36,  49,  69,  96, 119, 150, 177, 204,
	35,  62,  87, 113, 143, 168, 195, 229,
	35,  61,  85, 109, 124, 150, 174, 199, 224,
	35,  59,  84, 110, 122, 134, 155, 176, 195, 204
};
uint32 CharY[] =
{
	123, 123, 123, 123, 123, 123, 123, 123,
	145, 145, 145, 145, 145, 145, 145, 145,
	166, 166, 166, 166, 166, 166, 166, 166,
	188, 188, 188, 188, 188, 188, 188, 188, 188,
	210, 210, 210, 210, 210, 210, 210, 210, 210, 210
};
uint32 CharW[] =
{
	22,  22,  22,  22,  22,  22,  22,  22,
	12,  19,  25,  22,  29,  25,  25,  22,
	22,  25,  22,  25,  22,  22,  30,  23,
	22,  22,  22,  14,  22,  22,  22,  22,  22,
	22,  22,  22,  10,  10,  17,  21,  17, 9, 9
};
uint32 CharH[] =
{
	22,  22,  22,  22,  22,  22,  22,  22,
	21,  21,  21,  21,  21,  21,  21,  21,
	22,  22,  22,  22,  22,  22,  22,  22,
	22,  22,  22,  22,  22,  22,  22,  22,  22,
	22,  22,  22,  22,  22,  22,  22,  22,  22, 22
};

//
// Functions
//
	
void CFontManager::init()
{
	string res = CResourceManager::instance().get("font.tga");
	Texture = C3DTask::instance().driver().createTextureFile (res);
	Material = C3DTask::instance().createMaterial();
	Material.setTexture(Texture);
	Material.setBlend(true);
	Material.setZFunc(UMaterial::always);
	Material.setDoubleSided();

	FontWidth = 22;
	FontHeight = 20;

	// Set the cache size for the font manager (in bytes)
	C3DTask::instance().driver().setFontManagerMaxMemory (2000000);
	
	// Create a Text context for later text rendering
	//	TextContext = Driver->createTextContext (CPath::lookup("n019003l.pfb"));
	LittleTextContext = C3DTask::instance().driver().createTextContext (CPath::lookup(CConfigFileTask::instance().configFile().getVar("LittleFont").asString()));
	nlassert(LittleTextContext != 0);
	LittleTextContext->setKeep800x600Ratio(false);
	LittleTextContext->setHotSpot (UTextContext::BottomLeft);
	LittleTextContext->setFontSize (12);
	LittleTextContext->setShaded(true);

	BigFontSize = CConfigFileTask::instance().configFile().getVar("BigFontSize").asInt();
	BigTextContext = C3DTask::instance().driver().createTextContext (CPath::lookup(CConfigFileTask::instance().configFile().getVar("BigFont").asString()));
	nlassert(BigTextContext != 0);
	BigTextContext->setKeep800x600Ratio(false);
	BigTextContext->setHotSpot (UTextContext::BottomLeft);
	BigTextContext->setFontSize (BigFontSize);
	BigTextContext->setShaded(true);
	
	GUIFontSize = CConfigFileTask::instance().configFile().getVar("GUIFontSize").asInt();
	GUITextContext = C3DTask::instance().driver().createTextContext (CPath::lookup(CConfigFileTask::instance().configFile().getVar("GUIFont").asString()));
	nlassert(GUITextContext != 0);
	GUITextContext->setKeep800x600Ratio(false);
	GUITextContext->setHotSpot (UTextContext::BottomLeft);
	GUITextContext->setFontSize (GUIFontSize);
	GUITextContext->setColor(CRGBA(0,0,0));
	GUITextContext->setShaded(false);
}

void CFontManager::release()
{
	if(!Material.empty())
	{
		C3DTask::instance().driver().deleteMaterial(Material);
	}
	if(Texture)
	{
		C3DTask::instance().driver().deleteTextureFile(Texture);
		Texture = 0;
	}
}

void CFontManager::littlePrintf(const CRGBA &col, float x, float y, const char *format ...)
{
	H_AUTO(littlePrintf2);

	char *str;
	NLMISC_CONVERT_VARGS (str, format, 256);

	LittleTextContext->setColor (col);
	LittleTextContext->printfAt (8*x/C3DTask::instance().screenWidth(), (C3DTask::instance().screenHeight()-16*y-16)/C3DTask::instance().screenHeight(), str);
}

void CFontManager::littlePrintf(float x, float y, const char *format ...)
{
	H_AUTO(littlePrintf);

	char *str;
	NLMISC_CONVERT_VARGS (str, format, 256);

	LittleTextContext->setColor (CRGBA (255,255,255,255));
	LittleTextContext->printfAt (8*x/C3DTask::instance().screenWidth(), (C3DTask::instance().screenHeight()-16*y-16)/C3DTask::instance().screenHeight(), str);
}


void CFontManager::printf(const NLMISC::CRGBA &col, float x, float y, float scale, const char *format, ...)
{
	H_AUTO(printf);

	char *str;
	NLMISC_CONVERT_VARGS (str, format, 256);
	
	float px;
	
	float py;
	px = x;
	py = y+FontHeight;
	C3DTask::instance().driver().setFrustum(CFrustum(0, (float)C3DTask::instance().screenWidth(), 0, (float)C3DTask::instance().screenHeight(), -1, 1, false));
	Material.setColor(col);

	for (char *ptr = str; *ptr; ptr++)
	{
		*ptr = toupper(*ptr);

		if (*ptr == ' ')
		{
			px += 22*scale;
		}
		else if(*ptr == '\n')
		{
			px += x;
			py += 22*scale;
		}
		else
		{
			for(uint j = 0; j < CharOrder.size(); j++)
			{
				if (CharOrder[j] == *ptr)
				{
					CQuadUV		quad;
					quad.V0.set(px,C3DTask::instance().screenHeight()-py,0);
					quad.V1.set(px+CharW[j]*scale,C3DTask::instance().screenHeight()-py,0);
					quad.V2.set(px+CharW[j]*scale,C3DTask::instance().screenHeight()-py+CharH[j]*scale,0);
					quad.V3.set(px,C3DTask::instance().screenHeight()-py+CharH[j]*scale,0);

					int rx1 = CharX[j]+3;
					int ry1 = CharY[j]+23;
					int rx2 = CharX[j]+CharW[j]+3;
					int ry2 = CharY[j]+CharH[j]+23;
					
					quad.Uv0.U= rx1/256.0f;
					quad.Uv0.V= ry2/256.0f;
					quad.Uv1.U= rx2/256.0f;
					quad.Uv1.V= ry2/256.0f;
					quad.Uv2.U= rx2/256.0f;
					quad.Uv2.V= ry1/256.0f;
					quad.Uv3.U= rx1/256.0f;
					quad.Uv3.V= ry1/256.0f;

//					C3DTask::instance().driver().drawQuad (quad, Material);
					px += CharW[j]*scale;
					break;
				}
			}
		}
	}

	BigTextContext->setColor (col);
	BigTextContext->printfAt (x/(float)C3DTask::instance().screenWidth(), 1.0f-y/(float)C3DTask::instance().screenHeight(), str);
}


void CFontManager::printf3D(const NLMISC::CRGBA &col, const NLMISC::CVector &pos, float scale, const char *format, ...)
{
	H_AUTO(printf3D);
	
	char *str;
	NLMISC_CONVERT_VARGS (str, format, 256);

	C3DTask::instance().driver().setMatrixMode2D11 ();

	CVector p = pos;
	p.z += 0.02f;
	
	CMatrix cameraMatrix = C3DTask::instance().scene().getCam().getMatrix();

	CVector vv = cameraMatrix.getPos() - p;
	float dt = 1.0f / vv.norm();
	dt /= 10.0f;

	if (dt < 0.48f)
		dt = 0.48f;
	
	cameraMatrix.invert();

	p = cameraMatrix * p;

	// the text is behind us, don't display it
	if (p.y < 0.0f)
		return;

	p = C3DTask::instance().scene().getCam().getFrustum().project(p);

	float avx = -(float(strlen(str))/2.0f*22.0f*scale/10.0f);

	Material.setZFunc(UMaterial::lessequal);
	Material.setColor(col);
	
	for (char *ptr = str; *ptr; ptr++)
	{
		*ptr = toupper(*ptr);
		
		if (*ptr == ' ')
		{
			avx += 22.0f*scale/10.0f;
		}
		else
		{
			for(uint j = 0; j < CharOrder.size(); j++)
			{
				if (CharOrder[j] == *ptr)
				{
					CQuadUV q;
					q.V0.set(p.x+(avx-0.01f)*dt, p.y-0.01f*dt*(4.0f/3.0f), p.z);
					q.V1.set(p.x+(avx-0.01f)*dt, p.y+0.01f*dt*(4.0f/3.0f), p.z);
					q.V2.set(p.x+(avx+0.01f)*dt, p.y+0.01f*dt*(4.0f/3.0f), p.z);
					q.V3.set(p.x+(avx+0.01f)*dt, p.y-0.01f*dt*(4.0f/3.0f), p.z);

					float rx1 = CharX[j]+3+0.5f;
					float ry1 = CharY[j]+23+0.5f;
					float rx2 = CharX[j]+CharW[j]+3-0.5f;
					float ry2 = CharY[j]+CharH[j]+23-0.5f;
					
					q.Uv2.U= rx2/256.0f;
					q.Uv2.V= ry1/256.0f;
					q.Uv3.U= rx2/256.0f;
					q.Uv3.V= ry2/256.0f;
					q.Uv0.U= rx1/256.0f;
					q.Uv0.V= ry2/256.0f;
					q.Uv1.U= rx1/256.0f;
					q.Uv1.V= ry1/256.0f;	

					C3DTask::instance().driver().drawQuad (q, Material);

					avx += CharW[j]*scale/10.0f;
					break;
				}
			}
		}
	}

	Material.setZFunc(UMaterial::always);
}
