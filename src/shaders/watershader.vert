#version 330

layout(location = 0) in vec4 sphereCenter;
layout(location = 1) in float sphereRadius;

uniform mat4 modelToWorld;

layout(std140) uniform Camera
{
    mat4 worldToCamera;
    mat4 cameraToProjection;
};

out VertexData
{
	vec3 sphereCenter_camera;
	float sphereRadius;
} outData;


void main () {
    vec4 sphereCenter_camera = worldToCamera * modelToWorld * vec4(vec3(sphereCenter), 1.0);
    outData.sphereCenter_camera = vec3(sphereCenter_camera);
    outData.sphereRadius = sphereRadius;

    gl_Position = sphereCenter_camera;
}
