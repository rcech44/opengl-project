#pragma once

#include "DrawableObject.h"
#include "Scene.h"

DrawableObject::DrawableObject(Model* m, Shader* s, Scene* scene, int id)
{
	this->model = m;
	this->shader = s;
	this->scene = scene;
	this->id = id;
}

void DrawableObject::render()
{
	std::vector<Light>* lights = scene->getLights();
	glm::vec3 camera_position = scene->getCamera()->getPosition();
	glm::vec3 camera_direction = scene->getCamera()->getTarget();

	this->shader->useProgram();

	switch (this->shader->getShaderType())
	{
		case LightSource:
			this->shader->vec3Insert(this->color, "objectColor");
			break;

		// Render all objects with light logic
		case StandardObject:
			this->shader->intInsert(lights->size(), "lightCount");
			this->shader->vec3Insert(camera_position, "viewPos");
			this->shader->vec3Insert(this->color, "objectColor");
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

				// Process all light types 
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
				}
			}
			break;

		case ConstantObject:
			break;

		case SkyBox:
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			glDepthMask(GL_FALSE);
			this->shader->matrixInsert(glm::mat4(glm::mat3(this->scene->getCamera()->getView())), ViewMatrix);
			this->shader->matrixInsert(glm::mat4(this->scene->getCamera()->getProjection()), ProjectionMatrix);
			this->shader->intInsert(0, "UISky");
			glStencilFunc(GL_ALWAYS, this->getID(), 0xFF);
			this->model->render();
			glDepthMask(GL_TRUE);
			break;

		case StandardObjectTextured:
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			this->shader->intInsert(0, "textureUnitID");
			this->shader->intInsert(lights->size(), "lightCount");
			this->shader->vec3Insert(camera_position, "viewPos");
			//this->shader->vec3Insert(this->color, "objectColor");
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

				// Process all light types 
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
				}
			}
			break;

		case StandardObjectTexturedNormal:
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->texture);
			this->shader->intInsert(0, "textureUnitID_texture");

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, this->normal);
			this->shader->intInsert(1, "textureUnitID_normal");

			this->shader->intInsert(lights->size(), "lightCount");
			this->shader->vec3Insert(camera_position, "viewPos");
			//this->shader->vec3Insert(this->color, "objectColor");
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

				// Process all light types 
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
				}
			}
			break;
	}

	if (this->shader->getShaderType() != SkyBox)
	{
		this->shader->applyCamera();
		this->shader->matrixInsert(transform(), TransformMatrix);
		glStencilFunc(GL_ALWAYS, this->getID(), 0xFF);
		this->model->render();
	}
}

glm::mat4 DrawableObject::transform()
{
	for (auto& t : transformations)
	{
		switch (t.getType())
		{
			case Translation:
				this->last_position = glm::translate(last_position, t.getValues());
				break;
			case Rotation:
				this->last_position = glm::rotate(last_position, t.getValues().x, { 1.f, 0.f, 0.f });
				this->last_position = glm::rotate(last_position, t.getValues().y, { 0.f, 1.f, 0.f });
				this->last_position = glm::rotate(last_position, t.getValues().z, { 0.f, 0.f, 1.f });
				break;
			case Scale:
				this->last_position = glm::scale(last_position, t.getValues());
				break;
		}
	}

	transformations.clear();
	return this->last_position;
}

void DrawableObject::addTransformation(glm::vec3 tf, int type)
{
	this->transformations.push_back(Transformation(tf, type));
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
