#include "StdAfx.h"
#include "kpuStopwatch.h"

static s64 s_nFrequency = 0;

kpuStopwatch::kpuStopwatch(void)
{
	if( !s_nFrequency )
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		s_nFrequency = freq.QuadPart;
	}
}

kpuStopwatch::~kpuStopwatch(void)
{
}

void kpuStopwatch::SetSeconds(int nSeconds)
{
	s64 cycles = s_nFrequency * nSeconds;
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	m_nEndMark = now.QuadPart + cycles;
}

void kpuStopwatch::SetSeconds(float fSeconds)
{
	s64 cycles = (s64)((double)s_nFrequency * (double)fSeconds);
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	m_nEndMark = now.QuadPart + cycles;
}

bool kpuStopwatch::Expired() const
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	if( now.QuadPart >= m_nEndMark )
		return true;
	return false;
}
