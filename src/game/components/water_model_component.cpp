#include "water_model_component.hpp"

namespace game
{
  WaterModelComponent::WaterModelComponent(EntityWPtr entityWPtr, GLuint resolution, float width, float height, float depth) :
    ModelComponent(entityWPtr), _width(width), _height(height), _depth(depth) {
    _resolution = resolution;
    points.reserve(resolution*resolution*resolution*(width*height*depth));
    for(int i=0;i<resolution*width;i++) {
      for(int j=0;j<resolution*height;j++) {
        for(int k = 0; k< resolution*depth;k++) {
          points.push_back((float)i/resolution);
          points.push_back((float)j/resolution);
          points.push_back((float)k/resolution);
          points.push_back(0);
          auto minDim = std::min(width, std::min(height, depth));
          radius.push_back(1.0/(resolution*3.0));
        }
      }
    }
    glGenVertexArrays(1, &vao);
    glBindVertexArray (vao);

    glGenBuffers (1, &position_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, position_vbo);
    glBufferData (
      GL_ARRAY_BUFFER,
      points.size() * sizeof (GLfloat),
      &points[0],
      GL_DYNAMIC_DRAW
      );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers (1, &radius_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, radius_vbo);
    glBufferData (
      GL_ARRAY_BUFFER,
      radius.size() * sizeof (GLfloat),
      &radius[0],
      GL_STATIC_DRAW
      );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
  }

  void WaterModelComponent::init() {}
  
  WaterModelComponent::~WaterModelComponent() {
    glDeleteVertexArrays(1, &vao);
  }
}
