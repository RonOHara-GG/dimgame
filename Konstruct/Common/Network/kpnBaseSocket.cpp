#include "StdAfx.h"
#include "kpnBaseSocket.h"
#include "Common/Utility/kpuStopwatch.h"

typedef struct _sSendListItem
{
	kpnPacket*		m_pPacket;
	kpuStopwatch	m_sStopwatch;
	u32				m_unAddress;
	int				m_nPort;
} sSendListItem;

kpnBaseSocket::kpnBaseSocket(int nPort) : m_nPort(nPort)
{
	// Create the socket
	m_sSocket = socket(AF_INET, SOCK_DGRAM, 0);
	m_bOwnsSocketHandle = true;

	// Set the socket to non blocking mode
	unsigned long nblock = 1;
	ioctlsocket(m_sSocket, FIONBIO, &nblock);

	// Bind to the desired port
//	if( m_nPort )
	{
		sockaddr_in here;
		ZeroMemory(&here, sizeof(sockaddr_in));
		here.sin_family = AF_INET;
		here.sin_addr.S_un.S_addr = INADDR_ANY;
		here.sin_port = nPort;
		int err = bind(m_sSocket, (sockaddr*)&here, sizeof(sockaddr_in));
		if( err != 0 )
		{
			PrintError();
		}
	}

	m_unCurrentReadPacket = 0;
	m_unCurrentSendPacket = 0;
}

kpnBaseSocket::kpnBaseSocket(SOCKET sSocket, u32 unAddress, int nPort) : m_sSocket(sSocket), m_unAddress(unAddress), m_nPort(nPort)
{
	m_unCurrentReadPacket = 0;
	m_unCurrentSendPacket = 0;
	m_bOwnsSocketHandle = false;
}

kpnBaseSocket::~kpnBaseSocket(void)
{
	if( m_bOwnsSocketHandle )
		closesocket(m_sSocket);
}

void kpnBaseSocket::Initialize()
{
	u16 wVersionRequested;
	WSADATA wsaData;
	 
	wVersionRequested = MAKEWORD( 2, 2 );
	 
	WSAStartup( wVersionRequested, &wsaData );
}

void kpnBaseSocket::Shutdown()
{
	WSACleanup();
}

void kpnBaseSocket::SetAddress(const char* szAddress)
{
	m_unAddress = inet_addr(szAddress);
}

void kpnBaseSocket::SetAddress(u32 unAddress)
{
	m_unAddress = unAddress;
}

void kpnBaseSocket::SetPort(int nPort)
{
	m_nPort = nPort;
}

u32 kpnBaseSocket::GetLocalIP() const
{
	char name[256];
	gethostname(name, sizeof(name));
	struct hostent *phe = gethostbyname(name);

	return *(u32*)phe->h_addr_list[0];
}

bool kpnBaseSocket::ReadPacket(char* pOutBuffer, int& nOutBufferSize, sockaddr_in* pFrom)
{
	int nAddrSize = sizeof(sockaddr_in);
	int nBytesRead = recvfrom(m_sSocket, pOutBuffer, nOutBufferSize, 0, (sockaddr*)pFrom, &nAddrSize);
	if( nBytesRead > 0 )
	{
		nOutBufferSize = nBytesRead;
		return true;
	}
	if( nBytesRead < 0 )
		PrintError();
	return false;
}

