//
// Created by rocklct on 2/14/18.
//

#pragma once

#include <vector>
#include <string>
#include <cstdint>

const char JOIN_STRING_DEFAULT_SPLITTER = ' ';

std::vector<std::string> SplitString(const std::string &value, char seperator);

std::string TrimString(const std::string &value);

std::string RandomString(const std::string &candidate, int32_t length);

std::string JoinString(const std::vector<std::string> &words, char splitter = JOIN_STRING_DEFAULT_SPLITTER);

std::string Int2String(int32_t value);