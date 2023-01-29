#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2) override;

	btVector4 quatToAxisAngle(btQuaternion quat);

public:

	PhysVehicle3D* vehicle_car;
	float turn;
	float acceleration;
	float brake;
	int vidas = 3;

	float posicion_Spawn[3] = { 0, 15, 0 };
	btScalar rotacion_Spawn = 0;

	bool win = false;
	bool death = false;
	bool inicio = false;

	p2DynArray<Cube*> lista_Plataformas;


	float Superficie;
	float Velocidad_x;
	float Fuerza_dx;
	float Fuerza_dy;
	bool aero_d_s = 0;
	float aero_drag = 0.5f;
};