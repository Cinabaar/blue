#include "process.hpp"

namespace process {
    Process::Process() {
        _state = ProcessState::UNINITIALIZED;
    }

    ProcessUPtrList& Process::getSuccessors() {
        return _successors;
    }

    void Process::addSuccessor(ProcessUPtr process) {
        _successors.push_back(std::move(process));
    }
}
