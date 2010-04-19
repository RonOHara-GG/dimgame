#pragma once

template <typename T>
class kpuFixedArray
{
public:
	kpuFixedArray()
	{
		m_iNumElements = 0;
		m_iNumUsedElements = 0;
		m_pElements = 0;
	}

	kpuFixedArray(int iNumElements)
	{
		m_iNumElements = 0;
		m_iNumUsedElements = 0;
		m_pElements = 0;
		SetSize(iNumElements);
	}

	~kpuFixedArray(void)
	{
		if( m_pElements )
			free(m_pElements);	
	}

	void SetSize(int iNumElements)
	{		
		if( m_pElements )
		{
			T* pNewData = (T*)malloc(sizeof(T) * iNumElements);
			int iCopyCount = MIN(iNumElements, m_iNumElements);
			memcpy(pNewData, m_pElements, sizeof(T) * iCopyCount);

			int iDiff = iNumElements - m_iNumElements;
			if( iDiff > 0 )
				memset(&pNewData[iCopyCount], 0, sizeof(T) * iDiff);
		}
		else
		{
			m_pElements = (T*)malloc(sizeof(T) * iNumElements);
			memset(m_pElements, 0, sizeof(T) * iNumElements);
		}
		m_iNumElements = iNumElements;
		m_iNumUsedElements = 0;
	}

	const int GetNumElements()
	{
		return m_iNumElements;
	}

	const int GetNumElementsUsed()
	{
		return m_iNumUsedElements;
	}

	T& operator[](const int iIndex)
	{
		return m_pElements[iIndex];
	}

	T& GetElement(const int iIndex)
	{
		return m_pElements[iIndex];
	}

	int Add(const T& element)
	{
		if( m_iNumUsedElements < m_iNumElements )
		{
			int iIndex = m_iNumUsedElements;
			m_iNumUsedElements++;
			m_pElements[iIndex] = element;

			return iIndex;
		}

		return -1;
	}

protected:
	int		m_iNumElements;
	int		m_iNumUsedElements;
	T*		m_pElements;
};
