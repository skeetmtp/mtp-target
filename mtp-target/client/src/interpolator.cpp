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
/*
#include "stdpch.h"

#include <deque>

#include <nel/misc/quat.h>
#include <nel/misc/common.h>

#include <nel/3d/u_instance_material.h>

#include "3d/water_height_map.h"
#include "3d/water_pool_manager.h"

#include "extrapolator.h"

using namespace std;
using namespace NLMISC;
using namespace NL3D;
*/
#include "stdpch.h"

#include <nel/misc/quat.h>
#include <nel/misc/common.h>

#include "time_task.h"
#include "entity.h"
#include "interpolator.h"
#include "mtp_target.h"
#include "../../common/constant.h"

using namespace std;
using namespace NLMISC;
using namespace NL3D;


#if 0


void CEntity::resetFacing()
{
	Facing = 0.0f;
}


void CEntity::position(CVector &position,float servertime, bool onwater)
{
	double localtime = CTimeTask::instance().time();
	double dtime = localtime - PositionLastTime;
	PositionLastTime = localtime;
	
	if(FirstSetPos)
	{
		FirstSetPos = false;
		Position = position;
		Oldposition = Position;
		ServerStartTime = servertime;
		LocalTime = 0.0;
		LocalStartTime = CTimeTask::instance().time();
		nlinfo("[%d] first set position => sync time server: %g local: %g", id(), ServerStartTime, LocalTime);
	}
	ServerTime = servertime - ServerStartTime;
	PositionKeys.push_back(CPositionKey(position,ServerTime, onwater));
	if(DisplayDebug)			
	{
		nlinfo("[%d] local time = %g", id(), LocalTime);
		nlinfo("[%d] server time = %g", id(), ServerTime);
		nlinfo("[%d] position (key count=%d)[id=%d] (%g %g %g)", id(), PositionKeys.size(), id(), position.x,position.y,position.z);
	}
	HasValidPosition = true;
	PositionBuffer.push_back(position);
	while(PositionBuffer.size()>PositionBufferCount)
		PositionBuffer.pop_front();
}

CVector CEntity::computePosition(float time)
{
	deque<CPositionKey>::iterator it1,it2,it;

	CVector res = Position;

	if(FirstSetPos)
	{
		nlinfo("[%hu] no FirstSetPos ", (uint16)id());
		return res;
	}

	if(CMtpTarget::instance().controler().getControledEntity()==id() && PositionKeys.size()<2)
	{
		Lct += LctAdaptationInc;
		//if (_positionKeys.size() == 1) res = (*(_positionKeys.begin()))._position;
		if(DisplayDebug)			
			nlwarning("[%d] not enough key (1) to interpolate position",id());
		FrameOkCount = 0;
		return res;
	}

	// TODO: it crashs here sometimes because 'it' becomes out of bound (++ go out of the vector)
	for(it=PositionKeys.begin();(it+1)!=PositionKeys.end() && time>(it+1)->ServerTime;it++)
	{
		if ((*it).OnWater)
		{
			collisionWithWater((*it).OnWater);
		}
	}

	PositionKeys.erase(PositionKeys.begin(), it);

	it1 = PositionKeys.begin();

	if (PositionKeys.size() == 1)
		return res;

	if(CMtpTarget::instance().controler().getControledEntity()==id() && PositionKeys.size()<2 )
	{
		Lct += LctAdaptationInc;
		//if (_positionKeys.size() == 1) res = (*(_positionKeys.begin()))._position;
		if(DisplayDebug)			
			nlwarning("[%d] not enough key to interpolate position (%g, %g, %g, %g)",id(),it1->ServerTime,time,LocalTime,Lct);
		FrameOkCount = 0;
		return res;
	}

	it2 = it1 + 1;
	if(it2 == PositionKeys.end())
	{
		nlwarning("it1 is the last position, it should never happen, can't get it2");
		return res;
	}
		
	res = CPositionKey::Lerp(*it1, *it2, time);
	if(CMtpTarget::instance().controler().getControledEntity()==id())
		FrameOkCount++;
	
	if(CMtpTarget::instance().controler().getControledEntity()==id() && PositionKeys.size()<=(LctKeyCount-1))
	{
		FrameOkCount = 0;
	}
	if(CMtpTarget::instance().controler().getControledEntity()==id() && PositionKeys.size()>(LctKeyCount+1) && FrameOkCount > LctFrameCountBeforeDecreasing)
	{
		if(DisplayDebug)			
			nlwarning("[%hu] too much key , adapting lct...", (uint16)id());
		Lct -= LctAdaptationDec;
	}
		
	return res;

}

