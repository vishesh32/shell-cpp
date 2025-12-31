#include "parser.hpp"

std::unique_ptr<ASTNode> parseTokens(const std::vector<Token>& tokens) {
    size_t index = 0;
    return parseCommand(tokens, index);
}

// echo hello world > outA.txt > outB.txt

// Helper for parsing a single command from tokens
std::unique_ptr<ASTNode> parseCommand(const std::vector<Token>& tokens, size_t& index){
    //assuming first word is a command always
    if(index < tokens.size() && tokens[index].type == TokenType::Word){
        auto cmd = std::make_unique<CommandNode>();
        cmd->command_name = tokens[index].value;
        index++;

        std::unique_ptr<ASTNode> node = std::move(cmd);
        node->type = ASTNodeType::Command;
        std::vector<std::string> argv;
        while(index < tokens.size()){
            if(tokens[index].type == TokenType::RedirectStdOut){
                node = parseRedirection(std::move(node), tokens, index);
                break;
            }
            if(tokens[index].type == TokenType::RedirectStdErr){
                 node = parseRedirection(std::move(node), tokens, index);
                break;               
            }
        static_cast<CommandNode*>(node.get())->args.push_back(tokens[index].value);
        index++;
        }
        return node;
    }
    else{
        //handle error
        return nullptr;
    }
}


// Optional helper: parse redirection
std::unique_ptr<ASTNode> parseRedirection(std::unique_ptr<ASTNode> lhs, const std::vector<Token>& tokens, size_t& index){
    if (tokens[index].type == TokenType::RedirectStdOut){
    auto redirectNode = std::make_unique<RedirectNode>();
    redirectNode->type = ASTNodeType::Redirect;
    redirectNode->child = std::move(lhs);
    redirectNode->redirect_type = RedirectType::StdOut;

    index++; // consume >
    redirectNode->outfile = tokens[index].value;
    index++;

    return redirectNode;
    }
    else if(tokens[index].type == TokenType::RedirectStdErr){
        auto redirectNode = std::make_unique<RedirectNode>();
        redirectNode->type = ASTNodeType::Redirect;
        redirectNode->redirect_type = RedirectType::StdErr;
        redirectNode->child = std::move(lhs);

        index++; //consume '2>'
        redirectNode->outfile = tokens[index].value;
        index++;

        return redirectNode;
    //else handle error 
    } else { return nullptr; }
}


// Optional helper: parse pipelines
std::unique_ptr<ASTNode> parsePipeline(std::unique_ptr<ASTNode> lhs, const std::vector<Token>& tokens, size_t& index);
