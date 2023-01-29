#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"
#include "ModuleCamera3D.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle_car(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(2.5, 1.3, 4);
	car.chassis_offset.Set(0, 1, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Life properties ---------------------------------------
	float life_connection_height = 2.0f;
	float life_x = 0.83f;
	float life_y = 0.5f;
	float life_z = 0.5;
	float life_suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	//Life L
	car.num_lifes = 1;
	car.life1 = new Life;
	//Life L
	car.life1[0].active = true;
	car.life1[0].life_offset.Set(0.835, 1.8, -2);
	car.life1[0].size_life.x = life_x;
	car.life1[0].size_life.y = life_y;
	car.life1[0].size_life.z = life_z;
	//Life L
	car.life2 = new Life;
	//Life M
	car.life2[0].active = true;
	car.life2[0].life_offset.Set(0, 1.8, -2);
	car.life2[0].size_life.x = life_x;
	car.life2[0].size_life.y = life_y;
	car.life2[0].size_life.z = life_z;
	//Life L
	car.life3 = new Life;
	//Life M
	car.life3[0].active = true;
	car.life3[0].life_offset.Set(-0.835, 1.8, -2);
	car.life3[0].size_life.x = life_x;
	car.life3[0].size_life.y = life_y;
	car.life3[0].size_life.z = life_z;


	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width + 0.1f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width - 0.1f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;


	vehicle_car = App->physics->AddVehicle(car);
	vehicle_car->coli_player = true;
	vehicle_car->SetPos(posicion_Spawn[0], posicion_Spawn[1], posicion_Spawn[2]);
	vehicle_car->collision_listeners.add(this);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{

	turn = acceleration = brake = 0.0f;

	Velocidad_x = vehicle_car->GetKmh() * 3600 / 1000;
	Superficie = (vehicle_car->info.chassis_size.x * 0.5)*(vehicle_car->info.chassis_size.y * 0.5);
	Fuerza_dx = Velocidad_x * Velocidad_x * Superficie * aero_drag;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		acceleration = MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		acceleration = -MAX_ACCELERATION * 0.7;
	}

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
	}
	//Sacar vidas
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (vidas == 1) {
			vehicle_car->info.life3->active = false;
			vidas -= 1;
		}
		if (vidas == 2) {
			vehicle_car->info.life2->active = false;
			vidas -= 1;
		}
		if (vidas == 3) {
			vehicle_car->info.life1->active = false;
			vidas -= 1;
		}
	}
	//Resetear vidas
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		vehicle_car->info.life1->active = true;
		vehicle_car->info.life2->active = true;
		vehicle_car->info.life3->active = true;
		vidas = 3;
	}
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		if (aero_drag != 0) {
			aero_d_s = aero_drag;
			aero_drag = 0;
		}
		if (aero_drag == 0) {
			aero_drag = aero_d_s;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
	{
		aero_drag = +100;
	}
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		aero_drag = -100;
	}
	//Resetea posicion
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		turn = acceleration = brake = 0.0f;

		btTransform tr;
		tr.setIdentity();
		btQuaternion quat;
		quat.setEulerZYX(0, rotacion_Spawn, 0);
		tr.setRotation(quat);
		tr.setOrigin(btVector3(posicion_Spawn[0], posicion_Spawn[1], posicion_Spawn[2]));

		vehicle_car->vehicle->getRigidBody()->setCenterOfMassTransform(tr);
		turn = acceleration = brake = 0.0f;
		vehicle_car->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
		vehicle_car->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));
	}
	//Modificar massa veiculo
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		vehicle_car->info.mass += 100.0f;
	}
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		vehicle_car->info.mass += 100.0f;
	}
	if (win == true) {
		win = false;
		btTransform tr;
		tr.setIdentity();
		btQuaternion quat;
		quat.setEulerZYX(0, rotacion_Spawn, 0);
		tr.setRotation(quat);
		tr.setOrigin(btVector3(posicion_Spawn[0], posicion_Spawn[1], posicion_Spawn[2]));

		vehicle_car->vehicle->getRigidBody()->setCenterOfMassTransform(tr);
		turn = acceleration = brake = 0.0f;
		vehicle_car->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
		vehicle_car->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));

	}


	vehicle_car->ApplyEngineForce(acceleration);
	vehicle_car->Turn(turn);
	vehicle_car->Brake(brake);

	vehicle_car->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle_car->GetKmh());
	App->window->SetTitle(title);

	//Camera Position
	//vec3 posicion = vec3(vehicle->vehicle->getChassisWorldTransform().getOrigin().getX(),
	//					 vehicle->vehicle->getChassisWorldTransform().getOrigin().getY(),
	//					 vehicle->vehicle->getChassisWorldTransform().getOrigin().getZ());
	//App->camera->Look(vec3(posicion.x, posicion.y + 10, posicion.z + 10), posicion);

	if (death == true) {

		turn = acceleration = brake = 0.0f;

		btTransform tr;
		tr.setIdentity();
		btQuaternion quat;
		quat.setEulerZYX(0, rotacion_Spawn, 0);
		tr.setRotation(quat);
		tr.setOrigin(btVector3(posicion_Spawn[0], posicion_Spawn[1], posicion_Spawn[2]));

		vehicle_car->vehicle->getRigidBody()->setCenterOfMassTransform(tr);
		turn = acceleration = brake = 0.0f;
		vehicle_car->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
		vehicle_car->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));

		death = false;
	}

	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

	if (body1->coli_player == true && body2->coli_win == true) {

		win = true;

		btTransform tr;
		tr.setIdentity();
		btQuaternion quat;
		quat.setEulerZYX(0, rotacion_Spawn, 0);
		tr.setRotation(quat);
		tr.setOrigin(btVector3(posicion_Spawn[0], posicion_Spawn[1], posicion_Spawn[2]));

		vehicle_car->vehicle->getRigidBody()->setCenterOfMassTransform(tr);
		turn = acceleration = brake = 0.0f;
		vehicle_car->vehicle->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
		vehicle_car->vehicle->getRigidBody()->setAngularVelocity(btVector3(0, 0, 0));

	}

	if (body1->coli_player == true && body2->coli_death == true){

		if (vidas >= 1)
		{
			if (vidas == 1) {
				vehicle_car->info.life3->active = false;
				vidas -= 1;
			}
			if (vidas == 2) {
				vehicle_car->info.life2->active = false;
				vidas -= 1;
			}
			if (vidas == 3) {
				vehicle_car->info.life1->active = false;
				vidas -= 1;
			}

		}
		else {
			vehicle_car->info.life1->active = true;
			vehicle_car->info.life2->active = true;
			vehicle_car->info.life3->active = true;
			vidas = 3;
		}
		death = true;
	}
}


