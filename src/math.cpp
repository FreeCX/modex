#include "module.hpp"

extern "C" void init(Module *mod, uint32_t index) {
    mod->record("sum", index);
    mod->record("prod", index);
}

extern "C" void sum(Module *mod) {
    uint32_t size = mod->pop();
    uint32_t result = 0;
    for (uint32_t i = 0; i < size; i++) {
        result += mod->pop();
    }
    mod->push({result, 1});
}

extern "C" void prod(Module *mod) {
    uint32_t size = mod->pop();
    uint32_t result = 1;
    for (uint32_t i = 0; i < size; i++) {
        result *= mod->pop();
    }
    mod->push({result, 1});
}
