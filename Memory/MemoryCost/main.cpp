#include "stdafx.h"
#include <chrono>
#include <stdio.h>
#include <string.h>

class Timer
{
public:
    Timer()
    {
        start = clock.now();
    }
    // Returns the duration in seconds.
    double GetElapsed()
    {
        auto end = clock.now();
        auto duration = end - start;
        return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() * 1.e-9;
    }
private:
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock clock;

    Timer(const Timer&) = delete;
    Timer operator=(const Timer*) = delete;
};

void BusyWait(int ms)
{
    auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms);
    for (;;)
    {
        if (std::chrono::steady_clock::now() > end)
            break;
    }
}

void FastMeasure()
{
    printf("Busy waiting to raise the CPU frequency...\n");
    // Busy wait for a second so that the CPUs ramp up to full speed.
    BusyWait(500);
    const int bufSize = 32 * 1024 * 1024;
    const int iterationCount = 100;
    {
        Timer timer;
        for (int i = 0; i < iterationCount; ++i)
        {
            int* p = new int[bufSize / sizeof(int)];
            delete[] p;
        }
        printf("%1.4f s to allocate %d MB %d times.\n", timer.GetElapsed(), bufSize / (1024 * 1024), iterationCount);
    }
    {
        Timer timer;
        double deleteTime = 0.0;
        for (int i = 0; i < iterationCount; ++i)
        {
            int* p = new int[bufSize / sizeof(int)];
            Timer deleteTimer;
            delete[] p;
            deleteTime += deleteTimer.GetElapsed();
        }
        printf("%1.4f s to allocate %d MB %d times (%1.4f s to delete).\n", timer.GetElapsed(), bufSize / (1024 * 1024), iterationCount, deleteTime);
    }
    {
        // Initialize and zero the memory.
        int* p = new int[bufSize / sizeof(int)]();
        {
            // Repeatedly write to the already allocated memory.
            Timer timer;
            for (int i = 0; i < iterationCount; ++i)
            {
                memset(p, 1, bufSize);
            }
            printf("%1.4f s to write %d MB %d times.\n", timer.GetElapsed(), bufSize / (1024 * 1024), iterationCount);
        }
        {
            // Repeatedly read from the already allocated memory.
            Timer timer;
            int sum = 0;
            for (int i = 0; i < iterationCount; ++i)
            {
                for (size_t index = 0; index < bufSize / sizeof(int); ++index)
                {
                    sum += p[index];
                }
            }
            printf("%1.4f s to read %d MB %d times, sum = %d.\n", timer.GetElapsed(), bufSize / (1024 * 1024), iterationCount, sum);
        }
        delete[] p;
    }
    {
        // Repeatedly allocate, write, and free memory.
        Timer timer;
        double deleteTime = 0.0;
        for (int i = 0; i < iterationCount; ++i)
        {
            int* p = new int[bufSize / sizeof(int)];
            memset(p, 1, bufSize);
            Timer deleteTimer;
            delete[] p;
            deleteTime += deleteTimer.GetElapsed();
        }
        printf("%1.4f s to allocate and write %d MB %d times (%1.4f s to delete).\n", timer.GetElapsed(), bufSize / (1024 * 1024), iterationCount, deleteTime);
    }
    {
        // Repeatedly allocate, read, and free memory.
        Timer timer;
        int sum = 0;
        for (int i = 0; i < iterationCount; ++i)
        {
            int* p = new int[bufSize / sizeof(int)];
            for (size_t index = 0; index < bufSize / sizeof(int); ++index)
            {
                sum += p[index];
            }
            delete[] p;
        }
        printf("%1.4f s to allocate and read %d MB %d times, sum = %d.\n", timer.GetElapsed(), bufSize / (1024 * 1024), iterationCount, sum);
    }
}

int main(int argc, char* argv[])
{
    FastMeasure();

    return 0;
}
