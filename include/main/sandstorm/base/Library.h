//
// Created by rocklct on 1/24/18.
//

#pragma once

#include <string>
#include <iostream>


#include <dlfcn.h>

typedef void *LibraryHandle;

inline LibraryHandle SandstormLibraryLoad(const std::string &name) {
    std::string path("lib");
    path = "./" + path + name + ".so";
    std::cout << path << std::endl;
    LibraryHandle handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle) {

        std::cout << "load failed " << dlerror() << std::endl;
        exit(EXIT_FAILURE);
    }

    return handle;
}

#define HurricaneLibraryFree dlclose

#define HurricaneGetLibraryError() dlerror()


template<class Function>
Function SandstormLibraryGetSymbol(LibraryHandle libraryHandle, const std::string &libraryName) {
    return reinterpret_cast<Function>(dlsym(libraryHandle, libraryName.c_str()));
}

std::string GetLibraryPath();

