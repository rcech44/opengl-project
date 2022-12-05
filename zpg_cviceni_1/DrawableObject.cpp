#pragma once

#include "DrawableObject.h"
#include "ObjectMovement.h"
#include "Scene.h"

DrawableObject::DrawableObject(Model* m, Shader* s, Scene* scene, int id)
{
	this->model = m;
	this->shader = s;
	this->scene = scene;
	this->id = id;
	switch (m->getName())
	{
		case 0:
			this->name = "Monkey";
			break;
		case 1:
			this->name = "Sphere";
			break;
		case 2:
			this->name = "Plain";
			break;
		case 3:
			this->name = "Tree";
			break;
		case 4:
			this->name = "Bush";
			break;
		case 5:
			this->name = "GiftBox";
			break;
		case 6:
			this->name = "SkyCube";
			break;
		case 7:
			this->name = "Building";
			break;
		case 8:
			this->name = "Zombie";
			break;
		case 9:
			this->name = "Tree2";
			break;
		case 10:
			this->name = "Plain2";
			break;
		case 11:
			this->name = "Box";
			break;
	}
}

void DrawableObject::render()
{
	// Render this object

	// Get important variables
	std::vector<Light>* lights = scene->getLights();
	glm::vec3 camera_position = scene->getCamera()->getPosition();
	glm::vec3 camera_direction = scene->getCamera()->getTarget();

	// Use current shader so we can apply changes to it
	this->shader->useProgram();

	// Render object based on their types (constant color objects, textured objects...)
	switch (this->shader->getShaderType())
	{
		// Render all constant color objects
		case LightSource:
		case ConstantObject:
			this->shader->vec3Insert(this->color, "objectColor");
			break;

		// Render skybox
		case SkyBox:
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texture);

			// Disable depth mask - to ensure skybox is always on background
			glDepthMask(GL_FALSE);

			// Insert matrices, remove translations, but keep rotations
			this->shader->matrixInsert(glm::mat4(glm::mat3(this->scene->getCamera()->getView())), ViewMatrix);
			this->shader->matrixInsert(glm::mat4(this->scene->getCamera()->getProjection()), ProjectionMatrix);
			this->shader->intInsert(0, "UISky");

			// All fragments should pass stencil test and are allowed to write (0xFF)
			glStencilFunc(GL_ALWAYS, this->getID(), 0xFF);
			this->model->render();

			// Re-enable depth mask
			glDepthMask(GL_TRUE);
			break;

		// Render all objects with light logic with textures but without normals
		case StandardObject:
		case StandardObjectTextured:
		case StandardObjectTexturedNormal:
			if (this->shader->getShaderType() == StandardObjectTextured)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, this->texture);
				this->shader->intInsert(0, "textureUnitID");
			}
			else if (this->shader->getShaderType() == StandardObjectTexturedNormal)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, this->texture);
				this->shader->intInsert(0, "textureUnitID_texture");

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, this->normal);
				this->shader->intInsert(1, "textureUnitID_normal");
			}
			else
			{
				this->shader->vec3Insert(this->color, "objectColor");
			}
			this->shader->intInsert(lights->size(), "lightCount");
			this->shader->vec3Insert(camera_position, "viewPos");

			// Insert all light info into the shader
			for (size_t i = 0; i < lights->size(); i++)
			{
				std::string l_pos = "lights[" + std::to_string(i) + "].position";
				std::string l_color = "lights[" + std::to_string(i) + "].color";
				std::string l_type = "lights[" + std::to_string(i) + "].type";
				std::string l_cutoff = "lights[" + std::to_string(i) + "].cut";
				std::string l_out_cutoff = "lights[" + std::to_string(i) + "].out_cut";
				std::string l_dir = "lights[" + std::to_string(i) + "].direction";
				std::string l_strength = "lights[" + std::to_string(i) + "].strength";

				this->shader->intInsert(lights->at(i).type, l_type.c_str());
				this->shader->vec3Insert(lights->at(i).position, l_pos.c_str());
				this->shader->vec3Insert(lights->at(i).color, l_color.c_str());
				this->shader->vec3Insert(lights->at(i).direction, l_dir.c_str());
				this->shader->floatInsert(lights->at(i).strength, l_strength.c_str());

				// Process some light types 
				switch (lights->at(i).type)
				{
					case LightType::SpotlightCamera:

						// Check if flashlight is turned on or off
						if (scene->flashlightStatus())
						{
							this->shader->intInsert(1, "flashlightEnabled");
							this->shader->vec3Insert(camera_position, l_pos.c_str());
							this->shader->floatInsert(lights->at(i).cutoff, l_cutoff.c_str());
							this->shader->floatInsert(lights->at(i).outer_cutoff, l_out_cutoff.c_str());
							this->shader->vec3Insert(camera_direction, l_dir.c_str());
						}
						else this->shader->intInsert(0, "flashlightEnabled");
						break;

					case LightType::Spotlight:
						this->shader->floatInsert(lights->at(i).cutoff, l_cutoff.c_str());
						this->shader->floatInsert(lights->at(i).outer_cutoff, l_out_cutoff.c_str());
						break;
				}
			}
			break;

	}

	// Finally render object (except Skybox - has own custom rendering)
	if (this->shader->getShaderType() != SkyBox)
	{
		// Re-apply last camera matrices
		this->shader->applyCamera();

		// Apply transformations
		this->shader->matrixInsert(transform(), TransformMatrix);

		// Register object with its ID into stencil buffer and allow to write
		glStencilFunc(GL_ALWAYS, this->getID(), 0xFF);

		// Render...
		this->model->render();
	}
}

