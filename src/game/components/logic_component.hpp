#pragma once
#include "component.hpp"
#include "game/entity.hpp"

namespace game
{
  class LogicComponent : public Component
  {
  public:
    LogicComponent(EntityWPtr entityWPtr) : Component(entityWPtr) {}
    ~LogicComponent() {}
    virtual void update(double deltaTime) = 0;
    virtual ComponentType getComponentType() override { return ComponentType::LOGIC_COMPONENT; }
  };
}
