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
#include <nel/net/service.h>
#include <nel/misc/command.h>

#include "bot.h"
#include "main.h"
#include "level.h"
#include "module.h"
#include "physics.h"
#include "variables.h"
#include "lua_engine.h"
#include "entity_manager.h"
//#include "../../common/level_manager.h"


//
// Namespaces
//

using namespace std;
using namespace NLMISC;
using namespace NLNET;


//
// Classes
//



//
// Variables
//

TTime syncStartPhyTime = CTime::getLocalTime();
SyncPhyTime syncPhyTime("syncPhyTime");

IThread *thread = 0;

dWorldID		World = 0;

CSynchronized<dSpaceID>		Space("Space");

dJointGroupID	ContactGroup = 0;
dGeomID			Water = 0;

//list<CCollisionEntity> CollisionEntityList;
//vector<CVector> StartPointList;

CSynchronized<PauseFlags> physicPauseFlags("physicPauseFlags");
void checkPhysicPaused();


//
//
//

/*void startPointCallback(const mtLevelManager::mtStartPoint &startPoint)
{
//	StartPointList.push_back(CVector(startPoint.posX, startPoint.posY, startPoint.posZ));
}

void cameraCallback(const mtLevelManager::mtCamera &camera,unsigned int id)
{
	// there s no need to get info about the module because it s only "visual"
}

void boxCallback(const mtLevelManager::mtBox &box)
{
	CollisionEntityList.push_back(CCollisionEntity());
	CCollisionEntity *entity = &(*(--CollisionEntityList.end()));
	
	CVector scale(box.scaleX, box.scaleY, box.scaleZ);
	scale *= GSCALE;
	entity->setBox(scale);
	entity->setPosition(CVector(box.posX, box.posY, box.posZ));
	entity->setRotation(CAngleAxis(CVector(box.axisX, box.axisY, box.axisZ), box.angle));
	
	entity->Score			= box.score;
	entity->Accel			= box.accel;
	entity->Friction		= box.friction;
	entity->Bounce			= box.bounce;
	entity->LuaFunctionName	= box.luaFunctionName;
	entity->Name			= box.name;

	dGeomSetCategoryBits(entity->Geom, MT_SCENE_SPECTRUM); //ceci est un element de la scene
	dGeomSetCollideBits(entity->Geom, MT_CLIENT_SPECTRUM); //collide unikement avec les entity
}

void triCollCallback(mtLevelManager::mtTriColl &triColl)
{
  	CollisionEntityList.push_back(CCollisionEntity());
	CCollisionEntity *entity = &(*(--CollisionEntityList.end()));
	uint32 i;

	// vertices must be multiple of 3
	nlassert(triColl.vertices.size()%3 == 0);
 	entity->Vertices.resize(triColl.vertices.size());
	for(i=0;i<triColl.vertices.size();i+=3)
	{
		entity->Vertices[i+0] = triColl.vertices[i+0] * triColl.scaleX;
		entity->Vertices[i+1] = triColl.vertices[i+1] * triColl.scaleY;
		entity->Vertices[i+2] = triColl.vertices[i+2] * triColl.scaleZ;
	}
	entity->Indices.resize(triColl.indices.size());
	for(i=0;i<triColl.indices.size();i++)
		entity->Indices[i] = triColl.indices[i];

	dTriMeshDataID data = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuild(data, &entity->Vertices[0], 3*sizeof(dReal), entity->Vertices.size()/3, &entity->Indices[0], entity->Indices.size(), 3*sizeof(int));
	
	entity->Geom = dCreateTriMesh(Space, data, _dTriCallback, _dTriArrayCallback, _dTriRayCallback);
	nlassert(entity->Geom);
	dGeomSetData(entity->Geom, entity);

//  OLD ODE CODE FOR TRI COL
//	dGeomID userGeomId;
//	entity->geom = userGeomId = dCreateTriList(space, _dTriCallback, _dTriArrayCallback, _dTriRayCallback);
//	dGeomSetData(userGeomId, entity);
//
//	dVector3 *f = &entity->vertices[0];
//	dGeomTriListBuild(userGeomId, &entity->vertices[0], sizeof(dcVector3), entity->vertices.size(), &entity->indices[0], sizeof(int), entity->indices.size(), 3 * sizeof(int));

	entity->setPosition(CVector(triColl.posX, triColl.posY, triColl.posZ));
	entity->setRotation(CAngleAxis(CVector(triColl.axisX, triColl.axisY, triColl.axisZ), triColl.angle));
	
	entity->Score = triColl.score;
	entity->Accel = triColl.accel;
	entity->Friction = triColl.friction;
	entity->Bounce = triColl.bounce;
	entity->LuaFunctionName = triColl.luaFunctionName;
	entity->Name = triColl.name;

	dGeomSetCategoryBits(entity->Geom, MT_SCENE_SPECTRUM); //ceci est un element de la scene
	dGeomSetCollideBits(entity->Geom, MT_CLIENT_SPECTRUM); //collide unikement avec les entity
}

void moduleCallback(const mtLevelManager::mtModule &module)
{
	// there s no need to get info about the module because it s only "visual"
}




void resetCallback()
{
	CollisionEntityList.clear();
//	StartPointList.clear();

	// reload default values
	initVariables();

	CLuaEngine::stop();
	CLuaEngine::start();

}
*/
#define SET_VAR(__name) if(var == #__name) { __name = (float) atof(val.c_str()); __name##FromCfg=false; found=true; }

