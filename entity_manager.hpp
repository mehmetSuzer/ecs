
#ifndef __ENTITY_MANAGER_HPP__
#define __ENTITY_MANAGER_HPP__

#include <tuple>
#include <vector>
#include <archetype.hpp>

class EntityManager final
{
private:
    std::vector<Archetype> _archetypes;

    template<typename... Components>
    void AddArchetypeWith()
    {
        _archetypes.emplace_back();
        _archetypes.back().SetWith<Components...>();
    }

public:
    template<typename... Components>
    std::vector<std::tuple<uint32_t, Components*...>> GetEntitiesWith() noexcept
    {
        const ComponentSignature query = ComponentSignature::GenerateWith<Components...>();
        std::vector<std::tuple<uint32_t, Components*...>> arrays;

        for (Archetype& archetype : _archetypes) {
            if (archetype.GetSignature().IsSubsetMatchTo(query)) {
                arrays.emplace_back(archetype.GetNumEntities(), archetype.GetArrayOf<Components>()...);
            }
        }
        return arrays;
    }

    template<typename... Components>
    void AddEntityWith(Components&&... components)
    {
        const ComponentSignature query = ComponentSignature::GenerateWith<Components...>();
        for (Archetype& archetype : _archetypes) {
            if (archetype.GetSignature().IsExactMatchTo(query)) {
                archetype.AddEntityWith<Components...>(std::move(components)...);
                return;
            }
        }

        AddArchetypeWith<Components...>();
        _archetypes.back().AddEntityWith<Components...>(std::move(components)...);
    }

    template<typename... Components>
    void RemoveEntityAt(uint32_t index)
    {
        const ComponentSignature query = ComponentSignature::GenerateWith<Components...>();
        for (Archetype& archetype : _archetypes) {
            if (archetype.GetSignature().IsExactMatchTo(query)) {
                archetype.RemoveEntityAt<Components...>(index);
                return;
            }
        }
    }
};

#endif // __ENTITY_MANAGER_HPP__

