#include "entity.hpp"
#include "easylogging++.h"
namespace game
{
  Entity::Entity(std::string name) : name_{name} {}

  Entity::~Entity()
  {
    components_.clear();
  }

  bool Entity::hasComponent(ComponentType type) {
    return components_.find(type) != components_.end();
  }

  void Entity::addComponent(std::shared_ptr<Component> component) {
    components_[component->getComponentType()] = component;
  }

  void Entity::clearComponent(ComponentType type) {
    if(hasComponent(type))
      components_.erase(type);
  }

  void Entity::init()
  {
    for(auto& component : components_)
    {
      LOG(DEBUG) << "1";
      component.second->init();
      LOG(DEBUG) << "2";
    }
  }
}
