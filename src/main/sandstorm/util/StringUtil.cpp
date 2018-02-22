//
// Created by rocklct on 2/22/18.
//

#include "sandstorm/util/StringUtil.h"

#include <cstdlib>
#include <ctime>
#include <cctype>
#include <sstream>

using std::vector;
using std::string;

vector<string> SplitString(const string &value, char seperator) {
    vector<string> splitedStrings;

    size_t currentPos = 0;
    while (true) {
        size_t nextPos = value.find(seperator, currentPos);
        if (nextPos == string::npos) {
            string currentString = value.substr(currentPos);
            if (!currentString.empty()) {
                splitedStrings.push_back(currentString);
            }

            break;
        }

        splitedStrings.push_back(value.substr(currentPos, nextPos - currentPos));
        currentPos = nextPos + 1;
    }

    return splitedStrings;
}

std::string TrimString(const std::string &value) {
    std::string trimmed = value;

    while (!trimmed.empty() && isblank(trimmed.front())) {
        trimmed.erase(trimmed.begin());
    }

    while (!trimmed.empty() && isblank(trimmed.back())) {
        trimmed.erase(trimmed.end() - 1);
    }

    return trimmed;
}

std::string RandomString(const std::string &candidate, int32_t length) {
    srand(static_cast<uint32_t>(time(0)));

    std::string result;
    for (int32_t index = 0; index != length; ++index) {
        int32_t charIndex = rand() % candidate.size();
        result.push_back(candidate[charIndex]);
    }

    return result;
}

std::string Int2String(int32_t value) {
    std::ostringstream os;
    os << value;

    return os.str();
}

std::string JoinString(const std::vector<std::string> &words, char splitter) {
    std::string sentence;
    for (const std::string &word : words) {
        sentence += word + splitter;
    }

    sentence.pop_back();

    return sentence;
}