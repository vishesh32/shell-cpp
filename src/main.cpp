#include <iostream>
#include <string>
#include <unordered_set>
#include <cstdlib>   // getenv
#include <unistd.h>  // access, fork, execv
#include <sys/stat.h>
#include <vector>
#include <optional>
#include <sys/wait.h> // waitpid

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

std::vector<std::string> buildArgList(const std::string& args_str) {
  std::vector<std::string> list;

  size_t it = 0;
  while(it < args_str.size()){
    while (it < args_str.size() && args_str[it] == ' ') it++;

    size_t space_idx = args_str.find(' ', it);
    if(space_idx == std::string::npos){
      list.push_back(args_str.substr(it));
      break;
    }
    list.push_back(args_str.substr(it, space_idx - it));
    it = space_idx + 1;
  }
  return list;
}

std::vector<char*> buildArgV(std::vector<std::string>& args) {
    std::vector<char*> argv;
  argv.reserve(args.size() + 1);
    for (auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);
    return argv;
}
std::optional<std::string> findExecutableInPath(std::string& name){
  
  const char* path_env = std::getenv("PATH");
    if (!path_env) {
        return std::nullopt;
    }

    std::string path_str = path_env;
    std::vector<std::string> paths = splitPath(path_str);

    for(auto path : paths){
      std::string full_path = path + "/" + name;

      //returns 0 if X_OK is allowed - is an executable
      if(!access(full_path.c_str(), X_OK)){
          return full_path;
      } 
    }
    return std::nullopt;
}

// input: (type) command arg1 arg2 argx
void Type(std::string args){
  if(args.empty()){
    std::cout << "type: missing argument\n";
    return;
  }

  size_t command_idx =  args.find(' ');
  std::string command;
  if (command_idx == std::string::npos) command = args;
  else command = args.substr(0, command_idx);

  if(builtin.count(command)){
    std::cout << command << " is a shell builtin\n";
    return;
  }
  
  //otherwise isnt builtin -> search the path directories
  std::optional<std::string> full_path = findExecutableInPath(command);

  if(full_path){
    std::cout << command << " is " << *full_path << std::endl;
  }
  else{
    std::cout << command << ": not found\n"; 
  }
}

enum class Command{
  Echo,
  Exit,
  Default,
  Type
};

Command inputToCommand(const std::string& input, std::string& command, std::string& args){
  size_t command_idx =  input.find(' ');
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
  else{return Command::Default; }
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true){
  std::cout << "$ ";

  std::string input, command, args;
  std::getline(std::cin, input);
  Command _command = inputToCommand(input, command, args); 

  switch(_command){


  case(Command::Default):
    if(!builtin.count(command)){
      //search if in directories, then execute
      std::optional<std::string> full_path = findExecutableInPath(command);

        if(full_path){
          std::vector<std::string> arg_list = buildArgList(args);
          arg_list.insert(arg_list.begin(), command);
          std::vector<char*> argv = buildArgV(arg_list);

          pid_t pid = fork();
          if(pid == 0){ // child process
            execv(full_path->c_str(), argv.data());
          }else{
            int status;
            waitpid(pid, &status, 0); // wait for child to finish
          }
          //std::cout << command << " is " << *full_path << std::endl;
        }
        else{
          std::cout << command << ": command not found\n"; 
        }
    }
    break;
   case(Command::Echo):
    std::cout << args << std::endl;
    break;
  case(Command::Exit):
    return 0;
  case(Command::Type):
    Type(args);
    break;
  }
  }
}