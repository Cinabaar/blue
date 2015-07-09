#pragma once
#include <vector>
#include "entity.hpp"
namespace game
{
  class Scene
  {
  private:
    struct SceneImpl;
    std::unique_ptr<SceneImpl> pimpl;
  public:
    Scene();
    ~Scene();
    void render();
    void update(double deltaTime);
    void addEntity(EntitySPtr entity);
    void setActiveCamera(EntitySPtr camera);
  };
  using SceneUPtr = std::unique_ptr<Scene>;
}
