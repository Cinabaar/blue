#version 330

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

uniform mat4 modelToWorld;

layout(std140) uniform Camera
{
  mat4 worldToCamera;
  mat4 cameraToProjection;
};


void main () {
  gl_Position = cameraToProjection * worldToCamera * modelToWorld * vec4 (vertex, 1.0);
  //gl_Position = vec4(vertex, 1.0);
}
