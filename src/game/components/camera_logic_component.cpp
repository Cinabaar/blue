#include "camera_logic_component.hpp"
#include "nod/nod.hpp"
#include "event/event_manager.hpp"
#include "event/window/window_events.hpp"
#include "easylogging++.h"
#include "format.h"
#include <cmath>
#include "transform_component.hpp"
#include "camera_component.hpp"

namespace game
{
  using namespace event;
  using namespace std::placeholders;
  using std::ref;
  struct CameraLogicComponent::Impl
  {
    bool moveForward, moveBack, moveLeft, moveRight;
    glm::vec2 mouseMove;
    float moveSpeed, rotateSpeed;
    std::map<std::string, nod::connection> connections;
    void onUpPressed(IEventDataSPtr& event) {moveForward = true;}
    void onDownPressed(IEventDataSPtr& event) {moveBack = true;}
    void onLeftPressed(IEventDataSPtr& event) {moveLeft = true;}
    void onRightPressed(IEventDataSPtr& event) {moveRight = true;}
    void onUpReleased(IEventDataSPtr& event) {moveForward = false;}
    void onDownReleased(IEventDataSPtr& event) {moveBack = false;}
    void onRightReleased(IEventDataSPtr& event) {moveRight = false;}
    void onLeftReleased(IEventDataSPtr& event) {moveLeft = false;}
    void onMouseMoved(IEventDataSPtr& event)
    {
      auto data = std::static_pointer_cast<OnCursorPositionChanged>(event);
      mouseMove = data->getVec();
      LOG(INFO) << mouseMove.x << " " << mouseMove.y;
    }
    std::shared_ptr<CameraComponent> camera;
    std::shared_ptr<TransformComponent> transform;
  };

  CameraLogicComponent::CameraLogicComponent(EntityWPtr entityWPtr) :
    LogicComponent(entityWPtr),
    pimpl(std::unique_ptr<Impl>(new Impl()))
  {
    pimpl->moveForward = false;
    pimpl->moveBack = false;
    pimpl->moveRight = false;
    pimpl->moveLeft = false;
    pimpl->moveSpeed = 1;
    pimpl->rotateSpeed = 100;
    auto& eventManager = EventManager::get();
    auto& c = pimpl->connections;
    c["up"] = eventManager.addListener(EventType::KEYBOARD_UP_PRESSED, bind(&Impl::onUpPressed, ref(pimpl), _1));
    c["dp"] = eventManager.addListener(EventType::KEYBOARD_DOWN_PRESSED, bind(&Impl::onDownPressed, ref(pimpl), _1));
    c["lp"] = eventManager.addListener(EventType::KEYBOARD_LEFT_PRESSED, bind(&Impl::onLeftPressed, ref(pimpl), _1));
    c["rp"] = eventManager.addListener(EventType::KEYBOARD_RIGHT_PRESSED, bind(&Impl::onRightPressed, ref(pimpl), _1));
    c["ur"] = eventManager.addListener(EventType::KEYBOARD_UP_RELEASED, bind(&Impl::onUpReleased, ref(pimpl), _1));
    c["dr"] = eventManager.addListener(EventType::KEYBOARD_DOWN_RELEASED, bind(&Impl::onDownReleased, ref(pimpl), _1));
    c["lr"] = eventManager.addListener(EventType::KEYBOARD_LEFT_RELEASED, bind(&Impl::onLeftReleased, ref(pimpl), _1));
    c["rr"] = eventManager.addListener(EventType::KEYBOARD_RIGHT_RELEASED, bind(&Impl::onRightReleased, ref(pimpl), _1));
    c["mm"] = eventManager.addListener(EventType::MOUSE_CURSOR_POS, bind(&Impl::onMouseMoved, ref(pimpl), _1));
    c["wr"] = eventManager.addListener(EventType::WINDOW_RESIZED, [this](IEventDataSPtr& event) {
        auto data = std::dynamic_pointer_cast<OnWindowResized>(event);
        //LOG(DEBUG) << fmt::format("Received window resize event. Width: {0}, height: {1}.", data->getWidth(), data->getHeight());
        pimpl->camera->updatePerspective((float)data->getWidth()/data->getHeight());
      });
  }

  void CameraLogicComponent::update(double deltaTime)
  {
    //LOG(DEBUG)<<fmt::format("u:{0} d:{1} l:{2} r:{3} mx:{4} my:{5}",
    //                       pimpl->moveForward, pimpl->moveBack, pimpl->moveLeft, pimpl->moveRight, pimpl->mouseMove.x, pimpl->mouseMove.y);
    float zMove = pimpl->moveForward - pimpl->moveBack;
    float xMove = pimpl->moveRight - pimpl->moveLeft;
    auto& mouseMove = pimpl->mouseMove;
    auto& moveSpeed = pimpl->moveSpeed;
    auto& rotateSpeed = pimpl->rotateSpeed;
    auto& transform = pimpl->transform;
    transform->moveBy((zMove * transform->getForward() + xMove * transform->getRight()) * moveSpeed * (float)deltaTime);
    transform->rotateBy(deltaTime*rotateSpeed*mouseMove.x, vec3(0, -1, 0));
    transform->rotateBy(deltaTime*rotateSpeed*mouseMove.y, transform->getRight());
    mouseMove = vec2(0);
    pimpl->camera->updateView();
  }

  void CameraLogicComponent::init()
  {
    auto entitySPtr = entityWPtr_.lock();
    pimpl->transform = entitySPtr->getComponent<TransformComponent>(ComponentType::TRANSFORM_COMPONENT);
    pimpl->camera = entitySPtr->getComponent<CameraComponent>(ComponentType::CAMERA_COMPONENT);
  } 

  CameraLogicComponent::~CameraLogicComponent()
  {
    for(auto& connection : pimpl->connections)
    {
      connection.second.disconnect();
    }
  }
}
