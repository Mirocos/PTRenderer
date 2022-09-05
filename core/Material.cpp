//
// Created by zeming on 2021/12/17.
//


#include <Utils.h>
#include "Material.h"
#include "glm/geometric.hpp"



namespace PTRenderer{

    Material::Material(const glm::vec3 &_diffuse_color, const glm::vec3& _emission_color)
    : diffuse_color(_diffuse_color), emission_color(_emission_color){

    }

    glm::vec3 Material::toWorld(const glm::vec3 &a, const glm::vec3 &N) {
        glm::vec3 B, C;
        if (std::fabs(N.x) > std::fabs(N.y)){
            float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
            C = glm::vec3(N.z * invLen, 0.0f, -N.x *invLen);
        }
        else {
            float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
            C = glm::vec3(0.0f, N.z * invLen, -N.y *invLen);
        }
        B = glm::cross(C, N);
        return a.x * B + a.y * C + a.z * N;
    }

    glm::vec3 Material::getReflectDirection(const glm::vec3 &Wo, const glm::vec3 &N) {
        return 2.f * glm::dot(Wo, N) * N - Wo;
    }


    PhongMaterial::PhongMaterial(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,  const glm::vec3& emission,
                                 float ior, float shininess)
                                 : Material(diffuse, emission), ka(ambient), kd(diffuse), ks(specular), index_of_refraction(ior), shininess(shininess){

    }

    glm::vec3 PhongMaterial::shade(const glm::vec3 &N, const glm::vec3 &L, const glm::vec3 &V) {
        /*
         *  suppose L is from light to hit point
         *
         * */

        float NdotL = Utils::clamp<float>(glm::dot(N, L), 0.f, 1.f); // cosine between N & L

        glm::vec3 H =  glm::normalize(L + V);
        float NdotH =  Utils::clamp<float>(glm::dot(N, H), 0.f, 1.f);


        return  NdotL * diffuse_color * kd / (float)M_PI+ NdotH * std::pow(NdotH, 32.f) * specular_color * ks / (33.f / 2.f / (float)M_PI);
    }

    glm::vec3 PhongMaterial::sample(const glm::vec3& Wo, const glm::vec3& N, float& pdf)  {
        float ld = Utils::evalLuminance(kd);
        float ls = Utils::evalLuminance(ks);
        float lSum = ld + ls;
        std::vector<float> cdf{ld / lSum, 1.f};
        float typePdf = Utils::getUniformRandomFloat();
        float u1 = Utils::getUniformRandomFloat();
        float u2 = Utils::getUniformRandomFloat();
        float sinTheta, cosTheta;
        float sinPhi, cosPhi;
        if(typePdf <= cdf[0]){
            // DIFFUSE

            // cosine importance sampling
            sinTheta = sqrtf(u1);
            cosTheta = sqrtf(1-u1);
            sinPhi = sin(PI2 * u2);
            cosPhi = cos(PI2 * u2);
            pdf = cosTheta * INV_PI * ld / lSum;


            // uniform sampling
/*            cosTheta = u1;
            sinTheta = sqrtf(1-u1*u1);
            sinPhi = sin(PI2 * u2);
            cosPhi = cos(PI2 * u2);
            pdf = INV_PI2 * ld / lSum;*/
        } else {
            // SPECULAR
            // TODO: Still have Bugs
            cosTheta = powf(u1, 1.f/(shininess+1.f));
            sinTheta = sqrtf(1 - cosTheta * cosTheta);
            sinPhi = sin(PI2 * u2);
            cosPhi = cos(PI2 * u2);
            pdf = (2.f + shininess) * INV_PI2 * ls / lSum;   // without cos(alpha)^n
        }

        float x = sinTheta * cosPhi;
        float y = sinTheta * sinPhi;
        float z = cosTheta;

        glm::vec3 W_i_local = glm::vec3(x, y, z);
        glm::vec3 W_i = toWorld(W_i_local, N);

        if(typePdf > cdf[0]){
            // SPECULAR
            glm::vec3 R = getReflectDirection(Wo, N);
            if(glm::dot(N, W_i) <= 0.f)
                pdf = 0.f;
            else{
                float cosine = glm::dot(R, W_i);
                pdf *= powf(cosine, shininess);
            }
        }

        return W_i;
    }

    glm::vec3 PhongMaterial::eval(const glm::vec3 &Wi, const glm::vec3 &Wo, const glm::vec3 &N) {
        glm::vec3 R = getReflectDirection(Wo, N);
        float cosine = fmax(glm::dot(R, Wi), 0.f);
        return kd * (float)M_1_PI + ks * powf(cosine, shininess) * (shininess + 2.f) * (float)M_1_PI * 0.5f;
    }



    float CookTorrancetaMerial::chiGGX(float v) {
        return v > 0? 1 : 0;
    }

    float CookTorrancetaMerial::GGX_Distribution(const glm::vec3 &n, const glm::vec3 &h, float alpha) {
        // h is half vector
        float NdotH = glm::dot(n, h);
        float alpha2 = roughness * roughness;
        float NdotH2 = NdotH * NdotH;
        float den = NdotH2 * alpha2 + (1-NdotH2);

        return (chiGGX(NdotH) * alpha2) / ((float)M_PI * den * den);
    }

    float CookTorrancetaMerial::GGX_PartialGeometryTerm(const glm::vec3 &v, const glm::vec3 &n, const glm::vec3 &h,
                                                        float alpha) {
        float VdotH = Utils::clamp<float>(glm::dot(v,h), 0.f, 1.f);
        float VdotN = Utils::clamp<float>(glm::dot(v,n), 0.f, 1.f);
        float chi = chiGGX(VdotH / VdotH);
        float VdotH2 = VdotH * VdotH;
        float tan2 = (1 - VdotH2) / VdotH2;
        return (chi * 2.f) / (1 + std::sqrt(1 + alpha * alpha * tan2));
    }

    glm::vec3 CookTorrancetaMerial::Fresnel_Schlick(float cosTheta, float ior) {
        float f0 = std::abs((1.f - ior / (1.f + ior)));
        f0 = f0 * f0;
        glm::vec3 F0 = glm::vec3(f0);
        F0 = Utils::lerp<glm::vec3>(F0, diffuse_color, metallic);
        return F0 + (1.f - F0) * powf(1.f - cosTheta, 5.f);
    }

    glm::vec3
    CookTorrancetaMerial::shade(const glm::vec3 &normal, const glm::vec3 &light_dir, const glm::vec3 &view_dir) {
        glm::vec3 H = glm::normalize(light_dir + view_dir);
        float cosTheta = glm::dot(view_dir, H);
        glm::vec3 F = Fresnel_Schlick(cosTheta, index_of_refraction);  // F
        glm::vec3 ks = F;
        float D = GGX_Distribution(normal, H, roughness);
        float G = GGX_PartialGeometryTerm(view_dir, normal, H, roughness) * GGX_PartialGeometryTerm(light_dir, normal, H, roughness);



        glm::vec3 kd = (1.f - ks) * (1.f - metallic);
        return glm::vec3();
    }
}