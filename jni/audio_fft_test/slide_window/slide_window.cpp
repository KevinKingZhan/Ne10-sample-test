#include "slide_window.h"

#define LOG_TAG "CSlideWindow"

CSlideWindow::CSlideWindow(size_t windowSize, size_t step)
    : m_dataBuf(NULL)
{
    if (windowSize < step)
    {
        LOGI("windowSize must greater than step.");
    }
    m_dataBufCap = windowSize * 2;
    m_windowSize = windowSize;
    m_step = step;
    m_idx = 0;
    m_newDataSize = 0;
    m_latency = 0;
    m_isHamm = false;
    m_dataBuf = new float[m_dataBufCap];
    memset(m_dataBuf, 0, sizeof(float) * m_dataBufCap);
    m_retBuf = new float[m_windowSize];
    m_hammFunc = NULL;
}

CSlideWindow::~CSlideWindow()
{
    if (m_dataBuf)
    {
        delete[] m_dataBuf;
    }
    m_dataBuf = NULL;
    if (m_hammFunc)
    {
        delete[] m_hammFunc;
    }
    m_hammFunc = NULL;
    if (m_retBuf)
    {
        delete[] m_retBuf;
        m_retBuf = NULL;
    }
}

bool CSlideWindow::Process(const float inputBuf[], const size_t size)
{
    size_t leaveSize = size;
    bool ret = false;
    while (leaveSize > 0)
    {
        size_t idleSize = m_dataBufCap - m_idx;
        size_t n = leaveSize;
        if (n > idleSize)
        {
            n = idleSize;
        }
        memcpy(m_dataBuf + m_idx, inputBuf + size - leaveSize, sizeof(float) * n);
        m_idx += n;
        if (m_idx >= m_dataBufCap)
        {
            m_idx -= m_dataBufCap;
        }
        leaveSize -= n;
        m_newDataSize += n;
        m_latency += n;
        if (m_newDataSize >= m_windowSize)
        {
            m_latency = m_newDataSize;
            while (m_newDataSize >= m_windowSize)
            {
                m_newDataSize -= m_step;
            }
            int tmpBeginP = int(m_idx) - int(m_newDataSize + m_step);
            size_t tmpN = m_windowSize;
            if (tmpBeginP < 0)
            {
                size_t tmpN1 = 0 - tmpBeginP;
                if (tmpN1 > tmpN)
                {
                    tmpN1 = tmpN;
                }
                memcpy(m_retBuf, m_dataBuf + m_dataBufCap + tmpBeginP, sizeof(float) * tmpN1);
                tmpN -= tmpN1;
                tmpBeginP = 0;
            }
            memcpy(m_retBuf + m_windowSize - tmpN, m_dataBuf + tmpBeginP, sizeof(float) * tmpN);
            if (m_isHamm)
            {
                HammingWindow(m_retBuf);
            }
            ret = true;
        }
    }
    return ret;
}

size_t CSlideWindow::CheckOutSlideWindow(float outputBuf[], size_t &centerLatency, size_t &frontLatency)
{
    if (m_latency < m_windowSize)
    {
        return 0;
    }
    memcpy(outputBuf, m_retBuf, sizeof(float) * m_windowSize);
    frontLatency = m_latency;
    centerLatency = frontLatency;
    if (centerLatency > m_windowSize / 2)
    {
        centerLatency -= m_windowSize / 2;
    }
    return m_windowSize;
}

size_t CSlideWindow::CheckOutCurWindow(float outputBuf[])
{
    //TODO
    //throw CParamException("not implement");
    
    size_t endPos = m_idx;
    size_t leaveSize = m_windowSize;
    while (leaveSize > 0)
    {
        int beginPos = (int)(endPos - leaveSize);
        if (beginPos < 0)
        {
            beginPos = 0;
        }
        int tmpSize = (int)(endPos - beginPos);
        endPos = beginPos;
        if (endPos == 0)
        {
            endPos = m_windowSize;
        }
        memcpy(outputBuf + leaveSize - tmpSize, m_dataBuf + beginPos, sizeof(float) * tmpSize);
        leaveSize -= tmpSize;
    }

    return m_windowSize;
}

void CSlideWindow::SetHammWindow(bool isValid)
{
    m_isHamm = isValid;
    if (!m_hammFunc)
    {
        m_hammFunc = new float[m_windowSize];
    }
    float tmpF = 6.2831853072 / (m_windowSize - 1);
    for (size_t i = 0; i < m_windowSize; i++)
    {
        m_hammFunc[i] = 0.5 - 0.5 * cos(tmpF * i);
    }
}

void CSlideWindow::HammingWindow(float buffer[])
{
    for (size_t i = 0; i < m_windowSize; i++)
    {
        buffer[i] *= m_hammFunc[i];
    }
}

void CSlideWindow::Flush()
{
    m_newDataSize = 0;
}
