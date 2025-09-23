
#ifndef __COMPONENT_SIGNATURE_HPP__
#define __COMPONENT_SIGNATURE_HPP__

#include <bitset>
#include <component_registry.hpp>

class ComponentSignature final
{
private:
    std::bitset<MAX_NUM_COMPONENTS> _mask;

public:
    template<typename... Components>
    static inline ComponentSignature GenerateWith() noexcept
    {
        ComponentSignature signature;
        (signature._mask.set(ComponentRegistry::GetIDOf<Components>()),...);
        return signature;
    }

    inline bool IsUnset() const noexcept
    {
        return _mask.none();
    }

    inline bool IsExactMatchTo(const ComponentSignature& querySignature) const noexcept
    {
        return (_mask == querySignature._mask);
    }

    inline bool IsSubsetMatchTo(const ComponentSignature& querySignature) const noexcept
    {
        return ((_mask & querySignature._mask) == querySignature._mask);
    }
};

#endif // __COMPONENT_SIGNATURE_HPP__

