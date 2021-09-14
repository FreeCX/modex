#include "module.hpp"

extern "C" void modex_plugin_init(Module *mod, uint32_t index) {
    mod->record("demo", index);
}

extern "C" void greetings(Module *mod) {
    std::string name;
    mod->cout() << "Enter your name: ";
    mod->cin() >> name;
    mod->cout() << "Hello from libfoo.so to " << name << "!" << std::endl;
}
