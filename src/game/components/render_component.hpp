#pragma once
#include <memory>
#include "component.hpp"
#include "component_type.hpp"
#include "GL/glew.h"
#include "game/entity.hpp"
#include "game/scene.hpp"

namespace game
{
  class RenderComponent : public Component
  {
  private:
    struct RenderComponentImpl;
    std::unique_ptr<RenderComponentImpl> pimpl;
  protected:
    GLuint shaderProgram_;
  public:
    RenderComponent(EntityWPtr entityWPtr);
    ~RenderComponent();
    virtual void init() override;
    void setShaderProgram(GLuint shaderProgram);
    GLuint getShaderProgram() const;
    virtual void render();
    virtual ComponentType getComponentType() override;
  };
}
