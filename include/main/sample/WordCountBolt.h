//
// Created by rocklct on 2/23/18.
//

#pragma once

#include "sandstorm/bolt/IBolt.h"

#include <fstream>

#include <map>
#include <string>
#include <cstdint>


class WordCountBolt : public sandstorm::bolt::IBolt {
public:
    virtual sandstorm::bolt::IBolt *Clone() override {
        return new WordCountBolt(*this);
    }

    virtual void Prepare(std::shared_ptr<sandstorm::collector::OutputCollector> outputCollector) override;

    virtual void Cleanup() override;

    virtual std::vector<std::string> DeclareFields() override;

    virtual void Execute(const sandstorm::base::Tuple &tuple) override;

private:
    std::shared_ptr<sandstorm::collector::OutputCollector> _outputCollector;
    std::map<std::string, int32_t> _wordCounts;
    std::ofstream *_logFile;
};