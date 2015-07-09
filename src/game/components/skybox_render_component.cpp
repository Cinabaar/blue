#include "skybox_render_component.hpp"
#include "model_component.hpp"

namespace game
{
  struct SkyboxRenderComponent::SkyboxRenderComponentImpl
  {
    std::shared_ptr<ModelComponent> model;
  };
  
  SkyboxRenderComponent::SkyboxRenderComponent(EntityWPtr entity) :
    RenderComponent(entity),
    pimpl(std::unique_ptr<SkyboxRenderComponentImpl>(new SkyboxRenderComponentImpl))
  {}

  SkyboxRenderComponent::~SkyboxRenderComponent()
  { }

  void SkyboxRenderComponent::render()
  {
    glUseProgram (shaderProgram_);
    glDepthMask(GL_FALSE);
    glBindVertexArray(pimpl->model->getArrayObject());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, pimpl->model->getTexture());
    glDrawArrays(GL_TRIANGLES, 0, pimpl->model->vertexCount()/3);
    glDepthMask(GL_TRUE);
  }  

  void SkyboxRenderComponent::init()
  {
    pimpl->model = entityWPtr_.lock()->getComponent<ModelComponent>(ComponentType::MODEL_COMPONENT);
  }
}
