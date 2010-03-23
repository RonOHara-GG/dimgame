#pragma once

class kpuStopwatch
{
public:
	kpuStopwatch(void);
	~kpuStopwatch(void);

	void SetSeconds(int nSeconds);
	void SetSeconds(float fSeconds);
	bool Expired() const;

protected:
	s64		m_nEndMark;
};
