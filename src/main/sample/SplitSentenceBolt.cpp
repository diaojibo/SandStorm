//
// Created by rocklct on 2/23/18.
//

#include "sample/SplitSentenceBolt.h"

#include "sandstorm/util/StringUtil.h"

void SplitSentenceBolt::Prepare(std::shared_ptr<sandstorm::collector::OutputCollector> outputCollector) {
    _outputCollector = outputCollector;
}

void SplitSentenceBolt::Cleanup() {
}

std::vector<std::string> SplitSentenceBolt::DeclareFields() {
    return {"word"};
}

void SplitSentenceBolt::Execute(const sandstorm::base::Tuple &tuple) {
    std::string sentence = tuple[0].GetStringValue();
    int64_t sourceMicroseconds = tuple[1].GetInt64Value();
    int32_t id = tuple[2].GetInt32Value();

    std::vector<std::string> words = SplitString(sentence, ' ');

    for (const std::string &word : words) {
        _outputCollector->Emit({word, sourceMicroseconds, id});
    }
}