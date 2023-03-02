#pragma once
#include <afxwin.h>
#include "CommonStructures.h"
#include "CommonDefinitions.h"
class CServerThread :
	public CWinThread
{
protected:
	INT m_nPort;
	TPROTOCOL m_nProtocol;
public:
	BOOL InitServerThread(INT nPort, TPROTOCOL protocol);
};

