//
// Created by ilyazavalov on 01/11/22.
//

#include "WordleHelper.h"

const std::string WordleHelper::result_alphabet = "+-?";
const std::vector<std::function<bool(WordleHelper*,std::string)>> predicates = {WordleHelper::length_check, WordleHelper::banned_check, WordleHelper::required_check,  WordleHelper::pattern_check, WordleHelper::letters_check};

WordleHelper::WordleHelper(const std::vector<std::string> &voc, std::size_t len) {
    vocabulary = voc;
    word_length = len;
    pattern = std::string(len, '#');
    letters_not_at = std::vector<std::set<char>>(len);
}

void WordleHelper::new_attempt(const std::string &word, const std::string &result) {
    if(word.size() != word_length) {
        throw std::invalid_argument("the word has incorrect length");
    }
    if(result.size() != word_length) {
        throw std::invalid_argument("the result string has incorrect length");
    }
    if(std::any_of(result.begin(), result.end(), [](char c) { return result_alphabet.find(c) == std::string::npos; })) {
        throw std::invalid_argument("result string can contains only +-? symbols");
    }
    for(std::size_t i = 0; i < word_length; i++) {
        if(pattern[i] == '#') {
            continue;
        }
        if((pattern[i] != word[i] && result[i] == '+') || (pattern[i] == word[i] && result[i] != '+')) {
            throw std::invalid_argument("the result contradicts the previous");
        }
    }

    for(std::size_t i = 0; i < word_length; i++) {
        if(result[i] == '-') {
            banned.insert(word[i]);
        }
        else if(result[i] == '+') {
            pattern[i] = word[i];
            letters_not_at[i].clear();
        }
        else if(result[i] == '?') {
            required.insert(word[i]);
            letters_not_at[i].insert(word[i]);
        }
    }
}

bool WordleHelper::length_check(WordleHelper* obj, const std::string &s) {
    return s.size() == obj->word_length;
}

bool WordleHelper::banned_check(WordleHelper* obj, const std::string &s) {
    return std::all_of(s.begin(), s.end(), [obj](char c) { return !obj->banned.contains(c); });
}

bool WordleHelper::required_check(WordleHelper* obj, const std::string &s) {
    return std::all_of(obj->required.begin(), obj->required.end(), [&s](char c) { return s.find(c) != std::string::npos;  });
}

bool WordleHelper::pattern_check(WordleHelper* obj, const std::string &s) {
    for(std::size_t i = 0; i < obj->word_length; i++) {
        if(obj->pattern[i] != '#' && s[i] != obj->pattern[i]) {
            return false;
        }
    }
    return true;
}

bool WordleHelper::letters_check(WordleHelper* obj, const std::string &s) {
    for(std::size_t i = 0; i < obj->word_length; i++) {
        if(obj->letters_not_at[i].contains(s[i])) {
            return false;
        }
    }
    return true;
}

void WordleHelper::clear_all() {
    banned.clear();
    required.clear();
    pattern = std::string(word_length, '#');
    for(auto &v : letters_not_at) {
        v.clear();
    }
}

std::vector<std::string> WordleHelper::get_suitable_words() {
    std::vector<std::string> res;
    auto pred = [this](const std::string& s) { return std::all_of(::predicates.begin(), ::predicates.end(), [this, &s](const auto &p) { return p(this, s);});};
    std::copy_if(vocabulary.begin(), vocabulary.end(), std::back_inserter(res), pred);
    return res;
}
