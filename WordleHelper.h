//
// Created by ilyazavalov on 01/11/22.
//

#ifndef WORLDER_HELPER_WORDLEHELPER_H
#define WORLDER_HELPER_WORDLEHELPER_H

#include <vector>
#include <string>
#include <functional>
#include <set>
#include <stdexcept>
#include <algorithm>

class WordleHelper {
private:
    std::vector<std::string> vocabulary;
    std::set<char> banned;
    std::set<char> required;
    std::string pattern;
    std::vector<std::set<char>> letters_not_at;
    std::size_t word_length;

    static const std::vector<std::function<bool(WordleHelper*,std::string)>> predicates;
public:
    WordleHelper(const std::vector<std::string>& voc, std::size_t len);
    void new_attempt(const std::string& word, const std::string& result);
    std::vector<std::string> get_suitable_words();
    void clear_all();

    static bool length_check(WordleHelper* obj, const std::string& s);
    static bool banned_check(WordleHelper* obj, const std::string& s);
    static bool required_check(WordleHelper* obj, const std::string& s);
    static bool pattern_check(WordleHelper* obj, const std::string& s);
    static bool letters_check(WordleHelper* obj, const std::string& s);

    static const std::string result_alphabet;
};


#endif //WORLDER_HELPER_WORDLEHELPER_H
