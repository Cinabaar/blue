#pragma once

#include "CL/cl.h"
#include "clpp.h"
#include "logic_component.hpp"
#include <vector>

namespace game
{
  class WaterLogicComponent : public LogicComponent
  {
  private:
    struct WaterLogicComponentImpl;
    std::unique_ptr<WaterLogicComponentImpl> pimpl;
  public:
    WaterLogicComponent(EntityWPtr entityWPtr);
    ~WaterLogicComponent();
    virtual void update(double deltaTime) override;
    virtual void init() override;
  };
}
