#include "camera_component.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "transform_component.hpp"
#include "easylogging++.h"
#include "format.h"

namespace game
{
  struct CameraComponent::CameraComponentImpl
  {
    GLuint cameraUBO;
    GLuint index;
    glm::mat4 worldToCamera;
    glm::mat4 cameraToPerspective;
    std::shared_ptr<TransformComponent> transform;
  };

  CameraComponent::CameraComponent(EntityWPtr entityWPtr) :
    Component(entityWPtr),
    pimpl(unique_ptr<CameraComponentImpl>(new CameraComponentImpl()))
  {
    pimpl->index = 0;
    glGenBuffers(1, &pimpl->cameraUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, pimpl->cameraUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, pimpl->index, pimpl->cameraUBO, 0, sizeof(glm::mat4)*2);
  }
  CameraComponent::~CameraComponent() {}

  ComponentType CameraComponent::getComponentType()
  {
    return ComponentType::CAMERA_COMPONENT;
  }

  void CameraComponent::loadIntoProgram( GLuint shaderProgram )
  {
    auto uniformBlockIndex = glGetUniformBlockIndex(shaderProgram, "Camera");
    glUniformBlockBinding(shaderProgram, uniformBlockIndex, pimpl->index);
  }

  void CameraComponent::updateView()
  {
    auto& transformComponent = pimpl->transform;
    auto pos = transformComponent->getPosition();
    auto forward = transformComponent->getForward();
    auto up = transformComponent->getUp();
    pimpl->worldToCamera = glm::lookAt(pos, pos + forward, up);
    glBindBuffer(GL_UNIFORM_BUFFER, pimpl->cameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(pimpl->worldToCamera));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
  }

  void CameraComponent::updatePerspective( float aspectRatio )
  {
    pimpl->cameraToPerspective=glm::perspective(3.1415f/4.0f, aspectRatio, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, pimpl->cameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(pimpl->cameraToPerspective));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
  }

  void CameraComponent::init()
  {
    auto entity = entityWPtr_.lock();
    pimpl->transform = entity->getComponent<TransformComponent>(game::ComponentType::TRANSFORM_COMPONENT);
  }
}
