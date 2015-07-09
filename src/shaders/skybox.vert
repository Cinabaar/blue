#version 330

layout(location = 0) in vec3 vertex;

layout(std140) uniform Camera
{
  mat4 worldToCamera;
  mat4 cameraToProjection;
};

out vec3 texCoords;

void main () {
  texCoords = vertex;
  mat4 view = mat4(mat3(worldToCamera));
  gl_Position = cameraToProjection * view * vec4 (vertex, 1.0);
}
