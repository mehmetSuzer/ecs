
#ifndef __ENTITY_MANAGER_HPP__
#define __ENTITY_MANAGER_HPP__

#include <tuple>
#include <vector>
#include "archetype.hpp"

class EntityManager final : public Uninitialisable
{
private:
    inline static std::vector<Archetype> _archetypes;

    template<typename... Components>
    static void AddArchetypeWith()
    {
        _archetypes.emplace_back();
        _archetypes.back().SetWith<Components...>();
    }

public:
    EntityManager() = delete;
    ~EntityManager() = delete;

    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    EntityManager(EntityManager&&) = delete;
    EntityManager operator=(EntityManager&&) = delete;

    template<typename... Components>
    static std::vector<std::tuple<uint32_t, Components*...>> GetEntitiesWith() noexcept
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
    static std::tuple<uint32_t, Components*...> GetEntitiesExactlyWith() noexcept
    {
        const ComponentSignature query = ComponentSignature::GenerateWith<Components...>();
        for (Archetype& archetype : _archetypes) {
            if (archetype.GetSignature().IsExactMatchTo(query)) {
                return std::tuple<uint32_t, Components*...>(archetype.GetNumEntities(), archetype.GetArrayOf<Components>()...);
            }
        }
        return std::tuple<uint32_t, Components*...>(0u, static_cast<Components*>(nullptr)...);
    }

    template<typename... Components>
    static void AddEntityWith(Components&&... components)
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
    static void RemoveEntityAt(uint32_t index)
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