void optionCallback(const string &var, const string &val)
{
	bool found = false;

	SET_VAR(DefaultMaxOpenClose);
	SET_VAR(TimeBeforeStart);
	SET_VAR(TimeBeforeRestart);
	SET_VAR(TimeBeforeCheck);
//	SET_VAR(TimeTimeout);
	SET_VAR(NbWaitingClients);
	SET_VAR(OpenAccelCoef);
	SET_VAR(CloseAccelCoef);
	SET_VAR(SphereDensity);
	SET_VAR(Gravity);
	SET_VAR(BounceWater);
	SET_VAR(BounceVelWater);
	SET_VAR(BounceClient);
	SET_VAR(BounceVelClient);
	SET_VAR(BounceScene);
	SET_VAR(BounceVelScene);
	SET_VAR(MinVelBeforeEnd);
	SET_VAR(NbMaxClients);
	SET_VAR(ModuleScore);
	SET_VAR(ModuleBounce);
	SET_VAR(ModuleAccel);
	SET_VAR(ModuleFriction);
	
	if(var == "LevelName") { LevelName=val; LevelNameFromCfg=false; found=true; }
	if(var == "Author") { Author=val; AuthorFromCfg=false; found=true; }
	
	if(!found)
		nlwarning("Unknown variable '%s' in the level", var.c_str());
}
	

//
//
//

