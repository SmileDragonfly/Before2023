#pragma once

#define TOTAL_MESSAGE_TYPE	16
#define MESSAGE_TYPE_UNKNOW _T("UNK")
enum STATUS_FILE_TYPE
{
	STATUS_FILE_TYPE_MESSAGE = 0,
	STATUS_FILE_TYPE_ECA,
	STATUS_FILE_TYPE_TRACE_IP,
	STATUS_FILE_TYPE_APP_MONITOR,
	STATUS_FILE_TYPE_EJ_MONITOR,
	STATUS_FILE_TYPE_DCA,
	STATUS_FILE_TYPE_PROFILE,
	STATUS_FILE_TYPE_EJ_DATA,
	STATUS_FILE_TYPE_CPK,
	STATUS_FILE_TYPE_GDK,
	STATUS_FILE_TYPE_ECP,
	STATUS_FILE_TYPE_ISO,
	STATUS_FILE_TYPE_GRG,
	STATUS_FILE_TYPE_VER,
	STATUS_FILE_TYPE_DVS,
	STATUS_FILE_TYPE_RLT
};


class CExportMessageStatus
{
public:
	CExportMessageStatus();
	CExportMessageStatus(CString strMsgFolder);
	~CExportMessageStatus();
	
public:
	// set export folder
	void SetMessageFolder(CString strMsgFolder);
	
	// create and write file
	BOOL ExportStatus(int nMessageType, CStringA szContent, CString &sMessageFile);
	BOOL ExportStatus(int nMessageType, CString strContent, CString &sMessageFile);

private:
	// create temporary folder
	void CreateSubFolder(int nMessageType, CString &subfolder, CString &filename);

	// remove empty folder
	void RemoveSubFolderEmpty(CString sourcePath, CString currentFolder);


private:
	CString m_strMessageFolder;

};

