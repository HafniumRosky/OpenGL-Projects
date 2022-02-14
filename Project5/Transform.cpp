#include "Transform.h"

Transform::Transform(const vec3& scale, const vec3& rotation, const vec3& position)
	: m_scale(scale), m_rotation(rotation), m_position(position)
{
	
}

vec3 Transform::GetScale() const
{
	return m_scale;
}

vec3 Transform::GetRotation() const
{
	return m_rotation;
}


vec3 Transform::GetPosition() const
{
	return m_position;
}

mat4 Transform::GetLoaclToWorldMatrix() const
{
	return this->m_worldMatrix;
}

mat4 Transform::GetWorldToLocalMatrix() const
{
	mat4 invWorld = inverse(this->m_worldMatrix);
	return invWorld;
}

mat4 Transform::GetWorldInverseTransposeMatrix() const
{
	return m_worldInverseTransMatrix;
}

void Transform::ComputeLocalToWorldMatrix()
{
	mat4 translateMatrix = mat4(1.0f);
	mat4 rotateMatrix = mat4(1.0f);
	mat4 scaleMatrix = mat4(1.0f);
	mat4 targetToLocal = mat4(1.0f);
	mat4 localToTarget = mat4(1.0f);

	localToTarget = translate(localToTarget, -m_origin);
	translateMatrix = translate(translateMatrix, this->m_position);
	if (m_useQuaternion)
	{
		quat quaternion = quat(m_rotation);
		rotateMatrix = m_rotationMatrix * mat4_cast(quaternion);
		m_rotationMatrix = rotateMatrix;
		m_rotation = vec3(0.0f);
	}
	else
	{
		rotateMatrix = rotate(rotateMatrix, this->m_rotation.y, vec3(0.0f, 1.0f, 0.0f));
		rotateMatrix = rotate(rotateMatrix, this->m_rotation.x, vec3(1.0f, 0.0f, 0.0f));
		rotateMatrix = rotate(rotateMatrix, this->m_rotation.z, vec3(0.0f, 0.0f, 1.0f));
		m_rotationMatrix = rotateMatrix;
	}
	scaleMatrix = scale(scaleMatrix, this->m_scale);
	targetToLocal = translate(targetToLocal, m_origin);

	m_worldMatrix = localToTarget * translateMatrix * rotateMatrix * scaleMatrix * targetToLocal;
}

void Transform::ComputeWorldInverseTransposeMatrix()
{
	mat4 invWorld = GetWorldToLocalMatrix();
	m_worldInverseTransMatrix = transpose(invWorld);
}

void Transform::SetScale(const vec3& scale)
{
	m_scale = scale;
}

void Transform::SetScale(GLfloat x, GLfloat y, GLfloat z)
{
	m_scale = vec3(x, y, z);
}

void Transform::SetRotation(GLfloat x, GLfloat y, GLfloat z)
{
	m_rotation = vec3(x, y, z);
}

void Transform::SetRotation(const vec3& rotation)
{
	m_rotation = rotation;
}

void Transform::SetPosition(const vec3& position)
{
	m_position = position;
}

void Transform::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	m_position = vec3(x, y, z);
}

mat4 Transform::LookAt(const vec3& target, const vec3& up)
{
	return lookAt(this->m_position, target, up);
}

void Transform::Rotate(float angle, vec3 direction)
{
	mat4 matrix = mat4(1.0f);
	matrix = rotate(matrix, angle, direction);
	m_rotationMatrix = matrix * m_rotationMatrix;
}

void Transform::Rotate(vec3 eularAngle)
{
	if (m_useQuaternion)
	{
		m_rotation = eularAngle;
	}
	else
	{
		m_rotation += eularAngle;
	}
}

void Transform::Translate(vec3 dir, float dist)
{
	m_position += dir * dist;
}

vec3 Transform::GetForwardAxis()
{
	vec4 locFoward = vec4(0.0f, 0.0f, -1.0f, 0.0f);

	vec4 ret = m_rotationMatrix * locFoward;
	return vec3(ret.x, ret.y, ret.z);
}

vec3 Transform::GetUpAxis()
{
	vec4 locFoward = vec4(0.0f, 1.0f, 0.0f, 0.0f);
	vec4 ret = m_rotationMatrix * locFoward;
	return vec3(ret.x, ret.y, ret.z);
}

vec3 Transform::GetRightAxis()
{
	vec4 locFoward = vec4(1.0f, 0.0f, 0.0f, 0.0f);
	vec4 ret = m_rotationMatrix * locFoward;
	return vec3(ret.x, ret.y, ret.z);
}

//Set origin
void Transform::SetOrigin(vec3 pos)
{
	m_origin = pos;
}

//Get origin
vec3 Transform::GetOrigin()
{
	return m_origin;
}

//Set quaternion rotation
void Transform::UseQuaternionRotation()
{
	m_useQuaternion = true;
	m_rotation = vec3(0.0f);
}

//Set Eular angles rotation
void Transform::UseEularAngleRotation()
{
	m_useQuaternion = false;
	float m23Check = sqrtf(1.0f - m_rotationMatrix[2][1] * m_rotationMatrix[2][1]);
	if (isnan(m23Check))
		m23Check = 0.0f;
	m_rotation.z = atan2f(m_rotationMatrix[0][1], m_rotationMatrix[1][1]);
	m_rotation.x = atan2f(-m_rotationMatrix[2][1], m23Check);
	m_rotation.y = atan2f(m_rotationMatrix[2][0], m_rotationMatrix[2][2]);
}

bool Transform::UsingQuaternion()
{
	return m_useQuaternion;
}

void Transform::UpdateEularRotationMatrix()
{
	mat4 rotateMatrix = mat4(1.0f);
	rotateMatrix = rotate(rotateMatrix, this->m_rotation.y, vec3(0.0f, 1.0f, 0.0f));
	rotateMatrix = rotate(rotateMatrix, this->m_rotation.x, vec3(1.0f, 0.0f, 0.0f));
	rotateMatrix = rotate(rotateMatrix, this->m_rotation.z, vec3(0.0f, 0.0f, 1.0f));
	m_rotationMatrix = rotateMatrix;
}