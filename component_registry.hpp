
#ifndef __COMPONENT_REGISTRY_HPP__
#define __COMPONENT_REGISTRY_HPP__

#include <cstdint>
#include <cassert>
#include "uninitialisable.hpp"

static constexpr uint32_t MAX_NUM_COMPONENTS = 64u;

class ComponentRegistry final : public Uninitialisable
{
private:
    static inline uint32_t _nextID = 0u;

public:
    template<typename Component>
    static inline uint32_t GetIDOf()
    {
        static uint32_t id = _nextID++;
        assert(id < MAX_NUM_COMPONENTS && "The number of components registered has exceeded the limit!");
        return id;
    }
};

#endif // __COMPONENT_REGISTRY_HPP__

