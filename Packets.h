#pragma once
#include <iostream>
#include <Windows.h>
#define PACKETBUFFERSIZE 8192
#define PACKETHEADERSIZE 4

class CPackets
{

public:
	CPackets();
	CPackets(unsigned short idValue);
	CPackets(const CPackets& source);
	virtual ~CPackets();

	bool isValidHeader();
	bool isValidPacket();
	CPackets& id(unsigned short ID);
	unsigned short id();

	unsigned short getDataFieldSize();
	int getPacketSize() { return (getDataFieldSize() + PACKETHEADERSIZE); }
	int getReceivedSize() { return receiveSize; }
	char* getPacketBuffer() { return packetBuffer; }

	void copyToBuffer(char* buff, int size);
	void clear();
	void WriteData(void* buffer, int size);
	void ReadData(void* buffer, int size);

	CPackets&	operator = (CPackets& packet);

	CPackets&	operator << (bool arg);
	CPackets&	operator << (int arg);
	CPackets&	operator << (long arg);
	CPackets&	operator << (DWORD arg);
	CPackets&	operator << (__int64 arg);
	CPackets&	operator << (LPTSTR arg);
	CPackets&	operator << (CPackets& arg);
	
	CPackets&	operator >> (bool& arg);
	CPackets&	operator >> (int& arg);
	CPackets&	operator >> (long& arg);
	CPackets&	operator >> (DWORD& arg);
	CPackets&	operator >> (__int64& arg);
	CPackets&	operator >> (LPTSTR arg);
	CPackets&	operator >> (CPackets& arg);

protected:
	typedef struct
	{
		unsigned short* protocolID;
		unsigned short* dataSize;
	}HEADER;

	HEADER packetHeader;
private:
	char packetBuffer[PACKETBUFFERSIZE];
	char* dataField;
	char* endOfDataField;
	char* readPosition, *writePosition;
	int receiveSize;
};

