// Copyright (c) 2016 Microtec. All rights reserved.

#ifndef SYMPHONY_SCANNER_CONFIG_H_
#define SYMPHONY_SCANNER_CONFIG_H_

#include <vector>

using namespace std;

/*********************************************
* Author: HieuTT
* Module: Symphony Scanner
* Created: 
* Modified: 26-Aug-2016
* Change log:
* Description: load/reload config
**********************************************/
#define ValidateRange(value, vmin, vmax, vdefault) \
if ((value < vmin) || (value > vmax)) \
	value = vdefault;


#define DECLARE_PROPERTY_CSTRING(HEADER, member_name, default_value) \
	private: \
		CString m_##member_name; \
	public: \
		CString member_name##() \
		{ \
			return m_##member_name; \
		}; \
	public: \
		void member_name##(CString v) \
		{ \
			m_##member_name = v; \
			Write(_T(#HEADER),_T(#member_name),v); \
		};\
		void Get_##member_name() \
		{ \
			CString v = Read(_T(#HEADER), _T(#member_name), _T("")); \
			v = v.Trim();\
			if (v.IsEmpty()) \
			{\
				member_name(_T(#default_value));\
			}\
			m_##member_name = v;\
		};
		
#define DECLARE_PROPERTY_NUMBER(HEADER, member_name, default_value, min_value, max_value) \
	private: \
		long m_##member_name; \
	public: \
		long member_name##() \
		{ \
			return m_##member_name; \
		}; \
	public: \
		void member_name##(long v) \
		{ \
			m_##member_name = v; \
			CString sv; \
			sv.Format(_T("%i"), m_##member_name);\
			Write(_T(#HEADER),_T(#member_name),sv); \
		};\
		void Get_##member_name() \
		{ \
			CString v = Read(_T(#HEADER), _T(#member_name), _T("")); \
			v = v.Trim();\
			if (v.IsEmpty()) \
			{\
				member_name(default_value);\
			}\
			else\
			{\
				long nv = ToNumber(v);\
				long new_nv = nv;\
				ValidateRange(new_nv, min_value, max_value, default_value);\
				if (new_nv != nv)\
					member_name##(new_nv);\
				else\
					m_##member_name = new_nv;\
			}\
		};


#define MAX_DATA 513
#define BUFFER_SIZE MAX_DATA*sizeof(TCHAR)
#define CONFIG_FILE_NAME _T("Config.ini")

enum REG_MODE
{
	REG_MODE_READ_ATTR = 0,
	REG_MODE_ENUM_SUB_KEY = 1
};

/************************************************************************/
/* CConfig                                                              */
/************************************************************************/

class CConfig
{
public:
	CConfig(void);
	~CConfig(void);

protected:
	CString				 m_sRootPath;
	CString				 m_sConfigFilepath;
	const static CString m_sConfigFileName; 

public:
	// Header [PATH]
	DECLARE_PROPERTY_CSTRING(PATH, PATH_LOG, );
	DECLARE_PROPERTY_CSTRING(PATH, PATH_WORKING, );
	DECLARE_PROPERTY_CSTRING(PATH, GHOST_TOOL, Ghost64.exe);

	// Header [SCRIPTS]
	DECLARE_PROPERTY_CSTRING(SCRIPTS, FOLDER_SCRIPTS, Scripts);
	DECLARE_PROPERTY_CSTRING(SCRIPTS, CHECK_BOOT_SCRIPT, check_boot.bat);
	DECLARE_PROPERTY_CSTRING(SCRIPTS, CHANGE_BOOT_SCRIPT, change_boot.bat);
	DECLARE_PROPERTY_CSTRING(SCRIPTS, RESTORE_SCRIPT, restore.bat);
	DECLARE_PROPERTY_CSTRING(SCRIPTS, RESTART_SCRIPT, restart.bat);
	DECLARE_PROPERTY_CSTRING(SCRIPTS, SET_IP_SCRIPT, set_ip.bat);
	DECLARE_PROPERTY_NUMBER(SCRIPTS, TIMEOUT_COMMAND, 60000, 60000, 600000); // milli seconds
	DECLARE_PROPERTY_NUMBER(SCRIPTS, DELAY_RESTART, 15, 10, 600); //second

	// Header [GHOSTTOOL]
	DECLARE_PROPERTY_NUMBER(NORTON, TIMEOUT_GHOST_TOOL, 30, 1, 180); // minute
	DECLARE_PROPERTY_NUMBER(NORTON, CHECK_GHOST_TOOL_INTERVAL, 60, 30, 600); // second

public:
	BOOL    Init();
	BOOL    Init(CString strRootPath);
	CString Read(CString header, CString tag, CString _default = _T(""));
	BOOL	Write(CString header, CString tag, CString value);
	
	CString GetRootPath();

private:
	static CString ReadINI(CString strIniPath, CString strHeader, CString strTag, CString strDefault = _T(""));
	BOOL InitLog(CString pathLog);

};

#endif //SYMPHONY_SCANNER_CONFIG_H_