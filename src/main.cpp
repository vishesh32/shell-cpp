#include <iostream>
#include <string>
#include <unordered_set>

static const std::unordered_set<std::string> builtin = {
    "echo",
    "exit",
    "type"
};

void Type(std::string args){
  if(args.empty()){
    std::cout << "type: missing argument\n";
    return;
  }

  int command_idx =  args.find(" ");
  std::string command;
  if (command_idx == std::string::npos) command = args;
  else command = args.substr(0, command_idx);

  if(builtin.count(command)){
    std::cout << command << " is a shell builtin\n";
  }
  else{
    std::cout << command << ": not found\n";
  }
}

enum class Command{
  Echo,
  Exit,
  Error,
  Type
};

Command inputToCommand(const std::string& input, std::string& command, std::string& args){
  int command_idx =  input.find(" ");
  if(command_idx == std::string::npos){
    command = input;
    args.clear();
  }else{
    command = input.substr(0, command_idx);
    args = input.substr(command_idx+1, input.size());
  }

  if(command == "exit")  return Command::Exit;
  else if(command == "echo")  return Command::Echo;
  else if(command == "type")  return Command::Type;
  else{return Command::Error; }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true){
  std::cout << "$ ";

  std::string input, command, args;
  std::getline(std::cin, input);
  Command _command = inputToCommand(input, command, args); // input now contains command as a string, everything after the command as args.

  switch(_command){
  case(Command::Echo):
    std::cout << args << std::endl;
    break;
  case(Command::Exit):
    return 0;
  case(Command::Error):
    std::cout << command << ": command not found\n";
    break;
  case(Command::Type):
    Type(args);
    break;
  }
  }
}