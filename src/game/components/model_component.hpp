#pragma once
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "component.hpp"
#include "component_type.hpp"
#include "game/entity.hpp"

namespace game
{
  class ModelComponent : public Component
  {
  private:
    struct ModelComponentImpl;
    std::unique_ptr<ModelComponentImpl> pimpl;
  public:
    ModelComponent(EntityWPtr entityWPtr);
    ~ModelComponent();
    virtual void init() override;
    virtual ComponentType getComponentType() override; 
    virtual void setVertices(std::vector<float>& vertices);
    virtual void setTexture(GLuint texture);
    GLuint getArrayObject() const;
    GLuint getTexture() const;
    size_t vertexCount() const;
  };
}