CVector CEntity::smoothDirection()
{
	return Position - Oldposition;
}

CVector CEntity::direction()
{
	deque<CVector>::iterator it;
	CVector res(0,-1,0);

	if(FirstSetPos)
		return res;

	if(PositionBuffer.size()<2)
		return res;

	for(it = PositionBuffer.begin(); (it+1) != PositionBuffer.end(); it++)
	{
		res += *(it+1) - *it;
	}

	return res;
}

CVector CEntity::computeSpeed(float time)
{
	deque<CPositionKey>::iterator it1,it2,it;

	CVector res(0,0,0);

	if(FirstSetPos)
		return res;

	if(PositionKeys.size() < 2)
	{
		//if (_positionKeys.size() == 1) res = (*(_positionKeys.begin()))._position;
		return res;
	}

	for(it = PositionKeys.begin(); it != PositionKeys.end() && (it+1) != PositionKeys.end() && time > (it+1)->ServerTime; it++)
	{
		collisionWithWater((*it).OnWater);
	}
	
	PositionKeys.erase(PositionKeys.begin(), it);

	it1 = PositionKeys.begin();

	if(PositionKeys.size()<2)
	{
		//if (_positionKeys.size() == 1) res = (*(_positionKeys.begin()))._position;
		return res;
	}

	if(it1 == PositionKeys.end())
		return res;

	it2 = it1 + 1;

	if(it2 == PositionKeys.end())
		return res;
	
	CVector pos1 = (*it1).Position;
	CVector pos2 = (*it2).Position;
	float dt = (*it2).ServerTime - (*it1).ServerTime;

	return (pos2 - pos1) / dt;
}

static float rotLerp(float rsrc,float rdst,float pos)
{
	double nrsrc = fmod((double)rsrc, 2*NLMISC::Pi) + 2*NLMISC::Pi;
	nrsrc = fmod((double)nrsrc, 2*NLMISC::Pi);
	double nrdst = fmod((double)rdst, 2*NLMISC::Pi) + 2 * NLMISC::Pi ;
	nrdst = fmod((double)nrdst, 2*NLMISC::Pi);
	if(nrsrc<nrdst)
	{
		if(fabs(nrdst - nrsrc) > NLMISC::Pi)
			nrdst -= 2*NLMISC::Pi;
	}
	else
	{
		if(fabs(nrsrc - nrdst) > NLMISC::Pi)
			nrsrc -= 2*NLMISC::Pi;
	}
	double ipos = 1.0f - pos;

	float res = (float) (nrdst * pos + nrsrc * ipos);
	//nlinfo("%f => %f (%f) = %f",nrsrc,nrdst,pos,res);
	return res;
}



