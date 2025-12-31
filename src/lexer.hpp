#pragma once
#include <string>
#include <vector>

enum class TokenType {
    Word,        // normal words / arguments
    Pipe,        // |
    RedirectStdOut, // >, 1>
    RedirectStdErr,  // 2>
    AppendStdOut,  // >>, 1>>
    AppendStdErr,  // 2>>
    EndOfLine    // optional, marks end
};

struct Token {
    TokenType type;
    std::string value;
};

std::vector<Token> tokenize(const std::string& input);
