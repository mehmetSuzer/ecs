
#ifndef __COMPONENT_REGISTRY_HPP__
#define __COMPONENT_REGISTRY_HPP__

#include <cstdint>
#include <cassert>

static constexpr uint32_t MAX_NUM_COMPONENTS = 64u;

class ComponentRegistry final
{
private:
    static inline uint32_t _nextID = 0u;

public:
    ComponentRegistry() = delete;
    ~ComponentRegistry() = delete;

    ComponentRegistry(const ComponentRegistry&) = delete;
    ComponentRegistry& operator=(const ComponentRegistry&) = delete;

    ComponentRegistry(ComponentRegistry&&) = delete;
    ComponentRegistry operator=(ComponentRegistry&&) = delete;

    template<typename Component>
    static inline uint32_t GetIDOf()
    {
        static uint32_t id = _nextID++;
        assert(id < MAX_NUM_COMPONENTS && "The number of components registered has exceeded the limit!");
        return id;
    }
};

#endif // __COMPONENT_REGISTRY_HPP__

