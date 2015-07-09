#version 330

in vec3 texCoords;
uniform samplerCube cubeTexture;
out vec4 frag_colour;
void main () {
  frag_colour = texture(cubeTexture, texCoords);
}
