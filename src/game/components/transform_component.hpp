#pragma once

#include <memory>
#include <glm/glm.hpp>
#include "game/entity.hpp"
#include "game/components/component.hpp"

namespace game
{
  using std::unique_ptr;
  using std::shared_ptr;
  using namespace glm;

  class TransformComponent : public Component
  {
  private:
    struct TransformComponentImpl;
    unique_ptr<TransformComponentImpl> pimpl;

  public:
    TransformComponent(EntityWPtr entityWPtr);
    ~TransformComponent();
    mat4 getTransform() const;
    vec3 getPosition() const;
    vec3 getUp() const;
    vec3 getRight() const;
    vec3 getForward() const;
    quat getRotation() const;

    void setPosition(vec3 position);
    void setRotation(quat rotation); 
    void lookAt(vec3 eye, vec3 target, vec3 up);
    void moveBy(vec3 vector);
    void rotateBy(float angle, vec3 axis);
    void rotateBy(quat rotation);

    virtual ComponentType getComponentType() override;
    virtual void init() override;
  };

  using TransformSPtr = shared_ptr<TransformComponent>;
}
