#include <iostream>
#include <fstream>
#include <stdexcept>
#include "WordleHelper.h"

std::size_t diff_chars(const std::string& s) {
    return std::set<std::size_t>(s.begin(), s.end()).size();
}

int main() {
    std::ifstream fin("vocabulary.txt");

    if(!fin.is_open()) {
        std::cout << "cannot find vocabulary.txt" << std::endl;
        std::cout << "press enter to exit" << std::endl;
        std::string buf;
        std::cin >> buf;
        return 0;
    }

    std::vector<std::string> voc;
    std::size_t word_length;

    while(!fin.eof()) {
        std::string s;
        getline(fin, s);
        if(s.empty()) {
            continue;
        }
        voc.push_back(s);
    }

    std::cout << voc.size() << " words in vocabulary" << std::endl;
    std::cout << "enter length of words: ";
    std::cin >> word_length;
    std::cin.ignore();

    std::cout << "enter the word from your attempt, then result of your attempt (on two separated lines)" << std::endl;
    std::cout << "string with your result must consist of +-? symbols" << std::endl;
    std::cout << "just press enter to exit" << std::endl;

    WordleHelper helper(voc, word_length);
    while(true) {
        std::string word, result;
        getline(std::cin, word);
        if(word.empty()) {
            break;
        }
        getline(std::cin, result);
        if(result.empty()) {
            break;
        }

        try {
            helper.new_attempt(word, result);
        }
        catch(std::invalid_argument& exc) {
            std::cout << exc.what() << std::endl;
            std::cout << "try enter strings again" << std::endl;
            continue;
        }
        auto suitable_words = helper.get_suitable_words();

        std::cout << "there are " << suitable_words.size() << " suitable words" << std::endl;

        auto comp = [](const std::string &lhs, const std::string &rhs) { return diff_chars(lhs) > diff_chars(rhs);};
        std::sort(suitable_words.begin(), suitable_words.end(), comp);

        if(suitable_words.size() > 50) {
            std::cout << "the shorten list of words" << std::endl;
            for(std::size_t i = 0; i < 50; i++) {
                std::cout << suitable_words[i] << std::endl;
            }
        }
        else {
            for(const auto& i : suitable_words) {
                std::cout << i << std::endl;
            }
        }
    }

    return 0;
}