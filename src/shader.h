#pragma once

#include "resource.h"
#include "triangle.h"

#include <string>
#include <unordered_map>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace tiny
{
	struct Uniform
	{
		enum KIND
		{
			FLOAT,
			INT,
			VEC3,
			VEC4,
			MAT3,
			MAT4,
			Texture2D
		};
		KIND kind;

		union Value
		{
			float as_decimal;
			int as_integer;
			glm::vec3 as_vec3;
			glm::vec4 as_vec4;
			glm::mat3 as_mat3;
			glm::mat4 as_mat4;
			Image* as_image;
		};
		Value value;
	};

	struct Shader: Resource
	{
		// TODO: find a way to pass data between the two shaders

		// input is a position in local coordinate system
		// output is a position in viewport space
		void(*vertex_shader)(Shader* shader, tiny::Triangle& triangle);

		// input is a position in viewport space
		// output is the color of the fragment
		TGAColor(*fragment_shader)(Shader* shader, glm::vec3 normal, const TGAColor& color);
		std::unordered_map<std::string, Uniform> uniforms;
	};

	void
	shader_set_uniform(Shader* shader, const std::string& name, const Uniform& uniform);

	Shader*
	shader_lit_new(Resource_Manager* resource_manager, const std::string& tag);

	Shader*
	shader_diffuse_new(Resource_Manager* resource_manager, const std::string& tag);

	Shader*
	shader_wireframe_new(Resource_Manager* resource_manager, const std::string& tag);
};
