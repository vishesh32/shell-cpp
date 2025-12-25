#include "shell.hpp"

int main() {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;
        std::cout << "$ ";

    Shell shell;
    shell.run();
}
