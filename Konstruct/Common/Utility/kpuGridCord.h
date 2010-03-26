#pragma once

class kpuGridCord
{
public:
	kpuGridCord(int X = -1, int Y = -1) { m_iX = X; m_iY = Y; }
	~kpuGridCord(void) {};

	int GetX() { return m_iX; }	
	int GetY() { return m_iY; }

	int DistanceSquared(kpuGridCord cord2);

private:
	int m_iX;
	int m_iY;
};

inline int kpuGridCord::DistanceSquared(kpuGridCord cord2)
{
	int iXDist = (cord2.GetX() - m_iX) * (cord2.GetX() - m_iX);
	int iYDist = (cord2.GetY() - m_iY) * (cord2.GetY() - m_iY);

	return iXDist - iYDist;
}