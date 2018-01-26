//
// Created by Rocklct on 2017/12/29.
//

#pragma once

#include <dlfcn.h>

#include "sandstorm/topology/ITopology.h"

using sandstorm::topology::ITopology

namespace sandstorm {
    namespace base {
        ITopology *LoadTopology(const std::string &fileName) {
            //define a file pointer
            void *libm_handle = NULL;
            typedef ITopology *(TopologyGetter)();
            char *errorInfo;
            float result;

            // use dlopen to open the so file
            libm_handle = dlopen(fileName.c_str(), RTLD_LAZY);

            // check error
            if (!libm_handle) {
                printf("Open Error:%s.\n", dlerror());
                return 0;
            }

            //use dlsym to get the correspond function called GetTopology
            TopologyGetter GetTopology = dlsym(libm_handle, "GetTopology");
            errorInfo = dlerror();

            //check dlsym error
            if (errorInfo != NULL) {
                printf("Dlsym Error:%s.\n", errorInfo);
                return 0;
            }

            ITopology *topology = GetTopology;

            dlclose(libm_handle);

            return topology;
        }
    }
}
