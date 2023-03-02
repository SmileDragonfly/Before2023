#include "stdafx.h"
#include "JsonParser.h"

CJsonParser::CJsonParser()
{
	m_charSize = 0;
}

CJsonParser::~CJsonParser()
{
	m_charSize = 0;
}

/************************************************************************/
/* Kiem tra chuoi json				                                    */
/************************************************************************/
bool CJsonParser::CheckParser(CStringA strJson)
{
// 	CT2CA pszConvertedAnsiString (strJson);
// 	std::string sJSON (pszConvertedAnsiString);

	bool result = false;
	result = reader.parse(strJson.GetBuffer(), value);

	return result;
}

/************************************************************************/
/* Lay danh sach doi tuong thanh phan cua chuoi json                    */
/************************************************************************/
CStringArray* CJsonParser::GetMembers()
{
	std::vector<std::string> members;
	members = value.getMemberNames();
	
	CStringArray* rootMembers = new CStringArray;
	int nMember = (int)members.size();
	for (int i=0; i<nMember; i++)
	{
		//rootMembers->Add(members[i].c_str());
		
		int iSize = (int)members[i].size();
		WCHAR    str3[MAX_PATH];
		MultiByteToWideChar(0,0,members[i].c_str(),iSize, str3, iSize+1);
		LPCWSTR cstr4 = str3;
		rootMembers->Add(cstr4);
	}
	
	return rootMembers;
}

/************************************************************************/
/* Tach du lieu cua mot doi tuong                                       */
/************************************************************************/
char CJsonParser::GetObjectData(char *objName, char *memberName)
{
	const char *result;
	const Json::Value obj = value[objName];
	result = obj[memberName].asCString();
	return *result;
}

/************************************************************************/
/* Tach du lieu cua mot doi tuong                                       */
/* level = cap cua "key"												*/
/*  ...  = danh sach Object tuan tu tu Root den Key can lay du lieu		*/
/************************************************************************/
char CJsonParser::GetObjectData(int level, ...)
{
	const char *result = NULL;
	va_list argList;
	va_start(argList, level);
	if (level>0)
	{			
		Json::Value Obj = value;
		for (int i=0; i<level; i++)
		{
			char *strObjectName = va_arg(argList, LPSTR);
			Obj = Obj[strObjectName];
		}

		result = Obj.asCString();

		va_end(argList);
		return *result;
	}	

	va_end(argList);
	return (char)NULL;
}

/************************************************************************/
/* Tach du lieu cua mang										        */
/************************************************************************/
bool CJsonParser::GetArray(char *arrayname, CStringArray &arrayresult)
{
	const Json::Value newArray = value[arrayname];
	// Iterates over the sequence elements.
	for ( int index = 0; index < (int)newArray.size(); ++index )  
	{	
		arrayresult[index] = newArray[index].asCString();
	}
	return true;
}

/************************************************************************/
/* Tach du lieu cua mang										        */
/************************************************************************/
bool CJsonParser::GetArray(char *arrayname, std::vector<char> &arrayresult)
{
	const Json::Value newArray = value[arrayname];
	// Iterates over the sequence elements.
	for ( int index = 0; index < (int)newArray.size(); ++index )  
	{	
		arrayresult.push_back(*newArray[index].asCString());
	}
	return true;
}

/************************************************************************/
/* Tach du lieu cua mang										        */
/************************************************************************/
bool CJsonParser::GetArray(std::vector<char> vMember, 
						   std::vector<char> &arrayresult)
{
	Json::Value newArray = value;
	int nSize = (int)vMember.size();
	if (nSize>0)
	{
		for (int i=0; i<(int)vMember.size(); i++)
		{
			newArray = newArray[vMember.at(i)];
		}

		// Iterates over the sequence elements.
		for ( int index = 0; index < (int)newArray.size(); ++index )  
		{	
			arrayresult.push_back(*newArray[index].asCString());
		}

		return true;
	}

	return false;
}

/************************************************************************/
/* Tach du lieu cua mang											    */
/* level = cap cua "key"												*/
/*  ...  = danh sach Object tuan tu tu Root den Key can lay du lieu		*/
/************************************************************************/
std::vector<const char*> CJsonParser::GetArray(int level, ...)
{
	std::vector<const char*> vResult;
	va_list argList;	
	va_start( argList, level );

	if (level>0)
	{
		Json::Value newArray = value;
		for (int i=0; i<level; i++)
		{
			char *strObjectName = va_arg(argList, LPSTR);
			newArray = newArray[strObjectName];
		}

		//    delete[] strObjectName;
		// Iterates over the sequence elements.
		for ( int index = 0; index < (int)newArray.size(); ++index )  
		{	
			vResult.push_back(newArray[index].asCString());
		}
	}

	va_end( argList );	
	return vResult;
}

/************************************************************************/
/* Tach du lieu kieu bool                                               */
/************************************************************************/
bool CJsonParser::GetBool(char name, bool &result)
{
	result = value[name].asBool();
	return true;
}

/************************************************************************/
/* Tach du lieu kieu bool                                               */
/************************************************************************/
bool CJsonParser::GetDouble(char name, double &result)
{
	result = value[name].asDouble();
	return true;
}

/************************************************************************/
/* Tach du lieu kieu bool                                               */
/************************************************************************/
bool CJsonParser::GetFloat(char name, float &result)
{
	result = value[name].asFloat();
	return true;
}

/************************************************************************/
/* Tach du lieu kieu bool                                               */
/************************************************************************/
bool CJsonParser::GetInt(char name, int &result)
{
	result = value[name].asInt();
	return true;
}

/************************************************************************/
/* Tach du lieu kieu bool                                               */
/************************************************************************/
bool CJsonParser::GetString(char name, CString &result)
{
	result = value[name].asCString();
	return true;
}

/************************************************************************/
/* kiem tra xem co key hay khong                                       */
/************************************************************************/
bool  CJsonParser::isKey(char objName, char memberName)
{
	const Json::Value obj = value[objName];
	return !obj[memberName].isNull(); 
}

// CString CJsonParser::ToJsonString(char objName, char memberName)
// {
// 	CString result = _T("");
// 	const Json::Value obj = value[objName];
// 	std::string result_ = obj[memberName].toStyledString();
// 	result.Format(_T("{\"%s\":%s}"),memberName,result_.c_str());
// 	return result;
// }