#include "shell.hpp"


void Shell::run() {
 LineEditor editor;

    while (true) {
        std::cout << "$ ";
        std::string input = editor.readLine(); // replace getline

        if (input.empty()) continue;

        // take line, turn into vector of tokens using lexer; send tokens to parser to create AST, send AST to exec to run commands
        std::vector<Token> Tokens = tokenize(input);
        std::unique_ptr<ASTNode> ast = parseTokens(Tokens);
        execute(ast.get());
    }
}
