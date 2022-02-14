#ifndef LIGHT_HELPER_H
#define LIGHT_HELPER_H

#include <glm.hpp>

using namespace glm;

// Directional Light
struct DirectionalLight
{
	DirectionalLight() = default;

	DirectionalLight(const DirectionalLight&) = default;
	DirectionalLight& operator=(const DirectionalLight&) = default;

	DirectionalLight(DirectionalLight&&) = default;
	DirectionalLight& operator=(DirectionalLight&&) = default;

	DirectionalLight(const vec4& _lightColor,const vec3& _direction) :
		lightColor(_lightColor), direction(_direction) {}

	vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 direction = vec3(1.0f, 1.0f, 1.0f);
	float pad;
};

// Point Light
struct PointLight
{
	PointLight() = default;

	PointLight(const PointLight&) = default;
	PointLight& operator=(const PointLight&) = default;

	PointLight(PointLight&&) = default;
	PointLight& operator=(PointLight&&) = default;

	PointLight(const vec4& _lightColor, const vec3& _position, float _range, const float& _att) :
		lightColor(_lightColor), position(_position), range(_range), att(_att) {}

	vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 position = vec3(1.0f, 1.0f, 1.0f);
	float range = 25.0f;
	vec3 att = vec3(0.0f, 0.1f, 0.0f);
	float pad;
};

// Surface material
struct Material
{
	Material() = default;

	Material(const Material&) = default;
	Material& operator=(const Material&) = default;

	Material(Material&&) = default;
	Material& operator=(Material&&) = default;

	Material(const vec4& _ambient, const vec4& _diffuse, const vec4& _specular,
		const float _gloss) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular), gloss(_gloss) {}

	vec4 ambient = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 diffuse = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	vec4 specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float gloss = 20.0f;
	float pad[3];
};

struct PBRMaterial
{
	PBRMaterial() = default;

	PBRMaterial(const PBRMaterial&) = default;
	PBRMaterial& operator=(const PBRMaterial&) = default;

	PBRMaterial(PBRMaterial&&) = default;
	PBRMaterial& operator=(PBRMaterial&&) = default;

	PBRMaterial(const vec3& _albedo, const float& _metallic, const float& _roughness,
		const float _AO) :
		albedo(_albedo), metallic(_metallic), roughness(_roughness), AO(_AO) {}
	vec3 albedo;
	float metallic;
	float roughness;
	float AO;
	float pad[2];
};

#endif