void CEntity::update()
{
	double deltaTime = CTimeTask::instance().deltaTime();

	if(DisplayDebug)
	{
		nlinfo("lct = %g, in queue %2d ct %g st %g (dt = %g)", Lct, PositionKeys.size(), LocalTime, ServerTime, deltaTime);
	}

//	_localTime += deltaTime;

	ObjMatrix.setPos(CVector(0,0,0));
	if(!FirstSetPos)
	{
		Speed = computeSpeed(LocalTime - Lct);
		if(OpenClose)
		{
			if(CMtpTarget::instance().controler().getControledEntity()==id())
			{
				ObjMatrix.identity();
				ObjMatrix.rotateZ(Facing);
				ObjMatrix.rotateX(0.5f * (float)NLMISC::Pi * (1.0f - Rotation.x));
			}
			else
			{
				ObjMatrix.identity();
				ObjMatrix.rotateZ(Facing);
				float instantRotX = -0.5f * Speed.z * deltaTime / GScale - (float)NLMISC::Pi * 0.08f;
				float lpos = 5 * deltaTime / 1.0f;
				Rotation.x = rotLerp(Rotation.x, instantRotX, lpos);
				ObjMatrix.rotateX(Rotation.x);
			}
		}
		else
		{
			CVector speed = Speed;
			speed.z = 0.0f;
			if(speed.norm() > 0.01f*GScale)
			{
				CVector nspeed = speed;
				nspeed.normalize();
				CVector right = nspeed ^ CVector(0,0,1);
				float angleX = - speed.norm() * deltaTime / GScale;
				right = ObjMatrix.inverted().mulVector(right);
				ObjMatrix.rotate(CQuat(CAngleAxis(right, angleX)));
			}
		}

		Oldposition = Position;
		Position = computePosition((float)fabs(LocalTime - Lct));

		ObjMatrix.setPos(Position);

		CMatrix m, m2;
		m2.identity();
		//m2.scale(0.01f);
		m.setPos(ObjMatrix.getPos());
		m.setRot(m2 * ObjMatrix);

/*ace todo		if (OnWater && WaterModel)
		{
			//nlinfo("water set pos begin");
			NLMISC::CVector pos = m.getPos();
			pos.z = WaterModel->getAttenuatedHeight(NLMISC::CVector2f(pos.x, pos.y), CMtpTarget::instance().controler().Camera.getMatrix()->getPos());
			m.setPos(pos);
			//nlinfo("water set pos end");
		}
*//*		else
		{
			CWaterHeightMap &whm = GetWaterPoolManager().getPoolByID(0);
			const float waterRatio = whm.getUnitSize(); 
			const float invWaterRatio = 1.0f / waterRatio;
			sint px = (sint) (_position.x * invWaterRatio);
			sint py = (sint) (_position.y * invWaterRatio);			
			whm.perturbate(px, py, 2, 1.0f*GScale);
		}
*/
		if(OpenMesh)
			OpenMesh->setMatrix(m);
		if(CloseMesh)
			CloseMesh->setMatrix(m);
	}

	SoundsDescriptor.update3d(ObjMatrix.getPos(), CVector(0,0,0)); // todo : velocity

	CVector dir = smoothDirection();
	dir.z = 0.0f;
	if(dir.norm() > 0.001f && CMtpTarget::instance().State == CMtpTarget::eGame)
	{
		dir.normalize();
		float instantFacing = (float)acos(dir.y);
		if(dir.x>0.0f)
			instantFacing = - instantFacing;
		instantFacing += (float)NLMISC::Pi;
		float lpos = 20 * deltaTime / 1.0f;
		Facing = rotLerp(Facing, instantFacing, lpos);
		if(CMtpTarget::instance().controler().getControledEntity() == id())
			Facing = instantFacing;
	}

	if(TraceParticle != 0)
	{
		TraceParticle->setPos(Position);
		
		// we activate
		if (TraceParticle != 0 && ParticuleActivated != -1 && TraceParticle->isSystemPresent())
		{
			//if (ParticuleActivated == 0 || CMtpTarget::instance().controler().getControledEntity() != this)
			TraceParticle->activateEmitters(ParticuleActivated==1);
			if(ParticuleActivated==1 && CMtpTarget::instance().controler().getControledEntity()!=id())
				TraceParticle->show();

			ParticuleActivated = -1;
		}
	}
}


