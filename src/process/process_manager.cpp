#include "process_manager.hpp"

namespace process {

    void ProcessManager::attachProcess(ProcessUPtr processUPtr) {
        _processList.push_back(std::move(processUPtr));

    }

    void ProcessManager::update(double deltaTime) {
        auto processIt = _processList.begin();
        while (processIt != _processList.end()) {
            auto &process = *processIt;
            if (process->isUninitialized())
                process->init();
            if (process->isAlive())
                process->update(deltaTime);
            if (process->isDead()) {
                if (process->succeeded()) {
                    auto &successors = process->getSuccessors();
                    for (auto &successor : successors) {
                        attachProcess(std::move(successor));
                    }
                }
                process->end();
                _processList.erase(processIt++);
            }
            else
                processIt++;
        }
    }

    bool ProcessManager::ended() {
        return _processList.empty();

    }
}
