//
// Created by rocklct on 2/23/18.
//

#pragma once

#include "sandstorm/bolt/IBolt.h"

class SplitSentenceBolt : public sandstorm::bolt::IBolt {
public:

    virtual sandstorm::bolt::IBolt *Clone() override {
        return new SplitSentenceBolt;
    }

    virtual void Prepare(std::shared_ptr<sandstorm::collector::OutputCollector>
                         outputCollector) override;

    virtual void Cleanup() override;

    virtual std::vector<std::string> DeclareFields() override;

    virtual void Execute(const sandstorm::base::Tuple &tuple) override;


private:

    std::shared_ptr<sandstorm::collector::OutputCollector> _outputCollector;
};