void DrawableObject::setPosition(glm::vec3 pos)
{
	this->last_position = glm::translate(glm::mat4{1.f}, pos);
}

glm::mat4 DrawableObject::transform()
{
	// Apply all transformations in order
	for (auto& t : transformations)
	{
		switch (t.getType())
		{
			case Translation:
				this->last_position = glm::translate(last_position, t.getValues());
				break;
			case Rotation:
				// Rotate by all three axis
				this->last_position = glm::rotate(last_position, t.getValues().x, { 1.f, 0.f, 0.f });
				this->last_position = glm::rotate(last_position, t.getValues().y, { 0.f, 1.f, 0.f });
				this->last_position = glm::rotate(last_position, t.getValues().z, { 0.f, 0.f, 1.f });
				break;
			case RotationAxis:
				// Rotate by given axis
				this->last_position = glm::rotate(last_position, t.getAngle(), t.getValues());
				break;
			case Scale:
				this->last_position = glm::scale(last_position, t.getValues());
				break;
		}
	}

	// After transformations are done, clear the vector
	transformations.clear();

	return this->last_position;
}

void DrawableObject::addTransformation(glm::vec3 tf, int type)
{
	// Add transformation to "queue"
	this->transformations.push_back(Transformation(tf, type));
}

void DrawableObject::addTransformation(glm::vec3 tf, float angle, int type)
{
	// Add transformation to "queue"
	this->transformations.push_back(Transformation(tf, angle, type));
}

void DrawableObject::setColor(glm::vec3 color)
{
	this->color = color;
}

void DrawableObject::assignTexture(int t)
{
	this->texture = t;
}

void DrawableObject::assignTexture(int t, int n)
{
	this->texture = t;
	this->normal = n;
}

int DrawableObject::getID()
{
	return this->id;
}

glm::vec3 DrawableObject::getPosition()
{
	return glm::vec3(this->last_position[3][0], this->last_position[3][1], this->last_position[3][2]);
}

const char* DrawableObject::getName()
{
	return this->name;
}

void DrawableObject::assignMovement(ObjectMovement* m)
{
	this->movement = m;
}

void DrawableObject::move()
{
	if (this->movement)
	{
		this->movement->move(this);
	}
}