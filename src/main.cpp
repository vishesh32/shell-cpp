#include <iostream>
#include <string>
#include <unordered_set>
#include <cstdlib>   // getenv
#include <unistd.h>  // access
#include <sys/stat.h>
#include <vector>


static const std::unordered_set<std::string> builtin = {
    "echo",
    "exit",
    "type"
};

/// PATH = dir1:/dir2:/dir3
/// PATH="/usr/bin:/usr/local/bin:$PATH"
std::vector<std::string> splitPath(std::string path){
  std::vector<std::string> dir;

  size_t start = 0;

  while(true){
    size_t end = path.find(':', start);
    if(end == std::string::npos){
      dir.push_back(path.substr(start));
      break;
    }
    dir.push_back(path.substr(start, end - start));
    start = end + 1;
  }

  return dir;
}
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
    return;
  }
  
  const char* path_env = std::getenv("PATH");
    if (!path_env) {
        std::cout << command << ": not found\n";
        return;
    }

    std::string path_str = path_env;

    std::vector<std::string> paths = splitPath(path_str);

    for(auto path : paths){
      std::string full_path = path + "/" + command;

      //returns 0 if X_OK is allowed - is an executable
      if(!access(full_path.c_str(), X_OK)){
          std::cout << command << " is " << full_path << std::endl;
          return;
      } 
    }
    std::cout << command << ": not found\n"; 
    return;
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