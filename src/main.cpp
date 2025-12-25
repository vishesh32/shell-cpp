#include "shell.hpp"

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    Shell shell;
    shell.run();
}
