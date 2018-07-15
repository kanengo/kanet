#pragma once

class NoCopyable
{
public:
    NoCopyable();
    ~NoCopyable();
private:
    NoCopyable(const NoCopyable & rhs);
    NoCopyable(NoCopyable&& rhs);
    NoCopyable & operator=(const NoCopyable & rhs);
    NoCopyable & operator=(NoCopyable&& rhs);
};