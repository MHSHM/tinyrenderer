#include "shader.h"
#include "Math.h"

namespace tiny 
{
	inline static void
	_load(struct Resource* resource, Resource_Manager* resources_manager, const std::string& path)
	{
		// TODO:
	}

	inline static void
	_free(struct Resource* resource, Resource_Manager* resources_manager, const std::string& tag)
	{
        Shader* shader = (Shader*)resource;
        delete shader;
        shader = nullptr;
        resource_manager_remove_resource(resources_manager, shader, tag);
	}

    inline static TGAColor
    _empty_fragment_shader(Shader* shader, glm::vec3 normal, const TGAColor& color)
    {
        return {};
    }

	inline static void
	_lit_vertex_shader(Shader* shader, tiny::Triangle& triangle)
	{
        // perform different transformations
        // model matrix
        glm::mat4 rotate_y{ 1.0f }, scale{ 1.0f }, translate{ 1.0f };
        mathy::scale_mat(scale, glm::vec3(1.87f, 1.87f, 1.87f));
        mathy::translation_mat(translate, glm::vec3(0.0f, -0.5f, -3.5f));
        glm::mat4 model_mat = translate * scale * rotate_y;

        // view matrix
        glm::vec3 cam_pos = shader->uniforms["cam_pos"].value.as_vec3;
        glm::mat4 view{ 1.0f };
        mathy::view_mat(view, cam_pos);

        // proj matrix
        glm::mat4 proj{ 1.0f };
        mathy::perspective_projection_mat(proj, 0.1f, 100.0f, 90.0f);

        // model-view-proj transformation
        glm::vec4 homogenous_v0 = proj * view * model_mat * glm::vec4(triangle.data.v0, 1.0f);
        glm::vec4 homogenous_v1 = proj * view * model_mat * glm::vec4(triangle.data.v1, 1.0f);
        glm::vec4 homogenous_v2 = proj * view * model_mat * glm::vec4(triangle.data.v2, 1.0f);

        // homogenous to cartesian
        float z0 = triangle.data.v0.z;
        float z1 = triangle.data.v1.z;
        float z2 = triangle.data.v2.z;
        triangle.data.v0 = glm::vec3(homogenous_v0) / homogenous_v0.w;
        triangle.data.v1 = glm::vec3(homogenous_v1) / homogenous_v1.w;
        triangle.data.v2 = glm::vec3(homogenous_v2) / homogenous_v2.w;
        triangle.data.v0.z = z0;
        triangle.data.v1.z = z1;
        triangle.data.v2.z = z2;
	}

    inline static TGAColor
    _lit_fragment_shader(Shader* shader, glm::vec3 normal, const TGAColor& color)
    {
        glm::vec3 light_direction = shader->uniforms["light_direction"].value.as_vec3;
        float dot = std::max(mathy::dot(normal, -light_direction), 0.0f);
        TGAColor shaded_color = TGAColor(color.bgra[2] * dot, color.bgra[1] * dot, color.bgra[0] * dot);
        return shaded_color;
    }

	void shader_set_uniform(Shader* shader, const std::string& name, const Uniform& uniform)
	{
		shader->uniforms.emplace(std::make_pair(name, uniform));
	}

    Shader*
    shader_lit_new(Resource_Manager* resource_manager, const std::string& tag)
    {
        Shader* shader = new Shader;
        shader->load = &_load;
        shader->free = &_free;
        shader->vertex_shader = &_lit_vertex_shader;
        shader->fragment_shader = &_lit_fragment_shader;
        resource_manager_add_resource(resource_manager, shader, tag);
        return shader;
    }

    Shader*
    shader_diffuse_new(Resource_Manager* resource_manager, const std::string& tag)
    {
        Shader* shader = new Shader;
        shader->load = &_load;
        shader->free = &_free;
        shader->vertex_shader = &_lit_vertex_shader;
        shader->fragment_shader = &_lit_fragment_shader;
        resource_manager_add_resource(resource_manager, shader, tag);
        return shader;
    }

    Shader*
    shader_wireframe_new(Resource_Manager* resource_manager, const std::string& tag) 
    {
        Shader* shader = new Shader;
        shader->load = &_load;
        shader->free = &_free;
        shader->vertex_shader = &_lit_vertex_shader;
        shader->fragment_shader = &_empty_fragment_shader;
        resource_manager_add_resource(resource_manager, shader, tag);
        return shader;
    }
};
