#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <set>
#include <random>
#include <direct.h>


class RandomWord {
    std::default_random_engine dre;
    std::string table; // 字母表
    std::uniform_int_distribution<int> distAlph; // 从字母表中随机挑选字母序号
public:
    RandomWord(unsigned seed):dre(seed) {
        table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        distAlph = std::uniform_int_distribution<int>(0, table.length() - 1);
    }
    RandomWord() {
        table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        distAlph = std::uniform_int_distribution<int>(0, table.length() - 1);
    }

    // TODO: return random words with length less than max_length
    std::string make_random_word(size_t min_length = 3, size_t max_length = 5) {
        std::uniform_int_distribution<size_t> dist(min_length, max_length);
        size_t len = dist(dre);
        std::string randString;
        for(auto i = 0; i < len; ++i) {
            randString += table[distAlph(dre)];
        }
        return randString;
    }

    // TODO: return a list of random words
    std::vector<std::string> make_random_words(size_t num, size_t min_length = 3, size_t max_length = 5) {
        std::vector<std::string> words;
        for(auto i = 0; i < num; ++i) {
            words.push_back(make_random_word(min_length, max_length));
        }
        return words;
    }

};

// ----------------------------------------------------------------
// TODO: read file line by line
/**
 * @brief read file line by line
 * 
 * @param file: std::string 
 * @return std::vector<std::string> lines in file 
 */
std::vector<std::string> read_file_lines(const std::string & file) {
    std::fstream in_file(file, std::ios::in);
    std::vector<std::string> lines;
    std::string line;
    if(!in_file.is_open()) {
        std::cerr << "\n---------------------\n";
        std::cerr << "    file not found" << std::endl;
        std::cerr << "---------------------\n";
        char path[128];
        getcwd(path, 128);
        std::cerr << "the current working directory is " << path << std::endl;
        std::cerr << "---------------------\n";
        std::exit(1);
    }
    
    while(std::getline(in_file, line)) {
        if(line.length() > 0) {
            lines.push_back(line);
        }
    }

    in_file.close();
    return lines;
}

// TODO: write lines into file
/**
 * @brief write lines into file
 * 
 * @param file 
 * @param lines 
 */
void write_file_lines(const std::string& file, const std::vector<std::string>& lines) {
    std::fstream out(file, std::ios::out);
    if(!out.is_open()) {
        std::cerr << "\n---------------------\n";
        std::cerr << "    file not found" << std::endl;
        std::cerr << "---------------------\n";
        char path[128];
        getcwd(path, 128);
        std::cerr << "the current working directory is " << path << std::endl;
        std::cerr << "---------------------\n";
        std::exit(1);
    }
    
    for(auto line: lines) {
        out << line << std::endl;
    }

    out.close();
}

// ----------------------------------------------------------------
// TODO: return words of sentence
// DONE
/**
 * @brief parse a string to an array of words
 * 
 * @param str 
 * @return std::vector<std::string> 
 */
std::vector<std::string> read_words(const std::string & str) {
    std::vector<std::string> words;
    std::string word;
    int i = 0;
    do {
        while(std::isspace(str[i]) && i < str.length()) ++i;
        while(!std::isspace(str[i]) && i < str.length()) {
            word += str[i++];
        }
        if(word.length() != 0) words.push_back(word);
        word.clear();
    } while(i < str.length());
    return words;
}

class WordFilter {
    std::set<std::string> forbidden_words;
public:
    WordFilter() {}
    WordFilter(const std::string & file) {
        setForbiddenWords(file);
    }
    void setForbiddenWords(const std::string & file) {
        auto lines = read_file_lines(file);
        for(auto line : lines) {
            auto words = read_words(line);
            for(auto word: words) {
                forbidden_words.insert(word);
            }
        }
    }
    void filter_file(const std::string & file) {
        std::string insertLine;
        std::vector<std::string> insertLines;
        auto lines = read_file_lines(file);
        for(auto line : lines) {
            insertLine = "";
            auto words = read_words(line);
            for(auto word: words) {
                if(forbidden_words.count(word)) {
                    word = "**";
                }
                insertLine = insertLine + word + " ";
            }
            insertLines.push_back(insertLine);
        }
        write_file_lines(file, insertLines);
    }
};

int main(int, char**) {
    std::default_random_engine dre;
    std::uniform_int_distribution<int> rforbidden_words_num(10, 100);
    std::uniform_int_distribution<int> rwords_num_perLine(1, 5);
    std::uniform_int_distribution<int> rlines_num(30, 40);
    int forbidden_words_num = rforbidden_words_num(dre);
    int words_num_perLine;
    int lines_num;
    std::string file;

    file = "fobidden-words.txt";
    RandomWord randWord;
    auto forbidden_words = randWord.make_random_words(forbidden_words_num);
    write_file_lines(file, forbidden_words);
    WordFilter filter;
    filter.setForbiddenWords(file);

    file = "sourceFile.txt";
    /*
    lines_num = rlines_num(dre);
    std::vector<std::string> lines;
    for(int i = 0; i < lines_num; ++i) {
        words_num_perLine = rwords_num_perLine(dre);
        auto words = randWord.make_random_words(words_num_perLine);
        std::string line;
        for(auto word: words) {
            line += word + " ";
        }
        lines.push_back(line);
    }
    write_file_lines(file, lines);
    */

    filter.filter_file(file);

    return 0;
}
