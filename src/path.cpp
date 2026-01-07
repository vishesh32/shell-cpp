#include "path.hpp"

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

std::optional<std::string> findExecutableInPath(const std::string& name){
  
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

std::optional<std::vector<std::string>> listExecutablesInPath(){
    const char* path_env = std::getenv("PATH");
    if (!path_env) {
        return std::nullopt;
    }

    std::string path_str = path_env;
    std::vector<std::string> paths = splitPath(path_str);
    std::vector<std::string> executables;

    for(auto path : paths){
        DIR* dir = opendir(path.c_str()); 
        if (dir) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                std::string full_path = path + "/" + entry->d_name;
                if (access(full_path.c_str(), X_OK) == 0) {
                    executables.push_back(entry->d_name);
                }
            }
            closedir(dir);
        }
    }

    return executables;
}