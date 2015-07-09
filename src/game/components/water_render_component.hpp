#pragma once
#include "render_component.hpp"

namespace game
{  
  class WaterRenderComponent : public RenderComponent
  {
  private:
    struct WaterRenderComponentImpl;
    std::unique_ptr<WaterRenderComponentImpl> pimpl;
  public:
    WaterRenderComponent(EntityWPtr entityWPtr);
    ~WaterRenderComponent();
    virtual void render() override;
    virtual void init() override; 
  };
}
