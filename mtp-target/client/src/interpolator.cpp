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
#include "stdpch.h"

#include <nel/misc/quat.h>
#include <nel/misc/common.h>

#include "time_task.h"
#include "font_manager.h"
#include "entity.h"
#include "interpolator.h"
#include "mtp_target.h"
#include "../../common/constant.h"

using namespace std;
using namespace NLMISC;
using namespace NL3D;


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


const double CInterpolator::_minLct = 3.0  * MT_NETWORK_MY_UPDATE_PERIODE;  //less lct time doesnt improve really gameplay
const double CInterpolator::_maxLct = 12.0 * MT_NETWORK_MY_UPDATE_PERIODE; //if lct > _maxLct, the game become unplayable ...

double CInterpolator::_lct = 0.0;

CInterpolator::CInterpolator(double dt)
{
	_currentPosition        = CVector::Null;
	_currentSpeed           = CVector::Null;
	_currentDirection       = CVector::Null;
	_currentSmoothDirection = CVector::Null;
	_currentOnWater         = false;
	
	_entity = 0;
	_dt = dt;
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
	_outOfKeyCount = 0;
	
//	nlinfo("reset[0x%p] (%f)",this,CTimeTask::instance().time());
}

void CInterpolator::entity(CEntity *entity)
{ 
	_entity = entity;
};


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
		_addKeyTime += _dt; //time the key should arrived
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
		return;

	_outOfKeyCount--;
	if(_outOfKeyCount<0)
		_outOfKeyCount=0;
//	nlinfo("outofkeycount = %d ; keysize = %d",_outOfKeyCount,_keys.size());
	if(_entity->id()==CMtpTarget::instance().controler().getControledEntity())
		_autoAdjustLct();

	_localTime = CTimeTask::instance().time() - _startTime;
	_deltaTime = _localTime - _lastUpdateTime;
	_serverTime = _localTime - _lct;
	_lastUpdateTime = _localTime;

	_currentPosition        = _position(_serverTime);
	_currentSpeed           = _speed(_serverTime);
	_currentDirection       = _direction(_serverTime);
	_currentSmoothDirection = _currentDirection;
}


bool CInterpolator::outOfKey()
{
	return _keys.size()<2 || _outOfKeyCount>10;
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
	if(it==_keys.rend()) return(CVector::Null);
	CEntityInterpolatorKey key2 = *it;
	it++;
	if(it==_keys.rend()) return(CVector::Null);
	CEntityInterpolatorKey key1 = *it;
	return (key2.value().position - key1.value().position) / (float)_dt;
}

CVector CInterpolator::_direction(double time)
{
	deque<CEntityInterpolatorKey>::reverse_iterator it;
	it = _keys.rbegin();
	if(it==_keys.rend()) return(CVector::Null);
	CEntityInterpolatorKey key2 = *it;
	it++;
	if(it==_keys.rend()) return(CVector::Null);
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

CLinearInterpolator::CLinearInterpolator(double dt):CInterpolator(dt)
{
	
}

CLinearInterpolator::~CLinearInterpolator()
{
	
}



CVector CLinearInterpolator::_position(double time)
{
	double remainder = fmod(time,_dt);
	double lerpPos = remainder / _dt;
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

			CVector dpos = (nextKey.value().position - key.value().position ) / (float)_dt;
			//TODO put 10 into a .cfg variable
			if(dpos.norm()>10) 
				return nextKey.value().position;
			return CEntityInterpolatorKey::Lerp(key.value(),nextKey.value(),lerpPos).position;
		}
		nextKey = key;
		nextKeySet = true;
	}
	_outOfKeyCount++;
	return CInterpolator::_position(time);
}

CVector CLinearInterpolator::_speed(double time)
{
	double remainder = fmod(time,_dt);
	double lerpPos = remainder / _dt;
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
			return (nextKey.value().position - key.value().position)  / (float)_dt;
		}
		nextKey = key;
		nextKeySet = true;
	}
	return CInterpolator::_speed(time);
}

CVector CLinearInterpolator::_direction(double time)
{
	double remainder = fmod(time,_dt);
	double lerpPos = remainder / _dt;
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
	return CInterpolator::_direction(time);
}


void CLinearInterpolator::_autoAdjustLct()
{
	double lct;
	lct = _meanKeyDiffTime * 4;
	lct = max(lct,_minLct);
	lct = min(lct,_maxLct);
//	lct = _meanKeyDiffTime + 2 * _dt;
	_lct = lct;
//	_lct = 4 * _dt;
}




///////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////

CExtendedInterpolator::CExtendedInterpolator(double dt):CLinearInterpolator(dt)
{
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


