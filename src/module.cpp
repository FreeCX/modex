#include "module.hpp"

Module::Module() {}

Module::~Module() {
    for (void *h : handlers) {
        dlclose(h);
    }
}

void Module::record(const std::string &func, uint32_t index) {
    funcs[func] = index;
}

void Module::scan(const std::string &folder) {
    uint32_t index = 0;
    // находим файл *.so
    for (auto &item : fs::directory_iterator(folder)) {
        fs::path filename = item.path();
        if (item.is_regular_file() and filename.extension() == ".so") {
            std::cout << "load: " << filename;
            // загружаем
            void *handle = dlopen(filename.c_str(), RTLD_LAZY);
            if (handle == NULL) {
                throw std::runtime_error(dlerror());
            }
            std::cout << " ... success" << std::endl;

            void (*init)(Module *, uint32_t) = (void (*)(Module *, uint32_t)) dlsym(handle, "init");
            char *error = dlerror();
            if (error != NULL) {
                throw std::runtime_error(error);
            }

            //  вызываем init
            (*init)(this, index);

            // сохраняем и увеличиваем индекс
            handlers.push_back(handle);
            index += 1;
        }
    }
}

void Module::execute(const std::string &name) {
    uint32_t index = funcs[name];
    void *handler = handlers[index];

    void (*func)(Module *) = (void (*)(Module *))dlsym(handler, name.c_str());
    (*func)(this);
}

void Module::push(uint32_t data) {
    stack.push_back(data);
}

void Module::push(const std::vector<uint32_t> &data) {
    stack.insert(stack.end(), data.begin(), data.end());
}

uint32_t Module::pop() {
    if (stack.size() == 0) {
        throw std::runtime_error("stack is empty");
    }
    uint32_t last = stack.back();
    stack.pop_back();
    return last;
}

std::istream & Module::cin() {
    return std::cin;
}

std::ostream & Module::cout() {
    return std::cout;
}
