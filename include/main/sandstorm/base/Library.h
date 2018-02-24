//
// Created by rocklct on 1/24/18.
//

#pragma once

#include <string>
#include <iostream>


#include <dlfcn.h>

typedef void* LibraryHandle;
inline LibraryHandle SandstormLibraryLoad(const std::string& name) {
    std::string path("lib");
    path += name + ".so";
    LibraryHandle handle = dlopen(path.c_str(), RTLD_NOW);
    if ( !handle ) {
        exit(EXIT_FAILURE);
    }

    return handle;
}
#define HurricaneLibraryFree dlclose

#define HurricaneGetLibraryError() dlerror()


template <class Function>
Function SandstormLibraryGetSymbol(LibraryHandle libraryHandle, const std::string& libraryName) {
    return reinterpret_cast<Function>(dlsym(libraryHandle, libraryName.c_str()));
}

std::string GetLibraryPath();

