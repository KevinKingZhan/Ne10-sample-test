#ifndef SLIDE_WINDOW_HPP
#define SLIDE_WINDOW_HPP
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "CommonTools.h"

class CSlideWindow
{
public:
    CSlideWindow(size_t windowSize, size_t step);
    ~CSlideWindow();

    void SetHammWindow(bool isValid);

    bool Process(const float inputBuf[], const size_t size);
    size_t CheckOutSlideWindow(float outputBuf[], size_t &centerLatency, size_t &frontLatency);
    size_t CheckOutCurWindow(float outputBuf[]);
    void Flush();

private:
    size_t m_dataBufCap;
    size_t m_windowSize;
    size_t m_step;
    size_t m_idx;
    size_t m_newDataSize;
    size_t m_latency;
    size_t m_isHamm;
    float * m_dataBuf;
    float * m_retBuf;
    float * m_hammFunc;

private:
    void HammingWindow(float buffer[]);
};

#endif
