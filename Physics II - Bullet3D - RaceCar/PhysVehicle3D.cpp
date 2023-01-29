#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	chassis.Render();

	//Vidas
	Cube life(info.life1->size_life.x, info.life1->size_life.y, info.life1->size_life.z);

	if (info.life1->active == true) {
		life.color = Green;
	}
	else {
		life.color = Red;
	}
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&life.transform);
	btQuaternion q_lifes = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_lifes(info.life1->life_offset.x , info.life1->life_offset.y, info.life1->life_offset.z);
	offset_lifes = offset_lifes.rotate(q_lifes.getAxis(), q_lifes.getAngle());
	life.size = info.life1->size_life;
	life.transform.M[12] += offset_lifes.getX();
	life.transform.M[13] += offset_lifes.getY();
	life.transform.M[14] += offset_lifes.getZ();
	life.Render();
	//Vidas2
	Cube life_2(info.life2->size_life.x, info.life2->size_life.y, info.life2->size_life.z);

	if (info.life2->active == true) {
		life_2.color = Green;
	}
	else {
		life_2.color = Red;
	}
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&life_2.transform);
	btQuaternion q_life_2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_life_2(info.life2->life_offset.x, info.life2->life_offset.y, info.life2->life_offset.z);
	offset_life_2 = offset_life_2.rotate(q_life_2.getAxis(), q_life_2.getAngle());
	life_2.size = info.life2->size_life;
	life_2.transform.M[12] += offset_life_2.getX();
	life_2.transform.M[13] += offset_life_2.getY();
	life_2.transform.M[14] += offset_life_2.getZ();
	life_2.Render();
	//Vidas3
	Cube life_3(info.life3->size_life.x, info.life3->size_life.y, info.life3->size_life.z);
	if (info.life3->active == true) {
			life_3.color = Green;
	}
	else {
		life_3.color = Red;
	}
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&life_3.transform);
	btQuaternion q_lifes_3 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_life_3(info.life3->life_offset.x, info.life3->life_offset.y, info.life3->life_offset.z);
	offset_life_3 = offset_life_3.rotate(q_lifes_3.getAxis(), q_lifes_3.getAngle());
	life_3.size = info.life3->size_life;
	life_3.transform.M[12] += offset_life_3.getX();
	life_3.transform.M[13] += offset_life_3.getY();
	life_3.transform.M[14] += offset_life_3.getZ();
	life_3.Render();

}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}