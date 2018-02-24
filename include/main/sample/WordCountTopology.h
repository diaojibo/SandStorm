//
// Created by rocklct on 2/23/18.
//

#pragma once

#define BEGIN_EXTERN_C extern "C" {
#define END_EXTERN_C }

namespace sandstorm {
    namespace topology {
        class Topology;
    }

}

//sandstorm::topology::Topology *GetTopology();

BEGIN_EXTERN_C
        sandstorm::topology::Topology* GetTopology();
END_EXTERN_C