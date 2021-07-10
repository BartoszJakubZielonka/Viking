#pragma once

namespace Viking
{
    template<class T>
    class Library {
    public:
        Library() = default;
        ~Library() = default;

        void add(const std::string& name, const std::string& filepath) {
            if (exists(name)) {
                VI_CORE_WARN("Resource {0} already exist", name);
                return;
            }

            library.insert({ name, T::create(filepath) });
        }

        Ref<T> get(const std::string& name)
        {
            if (exists(name))
                return library.at(name);

            VI_CORE_ERROR("Resource {0} dosen't exist", name);
            return nullptr;
        }
    private:
        bool exists(const std::string& name) {
            return library.find(name) != library.end();
        }

        std::unordered_map<std::string, Ref<T>> library;
    };
}