void init()
{
	LctKeyCount = CConfigFileTask::instance().configFile().getVar("LctKeyCount").asInt();
	LctFrameCountBeforeDecreasing = CConfigFileTask::instance().configFile().getVar("LctFrameCountBeforeDecreasing").asInt();
	Lct = CConfigFileTask::instance().configFile().getVar("Lct").asFloat();
	LctAdaptationInc = CConfigFileTask::instance().configFile().getVar("LctAdaptationInc").asFloat();	
	LctAdaptationDec = CConfigFileTask::instance().configFile().getVar("LctAdaptationDec").asFloat();	
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////

CEntityState::CEntityState()
{
	this->position = CVector::Null;
	this->onWater = false;
}

CEntityState::CEntityState(CVector position,bool onWater)
{
	this->position = position;
	this->onWater = onWater;
}

CEntityState CEntityState::operator+( const CEntityState &other ) const
{
	return CEntityState( position + other.position , onWater || other.onWater );
}

CEntityState operator *( double coef, CEntityState &value )
{
	return CEntityState( (float)coef * value.position , value.onWater );
}

CEntityState operator*( CEntityState &value, double coef )
{
	return CEntityState( (float)coef * value.position , value.onWater );	
}



/////////////////////////////////////////////////////////////////////////////////////////////
//
//
/////////////////////////////////////////////////////////////////////////////////////////////


const double CInterpolator::_minLct = 2.0 * MT_NETWORK_UPDATE_PERIODE;  //less lct time doesnt improve really gameplay
const double CInterpolator::_maxLct = 12.0 * MT_NETWORK_UPDATE_PERIODE; //if lct > _maxLct, the game become unplayable ...

CInterpolator::CInterpolator()
{
	_currentPosition        = CVector::Null;
	_currentSpeed           = CVector::Null;
	_currentDirection       = CVector::Null;
	_currentSmoothDirection = CVector::Null;
	_currentOnWater         = false;
	
	reset();
}

CInterpolator::~CInterpolator()
{

}

void CInterpolator::reset()
{
	_keys.clear();
	_startTime      = 0.0;
	_serverTime     = 0.0;
	_localTime      = 0.0;
	_deltaTime      = 0.0;
	_lastUpdateTime = 0.0;
	_available      = false;
	_lct            = 0.0;
	_maxKeyDiffTime  = 0.0;
	_meanKeyDiffTime  = 0.0;
	
//	nlinfo("reset[0x%p] (%f)",this,CTimeTask::instance().time());
}

static int fcount = 0;
void CInterpolator::addKey(CEntityInterpolatorKey key)
{
	double time = CTimeTask::instance().time(); //current time
	if(!_available || _startTime==0.0)
	{
		_startTime = time;
		_addKeyTime = 0.0;
	}
	else
	{
		_addKeyTime += MT_NETWORK_UPDATE_PERIODE; //time the key should arrived
	}
	double localTime = time - _startTime;
	if(_addKeyTime>localTime)
	{
		_startTime -=  _addKeyTime - localTime;
		localTime = time - _startTime;
		//nlinfo("  start time correction : [0x%p] : %f-%f = %f (%f)",this,_addKeyTime,localTime,_addKeyTime-localTime,_startTime);
	}
	else
	{
		fcount++;
		/*
		if((fcount%100) == 0)
		{
			nlinfo("_lct = %f",_lct);
			nlinfo("_meanKeyDiffTime = %f",_meanKeyDiffTime);
			nlinfo("(%f)addKeyTime[0x%p] : %f-%f = %f (%f)",time,this,_addKeyTime,localTime,_addKeyTime-localTime,_startTime);
		}
		*/
		//nlinfo("(%f)addKeyTime[0x%p] : %f-%f = %f (%f)",time,this,_addKeyTime,localTime,_addKeyTime-localTime,_startTime);
		
	}
	_available = true;
	key._serverTime = _addKeyTime;
	_keys.push_back(key);

	double smoothNess = 0.9f;
	double diffKeyTime = _addKeyTime - localTime; //diff between expected key time and real key time
	double absDiffKeyTime = fabs(diffKeyTime);

	if(diffKeyTime<0.0f)
		_meanKeyDiffTime = _meanKeyDiffTime * smoothNess + absDiffKeyTime * (1.0f - smoothNess);

	if(absDiffKeyTime > _maxKeyDiffTime)
		_maxKeyDiffTime = absDiffKeyTime;
}


double CInterpolator::localTime() const
{
	return _localTime;
}


void CInterpolator::update()
{
	if(_keys.size()<2)
	{
		_outOfKey = true;
		return;
	}

	_autoAdjustLct();

	_outOfKey = false;

	_localTime = CTimeTask::instance().time() - _startTime;
	_deltaTime = _localTime - _lastUpdateTime;
	_serverTime = _localTime - _lct;
	_lastUpdateTime = _localTime;

	_currentPosition        = _position(_serverTime);
	_currentSpeed           = _speed(_serverTime);
	_currentDirection       = _direction(_serverTime);
	_currentSmoothDirection = _currentDirection;
/*
	if(_outOfKey)
		nlinfo("################################### OUT of key ###############################");
*/
}


bool CInterpolator::outOfKey()
{
	return _outOfKey;
}



CVector CInterpolator::position() const
{
	return _currentPosition;
}

CVector	CInterpolator::speed() const
{
	return _currentSpeed;
}

CVector CInterpolator::direction() const
{
	return _currentDirection;
}

CVector CInterpolator::smoothDirection() const
{
	return _currentDirection;
}

bool CInterpolator::onWater() const
{
	return _currentOnWater;
}



CVector CInterpolator::_position(double time)
{
	CEntityInterpolatorKey key = _keys.back();
	return key.value().position;
}


CVector CInterpolator::_speed(double time)
{
	deque<CEntityInterpolatorKey>::reverse_iterator it;
	it = _keys.rbegin();
	CEntityInterpolatorKey key2 = *it;
	it++;
	CEntityInterpolatorKey key1 = *it;
	return (key2.value().position - key1.value().position) / MT_NETWORK_UPDATE_PERIODE;
}

CVector CInterpolator::_direction(double time)
{
	deque<CEntityInterpolatorKey>::reverse_iterator it;
	it = _keys.rbegin();
	CEntityInterpolatorKey key2 = *it;
	it++;
	CEntityInterpolatorKey key1 = *it;
	return (key2.value().position - key1.value().position);
}


void CInterpolator::_autoAdjustLct()
{
	_lct = _minLct;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////

CLinearInterpolator::~CLinearInterpolator()
{
	
}



CVector CLinearInterpolator::_position(double time)
{
	double remainder = fmod(time,MT_NETWORK_UPDATE_PERIODE);
	deque<CEntityInterpolatorKey>::reverse_iterator it;
	CEntityInterpolatorKey nextKey = _keys.back();
	bool nextKeySet = false;
	for(it=_keys.rbegin();it!=_keys.rend();it++)
	{
		CEntityInterpolatorKey key = *it;
		if(key.serverTime()<time)
		{
			if(!nextKeySet)
				break;
			return CEntityInterpolatorKey::Lerp(key.value(),nextKey.value(),remainder).position;
		}
		nextKey = key;
		nextKeySet = true;
	}
	//_outOfKey = true;
	return CInterpolator::_position(time);
}

CVector CLinearInterpolator::_speed(double time)
{
	double remainder = fmod(time,MT_NETWORK_UPDATE_PERIODE);
	deque<CEntityInterpolatorKey>::reverse_iterator it;
	CEntityInterpolatorKey nextKey = _keys.back();
	bool nextKeySet = false;
	for(it=_keys.rbegin();it!=_keys.rend();it++)
	{
		CEntityInterpolatorKey key = *it;
		if(key.serverTime()<time)
		{
			if(!nextKeySet)
				break;
			return (nextKey.value().position - key.value().position)  / MT_NETWORK_UPDATE_PERIODE;
		}
		nextKey = key;
		nextKeySet = true;
	}
	//_outOfKey = true;
	return CInterpolator::_speed(time);
}

CVector CLinearInterpolator::_direction(double time)
{
	double remainder = fmod(time,MT_NETWORK_UPDATE_PERIODE);
	deque<CEntityInterpolatorKey>::reverse_iterator it;
	CEntityInterpolatorKey nextKey = _keys.back();
	bool nextKeySet = false;
	for(it=_keys.rbegin();it!=_keys.rend();it++)
	{
		CEntityInterpolatorKey key = *it;
		if(key.serverTime()<time)
		{
			if(!nextKeySet)
				break;
			return (nextKey.value().position - key.value().position);
		}
		nextKey = key;
		nextKeySet = true;
	}
	//_outOfKey = true;
	return CInterpolator::_direction(time);
}


void CLinearInterpolator::_autoAdjustLct()
{
	double lct;
	lct = _meanKeyDiffTime * 4;
	lct = max(lct,_minLct);
	lct = min(lct,_maxLct);
//	lct = _meanKeyDiffTime + 2 * MT_NETWORK_UPDATE_PERIODE;
	_lct = lct;
//	_lct = 4 * MT_NETWORK_UPDATE_PERIODE;
}




///////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////

CExtendedInterpolator::CExtendedInterpolator()
{
	_entity = 0;	
	reset();
}

CExtendedInterpolator::~CExtendedInterpolator()
{
	
}

void CExtendedInterpolator::reset()
{
	CInterpolator::reset();
	_currentRotation        = CVector::Null;
	_currentFacing          = 0.0;
	_lastRotationTime       = 0.0;	
}

void CExtendedInterpolator::update()
{

	CInterpolator::update();
	
	_currentRotation = _rotation(_deltaTime);

	nlassert(_entity!=0);
	if(_entity->openClose())
	{
		if(_entity->isLocal())
		{
			_currentMatrix.identity();
			_currentMatrix.rotateZ((float)_currentFacing);
			_currentMatrix.rotateX(0.5f * (float)NLMISC::Pi * (1.0f - _currentRotation.x));
		}
		else
		{
			_currentMatrix.identity();
			_currentMatrix.rotateZ((float)_currentFacing);
			float instantRotX = (float)(-0.5f * _currentSpeed.z * _deltaTime / GScale - (float)NLMISC::Pi * 0.08f);
			float lpos = (float)(5 * _deltaTime / 1.0f);
			_currentRotation.x = (float)RotLerp(_currentRotation.x, instantRotX, lpos);
			_currentMatrix.rotateX(_currentRotation.x);
		}
	}
	else
	{
		CVector speed = _currentSpeed;
		speed.z = 0.0f;
		if(speed.norm() > 0.01f*GScale)
		{
			CVector nspeed = speed;
			nspeed.normalize();
			CVector right = nspeed ^ CVector(0,0,1);
			float angleX = (float)( - speed.norm() * _deltaTime / GScale);
			right = _currentMatrix.inverted().mulVector(right);
			_currentMatrix.rotate(CQuat(CAngleAxis(right, angleX)));
		}
	}
	
	//_currentMatrix.identity();//TODO remove this line
	_currentMatrix.setPos(_currentPosition);


	CVector dir = _currentDirection;//smoothDirection();
	dir.z = 0.0f;
	if(dir.norm() > 0.001f && CMtpTarget::instance().State == CMtpTarget::eGame)
	{
		dir.normalize();
		float instantFacing = (float)acos(dir.y);
		if(dir.x>0.0f)
			instantFacing = - instantFacing;
		instantFacing += (float)NLMISC::Pi;
		double lpos = 20 * _deltaTime / 1.0f;
		_currentFacing = RotLerp(_currentFacing, instantFacing, lpos);
		if(_entity->isLocal())
			_currentFacing = instantFacing;
	}
	

	
/*
	_currentMatrix.identity();
	_currentMatrix.setPos(_currentPosition);
	_currentMatrix.rotate(CQuat(CAngleAxis(right(), _currentRotation.x)));
*/	
/*
	if(DisplayDebug)
	{
		nlinfo("lct = %g, in queue %2d ct %g st %g (dt = %g)", Lct, PositionKeys.size(), LocalTime, ServerTime, deltaTime);
	}
*/	

}


CVector CExtendedInterpolator::right() const
{
	return _currentDirection ^ CVector(0,0,1);;
}

CVector CExtendedInterpolator::rotation() const
{
	return _currentRotation;
}

double CExtendedInterpolator::facing() const
{
	return _currentFacing;
}

void CExtendedInterpolator::rotation(CVector rotation)
{
	_currentRotation = rotation;
}


void CExtendedInterpolator::entity(CEntity *entity)
{ 
	_entity = entity;
};

CMatrix CExtendedInterpolator::getMatrix() const
{
	return _currentMatrix;
}

CVector CExtendedInterpolator::_rotation(double deltaTime)
{
	if(_entity->openClose())
	{
		double instantRotX = -0.5f * _currentSpeed.z * deltaTime / GScale - (double)NLMISC::Pi * 0.08f;
		double lpos = 0.1f * deltaTime / 1.0f;
		_currentRotation.x = (float )RotLerp(_currentRotation.x, instantRotX, lpos);
	}
	return _currentRotation;
}

double  CExtendedInterpolator::RotLerp(double rsrc,double rdst,double pos)
{
	
				
	double nrsrc = fmod((double)rsrc, 2*NLMISC::Pi) + 2*NLMISC::Pi;
	nrsrc = fmod((double)nrsrc, 2*NLMISC::Pi);
	double nrdst = fmod((double)rdst, 2*NLMISC::Pi) + 2 * NLMISC::Pi ;
	nrdst = fmod((double)nrdst, 2*NLMISC::Pi);
	if(nrsrc<nrdst)
	{
		if(fabs(nrdst - nrsrc) > NLMISC::Pi)
			nrdst -= 2*NLMISC::Pi;
	}
	else
	{
		if(fabs(nrsrc - nrdst) > NLMISC::Pi)
			nrsrc -= 2*NLMISC::Pi;
	}
	double ipos = 1.0f - pos;
	
	double res = (double) (nrdst * pos + nrsrc * ipos);
	//nlinfo("%f => %f (%f) = %f",nrsrc,nrdst,pos,res);
	return res;
}