enum { NOTHING=0, CLIENT_CLIENT, CLIENT_SCENE, CLIENT_WATER } ct = NOTHING;

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	int i;

	if(o1==0 || o2==0)
		return;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	CEntity *entity = 0;
	CEntity *entity1 = 0;
	CEntity *entity2 = 0;
	CModule *module = 0;

	if(b1 && b2)
	{
		if(dAreConnected(b1,b2))
			return; // exit without doing anything if the two bodies are connected by a joint

		ct = CLIENT_CLIENT;
		entity1 =(CEntity*) dGeomGetData(o1);
		entity2 =(CEntity*) dGeomGetData(o2);
		entity = entity1;
	}
	else if(b1)
	{
		entity =(CEntity*) dGeomGetData(o1);
		nlassert(entity);
		if(o2 == Water)
			ct = CLIENT_WATER;
		else
		{
//			if(entity->jointed)
//				return;
			ct = CLIENT_SCENE;
			module = (CModule *) dGeomGetData(o2);
			nlassert(module != 0);
		}
	}
	else if(b2)
	{
		entity = (CEntity*) dGeomGetData(o2);
		nlassert(entity);
		if(o1 == Water)
			ct = CLIENT_WATER;
		else
		{
//			if(entity->jointed)
//				return;
			ct = CLIENT_SCENE;
			module = (CModule *) dGeomGetData(o1);
			nlassert(module != 0);
		}
	}
	else
	{
		// not interesting collision
		//nlinfo(">> entity touche somthing");
		ct = NOTHING;
		return;
	}
	
	const uint32 numContact = 25; //3 SKEET_WARNING dans le exemple tri_colide c up to 25 contact !?!
	dContact contact[numContact];
	for(i=0; i<numContact; i++)
	{
		switch(ct)
		{
		case CLIENT_WATER:
			contact[i].surface.mode = dContactBounce;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = BounceWater;
			contact[i].surface.bounce_vel = BounceVelWater;
			break;
		case CLIENT_CLIENT:

			contact[i].surface.mode = dContactBounce;
//			contact[i].surface.mu = dInfinity;
			contact[i].surface.mu = 0;
			contact[i].surface.mu2 = 0;
			contact[i].surface.bounce = BounceClient;
			contact[i].surface.bounce_vel = BounceVelClient;
			/*
			contact[i].surface.mode = dContactMu2;
			contact[i].surface.mu = dInfinity;
			contact[i].surface.mu2 = dInfinity;
			*/
			break;
		case CLIENT_SCENE:
/*
			contact[i].surface.mode = dContactBounce;
			contact[i].surface.mu = 500;
			contact[i].surface.mu2 = 500;
			contact[i].surface.bounce = 0.5;
			contact[i].surface.bounce_vel = 0.1;
*/
			if(module->collide())
			{
				if(module->bounce())
				{
					contact[i].surface.mode = dContactBounce;
					contact[i].surface.mu = dInfinity;
					contact[i].surface.mu2 = 0;
					contact[i].surface.bounce = BounceScene;
					contact[i].surface.bounce_vel = BounceVelScene;
				}
				else
				{
					contact[i].surface.mode = dContactMu2;
					contact[i].surface.mu = dInfinity;
					contact[i].surface.mu2 = dInfinity;
				}
			}
			else
			{
				nlassert(o1!=0 && 02!=0);
				if(int numc = dCollide(o1,o2,numContact,&contact[0].geom,sizeof(dContact)))
				{
					entity->collideModules.insert(module);
				}
				return;
			}

			break;
		}
	}

	nlassert(o1!=0 && 02!=0);
	if(int numc = dCollide(o1,o2,numContact,&contact[0].geom,sizeof(dContact)))
	{

		/*
		if(entity!=0)
			entity->jointed = true;
		*/
		for(i=0; i<numc; i++)
		{
			dJointID c = dJointCreateContact(World, ContactGroup, contact+i);
			dJointAttach(c, b1, b2);

			if(ct == CLIENT_SCENE)
			{
				nlassert(entity && module);

				entity->collideModules.insert(module);

				if(module->bounce())
					entity->ColState += " CSB";
				else
					entity->ColState += " CSNB";
				
				//nlinfo("Entity just touch a score(0x%p) = %d", module, module->Score);

				//SKEET_WARNING
				//entity->CurrentScore = module->score();
				entity->Accel = module->accel();
				entity->Friction = module->friction();
				
				// disable the command if the entity touch a target
				
				//if(entity->CurrentScore > 0)
				//{
				//	entity->FreezeCommand = true;
					
					//	nlinfo("entity %s just frozen because touch a score", entity->Name.c_str());
				//}
				

				if(entity->OpenClose)
				{
					if(!entity->FreezeCommand)
					{
						entity->SendCollideWhenFly = true;
						entity->CollideWhenFlyPos = CVector(contact[i].geom.pos[0],contact[i].geom.pos[1],contact[i].geom.pos[2]);
						nlinfo("entity %s just frozen because touch a scene in open mode at pos(%f,%f,%f)", entity->name().c_str(),entity->CollideWhenFlyPos.x,entity->CollideWhenFlyPos.y,entity->CollideWhenFlyPos.z);
					}

					entity->CurrentScore = 0;
					entity->NbOpenClose = entity->MaxOpenClose;
					entity->FreezeCommand = true;
					entity->InGame = false;
					dBodySetLinearVel(entity->Body, 0.0f, 0.0f, 0.0f);
					entity->Force = CVector::Null;
				}

//				if(entity->Name != "dummy")
//					nlinfo("%s current score is %d", entity->Name.c_str(), entity->CurrentScore);
			}
			else if(ct == CLIENT_WATER)
			{
				entity->collideWater = true;
				entity->ColState += " CW";

				if(!entity->FreezeCommand)
				{
					if(entity->OpenClose)
						entity->InGame = false;

					//entity->NbOpenClose = entity->MaxOpenClose;
					//entity->FreezeCommand = true;
					dBodySetLinearVel(entity->Body, 0.0f, 0.0f, 0.0f);
					entity->Force = CVector::Null;
					entity->OnTheWater = true;
					//entity->CurrentScore = 0;
//					nlinfo("on the water!!!");
				}
				
				entity->Friction = 999;
//				nlinfo(">>Water<<");
//	nlinfo("entity %s just frozzen because on the water", entity->Name.c_str());
			}
			else if(ct == CLIENT_CLIENT)
			{
				entity->collideEntity.insert(entity2);
				entity->ColState += " CC";
			}
			dJointDestroy(c);
		}
	}
}

