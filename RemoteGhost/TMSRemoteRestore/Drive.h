// Copyright (c) 2016 Microtec. All rights reserved.

#ifndef SYMPHONY_DRIVE_H_
#define SYMPHONY_DRIVE_H_

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

#define DRIVE_FILE_NAME _T("Drive.ini")

/************************************************************************/
/* CScript                                                              */
/************************************************************************/

class CDrive
{
public:
	CDrive(void);
	~CDrive(void);

protected:
	CString				 m_sRootPath;
	CString				 m_sDriveFilePath;
	const static CString m_sDriveFileName; 

public:
	// Header [FILE]
	DECLARE_PROPERTY_CSTRING(DRIVE, BOOT_DRIVE, );
	DECLARE_PROPERTY_CSTRING(DRIVE, BOOT_DRIVE_ID, );

public:
	BOOL    Init();
	CString Read(CString header, CString tag, CString _default = _T(""));
	BOOL	Write(CString header, CString tag, CString value);
	
	CString GetRootPath();
	CString GetFilePath();

private:
	static CString ReadINI(CString strIniPath, CString strHeader, CString strTag, CString strDefault = _T(""));

	std::vector<CString> vFileFolders;
	std::vector<CString> vPatterns;
};

#endif //SYMPHONY_DRIVE_H_