void kpnBaseSocket::Send(kpnPacket* pPacket, u32 unAddress, int nPort, bool bStream)
{
	if( !bStream )
	{
		// No stream information, just send the packet and hope it gets there
		pPacket->SetSequenceNumber(NON_STREAM_SEQUENCE);
		SendLowLevel(pPacket, pPacket->Size(), unAddress, nPort);
	}
	else
	{
		// Compress & Encrypt
		// TODO: Add compression & encryption

		// Fragment the packet if it is to large
		if( (pPacket->DataSize() + sizeof(kpnPacket)) > MAX_PACKET_SIZE )
		{
			// Need to fragment the packet
			u32 unSizeRemaining = pPacket->DataSize();
			u8* pDataPointer = (u8*)(pPacket + 1);
			while( unSizeRemaining )
			{
				// Determine the size of the fragment
				u32 unFragmentSize = min(unSizeRemaining, (MAX_PACKET_SIZE - sizeof(kpnPacket)));

				// Allocate space for the fragment
				kpnPacket* pFragment = (kpnPacket*)malloc(sizeof(kpnPacket) + unFragmentSize);

				// Copy the original header
				memcpy(pFragment, pPacket, sizeof(kpnPacket));

				// Copy the data into the packet
				u8* pPtr = (u8*)(pFragment + 1);
				memcpy(pPtr, pDataPointer, unFragmentSize);

				// Set the fragment information in the header
				pFragment->SetFragment(true);
				pFragment->SetSize(sizeof(kpnPacket) + unFragmentSize);
				pFragment->SetDataSize(unFragmentSize);

				// Send the packet off
				SendInternal(pFragment, unAddress, nPort);

				// Adjust the input data pointers
				unSizeRemaining -= unFragmentSize;
				pDataPointer += unFragmentSize;
			}
		}
		else
		{
			// Smaller than the max size, so no need to fragment
			// We do need to make a copy of it though
			kpnPacket* pSolid = (kpnPacket*)malloc(pPacket->Size());
			memcpy(pSolid, pPacket, pPacket->Size());

			// Send it off
			SendInternal(pSolid, unAddress, nPort);
		}
	}
}

void kpnBaseSocket::SendInternal(kpnPacket* pPacket, u32 unAddress, int nPort)
{
	// Create a send list item for this packet
	sSendListItem* pSendItem = new sSendListItem();
	pSendItem->m_pPacket = pPacket;
	pSendItem->m_sStopwatch.SetSeconds(PACKET_RESEND_SECONDS);
	pSendItem->m_unAddress = unAddress;
	pSendItem->m_nPort = nPort;

	// Set the ack
	pPacket->SetSequenceNumber(m_unCurrentSendPacket++);

	// Send the packet
	SendLowLevel(pPacket, pPacket->Size(), unAddress, nPort);

	// Add the packet to the send list
	m_lPacketSendList.AddTail(pSendItem);
}

void kpnBaseSocket::SendLowLevel(void* pData, int nSize, u32 unAddress, int nPort)
{
	sockaddr_in to;
	ZeroMemory(&to, sizeof(sockaddr_in));
	to.sin_family = AF_INET;
	to.sin_port = nPort;
	to.sin_addr.S_un.S_addr = unAddress;
	if( sendto(m_sSocket, (const char*)pData, nSize, 0, (sockaddr*)&to, sizeof(sockaddr_in)) != nSize )
	{
		printf("ERROR: Failed to send packet(%d bytes) to address:(%8.8x:%d)\n", nSize, unAddress, nPort);
	}
}

void kpnBaseSocket::SendAcknowledgement(kpnPacketInfo* pPacket)
{
	// Build an ack packet
	kpnPacket ack(0, sizeof(kpnPacket), 0);
	ack.SetAcknowledge(true);
	ack.SetSequenceNumber(pPacket->m_pPacket->SequenceNumber());

	// Send it
	SendLowLevel(&ack, ack.Size(), pPacket->m_unAddress, pPacket->m_nPort);
}

void kpnBaseSocket::Acknowledge(kpnPacket* pPacket)
{
	// Find the packet in the send list
	kpuLinkedList* pIter = m_lPacketSendList.Next();
	while( pIter )
	{
		sSendListItem* pItem = (sSendListItem*)pIter->GetPointer();
		if( pItem->m_pPacket->SequenceNumber() == pPacket->SequenceNumber() )
		{
			// Acknowledged this packet, need to clean it up
			// Free the packet memory
			free(pItem->m_pPacket);

			// Delete the list item
			delete pItem;
			
			// Remove the list node
			delete pIter;

			// Finished, so return
			break;
		}
		pIter = pIter->Next();
	}
}

