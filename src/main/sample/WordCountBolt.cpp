//
// Created by rocklct on 2/23/18.
//

#include "sample/WordCountBolt.h"

#include <sstream>

std::string itos(int32_t number) {
    std::ostringstream ss;
    ss << number;

    return ss.str();
}


void WordCountBolt::Prepare(std::shared_ptr<sandstorm::collector::OutputCollector> outputCollector) {
    _outputCollector = outputCollector;
    _logFile = new std::ofstream("timestamp" + itos(rand()) + ".txt");
}

void WordCountBolt::Cleanup() {
    delete _logFile;
}

std::vector<std::string> WordCountBolt::DeclareFields() {
    return {"word", "count"};
}

void WordCountBolt::Execute(const sandstorm::base::Tuple &tuple) {
    std::string word = tuple[0].GetStringValue();
    int64_t sourceMicroseconds = tuple[1].GetInt64Value();
    int32_t id = tuple[2].GetInt32Value();

    auto wordCountIterator = _wordCounts.find(word);
    if (wordCountIterator == _wordCounts.end()) {
        _wordCounts.insert({word, 0});
        wordCountIterator = _wordCounts.find(word);
    }

    wordCountIterator->second++;

    std::cout << word << ' ' << wordCountIterator->second;
    _outputCollector->Emit({word, wordCountIterator->second});

    int64_t currentMicroseconds = 0;

    *_logFile << sourceMicroseconds << ' ' << currentMicroseconds << std::endl;
}