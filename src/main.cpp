#include <iostream>
#include <string>
enum class Command{
  Input,
  Echo,
  Exit,
  Error
};

Command inputToCommand(std::string& input, std::string& args){
  int command_idx =  input.find(" ");
  std::string command = input.substr(0, command_idx);
  args = input.substr(command_idx+1, input.size());
  input = command;

  if(command == "input") return Command::Input;
  else if(command == "exit")  return Command::Exit;
  else if(command == "echo")  return Command::Echo;
  else{return Command::Error; }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true){
  std::cout << "$ ";

  std::string input, args;
  std::getline(std::cin, input);
  Command command = inputToCommand(input, args); // input now contains command as a string, everything after the command as args.

  switch(command){
  case(Command::Input):
    break;
  case(Command::Echo):
    std::cout << args << std::endl;
    break;
  case(Command::Exit):
    return 0;
  case(Command::Error):
    std::cout << input << ": command not found\n";
    break;
  }
  }
}