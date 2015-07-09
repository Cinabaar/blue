#include "scene.hpp"
#include "components/components.hpp"
#include "easylogging++.h"

namespace game
{

  struct Scene::SceneImpl
  {
    std::vector<EntitySPtr> entities;
    EntitySPtr activeCameraWPtr;
  };
  Scene::Scene() :
    pimpl(std::unique_ptr<SceneImpl>(new SceneImpl()))
  {}
  void Scene::render()
  {
    for(auto& entity : pimpl->entities)
    {
      if(entity->hasComponent(ComponentType::RENDER_COMPONENT)) {
        auto component = entity->getComponent<RenderComponent>(ComponentType::RENDER_COMPONENT);
        component->render();
      }
    }
  }

  void Scene::update(double deltaTime)
  {
    for(auto& entity : pimpl->entities)
    {
      if(entity->hasComponent(ComponentType::LOGIC_COMPONENT)) {
        auto component = entity->getComponent<LogicComponent>(ComponentType::LOGIC_COMPONENT);
        component->update(deltaTime);
      }
    }
  }

  void Scene::setActiveCamera( EntitySPtr camera )
  {
    pimpl->activeCameraWPtr = camera;
  }

  void Scene::addEntity( EntitySPtr entity )
  {
    pimpl->entities.push_back(entity);
  }

  Scene::~Scene() {}
}
