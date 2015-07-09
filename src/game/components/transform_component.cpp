#include "transform_component.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "easylogging++.h"
#include "format.h"

namespace game
{
  struct TransformComponent::TransformComponentImpl
  { 
    glm::vec3 translation;
    glm::quat rotation;
  };

  TransformComponent::TransformComponent(EntityWPtr entityWPtr) : 
    Component(entityWPtr),
    pimpl(unique_ptr<TransformComponentImpl>(new TransformComponentImpl()))
  {
    pimpl->translation = glm::vec3(0);
    pimpl->rotation = glm::quat();
  }

  mat4 TransformComponent::getTransform() const {
    return glm::translate(glm::mat4(1), pimpl->translation) * glm::mat4_cast(pimpl->rotation);
  }

  vec3 TransformComponent::getPosition() const {
    return pimpl->translation;
  }

  vec3 TransformComponent::getRight() const {
    return glm::normalize(glm::vec3(pimpl->rotation * glm::vec4(1, 0, 0, 0)));
  }

  vec3 TransformComponent::getUp() const {
    return glm::normalize(glm::vec3(pimpl->rotation * glm::vec4(0, 1, 0, 0)));
  }
  vec3 TransformComponent::getForward() const {
    return glm::normalize(glm::vec3(pimpl->rotation * glm::vec4(0, 0, -1, 0)));
  }

  void TransformComponent::moveBy(glm::vec3 vector) {
    pimpl->translation += vector;
  }

  void TransformComponent::rotateBy(float angle, glm::vec3 axis) {
    rotateBy(glm::angleAxis(angle, axis));
  }

  void TransformComponent::rotateBy( quat rotation )
  {
    pimpl->rotation = glm::normalize(glm::normalize(rotation) * pimpl->rotation);
  }

  void TransformComponent::setRotation( quat rotation )
  {
    pimpl->rotation = glm::normalize(rotation);
  }

  void TransformComponent::setPosition( vec3 position )
  {
    pimpl->translation = position;
  }

  void TransformComponent::lookAt( vec3 eye, vec3 target, vec3 up )
  {
    auto lookAt = glm::lookAt(eye, target, up);
    LOG(INFO) << fmt::format("\n{0:3} {1:3} {2:3} {3:3}\n", lookAt[0][0], lookAt[1][0], lookAt[2][0], lookAt[3][0]) 
              << fmt::format("{0:3} {1:3} {2:3} {3:3}\n", lookAt[0][1], lookAt[1][1], lookAt[2][1], lookAt[3][1]) 
              << fmt::format("{0:3} {1:3} {2:3} {3:3}\n", lookAt[0][2], lookAt[1][2], lookAt[2][2], lookAt[3][2]) 
              << fmt::format("{0:3} {1:3} {2:3} {3:3}\n", lookAt[0][3], lookAt[1][3], lookAt[2][3], lookAt[3][3]);
    pimpl->rotation = glm::normalize(glm::conjugate(glm::quat_cast(lookAt)));  
    pimpl->translation = eye; 

    LOG(INFO) << fmt::format("rotation: ({0}, {1}, {2}, {3})", pimpl->rotation.w, pimpl->rotation.x, pimpl->rotation.y, pimpl->rotation.z);
  }

  void TransformComponent::init() {}

  ComponentType TransformComponent::getComponentType()
  {
    return ComponentType::TRANSFORM_COMPONENT;
  }

  TransformComponent::~TransformComponent()
  {}
}