void kpnBaseSocket::Update(DispatchFunction* pDispatchFunctions)
{
	// Handle incoming packets
	{
		char incomingPacket[MAX_PACKET_SIZE];
		int nSize = MAX_PACKET_SIZE;
		sockaddr_in from;
		while( ReadPacket(incomingPacket, nSize, &from) )
		{
			kpnPacket* pPacket = (kpnPacket*)incomingPacket;

			// If this is an acknowledgement, handle it differently
			if( pPacket->IsAcknowledge() )
			{
				Acknowledge(pPacket);
				continue;
			}

			kpnPacketInfo* pPacketInfo = new kpnPacketInfo();
			pPacketInfo->m_nPort = from.sin_port;
			pPacketInfo->m_unAddress = from.sin_addr.S_un.S_addr;

			// If this is a broadcast packet, insert it at the top of the list
			if( pPacket->SequenceNumber() == NON_STREAM_SEQUENCE )
			{
				pPacket = (kpnPacket*)malloc(nSize);
				memcpy(pPacket, incomingPacket, nSize);
				pPacketInfo->m_pPacket = pPacket;
				m_lPackets.Insert(pPacketInfo);
			}

			// Make sure we need this packet
			else if( pPacket->SequenceNumber() >= m_unCurrentReadPacket )
			{
				// Copy the packet data into a more permanent spot
				pPacket = (kpnPacket*)malloc(nSize);
				memcpy(pPacket, incomingPacket, nSize);
				pPacketInfo->m_pPacket = pPacket;

				// Find a spot in the list for the packet
				kpuLinkedList* pIter = m_lPackets.Next();
				if( !pIter )
				{
					m_lPackets.AddTail(pPacketInfo);
				}
				else
				{
					while( pIter )
					{
						kpnPacketInfo* pIPacket = (kpnPacketInfo*)pIter->GetPointer();
						if( pIPacket->m_pPacket->SequenceNumber() > pPacket->SequenceNumber() )
						{
							pIter->InsertBefore(pPacketInfo);
							break;
						}
						else if( pIPacket->m_pPacket->SequenceNumber() < pPacket->SequenceNumber() )
						{
							if( pIter->Next() )
							{
								if( ((kpnPacketInfo*)pIter->Next()->GetPointer())->m_pPacket->SequenceNumber() > pPacket->SequenceNumber() )
								{
									// Insert here
									pIter->Insert(pPacketInfo);
									break;
								}
							}
							else
							{
								// End of the road
								pIter->Insert(pPacketInfo);
								break;
							}
						}

						pIter = pIter->Next();
					}
				}

				// Acknowledge reciept of this packet
				SendAcknowledgement(pPacketInfo);
			}

			// Fix size for the next read
			nSize = MAX_PACKET_SIZE;
		}

		// Process any fragments
		kpuLinkedList* pIter = m_lPackets.Next();
		while( pIter )
		{
			kpnPacketInfo* pPacketInfo = (kpnPacketInfo*)pIter->GetPointer();
			kpnPacket* pPacket = pPacketInfo->m_pPacket;
			if( pPacket->IsFragment() )
			{
				// See if we have all the packets to reconstruct this packet
				u32 unSizeRemaining = pPacket->DataSize() - (pPacket->Size() - sizeof(kpnPacket));
				u32 unNextNumber = pPacket->SequenceNumber() + 1;
				kpuLinkedList* pIIter = pIter->Next();
				while( unSizeRemaining && pIIter )
				{
					kpnPacket* pIPacket = ((kpnPacketInfo*)pIIter->GetPointer())->m_pPacket;

					// Make sure we have the correct next packet
					if( pIPacket->SequenceNumber() != unNextNumber )
						break;

					// Subtract the size of this packet
					unSizeRemaining -= (pIPacket->Size() - sizeof(kpnPacket));

					unNextNumber++;
					pIIter = pIIter->Next();
				}

				if( !unSizeRemaining )
				{
					// Fragment complete.  Rejoin into one packet
					kpnPacket* pWholePacket = (kpnPacket*)malloc(pPacket->DataSize() + sizeof(kpnPacket));
					u8* pPtr = (u8*)pWholePacket + pPacket->Size();
					memcpy(pWholePacket, pPacket, pPacket->Size());

					kpuLinkedList* pUIter = pIter->Next();
					while( pUIter != pIIter )
					{
						// Copy the fragment data
						kpnPacketInfo* pFragmentInfo = (kpnPacketInfo*)pUIter->GetPointer();
						kpnPacket* pFragment = pFragmentInfo->m_pPacket;
						u8* pDataPtr = (u8*)pFragment + sizeof(kpnPacket);
						memcpy(pPtr, pDataPtr, pFragment->DataSize());
						pPtr += pFragment->DataSize();

						// Delete this packet & remove it from the packet list
						free(pFragment);
						delete pFragmentInfo;
						delete pUIter;

						pUIter = pIter->Next();
					}

					// Delete the original packet and replace it with the new one
					pWholePacket->SetFragment(false);
					free(pPacket);
					pPacketInfo->m_pPacket = pWholePacket;
				}
			}
			pIter = pIter->Next();
		}

		// Decrypt & Decompress data
		// TODO: Add encryption & compression

		// Dispatch the packets to their appropriate handlers
		pIter = m_lPackets.Next();
		while( pIter )
		{
			kpnPacketInfo* pPacketInfo = (kpnPacketInfo*)pIter->GetPointer();
			kpnPacket* pPacket = pPacketInfo->m_pPacket;
			if( !pPacket->IsFragment() )
			{
				// Dispatch the packet
				u32 unLocalPacketID = pPacket->ID() & 0x0000FFFF;
				if( pDispatchFunctions[unLocalPacketID] )
					pDispatchFunctions[unLocalPacketID](this, pPacketInfo);
				else
					printf("ERROR: No packet handler provided for packet id: 0x%8.8x\n", pPacket->ID());

				// Delete the packet & remove it from the list
				free(pPacket);
				delete pPacketInfo;

				kpuLinkedList* pKill = pIter;
				pIter = pIter->Prev();
				delete pKill;
			}

			pIter = pIter->Next();
		}
	}

	// Handle outgoint packets
	{
		kpuLinkedList* pIter = m_lPacketSendList.Next();
		while( pIter )
		{
			sSendListItem* pItem = (sSendListItem*)pIter->GetPointer();
			if( pItem->m_sStopwatch.Expired() )
			{
				// Timer expired and we havnt seen an acknowledgement yet.  Send it again
				SendLowLevel(pItem->m_pPacket, pItem->m_pPacket->Size(), pItem->m_unAddress, pItem->m_nPort);

				// Reset the timer
				pItem->m_sStopwatch.SetSeconds(PACKET_RESEND_SECONDS);
			}

			// Move on to the next item in the list
			pIter = pIter->Next();
		}
	}
}

