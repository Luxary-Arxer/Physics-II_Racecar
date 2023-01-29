#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePhysics3D.h"
#include "PhysVehicle3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateCube(float size_x, float size_y, float size_z, float pos_x, float pos_y, float pos_z, float rot_x, float rot_y, float rot_z, float angle, float r, float g, float b)
{
	Cube box(size_x, size_y, size_z);
	box.SetPos(pos_x, pos_y, pos_z);
	box.color.r = r;
	box.color.g = g;
	box.color.b = b;

	if (angle != 0)
	{
		box.SetRotation(angle, vec3(rot_x, rot_y, rot_z));
	}

	cubes.Insert(box, cubes.Count());
}