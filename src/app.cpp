#include "module.hpp"

int main() {
    Module mod;

    // load so libraries
    mod.scan("./modules/");

    // stack: 10 20 30
    mod.push({ 10, 20, 30 });
    // stack: 10 20 30 3
    mod.push(3);
    // stack: 60 1
    mod.execute("sum");
    // stack: 60
    mod.pop();
    // stack: 60 7 2
    mod.push({7, 2});
    // stack: 420 1
    mod.execute("prod");

    std::cout << std::endl;

    uint32_t count = mod.pop();
    uint32_t result = mod.pop();
    std::cout << "> process numbers with libmath.so" << std::endl;
    std::cout << "(" << count << ") result = " << result << std::endl << std::endl;

    // print hello for user
    std::cout << "> process io with libfoo.so" << std::endl;
    mod.execute("greetings");
}
