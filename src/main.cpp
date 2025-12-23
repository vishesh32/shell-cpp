#include <iostream>
#include <string>
enum class Command{
  Input,
  Error
};

Command inputToCommand(std::string input){
  if(input == "input") return Command::Input;
  else{return Command::Error; }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "$ ";

  std::string input;
  std::cin >> input;
  Command command = inputToCommand(input);

  switch(command){
  case(Command::Input):
    break;
  case(Command::Error):
    std::cout << '\n';
    std::cout << input << " command not found";
    break;
  }

}
