//
// Created by Xu Zeming on 2022/1/9.
//
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "SceneParser.h"

namespace PTRenderer {

    SceneParser::SceneParser(const std::string &filenanme, std::shared_ptr<Scene> scene_ptr) {

        num_materials = 0;
        materials.resize(0);
        current_material = nullptr;
        group = nullptr;
        file = nullptr;
        file = fopen(filenanme.c_str(), "r");
        parse_file();
        fclose(file);
        file = nullptr;


    }

    void SceneParser::parse_file() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        while(getToken(token)){
            if(!strcmp(token, "OrthographicCamera")){
                parse_orthographic_amera();
            } else if (!strcmp(token, "PerspectiveCamera")) {
                parse_perspective_camera();
            } else if (!strcmp(token, "Background")) {
                parse_background();
            } else if (!strcmp(token, "Lights")) {
                parse_lights();
            } else if (!strcmp(token, "Materials")) {
                parse_materials();
            } else if (!strcmp(token, "Group")) {
                group = parse_group();
            } else {
                printf ("Unknown token in parseFile: '%s'\n", token);
                exit(0);
            }

        }
    }

    int SceneParser::getToken(char *token) {
        assert(file != nullptr);
        int success = fscanf(file, "%s", token);
        if(success == EOF){
            token[0] = '\0';
            return 0;
        }
        return 1;
    }


    Vec3f SceneParser::readVec3f() {
        float x,y,z;
        int count = fscanf(file,"%f %f %f",&x,&y,&z);
        if (count != 3) {
            printf ("Error trying to read 3 floats to make a Vec3f\n");
            assert (0);
        }
        return Vec3f(x,y,z);
    }


    Vec2f SceneParser::readVec2f() {
        float u,v;
        int count = fscanf(file,"%f %f",&u,&v);
        if (count != 2) {
            printf ("Error trying to read 2 floats to make a Vec2f\n");
            assert (0);
        }
        return Vec2f(u,v);
    }


    float SceneParser::readFloat() {
        float answer;
        int count = fscanf(file,"%f",&answer);
        if (count != 1) {
            printf ("Error trying to read 1 float\n");
            assert (0);
        }
        return answer;
    }


    int SceneParser::readInt() {
        int answer;
        int count = fscanf(file,"%d",&answer);
        if (count != 1) {
            printf ("Error trying to read 1 int\n");
            assert (0);
        }
        return answer;
    }

    void SceneParser::parse_orthographic_amera() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        // read in the camera parameters
        getToken(token); assert (!strcmp(token, "{"));
        getToken(token); assert (!strcmp(token, "center"));
        Vec3f center = readVec3f();
        getToken(token); assert (!strcmp(token, "direction"));
        Vec3f direction = readVec3f();
        getToken(token); assert (!strcmp(token, "up"));
        Vec3f up = readVec3f();
        getToken(token); assert (!strcmp(token, "size"));
        float size = readFloat();
        getToken(token); assert (!strcmp(token, "}"));
        scene->set_camera(std::make_shared<OrthographicCamera>(center, up, direction, size));
    }

    void SceneParser::parse_perspective_camera() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        // read in the camera parameters
        getToken(token); assert (!strcmp(token, "{"));
        getToken(token); assert (!strcmp(token, "center"));
        Vec3f center = readVec3f();
        getToken(token); assert (!strcmp(token, "direction"));
        Vec3f direction = readVec3f();
        getToken(token); assert (!strcmp(token, "up"));
        Vec3f up = readVec3f();
        getToken(token); assert (!strcmp(token, "angle"));
        float angle_degrees = readFloat();

        // implement inside PerspectiveCamera
        // float angle_radians = DegreesToRadians(angle_degrees);
        getToken(token); assert (!strcmp(token, "}"));
        scene->set_camera(std::make_shared<PerspectiveCamera>(center, up, direction, angle_degrees));
    }

    void SceneParser::parse_background() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        // read in the background color
        getToken(token); assert (!strcmp(token, "{"));
        while (1) {
            getToken(token);
            if (!strcmp(token, "}")) {
                break;
            } else if (!strcmp(token, "color")) {
                background_color = readVec3f();
            } else if (!strcmp(token, "ambientLight")) {
                ambient_light = readVec3f();
            } else {
                printf ("Unknown token in parseBackground: '%s'\n", token);
                assert(0);
            }
        }
    }

    void SceneParser::parse_lights() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        getToken(token); assert (!strcmp(token, "{"));
        // read in the number of objects
        getToken(token); assert (!strcmp(token, "numLights"));
        int num_lights = readInt();
        // read in the objects
        int count = 0;
        while (num_lights > count) {
            getToken(token);
            if (!strcmp(token, "DirectionalLight")) {
                parse_directional_light();
            } else if (!strcmp(token, "PointLight")) {
                parse_point_light();
            } else {
                printf ("Unknown token in parseLight: '%s'\n", token);
                exit(0);
            }
            count++;
        }
        getToken(token); assert (!strcmp(token, "}"));
    }

    void SceneParser::parse_materials() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        getToken(token); assert (!strcmp(token, "{"));
        // read in the number of objects
        getToken(token); assert (!strcmp(token, "numMaterials"));
        num_materials = readInt();
        materials.resize(num_materials);
        // read in the objects
        int count = 0;
        while (num_materials > count) {
            getToken(token);
            if (!strcmp(token, "Material") ||
                !strcmp(token, "PhongMaterial")) {
                materials[count] = parse_phong_material();
            } else {
                printf ("Unknown token in parseMaterial: '%s'\n", token);
                exit(0);
            }
            count++;
        }
        getToken(token); assert (!strcmp(token, "}"));
    }

    std::shared_ptr<Group> SceneParser::parse_group() {
//
        // each group starts with an integer that specifies
        // the number of objects in the group
        //
        // the material index sets the material of all objects which follow,
        // until the next material index (scoping for the materials is very
        // simple, and essentially ignores any tree hierarchy)
        //
        char token[MAX_PARSER_TOKEN_LENGTH];
        getToken(token); assert (!strcmp(token, "{"));

        // read in the number of objects
        getToken(token); assert (!strcmp(token, "numObjects"));
        int num_objects = readInt();
        std::shared_ptr<Group> answer = std::make_shared<Group>(num_objects);

        // read in the objects
        int count = 0;
        while (num_objects > count) {
            getToken(token);
            if (!strcmp(token, "MaterialIndex")) {
                // change the current material
                int index = readInt();
                assert (index >= 0 && index <= get_num_materials());
                current_material = get_material(index);
            } else {
                std::shared_ptr<Primitives> object =  parse_object(token);
                assert (object != NULL);
                answer->add_primitive(count,object);
                count++;
            }
        }
        getToken(token); assert (!strcmp(token, "}"));

        // return the group
        return answer;
    }

    void SceneParser::parse_directional_light() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        getToken(token); assert (!strcmp(token, "{"));
        getToken(token); assert (!strcmp(token, "direction"));
        Vec3f direction = readVec3f();
        getToken(token); assert (!strcmp(token, "color"));
        Vec3f color = readVec3f();
        getToken(token); assert (!strcmp(token, "}"));
        scene->add_light(std::make_shared<DirectionLight>(direction, color));
    }

    void SceneParser::parse_point_light() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        getToken(token); assert (!strcmp(token, "{"));
        getToken(token); assert (!strcmp(token, "position"));
        Vec3f position = readVec3f();
        getToken(token); assert (!strcmp(token, "color"));
        Vec3f color = readVec3f();
        float att[3] = { 1, 0, 0 };
        getToken(token);

        // [TODO] attenuation may use later
        if (!strcmp(token,"attenuation")) {
            att[0] = readFloat();
            att[1] = readFloat();
            att[2] = readFloat();
            getToken(token);
        }
        assert (!strcmp(token, "}"));
        scene->add_light(std::make_shared<PointLight>(position, color));
    }

    int SceneParser::get_num_materials() {
        return num_materials;
    }

    std::shared_ptr<Material> SceneParser::parse_phong_material() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        Vec3f diffuseColor(1,1,1);
        Vec3f specularColor(0,0,0);
        float exponent = 1;
        Vec3f reflectiveColor(0,0,0);
        Vec3f transparentColor(0,0,0);
        float indexOfRefraction = 1;
        getToken(token); assert (!strcmp(token, "{"));
        while (1) {
            getToken(token);
            if (!strcmp(token, "diffuseColor")) {
                diffuseColor = readVec3f();
            } else if (!strcmp(token, "specularColor")) {
                specularColor = readVec3f();
            } else if  (!strcmp(token, "exponent")) {
                exponent = readFloat();
            } else if (!strcmp(token, "reflectiveColor")) {
                reflectiveColor = readVec3f();
            } else if (!strcmp(token, "transparentColor")) {
                transparentColor = readVec3f();
            } else if (!strcmp(token, "indexOfRefraction")) {
                indexOfRefraction = readFloat();
            } else {
                assert (!strcmp(token, "}"));
                break;
            }
        }
         return std::make_shared<PhongMaterial>(diffuseColor,specularColor,exponent,
                                             reflectiveColor,transparentColor,
                                             indexOfRefraction);
    }

    std::shared_ptr<Material> SceneParser::get_material(int index) {
        return materials[index];
    }

    std::shared_ptr<Primitives> SceneParser::parse_object(char *token) {
        std::shared_ptr<Primitives> answer = nullptr;
        if (!strcmp(token, "Group")) {
            answer = parse_group();
        }
//        [TODO] Sphere Class later
//        else if (!strcmp(token, "Sphere")) {
//            parse_sphere();
//        }
//        else if (!strcmp(token, "Plane")) {
//            parse_plane();
//        }
        else if (!strcmp(token, "Triangle")) {
            answer = parse_triangle();
        } else if (!strcmp(token, "TriangleMesh")) {
            answer = parse_triangle_mesh();
        }
//        else if (!strcmp(token, "Transform")) {
//            parse_transform();
//        }
        else {
            printf ("Unknown token in parseObject: '%s'\n", token);
            exit(0);
        }
        return answer;
    }

    std::shared_ptr<Primitives>  SceneParser::parse_sphere() {
//        char token[MAX_PARSER_TOKEN_LENGTH];
//        getToken(token); assert (!strcmp(token, "{"));
//        getToken(token); assert (!strcmp(token, "center"));
//        Vec3f center = readVec3f();
//        getToken(token); assert (!strcmp(token, "radius"));
//        float radius = readFloat();
//        getToken(token); assert (!strcmp(token, "}"));
//        assert (current_material != NULL);
//        return new Sphere(center,radius,current_material);
        return nullptr;
    }

    std::shared_ptr<Triangle>  SceneParser::parse_triangle() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        getToken(token); assert (!strcmp(token, "{"));
        getToken(token);
        assert (!strcmp(token, "vertex0"));
        Vec3f v0 = readVec3f();
        getToken(token);
        assert (!strcmp(token, "vertex1"));
        Vec3f v1 = readVec3f();
        getToken(token);
        assert (!strcmp(token, "vertex2"));
        Vec3f v2 = readVec3f();
        getToken(token); assert (!strcmp(token, "}"));
        assert (current_material != NULL);
        return std::make_shared<Triangle>(v0, v1, v2, current_material);
    }

    std::shared_ptr<Group> SceneParser::parse_triangle_mesh() {
        char token[MAX_PARSER_TOKEN_LENGTH];
        char filename[MAX_PARSER_TOKEN_LENGTH];
        // get the filename
        getToken(token); assert (!strcmp(token, "{"));
        getToken(token); assert (!strcmp(token, "obj_file"));
        getToken(filename);
        getToken(token); assert (!strcmp(token, "}"));
        const char *ext = &filename[strlen(filename)-4];
        assert(!strcmp(ext,".obj"));
        // read it once, get counts
        FILE *file = fopen(filename,"r");
        assert (file != NULL);
        int vcount = 0; int fcount = 0;
        while (1) {
            int c = fgetc(file);
            if (c == EOF) { break;
            } else if (c == 'v') {
                assert(fcount == 0); float v0,v1,v2;
                fscanf (file,"%f %f %f",&v0,&v1,&v2);
                vcount++;
            } else if (c == 'f') {
                int f0,f1,f2;
                fscanf (file,"%d %d %d",&f0,&f1,&f2);
                fcount++;
            } // otherwise, must be whitespace
        }
        fclose(file);
        // make arrays
        Vec3f *verts = new Vec3f[vcount];
        std::shared_ptr<Group> answer = std::make_shared<Group>(fcount);
        // read it again, save it
        file = fopen(filename,"r");
        assert (file != NULL);
        int new_vcount = 0; int new_fcount = 0;
        while (1) {
            int c = fgetc(file);
            if (c == EOF) { break;
            } else if (c == 'v') {
                assert(new_fcount == 0); float v0,v1,v2;
                fscanf (file,"%f %f %f",&v0,&v1,&v2);
                verts[new_vcount] = Vec3f(v0,v1,v2);
                new_vcount++;
            } else if (c == 'f') {
                assert (vcount == new_vcount);
                int f0,f1,f2;
                fscanf (file,"%d %d %d",&f0,&f1,&f2);
                // indexed starting at 1...
                assert (f0 > 0 && f0 <= vcount);
                assert (f1 > 0 && f1 <= vcount);
                assert (f2 > 0 && f2 <= vcount);
                assert (current_material != NULL);
                std::shared_ptr<Triangle> t = std::make_shared<Triangle>(verts[f0-1],verts[f1-1],verts[f2-1],current_material);
                answer->add_primitive(new_fcount,t);
                new_fcount++;
            } // otherwise, must be whitespace
        }
        delete [] verts;
        assert (fcount == new_fcount);
        assert (vcount == new_vcount);
        fclose(file);
        return answer;
    }
}