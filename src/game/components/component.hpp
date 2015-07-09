#pragma once
#include "component_type.hpp"
#include <memory>

namespace game
{
  class Entity;
  class Component
  {
  protected:
    std::weak_ptr<Entity> entityWPtr_;
  public:
    Component(std::weak_ptr<Entity> entityWPtr) : entityWPtr_(entityWPtr) {};
    virtual ~Component() {}
    virtual ComponentType getComponentType() = 0;
    virtual void init() = 0;
  };

  using ComponentSPtr = std::shared_ptr<Component>;
}
