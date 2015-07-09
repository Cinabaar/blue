#pragma once

#include <memory>
#include <list>
#include "util/util.hpp"

namespace process {
  class Process;

  using ProcessSPtr = std::shared_ptr<Process>; 
  using ProcessSPtrList = std::list<ProcessSPtr>;
  using ProcessUPtr = std::unique_ptr<Process> ;
  using ProcessUPtrList = std::list<ProcessUPtr>;

  class Process : NonCopyable<Process> {
  public:
    enum class ProcessState {
      UNINITIALIZED,
        RUNNING,
        SUCCEEDED,
        FAILED,
        ABORTED,
        };

  protected:
    ProcessState _state;
    ProcessUPtrList _successors;

  public:
    Process();
    virtual ~Process() {}

    virtual void update(double deltaTime) = 0;
    virtual void init() {_state = ProcessState::RUNNING;};
    virtual void end() {_state = ProcessState::SUCCEEDED;};

    ProcessUPtrList& getSuccessors();

    void addSuccessor(ProcessUPtr process);

    bool succeeded() const {
      return _state == ProcessState::SUCCEEDED;
    }

    bool isUninitialized() const {
      return _state == ProcessState::UNINITIALIZED;
    }

    bool isAlive() const {
      return _state == ProcessState::RUNNING;
    }

    bool isDead() const {
      return _state == ProcessState::SUCCEEDED ||
        _state == ProcessState::FAILED ||
        _state == ProcessState::ABORTED;
    }
  };
}
