#include "lexer.hpp"
#include <cctype>

// enum class TokenType {
//     Word,        // normal words / arguments
//     Pipe,        // |
//     RedirectStdOut, // > / 1>
//     RedirectIn,  // <
//     EndOfLine    // optional, marks end
// };

// struct Token {
//     TokenType type;
//     std::string value;
// };

//take input string and turn into tokens e.g echo xxx>output.txt
std::vector<Token> tokenize(const std::string& input){
    std::vector<Token> Tokens;
    size_t i = 0;
    while(i < input.size()){
        if(input[i] == ' '){
            i++;
            continue;
        } else if(input[i] == '1' && i != input.size() - 1 && input[i+1] == '>' && (i == 0 || input[i - 1] == ' ')){ //stdout is 1>
            Tokens.push_back(Token{TokenType::RedirectStdOut, ">"});
            i = i + 2;
            continue;
        } else if(input[i] == '2' && i != input.size() - 1 && input[i+1] == '>' && (i == 0 || input[i - 1] == ' ')){
            Tokens.push_back(Token{TokenType::RedirectStdErr, "2>"});
            i = i + 2;
        } else if(input[i] == '>'){ //stdout is >
            Tokens.push_back(Token{TokenType::RedirectStdOut, ">"});
            i++;
            continue;
        } else if(input[i] == '\'' || input[i] == '\"'){ // quoted string
            char quote = input[i];
            i++; // skip opening quote
            std::string word;
            while(i < input.size() && input[i] != quote){
                word += input[i];
                i++;
            }
            if(i < input.size()) i++; // skip closing quote
            Tokens.push_back(Token{TokenType::Word, word});
            continue;
        }//otherwise should be a word
        std::string word;
        while(i < input.size()){
            if(i == input.size()-1){ //end of input
                word += input[i];
                Tokens.push_back(Token{TokenType::Word, word});
                i++;
                break;                
            } else if (i + 1 < input.size() && (input[i + 1] == '>' || input[i + 1] == ' ')) { //delimiter boundary
                word += input[i];
                Tokens.push_back(Token{TokenType::Word, word});
                i++;
                break;
            }

            word += input[i];
            i++;
        }
    }
    return Tokens;
}