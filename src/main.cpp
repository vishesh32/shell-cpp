#include <iostream>
#include <string>
enum class Command{
  Input,
  Exit,
  Error
};

Command inputToCommand(std::string input){
       if(input == "input") return Command::Input;
  else if(input == "exit")  return Command::Exit;
  else{return Command::Error; }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true){
  std::cout << "$ ";

  std::string input;
  std::cin >> input;
  Command command = inputToCommand(input);

  switch(command){
  case(Command::Input):
    break;
  case(Command::Exit):
    return 0;
  case(Command::Error):
    std::cout << input << ": command not found\n";
    break;
  }
  }
}