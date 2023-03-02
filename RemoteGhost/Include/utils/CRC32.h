#pragma once
#include "afx.h"


#define ____CCRC32_MAX_BUFFER_SIZE	4096

class CCRC32 :	public CObject
{
	//DECLARE_DYNAMIC(CCRC32);
public:
	CCRC32(void);
	virtual ~CCRC32(void);

public:	
	static DWORD Crc32Bytes(BYTE* pbyte, DWORD bufferlength, DWORD &dwCrc32);	
	static DWORD Crc32File(LPCTSTR szFilename, DWORD &dwCrc32);

protected:	
	static inline void CalcCrc32(const BYTE byte, DWORD &dwCrc32);

	static DWORD s_arrdwCrc32Table[256];
};
