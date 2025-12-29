#include "shell.hpp"


void Shell::run() {
    while (true) {
        std::cout << "$ ";
        std::string input;
        std::getline(std::cin, input);

        // take line, turn into vector of tokens using lexer; send tokens to parser to create AST, send AST to exec to run commands
        std::vector<Token> Tokens = tokenize(input);
        std::unique_ptr<ASTNode> ast = parseTokens(Tokens);
        execute(ast.get());
        //Command cmd = parseInput(input);

        //if (cmd.type == CommandType::Exit) return;

        //if (isBuiltin(cmd)) {
        //    runBuiltin(cmd);
        // } else {
        //     executeExternal(cmd);
        // }
    }
}
