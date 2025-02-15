#include "Utility.h"
#include "iostream"

namespace ep
{
    Timer::Timer()
    {
        m_start = ch.now();
    }

    Timer::~Timer()
    {
        m_end = ch.now();
        m_duration = m_end - m_start;
        std::cout << "Time took = " << m_duration.count() * 1000.0f << " ms" << std::endl;
    }
}

