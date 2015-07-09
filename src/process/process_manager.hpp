#pragma once

#include <list>
#include "process.hpp"

namespace process {
    class ProcessManager {
    private:
        ProcessUPtrList _processList;
    public:
        void update(double deltaTime);
        void attachProcess(ProcessUPtr processUPtr);
        bool ended();
    };
}
