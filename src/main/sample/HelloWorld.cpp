//
// Created by rocklct on 2/23/18.
//

#include "sample/HelloWorldSpout.h"
#include "sandstorm/util/StringUtil.h"

#include <thread>
#include <chrono>

void HelloWorldSpout::Prepare(std::shared_ptr<sandstorm::collector::OutputCollector> outputCollector) {
    _outputCollector = outputCollector;
    _words = SplitString("Hello world there are some words we generate new sentence randomly", ' ');
}

void HelloWorldSpout::Cleanup() {
}

std::vector<std::string> HelloWorldSpout::DeclareFields() {
    return {"sentence"};
}


void HelloWorldSpout::NextTuple() {
    static int32_t id = 0;

    int64_t currentMicroseconds = 0;

    ++id;

    std::vector<std::string> words(5);
    for (int32_t i = 0; i < 5; i++) {
        words[i] = _words[rand() % _words.size()];
    }

    std::string sentence = JoinString(words);
    _outputCollector->Emit({
                                   sentence, currentMicroseconds, id
                           });

}