static float worldStep = 0.001f;

class PhysicsThread : public IRunnable
{
	void run()
	{
		PhysicThreadId = getThreadId();
		static TTime lastTime = CTime::getLocalTime();
		static float deltaTime = worldStep;
		while(true)
		{
			TTime newTime = CTime::getLocalTime();
			deltaTime = (float)(newTime - lastTime)/1000.0f;
			int   nbLoop = int(deltaTime / worldStep);
			float missedTime = deltaTime - nbLoop * worldStep;
			//nlinfo("physic nbloop = %d = %f / %f (missed %f)",nbLoop,deltaTime,worldStep,missedTime);
			lastTime = newTime-(TTime)missedTime;
			{
				SyncPhyTime::CAccessor acces(&syncPhyTime);
				acces.value() = newTime;
				//				nlinfo("%f %"NL_I64"d", (float)(newTime-syncStartPhyTime)/1000.0f, newTime);
			}

			{
				CEntityManager::EntityConstIt it;
				for(int step=0;step<nbLoop;step++)
				{
					//_CrtCheckMemory();

					for(it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
					{
						CEntity *e = (*it);

						e->ColState = "";

						if(!e->InGame)
						{
							dBodySetLinearVel(e->Body, 0, 0, 0);
						}
						else
						{
							//SKEET_WARNING tentative de virer le bug du score a zero sur tricol
							//e->CurrentScore = 0;
							if(e->OpenClose)
							{
								if(e->FreezeCommand)
								{
									dBodyAddForce(e->Body, 0, 0, 0);
								}
								else
								{
									// we must set the friction force for planning
									dMass mass;
									dBodyGetMass(e->Body, &mass);
									//dBodyAddForce(e->Body, e->ForceX, e->ForceY, + e->ForceZ - mass.mass * Gravity * 0.8);

									float angle = e->Force.z - 0.5f;
									angle = angle * 3.1415f;
									float hspeed,zspeed;
									if(angle<0.0f)
										hspeed =  OpenAccelCoef;
									else
									{
										hspeed = OpenAccelCoef * (float)cos(angle);
										if(hspeed<OpenMinHSpeed)
											hspeed=OpenMinHSpeed;
									}
									float minAngleToZSpeed = 1.0f - OpenMinAngleToZSpeed;
									if(e->Force.z<minAngleToZSpeed)
										zspeed = - OpenZSpeed * (float)sin(((minAngleToZSpeed - e->Force.z) / minAngleToZSpeed) * 3.1415f * 0.5f);
									else
										zspeed = 0;
									if(-zspeed<OpenMinZSpeed)
										zspeed=-OpenMinZSpeed;
									double dx = cos(e->Force.x) * hspeed;
									double dy = sin(e->Force.x) * hspeed;
									dBodyAddForce(e->Body, (dReal)dx, (dReal)dy, zspeed);
									//nlinfo("angle = %f,  zspeed = %f ,   hspeed = %f",angle,zspeed,hspeed);
	/*
									dMatrix3 R;
									dRFromEulerAngles(R,e->ForceZ * 3.1415 - 3.1415 / 2.0f , -0.5f * (e->ForceX + 3.1415 / 2.0f),0);
									dBodySetRotation(e->Body,R);
	*/
								}
								dBodySetLinearVel(e->Body, 0, 0, 0);
								dBodySetAngularVel(e->Body, 0, 0, 0);
							}
							else
							{
								const dReal *curentLenVel;
								if(e->Accel)
									dBodyAddForce(e->Body, e->Force.x, e->Force.y, e->Force.z);
								curentLenVel = dBodyGetLinearVel(e->Body);
								CVector currentLinearVelocity(curentLenVel[0],curentLenVel[1],curentLenVel[2]);
								if(e->maxLinearVelocity()>0 && currentLinearVelocity.norm()>e->maxLinearVelocity())
								{
									currentLinearVelocity.normalize();
									currentLinearVelocity *= e->maxLinearVelocity();
									dBodySetAngularVel(e->Body, currentLinearVelocity.x,currentLinearVelocity.y,currentLinearVelocity.z);
								}
							}

							//if(e->NbOpenClose >= e->MaxOpenClose)
							if(e->Friction)
							{
								if(e->Friction>1000)
									e->Friction = 1000.0f;
								const dReal *avel = dBodyGetAngularVel(e->Body);
								dReal friction = 1.0f - (e->Friction / 1000.0f);
								dBodySetAngularVel(e->Body, avel[0]*friction, avel[1]*friction, avel[2]*friction);
							}
						}
						e->Friction = e->DefaultFriction;
	//					e->jointed = false;
						if(e->type()==CEntity::Bot && !e->FreezeCommand && e->InGame && e->NbOpenClose<e->MaxOpenClose)
						{
							CVector v;
							CBot *b = (CBot *)e;
							b->getBotDelta(v);
							if(!v.isNull())
							{
								const dReal *oldp;
								oldp = dBodyGetPosition(b->Body);
								dBodySetPosition(b->Body, oldp[0]+v.x,oldp[1]+v.y,oldp[2]+v.z);
							}
						}

					}

					{
						CSynchronized<dSpaceID>::CAccessor acces(&Space);
						dSpaceCollide(acces.value(), 0, &nearCallback);
						dWorldStep(World, worldStep);
					}


					// reset bad evaluation
					for(it = CEntityManager::instance().entities().begin(); it != CEntityManager::instance().entities().end(); it++)
					{
						const dReal *vel = dBodyGetLinearVel((*it)->Body);
						if(fabs(vel[0])>1000.0 || fabs(vel[1])>1000.0 || fabs(vel[2])>1000.0)
							dBodySetLinearVel((*it)->Body, 0.0f, 0.0f, 0.0f);
					}

					dJointGroupEmpty(ContactGroup);
				}
			
				/*
				CLuaEngine::levelUpdate();
				for(it = acces.value().begin(); it != acces.value().end(); it++)
				{
					CEntity *entity = (*it);
					CLuaEngine::clientUpdate(entity);
				}
				*/
				
				/*CollisionEntityListIt eit;
				for(eit = CollisionEntityList.begin(); eit != CollisionEntityList.end(); eit++)
				{
					CCollisionEntity *entity = &(*eit);
					CLuaEngine::collisionEntityUpdate(entity);
				}*/
			}
			nlSleep(10);
			checkPhysicPaused();
		}
	}
	
};

static PhysicsThread *physicThread = 0;

//
// Functions
//
void initPhysics()
{
	World = dWorldCreate();
	nlassert(World);
	dWorldSetGravity(World, 0.0f, 0.0f, 0.0f);
	nlinfo("set gravity : off");
	//dWorldSetERP(World,0.2f);
	//decrease CFM to prevent bodies from follow module edge when 2 modules are in contact
	dWorldSetCFM(World,(float)1e-2); //default is 1e-5
	
	{
		CSynchronized<dSpaceID>::CAccessor acces(&Space);
		acces.value() = dHashSpaceCreate(0);
		nlassert(acces.value());
	}
	
	ContactGroup = dJointGroupCreate(0);
	nlassert(ContactGroup);
	
//ace	mtLevelManager::init(IService::getInstance()->ConfigFile, moduleCallback, startPointCallback, cameraCallback, resetCallback, optionCallback, boxCallback, triCollCallback);
	//loadFakeTriMesh();
	
	{
		CSynchronized<dSpaceID>::CAccessor acces(&Space);
		Water = dCreatePlane(acces.value(), 0.0f, 0.0f, 1.0f, 0.0f);
	}
	nlassert(Water);
	dGeomSetCategoryBits(Water, MT_SCENE_SPECTRUM); //ceci est un element de la scene
	dGeomSetCollideBits(Water, MT_CLIENT_SPECTRUM); //collide unikement avec les entity
	
	//		addClient("dummy");
	
	physicThread = new PhysicsThread;
	thread = IThread::create(physicThread);
	thread->start();
}


void releasePhysics()
{
	if(thread)
	{
		thread->terminate();
		delete thread;
		if(physicThread)
			delete physicThread;
	}

//	CollisionEntityList.clear();
//	CLuaEngine::stop();
	
	if(ContactGroup)
	{
		dJointGroupEmpty(ContactGroup);
		dJointGroupDestroy(ContactGroup);
		ContactGroup = 0;
	}

	{
		CSynchronized<dSpaceID>::CAccessor acces(&Space);
		
		if(acces.value())
		{
			dSpaceDestroy(acces.value());
			acces.value() = 0;
		}
	}

	if(World)
	{
		dWorldDestroy(World);
		World = 0;
	}

	 dCloseODE();
}


//
// Commands
//

NLMISC_COMMAND(addOpenAccelCoef, "add accel coef", "") { OpenAccelCoef += 1.0f; return true; }
NLMISC_COMMAND(subOpenAccelCoef, "sub accel coef", "") { OpenAccelCoef -= 1.0f; return true; }

NLMISC_COMMAND(addCloseAccelCoef, "add accel coef", "") { CloseAccelCoef += 1.0f; return true; }
NLMISC_COMMAND(subCloseAccelCoef, "sub accel coef", "") { CloseAccelCoef -= 1.0f; return true; }

NLMISC_COMMAND(addGravity, "add gravity", "") { Gravity += 0.5f; return true; }
NLMISC_COMMAND(subGravity, "sub gravity", "") { Gravity -= 0.5f; return true; }

#define DISP_VAR(__name) log.displayNL("%s = %g (from %s)", #__name, __name, (__name##FromCfg?"cfg":"level"));

/* TODO: mettre ca dans level.cpp
NLMISC_COMMAND(displayLevel, "display the current level", "")
{
	log.displayNL("LevelName = %s (from %s)", LevelName.c_str(), (LevelNameFromCfg?"cfg":"level"));
	log.displayNL("Author = %s (from %s)", Author.c_str(), (AuthorFromCfg?"cfg":"level"));
	
	DISP_VAR(DefaultMaxOpenClose);
	DISP_VAR(TimeBeforeStart);
	DISP_VAR(TimeBeforeRestart);
	DISP_VAR(TimeBeforeCheck);
	DISP_VAR(TimeTimeout);
	DISP_VAR(NbWaitingClients);
	DISP_VAR(OpenAccelCoef);
	DISP_VAR(CloseAccelCoef);
	DISP_VAR(SphereDensity);
	DISP_VAR(Gravity);
	DISP_VAR(BounceWater);
	DISP_VAR(BounceVelWater);
	DISP_VAR(BounceClient);
	DISP_VAR(BounceVelClient);
	DISP_VAR(BounceScene);
	DISP_VAR(BounceVelScene);
	DISP_VAR(MinVelBeforeEnd);
	DISP_VAR(NbMaxClients);
	DISP_VAR(ModuleScore);
	DISP_VAR(ModuleBounce);
	DISP_VAR(ModuleAccel);
	DISP_VAR(ModuleFriction);

	std::list<CCollisionEntity>::iterator it;
	sint i;
	log.displayNL("The level contains %d collision entity:", CollisionEntityList.size());
	for(i = 0, it = CollisionEntityList.begin(); it != CollisionEntityList.end(); it++, i++)
	{
		CVector pos;
		(*it).getPosition(pos);
		log.displayNL("Collision entity %d :'%s' pos %.2g %.2g %.2g", i, (*it).Name.c_str(), pos.x, pos.y, pos.z);
		log.displayNL("  score %d accel %g friction %g %sbounce luafct '%s'", (*it).Score, (*it).Accel, (*it).Friction, ((*it).Bounce?"":"no "), (*it).LuaFunctionName.c_str());
	}

	return true;
}
*/


void checkPhysicPaused()
{
	{
		bool pause;
		{
			CSynchronized<PauseFlags>::CAccessor acces(&physicPauseFlags);
			pause = acces.value().pauseCount>0;
			if(pause)
				acces.value().ackPaused = true;
		}
		while (pause) 
		{
			nlSleep(10);
			{
				CSynchronized<PauseFlags>::CAccessor acces(&physicPauseFlags);
				pause = acces.value().pauseCount>0;
				acces.value().ackPaused = true;
			}
		}
	}
	{
		CSynchronized<PauseFlags>::CAccessor acces(&physicPauseFlags);
		acces.value().ackPaused = false;
	}	
}

bool pausePhysics(bool waitAck)
{
	/*
	bool pause;
	{
		CSynchronized<PauseFlags>::CAccessor acces(&physicPauseFlags);
		pause = acces.value().pauseCount>0;
	}
	if(!pause) 
	*/
	{
		bool ackPaused;
		{
			CSynchronized<PauseFlags>::CAccessor acces(&physicPauseFlags);
			ackPaused = false;
			acces.value().ackPaused = false;
			acces.value().pauseCount++;
		}
		if(!waitAck) return true;
		while(!ackPaused)
		{
			nlSleep(10);
			{
				CSynchronized<PauseFlags>::CAccessor acces(&physicPauseFlags);
				ackPaused = acces.value().ackPaused;
			}
		}
	}
	return true;
}

bool isPhysicsPaused()
{
	bool ackPaused;
	{
		CSynchronized<PauseFlags>::CAccessor acces(&physicPauseFlags);
		ackPaused = acces.value().ackPaused;
	}
	return ackPaused;
}

void resumePhysics()
{
	CSynchronized<PauseFlags>::CAccessor acces(&physicPauseFlags);
	if(acces.value().pauseCount>0) 
	{
		acces.value().pauseCount--;
		nlassert(acces.value().pauseCount>=0);
		if(acces.value().pauseCount==0)
			acces.value().ackPaused = false;
	}	
}

