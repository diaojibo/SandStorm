//
// Created by rocklct on 2/23/18.
//

#include "sample/WordCountTopology.h"

#include "sample/HelloWorldSpout.h"
#include "sample/SplitSentenceBolt.h"
#include "sample/WordCountBolt.h"

#include "sandstorm/topology/Topology.h"

sandstorm::topology::Topology *GetTopology() {
    sandstorm::topology::Topology *topology = new sandstorm::topology::Topology("word-count-topology");

    topology->SetSpout("hello-world-spout", new HelloWorldSpout)
            .ParallismHint(1);

    topology->SetBolt("split-sentence-bolt", new SplitSentenceBolt)
            .Random("hello-world-spout")
            .ParallismHint(3);

    topology->SetBolt("word-count-bolt", new WordCountBolt)
            .Field("split-sentence-bolt", "word")
            .ParallismHint(2);

    return topology;
}