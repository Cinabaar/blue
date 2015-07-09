#pragma once
#include "render_component.hpp"
namespace game
{
class SkyboxRenderComponent : public RenderComponent
{
private:
  struct SkyboxRenderComponentImpl;
  std::unique_ptr<SkyboxRenderComponentImpl> pimpl;
public:
  SkyboxRenderComponent(EntityWPtr entity);
  ~SkyboxRenderComponent();
  virtual void render() override;
  virtual void init() override;
};
}
