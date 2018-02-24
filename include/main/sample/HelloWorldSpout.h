//
// Created by rocklct on 2/23/18.
//

#pragma once

#include "sandstorm/spout/ISpout.h"

#include <vector>

class HelloWorldSpout : public sandstorm::spout::ISpout {
public:
    virtual sandstorm::spout::ISpout *Clone() override {
        return new HelloWorldSpout(*this);
    }

    virtual void Prepare(std::shared_ptr<sandstorm::collector::OutputCollector> outputCollector) override;

    virtual void Cleanup() override;

    virtual std::vector<std::string> DeclareFields() override;

    virtual void NextTuple() override;

private:

    std::shared_ptr<sandstorm::collector::OutputCollector> _outputCollector;

    std::vector<std::string> _words;
};