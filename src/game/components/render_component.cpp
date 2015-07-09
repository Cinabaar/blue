#include "render_component.hpp"
#include "transform_component.hpp"
#include "model_component.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace game
{
  struct RenderComponent::RenderComponentImpl
  {
    std::shared_ptr<TransformComponent> transform;
    std::shared_ptr<ModelComponent> model;
  };
  
  RenderComponent::RenderComponent(EntityWPtr entityWPtr) :
    Component(entityWPtr),
    pimpl(std::unique_ptr<RenderComponentImpl>(new RenderComponentImpl()))
  {}
  
  void RenderComponent::setShaderProgram( GLuint shaderProgram )
  {
    shaderProgram_ = shaderProgram;
  }

  GLuint RenderComponent::getShaderProgram() const
  {
    return shaderProgram_;
  }

  void RenderComponent::render()
  {
    glUseProgram (shaderProgram_);
    int modelToWorldLocation = glGetUniformLocation (shaderProgram_, "modelToWorld");
    glUniformMatrix4fv (modelToWorldLocation, 1, GL_FALSE, glm::value_ptr(pimpl->transform->getTransform()));
    glBindVertexArray(pimpl->model->getArrayObject());
    glDrawArrays(GL_TRIANGLES, 0, pimpl->model->vertexCount()/3);
  } 

  void RenderComponent::init()
  {
    
    std::shared_ptr<Entity> entity = entityWPtr_.lock();
    pimpl->transform = entity->getComponent<TransformComponent>(game::ComponentType::TRANSFORM_COMPONENT);
    pimpl->model = entity->getComponent<ModelComponent>(ComponentType::MODEL_COMPONENT);
  }

  ComponentType RenderComponent::getComponentType()
  {
    return ComponentType::RENDER_COMPONENT;
  }

  RenderComponent::~RenderComponent()
  {}
}
