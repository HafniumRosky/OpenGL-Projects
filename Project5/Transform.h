#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

using namespace glm;

class Transform
{
private:
	vec3 m_origin = { 0.0f, 0.0f, 0.0f };
	vec3 m_position = { 0.0f, 0.0f, 0.0f };								// Position
	vec3 m_rotation = { 0.0f, 0.0f, 0.0f };								// Rotation (Radian system)
	vec3 m_scale = { 1.0f, 1.0f, 1.0f };								// Scale

	bool m_useQuaternion = false;

	mat4 m_worldMatrix;
	mat4 m_rotationMatrix = mat4(1.0f);
	mat4 m_worldInverseTransMatrix;

public:
	quat m_quat = quat(vec3(0.0f));
	Transform() = default;
	Transform(const vec3& scale, const vec3& rotation, const vec3& position);
	~Transform() = default;

	Transform(const Transform&) = default;
	Transform& operator=(const Transform&) = default;

	Transform(Transform&&) = default;
	Transform& operator=(Transform&&) = default;

	// Get object scale
	vec3 GetScale() const;

	// Get object rotation (Radian system)
	// The object rotates in the order of Z-X-Y axis
	vec3 GetRotation() const;

	// Get object position
	vec3 GetPosition() const;

	//Get quaternion
	quat GetQuaternion() const;

	// Get model to world matrix
	mat4 GetLoaclToWorldMatrix() const;

	// Get world to model matrix
	mat4 GetWorldToLocalMatrix() const;

	//Get world inverset ranspose matrix
	mat4 GetWorldInverseTransposeMatrix() const;


	// Compute model to world matrix
	void ComputeLocalToWorldMatrix();

	// Compute world inverse transpose matrix
	void ComputeWorldInverseTransposeMatrix();

	// Look to a direction
	mat4 LookAt(const vec3& target, const vec3& up);
	void LookTo(const vec3& direction, const vec3& up = { 0.0f, 1.0f, 0.0f });

	// Set object scale
	void SetScale(const vec3& scale);

	// Set object scale
	void SetScale(GLfloat x, GLfloat y, GLfloat z);

	// Set object rotation (Radian system)
	void SetRotation(GLfloat x, GLfloat y, GLfloat z);
	void SetRotation(const vec3& rotation);

	// Set object position with vec
	void SetPosition(const vec3& position);
	// Set object position
	void SetPosition(GLfloat x, GLfloat y, GLfloat z);

	//Set quaternion
	void SetQuaternion(quat quaternion);

	//Rotate at certain position
	void Rotate(float angle, vec3 direction);

	//Reset eularAnlge
	void Rotate(vec3 eularAngle);

	//Translate from current position
	void Translate(vec3 dir, float dist);

	//Get forward axis
	vec3 GetForwardAxis();

	//Get up axis
	vec3 GetUpAxis();

	//Get right axis
	vec3 GetRightAxis();

	//Set origin
	void SetOrigin(vec3 pos);

	//Get origin
	vec3 GetOrigin();

	//Set rotation using quat
	void UseQuaternionRotation();

	//Set rotation using eular
	void UseEularAngleRotation();

	//Check current rotation method
	bool UsingQuaternion();

	//Update matrix for eular
	void UpdateEularRotationMatrix();
};

#endif