#pragma once
#include "component.hpp"
#include "game/entity.hpp"
#include "GL/glew.h"

namespace game
{
  class CameraComponent : public Component
  {
    struct CameraComponentImpl;
    std::unique_ptr<CameraComponentImpl> pimpl;
  public:
    CameraComponent(EntityWPtr entityWPtr);
    ~CameraComponent();
    virtual ComponentType getComponentType() override;
    void updateView();
    void updatePerspective(float aspectRatio);
    void loadIntoProgram(GLuint shaderProgram);
    virtual void init() override;
  };
}
