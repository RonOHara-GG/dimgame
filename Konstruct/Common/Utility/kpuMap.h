#pragma once

#include "Common\Utility\kpuLinkedList.h"

template <typename TKey, typename TVal>
class kpuMap
{
#define NUM_BUCKETS	256

struct sKeyVal
{
	sKeyVal(u32 uKey, const TVal& tVal)
	{
		m_uKey = uKey;
		m_tVal = tVal;
	}

	u32		m_uKey;
	TVal	m_tVal;
};

public:
	kpuMap()
	{
	}

	~kpuMap()
	{
		for( int i = 0; i < NUM_BUCKETS; i++ )
		{
			kpuLinkedList* pIter = m_aBuckets[i].First();
			while( pIter )
			{
				sKeyVal* pKeyVal = (sKeyVal*)pIter->GetPointer();
				delete pKeyVal;
				delete pIter;

				pIter = m_aBuckets[i].First();
			}
		}
	}

	inline u8 BucketFromKey(u32 uKey)
	{
		u8* pKey = (u8*)&uKey;
		return pKey[0] ^ pKey[1] ^ pKey[2] ^ pKey[3];
	}

	u32 ConvertKeyToU32(const u32& key)		{ return key; }
	u32 ConvertKeyToU32(const char* key)	{ return StringHash(key); }

	void Add(const TKey& key, const TVal& val)
	{
		u32 uKey = ConvertKeyToU32(key);

		u8 uBucket = BucketFromKey(uKey);

		sKeyVal* pKeyVal = new sKeyVal(uKey, val);
		m_aBuckets[uBucket].AddTail(pKeyVal);
	}

	void Remove(const TKey& key, const TVal& val)
	{
		u32 uKey = ConvertKeyToU32(key);
		u8 uBucket = BucketFromKey(uKey);

		kpuLinkedList* pIter = m_aBuckets[uBucket].First();
		while( pIter )
		{
			sKeyVal* pKeyVal = (sKeyVal*)pIter->GetPointer();
			if( pKeyVal && pKeyVal->m_uKey == uKey )
			{
				delete pKeyVal;
				delete pIter;
				return;
			}

			pIter = pIter->Next();
		}
	}

	TVal* operator[](const TKey& key)
	{
		u32 uKey = ConvertKeyToU32(key);
		u8 uBucket = BucketFromKey(uKey);

		kpuLinkedList* pIter = m_aBuckets[uBucket].First();
		while( pIter )
		{
			sKeyVal* pKeyVal = (sKeyVal*)pIter->GetPointer();
			if( pKeyVal && pKeyVal->m_uKey == uKey )
				return &pKeyVal->m_tVal;

			pIter = pIter->Next();
		}
		return 0;
	}

protected:
	kpuLinkedList	m_aBuckets[NUM_BUCKETS];
};