void kpnBaseSocket::PrintError() const
{
	int nErr = WSAGetLastError();
	switch( nErr )
	{
		case 0:	
			break;
		case WSANOTINITIALISED:
			printf("Socket Error: Not Initialized\n");
			break;
		case WSAENETDOWN:
			printf("Socket Error: Network is down\n");
			break;
		case WSAEACCES:
			printf("Socket Error: Broadcasting not enabled\n");
			break;
		case WSAEADDRINUSE:
			printf("Socket Error: Address in use\n");
			break;
		case WSAEADDRNOTAVAIL:
			printf("Socket Error: Invalid Address\n");
			break;
		case WSAEFAULT:
			printf("Socket Error: Invalid Name\n");
			break;
		case WSAEINPROGRESS:
			printf("Socket Error: Blocking operation in progress\n");
			break;
		case WSAEINVAL:
			printf("Socket Error: Already bound\n");
			break;
		case WSAENOBUFS:
			printf("Socket Error: To Many Connections\n");
			break;
		case WSAENOTSOCK:
			printf("Socket Error: Not a Socket\n");
			break;
		case WSAEWOULDBLOCK:
			break;
		case WSAECONNRESET:
			printf("Socket Error: An existing connection was forcibly closed by the remote host\n");
			break;
		default:
			printf("Unknown socket error\n");
			break;
	}
}