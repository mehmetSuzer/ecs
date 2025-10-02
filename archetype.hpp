
#ifndef __ARCHETYPE_HPP__
#define __ARCHETYPE_HPP__

#include <unordered_map>
#include <component_signature.hpp>

class Archetype final
{
private:
    static constexpr uint32_t CAPACITY_EXTEND_MULTIPLIER = 2u;

    uint32_t _numEntities = 0u;
    uint32_t _capacity = 0u;
    ComponentSignature _signature;
    std::unordered_map<uint32_t, void*> _componentArrays;

    bool _destroyResources = true;

    template<typename Component>
    void AllocateFor(uint32_t capacity)
    {
        Component* array = GetArrayOf<Component>();
        array = static_cast<Component*>(std::realloc(array, sizeof(Component) * capacity));
        assert(array && "The memory allocation has failed!");
        const uint32_t componentID = ComponentRegistry::GetIDOf<Component>();
        _componentArrays[componentID] = array;
    }

public:
    Archetype() = default;

    ~Archetype() noexcept
    {
        if (_destroyResources) {
            for (std::pair<const uint32_t, void*>& pair : _componentArrays) {
                std::free(pair.second);
            }
        }
    }

    Archetype(const Archetype&) = delete;
    Archetype& operator=(const Archetype&) = delete;

    Archetype(Archetype&& other) noexcept
        : _numEntities(std::move(other._numEntities)),
          _capacity(std::move(other._capacity)),
          _signature(std::move(other._signature)),
          _componentArrays(std::move(other._componentArrays))
    {
        other._destroyResources = false;
    }
   
    template<typename... Components>
    void SetWith(uint32_t capacity = 1u)
    {
        ComponentSignature query = ComponentSignature::GenerateWith<Components...>();
        assert((GetSignature().IsUnset() || GetSignature().IsExactMatchTo(query)) && "The query components do not match!");

        _capacity = capacity;
        _signature = query;
        (AllocateFor<Components>(capacity),...);
    }
    
    inline uint32_t GetNumEntities() const noexcept
    {
        return _numEntities;
    }

    inline uint32_t GetCapacity() const noexcept
    {
        return _capacity;
    }

    inline bool IsEmpty() const noexcept
    {
        return (GetNumEntities() == 0u);
    }

    inline bool IsFull() const noexcept
    {
        return (GetNumEntities() == GetCapacity());
    }

    inline ComponentSignature GetSignature() const noexcept
    {
        return _signature;
    }

    template<typename Component>
    Component* GetArrayOf() noexcept
    {
        const uint32_t componentID = ComponentRegistry::GetIDOf<Component>();
        std::unordered_map<uint32_t, void*>::iterator iter = _componentArrays.find(componentID);
        if (iter != _componentArrays.end()) {
            return static_cast<Component*>(iter->second);
        }
        return nullptr;
    }

    template<typename... Components>
    void AddEntityWith(Components&&... components)
    {
        ComponentSignature query = ComponentSignature::GenerateWith<Components...>();
        assert((GetSignature().IsUnset() || GetSignature().IsExactMatchTo(query)) && "The query components do not match!");
        
        if (IsFull()) {
            const uint32_t newCapacity = GetCapacity() * CAPACITY_EXTEND_MULTIPLIER;
            SetWith<Components...>(newCapacity);
        }

        const uint32_t firstEmptyIndex = GetNumEntities();
        ((GetArrayOf<Components>()[firstEmptyIndex] = std::move(components)),...);
        ++_numEntities;
    }

    template<typename... Components>
    void RemoveEntityAt(uint32_t index)
    {
        ComponentSignature query = ComponentSignature::GenerateWith<Components...>();
        assert((GetSignature().IsUnset() || GetSignature().IsExactMatchTo(query)) && "The query components do not match!");
        
        if (index < GetNumEntities()) {
            const uint32_t lastFullIndex = GetNumEntities() - 1u;
            ((GetArrayOf<Components>()[index] = std::move(GetArrayOf<Components>()[lastFullIndex])),...);
        }
    }
};

#endif // __ARCHETYPE_HPP__

