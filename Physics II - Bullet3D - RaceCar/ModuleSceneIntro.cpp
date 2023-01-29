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

	//Death field
	CreateCube(1000.0, 3.0, 1000.0, 0, 0, 0, 0, 0, 0, 0, 3.0, 0, 0, 0.5);
	//Plataformas
	CreateCube(20, 3, 200, 0, 10 + 1.5, 40, 0, 0, 0, 0, 1.3, 1.3, 1.3, 1);
	//Victoria
	CreateCube(5,5,5, 0, 10 + 1.5 +2,150, 0, 0, 0, 0, 0, 0, 1, 1);
	//Rampa
	CreateCube(20, 20, 0.5, 0, 60, 40, 0, 0, 0, 0, 0, 0.1, 0.1, 0);
	CreateCube(0.5, 20, 0.5, 0, 60, 40, 0, 0, 0, 0, 0, 0.1, 0.1, 0);


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

	if(!mapa_Generado)
	{
		for (size_t i = 0; i < cubes.Count(); i++)
		{
			if (i == 0) {
				PhysBody3D* BoxCubo = App->physics->AddBody(*cubes.At(i), 0);
				BoxCubo->coli_death = true;
				cubes.At(i)->physObject = BoxCubo;
			}
			if (i == 1) {
				PhysBody3D* BoxCubo = App->physics->AddBody(*cubes.At(i), 0);
				BoxCubo->coli_platform = true;
				cubes.At(i)->physObject = BoxCubo;
			}
			if (i == 2) {
				PhysBody3D* BoxCubo = App->physics->AddBody(*cubes.At(i), 0);
				BoxCubo->coli_win = true;
				cubes.At(i)->physObject = BoxCubo;
			}
			if (i == 3) {
				PhysBody3D* cuboCreado = App->physics->AddBody(*cubes.At(i), 5);
				cubes.At(i)->physObject = cuboCreado;

			}
			else {
				PhysBody3D* cuboCreado = App->physics->AddBody(*cubes.At(i), 0);
				cubes.At(i)->physObject = cuboCreado;
			}
			//constraint
			//App->physics->AddConstraintHinge(*cubes.At(4)->physObject, *cubes.At(5)->physObject, vec3(10, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0), vec3(0, 1, 0), false);
		}

		mapa_Generado = true;
	}
	for (size_t i = 0; i < cubes.Count(); i++)
	{
		cubes.At(i)->Render();
	}
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateCube(float size_x, float size_y, float size_z, float pos_x, float pos_y, float pos_z, float rot_x, float rot_y, float rot_z,float angle, float r, float g, float b, float a)
{
	Cube box(size_x, size_y, size_z);
	//box.color = Red;
	box.color.r = r;
	box.color.g = g;
	box.color.b = b;
	box.color.a = a;
	box.SetPos(pos_x, pos_y, pos_z);

	if (angle != 0)
	{
		box.SetRotation(angle, vec3(rot_x, rot_y, rot_z));
	}

	cubes.Insert(box, cubes.Count());
}