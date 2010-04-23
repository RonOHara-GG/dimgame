#pragma once

class kpuLinkedList
{
public:
	kpuLinkedList();
	~kpuLinkedList();

	void			AddHead(void* pData);
	void			AddTail(void* pData);

	void			Insert(void* pData);
	void			InsertBefore(void* pData);
	
	void*			GetPointer() const;
	void			SetPointer(void* pData);


	kpuLinkedList*	Next() const;
	kpuLinkedList*	Prev() const;
	kpuLinkedList*	First() const;
	kpuLinkedList*	Last() const;

protected:
	kpuLinkedList*	m_pNext;
	kpuLinkedList*	m_pPrev;
	void*			m_pData;
};


/////////////////////////////////////////////////////////////////////////
// Implementation

inline kpuLinkedList::kpuLinkedList() : m_pNext(0), m_pPrev(0), m_pData(0)
{
}

inline kpuLinkedList::~kpuLinkedList()
{
	// Remove this node from the list
	if( m_pPrev )
		m_pPrev->m_pNext = m_pNext;
	if( m_pNext )
		m_pNext->m_pPrev = m_pPrev;
}

inline void* kpuLinkedList::GetPointer() const
{
	return m_pData;
}

inline void kpuLinkedList::SetPointer(void* pData)
{
	m_pData = pData;
}

inline kpuLinkedList* kpuLinkedList::Next() const
{
	return m_pNext;
}

inline kpuLinkedList*	kpuLinkedList::Prev() const
{
	return m_pPrev;
}

inline kpuLinkedList*	kpuLinkedList::First() const
{
	kpuLinkedList* pIter = (kpuLinkedList*)this;
	while( pIter->Prev() )
		pIter = pIter->Prev();

	return pIter;
}

inline kpuLinkedList*	kpuLinkedList::Last() const
{
	kpuLinkedList* pIter = (kpuLinkedList*)this;
	while( pIter->Next() )
		pIter = pIter->Next();

	return pIter;
}

inline void kpuLinkedList::AddHead(void* pData)
{
	// Go to the head node
	kpuLinkedList* pIter = this;
	while( pIter->Prev() )
		pIter = pIter->Prev();

	// At the head node, insert here
	kpuLinkedList* pNode = new kpuLinkedList();
	pNode->m_pData = pData;
	pNode->m_pPrev = pIter;
	pNode->m_pNext = pIter->Next();
	if( pIter->Next() )
		pIter->Next()->m_pPrev = pNode;
	pIter->m_pNext = pNode;

}

inline void kpuLinkedList::AddTail(void* pData)
{
	// Find the tail
	kpuLinkedList* pIter = this;
	while( pIter->Next() )
		pIter = pIter->Next();

	// Add a new node there
	kpuLinkedList* pNode = new kpuLinkedList();
	pNode->m_pData = pData;
	pNode->m_pPrev = pIter;
	pIter->m_pNext = pNode;

}

inline void kpuLinkedList::Insert(void* pData)
{
	kpuLinkedList* pNewNode = new kpuLinkedList();
	pNewNode->m_pData = pData;
	pNewNode->m_pPrev = this;
	pNewNode->m_pNext = m_pNext;
	if( m_pNext )
		m_pNext->m_pPrev = pNewNode;
	m_pNext = pNewNode;

}

inline void kpuLinkedList::InsertBefore(void* pData)
{
	kpuLinkedList* pNewNode = new kpuLinkedList();
	pNewNode->m_pData = pData;
	pNewNode->m_pPrev = m_pPrev;
	pNewNode->m_pNext = this;
	if( m_pPrev )
		m_pPrev->m_pNext = pNewNode;
	m_pPrev = pNewNode;

}

