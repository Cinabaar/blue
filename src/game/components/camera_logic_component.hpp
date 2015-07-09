#pragma once
#include "logic_component.hpp"

namespace game
{
  class CameraLogicComponent : public LogicComponent
  {
    struct Impl;
    std::unique_ptr<Impl> pimpl;
  public:
    CameraLogicComponent(EntityWPtr entityWPtr);
    ~CameraLogicComponent();
    
    virtual void update(double deltaTime) override;
    virtual void init() override;
  };
}
