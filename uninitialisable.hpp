
#ifndef __UNINITIALISABLE_HPP__
#define __UNINITIALISABLE_HPP__

class Uninitialisable
{
public:
    Uninitialisable() = delete;
    ~Uninitialisable() = delete;

    Uninitialisable(const Uninitialisable&) = delete;
    Uninitialisable& operator=(const Uninitialisable&) = delete;

    Uninitialisable(Uninitialisable&&) = delete;
    Uninitialisable operator=(Uninitialisable&&) = delete;
};

#endif // __UNINITIALISABLE_HPP__

