#include "water_render_component.hpp"
#include "water_model_component.hpp"
#include "transform_component.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace game
{
  struct WaterRenderComponent::WaterRenderComponentImpl
  {
    std::shared_ptr<TransformComponent> transform;
    std::shared_ptr<WaterModelComponent> model;
  };
  
  void WaterRenderComponent::render()
  {
    auto program = getShaderProgram();
    glUseProgram (program);
    int modelToWorldLocation = glGetUniformLocation (program, "modelToWorld");
    glUniformMatrix4fv (modelToWorldLocation, 1, GL_FALSE, glm::value_ptr(pimpl->transform->getTransform()));
    glBindVertexArray(pimpl->model->vao);
    glDrawArrays(GL_POINTS, 0, pimpl->model->points.size()/3);
  }

  void WaterRenderComponent::init()
  {
    auto entity = entityWPtr_.lock();
    pimpl->transform = entity->getComponent<TransformComponent>(ComponentType::TRANSFORM_COMPONENT);
    pimpl->model = entity->getComponent<WaterModelComponent>(ComponentType::MODEL_COMPONENT);
  }

  WaterRenderComponent::WaterRenderComponent(EntityWPtr entityWPtr) :
    RenderComponent(entityWPtr),
    pimpl(std::unique_ptr<WaterRenderComponentImpl>(new WaterRenderComponentImpl()))
  {}

  WaterRenderComponent::~WaterRenderComponent() {}
}
