//
// Created by Xu Zeming on 2022/1/9.
//

#ifndef PTRENDERER_SCENEPARSER_H
#define PTRENDERER_SCENEPARSER_H




#include <string>
#include "Scene.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

using Vec3f = glm::vec3;
using Vec2f = glm::vec2;

#define MAX_PARSER_TOKEN_LENGTH 100


namespace PTRenderer {
    class SceneParser {
    public:
        SceneParser(const std::string& filenanme, std::shared_ptr<Scene> scene_ptr);


    private:
        void parse_file();

        int getToken(char token[MAX_PARSER_TOKEN_LENGTH]);

        float readFloat();

        int readInt();

        Vec2f readVec2f();

        Vec3f readVec3f();

        void parse_orthographic_amera();

        void parse_perspective_camera();

        void parse_background();

        void parse_lights();

        void parse_materials();



        void parse_directional_light();

        void parse_point_light();

        int get_num_materials();

        std::shared_ptr<Material> parse_phong_material();

        int num_materials;
        Vec3f background_color;
        Vec3f ambient_light;
        FILE *file;
        std::shared_ptr<Scene> scene;
        std::vector<std::shared_ptr<Material>> materials;
        std::shared_ptr<Material> current_material;
        std::shared_ptr<Group> group;


        std::shared_ptr<Material> get_material(int index);

        std::shared_ptr<Primitives> parse_object(char token[100]);

        std::shared_ptr<Group> parse_group();

        std::shared_ptr<Primitives>  parse_sphere();

        std::shared_ptr<Triangle>  parse_triangle();

        std::shared_ptr<Group> parse_triangle_mesh();
    };
}


#endif //PTRENDERER_SCENEPARSER_H
