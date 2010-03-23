#pragma once

#define NON_STREAM_SEQUENCE		0xFFFFFFFF

class kpnPacket
{
public:
	kpnPacket(u32 unID, u32 unSize, u32 unDataSize) : m_unID(unID), m_unSize(unSize), m_unDataSize(unDataSize), m_bFragment(false), m_bAcknowledge(false)		{}

	u32			ID() const					{ return m_unID; }
	u32			Size() const				{ return m_unSize; }
	u32			DataSize() const			{ return m_unDataSize; }
	u32			SequenceNumber() const		{ return m_unSequenceNumber; }
	bool		IsFragment() const			{ return m_bFragment; }
	bool		IsAcknowledge() const		{ return m_bAcknowledge; }

	void		SetFragment(bool bFragment)	{ m_bFragment = bFragment; }
	void		SetAcknowledge(bool bAck)	{ m_bAcknowledge = bAck; }
	void		SetSequenceNumber(u32 nSeq)	{ m_unSequenceNumber = nSeq; }
	void		SetSize(u32 unSize)			{ m_unSize = unSize; }
	void		SetDataSize(u32 unDataSize)	{ m_unDataSize = unDataSize; }

protected:
	u32			m_unID;
	u32			m_unSize;
	u32			m_unDataSize;
	u32			m_unSequenceNumber;
	bool		m_bFragment;
	bool		m_bAcknowledge;
};

// Structure to describe where a packet came from
typedef struct _kpnPakcketInfo
{
	u32			m_unAddress;
	int			m_nPort;
	kpnPacket*	m_pPacket;
} kpnPacketInfo;