#pragma once

template <typename T>
class kpuArrayList
{
public:
        kpuArrayList(int iAllocElements = 0, int iAllocCount = 4)
        {
                m_iAllocatedElements = 0;
                m_iNumElements = 0;
                m_pData = 0;
                m_iAllocCount = iAllocCount;

                AllocateElements(iAllocElements);
        }

        ~kpuArrayList(void)
        {
                if( m_pData )
                        free(m_pData);
        }

        int Count() { return m_iNumElements; }

		void RemoveAt(int iIndex)
		{
			for(int i = iIndex + 1; i < m_iNumElements; i++)
			{
				m_pData[i - 1] = m_pData[i];
			}

			m_iNumElements--;
		}

		bool Remove(T const& element)
		{
			for(int i = 0; i < m_iNumElements; i++)
			{
				if(m_pData[i] == element)
				{
					RemoveAt(i);
					return true;
				}
			}

			return false;
		}

        int Add(T const& element)
        {
                if( m_iNumElements >= m_iAllocatedElements )
                {
                        // Need to make space
                        AllocateElements(m_iAllocCount);
                }

                int iIndex = m_iNumElements;
                m_pData[iIndex] = element;
                m_iNumElements++;
                return iIndex;
        }

		void Override(T const& element, int iIndex)
		{
			m_pData[iIndex] = element;
		}

		bool Contains(T const& element)
		{
			for(int i = 0; i < m_iNumElements; i++)
			{
				if(&element == &m_pData[i])
					return true;
			}

			return false;
		}

        T& operator[](int iIndex)
        {
                return m_pData[iIndex];
        }

private:
        void AllocateElements(int iNumToAlloc)
        {
                if( iNumToAlloc > 0 )
                {
                        m_iAllocatedElements += iNumToAlloc;
                        if( m_pData )
                                m_pData = (T*)realloc(m_pData, m_iAllocatedElements * sizeof(T));                        
                        else
                                m_pData = (T*)malloc(m_iAllocatedElements * sizeof(T));
                }
        }

protected:
        T*                 m_pData;
        int                m_iNumElements;
        int                m_iAllocatedElements;
        int                m_iAllocCount;
};

