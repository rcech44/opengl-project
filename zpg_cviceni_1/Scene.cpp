#pragma once

#include "Scene.h"
#include "Models/suziSmooth.h"
#include "Models/sphere.h"
#include "Models/gift.h"
#include "Models/tree.h"
#include "ShadersCollection.h"

// Name conflict - both const float plain[]
namespace plain
{
	#include "Models/plain.h"
}
namespace bushes
{
	#include "Models/bushes.h"
}


void Scene::addObject(DrawableObject* obj)
{
	this->objects.emplace_back(*obj);
	printf("[SCENE] Added one object to render vector. Current objects: %d\n", objects.size());
}

Camera* Scene::getCamera()
{
	return &camera;
}

void Scene::update()
{
	glClearColor(0.07f, 0.07f, 0.07f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//camera.apply();
	for (auto& d : objects)
	{
		//d.addRotation(glm::vec3(0.f, -0.003f, 0.f));
		camera.apply();
		d.render(getCamera());
	}
}

void Scene::init()
{
	// Create shaders
	Shader* sh1 = new Shader("Shaders/light_phong.fs", "Shaders/light_camera.vs", StandardObject);				// standard shader
	Shader* sh2 = new Shader("Shaders/fs1.fs", "Shaders/vs1.vs", LightSource);									// light source shader
	Shader* sh3 = new Shader("Shaders/fs1.fs", "Shaders/vs1.vs", ConstantObject);								// object with constant color shader

	// Create objects
	Model* m1 = new Model(suziSmooth, sizeof(suziSmooth), GL_TRIANGLES, 2904, 3, 3);
	Model* m2 = new Model(sphere, sizeof(sphere), GL_TRIANGLES, 2880, 3, 3);
	//Model* m3 = new Model(cube1, sizeof(cube1), GL_TRIANGLES, 36, 4, 4);
	Model* m4 = new Model(plain::plain, sizeof(plain::plain), GL_TRIANGLES, 6, 3, 3);
	Model* m5 = new Model(tree, sizeof(tree), GL_TRIANGLES, 92814, 3, 3);
	Model* m6 = new Model(bushes::plain, sizeof(bushes::plain), GL_TRIANGLES, 8730, 3, 3);
	Model* m7 = new Model(gift, sizeof(gift), GL_TRIANGLES, 66624, 3, 3);

	// Models init
	m1->set();
	m2->set();
	//m3->set();
	m4->set();
	m5->set();
	m6->set();
	m7->set();

	// Shaders init
	sh1->set();
	sh2->set();
	sh3->set();

	// Scene 0 - high number of trees, monkeys, gift, bushes and spheres randomly placed with light source above with "skybox" and ground
	// Scene 1 - four spheres around light source 
	int set_scene = 0;

	switch (set_scene)
	{
		case 0:
		{	
			int tree_count = 300;
			int monkey_count = 50;
			int sphere_count = 50;
			int bushes_count = 150;
			int gift_count = 50;
			srand(static_cast <unsigned> (time(0)));
			for (int i = 0; i < tree_count; i++)
			{
				DrawableObject do_x = DrawableObject(m5, sh1);
				float color = 0.3f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.9f - 0.3f)));
				do_x.setColor(glm::vec3(0.2f, color, 0.2f));
				// https://stackoverflow.com/questions/686353/random-float-number-generation
				float x = -70.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (70.0f - (-70.0f))));
				float y = -70.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (70.0f - (-70.0f))));
				do_x.move(glm::vec3(x, 1.0f, y));
				addObject(&do_x);
			}
			for (int i = 0; i < monkey_count; i++)
			{
				DrawableObject do_x = DrawableObject(m1, sh1);
				float color_r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				float color_g = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				float color_b = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				do_x.setColor(glm::vec3(color_r, color_g, color_b));
				// https://stackoverflow.com/questions/686353/random-float-number-generation
				float x = -30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30.0f - (-30.0f))));
				float y = -30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30.0f - (-30.0f))));
				do_x.move(glm::vec3(x, 1.0f, y));
				addObject(&do_x);
			}
			for (int i = 0; i < sphere_count; i++)
			{
				DrawableObject do_x = DrawableObject(m2, sh1);
				float color_r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				float color_g = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				float color_b = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				do_x.setColor(glm::vec3(color_r, color_g, color_b));
				// https://stackoverflow.com/questions/686353/random-float-number-generation
				float x = -30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30.0f - (-30.0f))));
				float y = -30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30.0f - (-30.0f))));
				float scale = 0.2f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f - (0.2f))));
				do_x.move(glm::vec3(x, 1.0f, y));
				do_x.scale(glm::vec3(scale, scale, scale));
				addObject(&do_x);
			}
			for (int i = 0; i < bushes_count; i++)
			{
				DrawableObject do_x = DrawableObject(m6, sh1);
				float color_r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				float color_g = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				float color_b = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				do_x.setColor(glm::vec3(color_r, color_g, color_b));
				// https://stackoverflow.com/questions/686353/random-float-number-generation
				float x = -30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30.0f - (-30.0f))));
				float y = -30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30.0f - (-30.0f))));
				float scale = 2.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.f - (2.f))));
				do_x.move(glm::vec3(x, 1.0f, y));
				do_x.scale(glm::vec3(scale, scale, scale));
				addObject(&do_x);
			}
			for (int i = 0; i < gift_count; i++)
			{
				DrawableObject do_x = DrawableObject(m7, sh1);
				float color_r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				float color_g = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				float color_b = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f)));
				do_x.setColor(glm::vec3(color_r, color_g, color_b));
				// https://stackoverflow.com/questions/686353/random-float-number-generation
				float x = -30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30.0f - (-30.0f))));
				float y = -30.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (30.0f - (-30.0f))));
				float scale = 2.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.f - (2.f))));
				do_x.move(glm::vec3(x, 1.0f, y));
				do_x.scale(glm::vec3(scale, scale, scale));
				addObject(&do_x);
			}
			DrawableObject do_ground = DrawableObject(m4, sh1);
			DrawableObject do_sky = DrawableObject(m2, sh3);
			do_ground.setColor(glm::vec3(0.6f, 0.3f, 0.0f));
			do_sky.setColor(glm::vec3(0.2f, 0.2f, 0.7f));
			do_ground.scale(glm::vec3(150.0f, 150.0f, 150.0f));
			do_sky.scale(glm::vec3(150.0f, 150.0f, 150.0f));
			addObject(&do_ground);
			addObject(&do_sky);
			break;	
		}

		case 1:
		{
			DrawableObject do1 = DrawableObject(m2, sh1);
			DrawableObject do2 = DrawableObject(m2, sh1);
			DrawableObject do3 = DrawableObject(m2, sh1);
			DrawableObject do4 = DrawableObject(m2, sh1);
			DrawableObject do5 = DrawableObject(m2, sh2);
			DrawableObject do6 = DrawableObject(m4, sh1);
			DrawableObject do7 = DrawableObject(m2, sh3);

			do1.setColor(glm::vec3(0.5f, 0.0f, 0.0f));
			do2.setColor(glm::vec3(0.0f, 0.5f, 0.0f));
			do3.setColor(glm::vec3(0.0f, 0.0f, 0.5f));
			do4.setColor(glm::vec3(0.5f, 0.5f, 0.0f));
			do5.setColor(glm::vec3(1.0f, 1.0f, 1.0f));
			do6.setColor(glm::vec3(0.6f, 0.3f, 0.0f));
			do7.setColor(glm::vec3(0.2f, 0.2f, 0.7f));

			do1.move(glm::vec3(-3.0f, 1.0f, 0.0f));
			do2.move(glm::vec3(0.0f, 1.0f, -3.0f));
			do3.move(glm::vec3(3.0f, 1.0f, 0.0f));
			do4.move(glm::vec3(0.0f, 1.0f, 3.0f));
			do5.scale(glm::vec3(0.4f, 0.4f, 0.4f));
			//do5.move(glm::vec3(0.0f, 3.f, 0.f));
			//do6.move(glm::vec3(0.0f, 0.0f, 0.0f));
			do6.scale(glm::vec3(200.0f, 200.0f, 200.0f));
			do6.move(glm::vec3(0.0f, -6.0f, 0.0f));
			do6.scale(glm::vec3(150.0f, 150.0f, 150.0f));
			do7.scale(glm::vec3(150.0f, 150.0f, 150.0f));

			addObject(&do1);
			addObject(&do2);
			addObject(&do3);
			addObject(&do4);
			addObject(&do5);
			addObject(&do6);
			addObject(&do7);
			break;
		}
	}

	// Register observers
	getCamera()->registerObserver(*sh1);
	getCamera()->registerObserver(*sh2);
	getCamera()->registerObserver(*sh3);
}
