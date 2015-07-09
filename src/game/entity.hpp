#pragma once

#include <memory>
#include <map>
#include "components/component.hpp"
#include "components/component_type.hpp"

namespace game
{
  class Entity {
  private:
    std::string name_;
    std::map<ComponentType, std::shared_ptr<Component>> components_;
  public:
    Entity(std::string name);
    ~Entity();
    bool hasComponent(ComponentType type);
    void addComponent(ComponentSPtr component);
    void clearComponent(ComponentType type);
    template<class T>
    std::shared_ptr<T> getComponent(ComponentType type) {
      static_assert(std::is_base_of<Component, T>::value, "Get component called with non-component type");
      return std::static_pointer_cast<T>(components_.at(type));
    }
    void init();
    
  };
  
  using EntityUPtr = std::unique_ptr<Entity>;
  using EntitySPtr = std::shared_ptr<Entity>; 
  using EntityWPtr = std::weak_ptr<Entity>;
}
