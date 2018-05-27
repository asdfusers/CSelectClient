#include "stdafx.h"
#include "Packets.h"

CPackets::CPackets(unsigned short idValue) : dataField(0), readPosition(0), writePosition(0), receiveSize(0)
{
	clear();
	id(idValue);
}

CPackets::CPackets(const CPackets & source) : dataField(0), readPosition(0), writePosition(0), receiveSize(0)
{
	clear();
	memcpy(packetBuffer, source.packetBuffer, PACKETBUFFERSIZE);
	receiveSize = source.receiveSize;

	DWORD offset;
	offset = (DWORD)source.readPosition - (DWORD)source.dataField;
	readPosition += offset;

	offset = (DWORD)source.writePosition - (DWORD)source.dataField;
	writePosition += offset;

}

CPackets::CPackets() : dataField(0), readPosition(0), writePosition(0), receiveSize(0)
{
	clear();
}


CPackets::~CPackets()
{
}

bool CPackets::isValidHeader()
{
	return (getPacketSize() >= PACKETHEADERSIZE);
}

bool CPackets::isValidPacket()
{
	if (isValidHeader() == false)
		return false;

	return (getPacketSize() >= receiveSize);
}

CPackets & CPackets::id(unsigned short ID)
{
	*packetHeader.protocolID = ID;

	return *this;
}

unsigned short CPackets::id()
{
	return *packetHeader.protocolID;
}

unsigned short CPackets::getDataFieldSize()
{
	return *packetHeader.dataSize;
}

void CPackets::copyToBuffer(char * buff, int size)
{
	clear();
	memcpy(packetBuffer, buff, size);
	receiveSize += size;
}

void CPackets::clear()
{
	ZeroMemory(packetBuffer, PACKETBUFFERSIZE);
	packetHeader.dataSize = (unsigned short*)(packetBuffer + 0);
	packetHeader.protocolID = (unsigned short*)(packetBuffer + 2);
	dataField = (char*)(packetBuffer + 4);
	readPosition = writePosition = dataField;
	endOfDataField = &dataField[PACKETBUFFERSIZE - 1];
	id(0);
	receiveSize = 0;
}

void CPackets::WriteData(void * buffer, int size)
{
	memcpy(writePosition, buffer, size);
	writePosition += size;
	receiveSize += size;
	*packetHeader.dataSize += size;

}

void CPackets::ReadData(void * buffer, int size)
{
	memcpy(buffer, readPosition, size);
	readPosition += size;
}

CPackets & CPackets::operator=(CPackets & packet)
{
	memcpy(dataField, packet.dataField, packet.getDataFieldSize());

	*packetHeader.protocolID = packet.id();
	*packetHeader.dataSize = packet.getDataFieldSize();

	return *this;
}

CPackets & CPackets::operator<<(bool arg)
{
	WriteData(&arg, sizeof(bool));

	return *this;
}

CPackets & CPackets::operator<<(int arg)
{
	WriteData(&arg, sizeof(int));

	return *this;
}

CPackets & CPackets::operator<<(long arg)
{
	WriteData(&arg, sizeof(long));

	return *this;
}

CPackets & CPackets::operator<<(DWORD arg)
{
	WriteData(&arg, sizeof(DWORD));

	return *this;
}

CPackets & CPackets::operator<<(__int64 arg)
{
	WriteData(&arg, sizeof(__int64));

	return *this;
}

CPackets &CPackets::operator<<(LPTSTR arg)
{
	char* pStr;
	int strSize = WideCharToMultiByte(CP_ACP, 0, arg, -1, NULL, 0, NULL, NULL);
	pStr = new char[strSize];
	WideCharToMultiByte(CP_ACP, 0, arg, -1, pStr, strSize, 0, 0);

 	WriteData(pStr, strSize);

	return *this;
}

CPackets & CPackets::operator<<(CPackets & arg)
{
	unsigned int idValue = arg.id();
	unsigned int size = arg.getDataFieldSize();

	WriteData(&idValue, sizeof(unsigned int));
	WriteData(&size, sizeof(unsigned int));
	WriteData(arg.dataField, size);

	return *this;
}

CPackets & CPackets::operator<<(std::string arg)
{
	int length = arg.length();
	WriteData(&arg, length);

	return *this;

}



CPackets & CPackets::operator >> (bool & arg)
{
	ReadData(&arg, sizeof(bool));

	return *this;
}

CPackets & CPackets::operator >> (int & arg)
{
	ReadData(&arg, sizeof(int));

	return *this;
}

CPackets & CPackets::operator >> (long & arg)
{
	ReadData(&arg, sizeof(long));

	return *this;
}

CPackets & CPackets::operator >> (DWORD & arg)
{
	ReadData(&arg, sizeof(DWORD));

	return *this;
}

CPackets & CPackets::operator >> (__int64 & arg)
{
	ReadData(&arg, sizeof(__int64));

	return *this;
}

CPackets & CPackets::operator >> (LPTSTR arg)
{


	ReadData(arg, lstrlen((LPTSTR)readPosition) * sizeof(wchar_t) + sizeof(wchar_t));

	return *this;
}

CPackets & CPackets::operator >> (CPackets & arg)
{
	int idValue, size;
	char buffer[PACKETBUFFERSIZE];

	ReadData(&idValue, sizeof(int));
	ReadData(&size, sizeof(int));

	ReadData(buffer, size);

	arg.id(idValue);
	arg.WriteData(buffer, size);

	return *this;
}

CPackets & CPackets::operator >> (std::string & arg)
{
	int length = sizeof(arg);
	ReadData(&arg, length);

	return *this;

}


