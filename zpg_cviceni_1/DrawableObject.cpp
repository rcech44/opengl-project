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
	glm::vec3 viewPos = scene->getCamera()->getPosition();
	glm::vec3 viewDirection = scene->getCamera()->getTarget();

	this->shader->useProgram();
	int t;

	switch (this->shader->getShaderType())
	{
		// Render objects that act like light source - they have constant color
		case LightSource:
			if (this->light != nullptr)
			{
				setPosition(glm::vec3(-glm::cos(glm::radians(this->scene->orbit)) * 30, this->light->position.y, glm::sin(glm::radians(this->scene->orbit)) * 30));
			}
			this->shader->vec3Insert(this->color, "objectColor");
			break;

		// Render all objects with light logic
		case StandardObject:
			this->shader->intInsert(lights->size(), "lightCount");
			this->shader->vec3Insert(viewPos, "viewPos");
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

				// Process all light types
				switch (lights->at(i).type)
				{
					case LightType::SpotlightCamera:

						// Check if flashlight is turned on or off
						if (scene->flashlightStatus())
						{
							this->shader->intInsert(1, "flashlightEnabled");
							this->shader->vec3Insert(viewPos, l_pos.c_str());
							this->shader->vec3Insert(lights->at(i).color, l_color.c_str());
							this->shader->intInsert(lights->at(i).type, l_type.c_str());
							this->shader->floatInsert(lights->at(i).cutoff, l_cutoff.c_str());
							this->shader->floatInsert(lights->at(i).outer_cutoff, l_out_cutoff.c_str());
							this->shader->vec3Insert(viewDirection, l_dir.c_str());
							this->shader->floatInsert(lights->at(i).strength, l_strength.c_str());
						}
						else this->shader->intInsert(0, "flashlightEnabled");
						break;
					case LightType::PointOrbital:
						lights->at(i).setPosition(glm::vec3( -glm::cos(glm::radians(this->scene->orbit)) * 30, lights->at(i).position.y, glm::sin(glm::radians(this->scene->orbit)) * 30));
						this->shader->vec3Insert(lights->at(i).position, l_pos.c_str());
						this->shader->vec3Insert(lights->at(i).color, l_color.c_str());
						this->shader->intInsert(LightType::Point, l_type.c_str());
						this->shader->floatInsert(lights->at(i).cutoff, l_cutoff.c_str());
						this->shader->vec3Insert(lights->at(i).direction, l_dir.c_str());
						this->shader->floatInsert(lights->at(i).strength, l_strength.c_str());
						break;
					default:
						this->shader->vec3Insert(lights->at(i).position, l_pos.c_str());
						this->shader->vec3Insert(lights->at(i).color, l_color.c_str());
						this->shader->intInsert(lights->at(i).type, l_type.c_str());
						this->shader->floatInsert(lights->at(i).cutoff, l_cutoff.c_str());
						this->shader->vec3Insert(lights->at(i).direction, l_dir.c_str());
						this->shader->floatInsert(lights->at(i).strength, l_strength.c_str());
						break;
				}
			}
			break;

		// Render all objects that have constant color
		case ConstantObject:
			//this->shader->vec3Insert(this->color, "objectColor");
			//this->shader->intInsert(this->texture, "textureUnitID");
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
			this->shader->vec3Insert(viewPos, "viewPos");
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

				// Process all light types 
				switch (lights->at(i).type)
				{
				case LightType::SpotlightCamera:

					// Check if flashlight is turned on or off
					if (scene->flashlightStatus())
					{
						this->shader->intInsert(1, "flashlightEnabled");
						this->shader->vec3Insert(viewPos, l_pos.c_str());
						this->shader->vec3Insert(lights->at(i).color, l_color.c_str());
						this->shader->intInsert(lights->at(i).type, l_type.c_str());
						this->shader->floatInsert(lights->at(i).cutoff, l_cutoff.c_str());
						this->shader->floatInsert(lights->at(i).outer_cutoff, l_out_cutoff.c_str());
						this->shader->vec3Insert(viewDirection, l_dir.c_str());
						this->shader->floatInsert(lights->at(i).strength, l_strength.c_str());
					}
					else this->shader->intInsert(0, "flashlightEnabled");
					break;
				case LightType::PointOrbital:
					lights->at(i).setPosition(glm::vec3(-glm::cos(glm::radians(this->scene->orbit)) * 30, lights->at(i).position.y, glm::sin(glm::radians(this->scene->orbit)) * 30));
					this->shader->vec3Insert(lights->at(i).position, l_pos.c_str());
					this->shader->vec3Insert(lights->at(i).color, l_color.c_str());
					this->shader->intInsert(LightType::Point, l_type.c_str());
					this->shader->floatInsert(lights->at(i).cutoff, l_cutoff.c_str());
					this->shader->vec3Insert(lights->at(i).direction, l_dir.c_str());
					this->shader->floatInsert(lights->at(i).strength, l_strength.c_str());
					break;
				default:
					this->shader->vec3Insert(lights->at(i).position, l_pos.c_str());
					this->shader->vec3Insert(lights->at(i).color, l_color.c_str());
					this->shader->intInsert(lights->at(i).type, l_type.c_str());
					this->shader->floatInsert(lights->at(i).cutoff, l_cutoff.c_str());
					this->shader->vec3Insert(lights->at(i).direction, l_dir.c_str());
					this->shader->floatInsert(lights->at(i).strength, l_strength.c_str());
					break;
				}
			}
			break;
	}

	if (this->shader->getShaderType() == SkyBox)
	{
		//glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
	}
	else
	{
		this->shader->applyCamera();
		this->shader->matrixInsert(transform(), TransformMatrix);
		glStencilFunc(GL_ALWAYS, this->getID(), 0xFF);
		this->model->render();
	}
}

glm::mat4 DrawableObject::transform()
{
	glm::mat4 t = glm::translate(glm::mat4{ 1.f }, translation);
	t = glm::scale(t, size);
	t = glm::rotate(t, rotation.x, { 1.f, 0.f, 0.f });
	t = glm::rotate(t, rotation.y, { 0.f, 1.f, 0.f });
	t = glm::rotate(t, rotation.z, { 0.f, 0.f, 1.f });
	return t;
}

void DrawableObject::rotate(glm::vec3 rot)
{
	this->rotation = rot;
}

void DrawableObject::setPosition(glm::vec3 pos)
{
	this->translation = pos;
}

void DrawableObject::addRotation(glm::vec3 rot)
{
	/*if (auto_rotate_multiplier > 0.f) this->rotation += rot * auto_rotate_multiplier;
	else if (auto_rotate_multiplier == 0.f) */
	this->rotation += rot;
}

void DrawableObject::move(glm::vec3 difference)
{
	this->translation += difference;
}

void DrawableObject::scale(glm::vec3 scale)
{
	this->size = scale;
}

void DrawableObject::setAutoRotateSettings(float multiplier)
{
	this->auto_rotate_multiplier = multiplier;
}

void DrawableObject::setColor(glm::vec3 color)
{
	this->color = color;
}

void DrawableObject::assignLight(Light& l)
{
	this->light = &l;
}

void DrawableObject::assignTexture(int t)
{
	this->texture = t;
}

int DrawableObject::getID()
{
	return this->id;
}
