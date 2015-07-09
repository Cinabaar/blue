#pragma once
#include "model_component.hpp"

namespace game
{
  class WaterModelComponent : public ModelComponent {
  private:

    GLuint _resolution;
    float _width;
    float _height;
    float _depth;

  public:
    WaterModelComponent(EntityWPtr entityWPtr, GLuint resolution, float width, float height, float depth);
    ~WaterModelComponent();
    GLuint position_vbo;
    GLuint radius_vbo;
    GLuint vao;
    std::vector<float> points;
    std::vector<float> radius;

    float width() const { return _width; }
    float height() const { return _height; }
    float depth() const { return _depth; }
    GLuint particle_count() const { return _resolution*_resolution*_resolution*_width*_height*_depth; }
    float particle_mass() const { return (_width * _height * _depth * 1000) / _resolution; }
    virtual void init() override; 
  };
}
