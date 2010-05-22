#pragma once

class kpuStopwatch
{
public:
	kpuStopwatch(void);
	~kpuStopwatch(void);

	void SetSeconds(int nSeconds);
	void SetSeconds(float fSeconds);

	void Start() { QueryPerformanceCounter((LARGE_INTEGER*)&m_iStart); }
	void End() { QueryPerformanceCounter((LARGE_INTEGER*)&m_iEnd); }
	void Reset() { m_iStart = m_iEnd = 0; }
	float GetMilliseconds();

	bool Expired() const;

protected:
	s64		m_nEndMark;
	__int64 m_iStart;
	__int64 m_iEnd;
};
