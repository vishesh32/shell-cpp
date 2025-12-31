#pragma once
#include <string>
#include <vector>
#include <memory>
#include "lexer.hpp"  // for Token
#include "parse.hpp"  // optionally for Command if you want to reuse parseInput
#include <optional>

enum class ASTNodeType{
    Command, Redirect, Pipeline
};


struct ASTNode{
    ASTNodeType type;
    virtual ~ASTNode() = default;
};

struct CommandNode : ASTNode {
    // For command nodes
    std::string command_name;            // e.g., "echo"
    std::vector<std::string> args;       // e.g., ["hello", "world"]

    CommandNode(){
        type = ASTNodeType::Command;
    }
};

enum class RedirectType{
    StdOut,
    StdErr,
    AppendStdOut,
    AppendStdErr
};

struct RedirectNode : ASTNode {
    std::unique_ptr<ASTNode> child;
    RedirectType redirect_type;
    std::string outfile;

    RedirectNode() {
        type = ASTNodeType::Redirect;
    }
};

struct PipelineNode : ASTNode {
    // For pipeline nodes
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    PipelineNode() {
        type = ASTNodeType::Pipeline;
    }
};

std::unique_ptr<ASTNode> parseTokens(const std::vector<Token>& tokens);

std::unique_ptr<ASTNode> parseCommand(const std::vector<Token>& tokens, size_t& index);

std::unique_ptr<ASTNode> parseRedirection(std::unique_ptr<ASTNode> lhs, const std::vector<Token>& tokens, size_t& index);

std::unique_ptr<ASTNode> parsePipeline(std::unique_ptr<ASTNode> lhs, const std::vector<Token>& tokens, size_t& index);
