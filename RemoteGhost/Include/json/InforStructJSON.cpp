#include "stdafx.h"
#include "InforStructJSON.h"


// CStringW ConvertUTF8ToUnicode(char* pUtf8)
// {
// 	if (pUtf8==NULL) 
// 		return _T(""); // nothing to do
// 
// 	CStringW uni(_T(""));
// 	int len = MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, NULL, 0);
// 	
// 	// get length (cc) of the new widechar excluding the \0 terminator first
// 	if (len>1)
// 	{ 
// 		// convert
// 		wchar_t *buf = (wchar_t *)calloc(len+1,sizeof(wchar_t));
// 		if (buf)
// 			MultiByteToWideChar(CP_UTF8, 0, pUtf8, -1, buf, len);
// 		uni = CString(buf);
// 
// 		if (buf)
// 			free(buf);
// 	}
// 	return uni;
// }
// 
// CStringA ConvertUnicodeToUTF8(wchar_t* pUnicode)
// {
// 	CStringA utf8("");
// 	int len = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, NULL, 0, 0, 0);
// 	if (len>1)
// 	{ 
// 		char* pUTF8 = (char *)calloc(len+1,sizeof(char));
// 		if (pUTF8) 
// 			WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, pUTF8, len, 0, 0);
// 
// 		utf8 = CStringA(pUTF8);
// 
// 		if (pUTF8)
// 			free(pUTF8);
// 	}
// 	return utf8;
// }

CInforStructJSON::CInforStructJSON(void) :
m_bUpdate(FALSE),
m_curNode(NULL),
m_bCheckParser(false),
m_strDoc("")
{
}


CInforStructJSON::~CInforStructJSON(void)
{
	m_bCheckParser = false;
}


CString CInforStructJSON::GetJsonDoc()
{
	m_strDoc.Delete(m_strDoc.GetLength()-1);
	CStringA strTmp = "{";
	strTmp += m_strDoc; 
	strTmp += "}";

	return ConvertUTF8ToUnicode(strTmp.GetBuffer());
}

CStringA CInforStructJSON::GetJsonDocA()
{
	m_strDoc.Delete(m_strDoc.GetLength() - 1);
	CStringA strTmp = "{";
	strTmp += m_strDoc;
	strTmp += "}";

	return strTmp;
}

bool CInforStructJSON::SetJSONDoc(CString strDoc)
{
	m_strDoc = ConvertUnicodeToUTF8(strDoc.GetBuffer(strDoc.GetLength()));

 	//CString sLog(m_strDoc);
 	//LOGNULL(sLog);

	m_bCheckParser = jsonParser.CheckParser(m_strDoc);	
	return m_bCheckParser;
}

bool CInforStructJSON::SetJSONDocA(CStringA strDocA)
{
	m_strDoc = strDocA;
	
	m_bCheckParser = jsonParser.CheckParser(m_strDoc);
	return m_bCheckParser;
}


//////////////////////////////////////////////////////////////////////////
// Set value
void CInforStructJSON::SetValue(CStringA strFieldName,int value)
{		
	CStringA strTmp,strValue;
	strValue.Format("%d",value);
	strTmp.Format("\"%s\":%s,",strFieldName,strValue);
	m_strDoc+= strTmp;
}
void CInforStructJSON::SetValue(CStringA strFieldName,long value)
{	
	CStringA strTmp,strValue;
	strValue.Format("%d",value);
	strTmp.Format("\"%s\":%s,",strFieldName,strValue);
	m_strDoc+= strTmp;
}
void CInforStructJSON::SetValue(CStringA strFieldName,DWORD value)
{	
	CStringA strTmp,strValue;
	strValue.Format("%lu",value);
	strTmp.Format("\"%s\":%s,",strFieldName,strValue);
	m_strDoc+= strTmp;
}
void CInforStructJSON::SetValue(CStringA strFieldName,double value)
{
	CStringA strTmp,strValue;
	strValue.Format("%f",value);
	strTmp.Format("\"%s\":%s,",strFieldName,strValue);
	m_strDoc+= strTmp;
}

void CInforStructJSON::SetValue(CStringA strFieldName,CString value)
{
	CStringA sValue = CHTMLUtils::EscapeJson(value);
	
	CStringA strTmp;
	strTmp.Format("\"%s\":\"%s\",",strFieldName,sValue);
	m_strDoc+= strTmp;
}

void CInforStructJSON::SetValue(CStringA strFieldName,COleDateTime value)
{
	CStringA strTmp,strValue;
	strValue.Format("%f",value.m_dt);
	strTmp.Format("\"%s\":\"%s\",",strFieldName,strValue);
	m_strDoc+= strTmp;
}

void CInforStructJSON::SetValue(CStringA strFieldName,DWORDLONG value) 
{
	CStringA strTmp,strValue;
	strValue.Format("%I64u",value);
	strTmp.Format("\"%s\":\"%s\",",strFieldName,strValue);
	m_strDoc+= strTmp;
}

void CInforStructJSON::SetValue(CStringA strFieldName,string value)
{
	CStringA sValue = CHTMLUtils::EscapeJson(CString(value.c_str()));

	CStringA strTmp;
	strTmp.Format("\"%s\":\"%s\",",strFieldName,sValue);
	m_strDoc+= strTmp;
}



//////////////////////////////////////////////////////////////////////////
void CInforStructJSON::GetValue(CStringA strFieldName,int &value,Json::Value  * json_node)
{
// 	if (json_node == NULL)
// 	{
		Json::Value json_value;
		if (FindValue(strFieldName,json_value,json_node))
		{
			if (!json_value.isObject() && !json_value.isArray())
			{		
				value = json_value.asInt();
			}
//			return;
		}		
// 	}
// 	
// 	if (!json_node->isObject() && !json_node->isArray())
// 	{		
// 		Json::Value json_value;
// 		if (FindValue(strFieldName,json_value,json_node))
// 		{
// 			if (!json_value.isObject() && !json_value.isArray())
// 			{		
// 				value = json_value.asInt();
// 			}
// 			return;
// 		}
// 
// 		value = json_node->asInt();
// 	}
}
void CInforStructJSON::GetValue(CStringA strFieldName,long &value,Json::Value  * json_node)
{
// 	if (json_node == NULL)
// 	{
		Json::Value json_value;
		if (FindValue(strFieldName,json_value,json_node))
		{
			if (!json_value.isObject() && !json_value.isArray())
			{		
				value = json_value.asInt64();
			}
//			return;
		}		
// 	}
// 
// 	if (!json_node->isObject() && !json_node->isArray())
// 	{		
// 		value = json_node->asInt64();
// 	}
}
void CInforStructJSON::GetValue(CStringA strFieldName,DWORD &value,Json::Value  * json_node)
{
// 	if (json_node == NULL)
// 	{
		Json::Value json_value;
		if (FindValue(strFieldName,json_value,json_node))
		{
			if (!json_value.isObject() && !json_value.isArray())
			{		
				value = json_value.asUInt64();
			}
//			return;
		}
// 	}
// 
// 	if (!json_node->isObject() && !json_node->isArray())
// 	{		
// 		value = json_node->asUInt64();
// 	}
}
void CInforStructJSON::GetValue(CStringA strFieldName,double &value,Json::Value  * json_node)
{
// 	if (json_node == NULL)
// 	{
		Json::Value json_value;
		if (FindValue(strFieldName,json_value,json_node))
		{
			if (!json_value.isObject() && !json_value.isArray())
			{		
				value = json_value.asDouble();
			}
//			return;
		}
// 	}
// 
// 	if (!json_node->isObject() && !json_node->isArray())
// 	{		
// 		value = json_node->asDouble();
// 	}
}
void CInforStructJSON::GetValue(CStringA strFieldName,CString &value,Json::Value  * json_node)
{
	Json::Value json_value;
	if (FindValue(strFieldName, json_value, json_node))
	{
		if (!json_value.isObject() && !json_value.isArray())
		{
			value = CHTMLUtils::UnEscapeJson(std::string(json_value.asCString()));
		}
	}
}
void CInforStructJSON::GetValue(CStringA strFieldName,COleDateTime &value,Json::Value  * json_node)
{
	/*
	if (json_node == NULL)
	{
		Json::Value json_value;
		if (FindValue(strFieldName,json_value,json_node))
		{
			if (!json_value.isObject() && !json_value.isArray())
			{		
				value =  _ttof(json_value.asCString());//json_value.asFloat();
			}
			return;
		}
	}

	if (!json_node->isObject() && !json_node->isArray())
	{		
		value.m_dt = _ttof(json_node->asCString());
	}*/
}

void CInforStructJSON::GetValue(CStringA strFieldName,DWORDLONG &value,Json::Value  * json_node)
{
	// 	if (json_node == NULL)
	// 	{
	Json::Value json_value;
	if (FindValue(strFieldName,json_value,json_node))
	{
		if (!json_value.isObject() && !json_value.isArray())
		{		
			value = json_value.asLargestUInt();
		}
		//			return;
	}
	// 	}
	// 
	// 	if (!json_node->isObject() && !json_node->isArray())
	// 	{		
	// 		value = json_node->asUInt64();
	// 	}
}

void CInforStructJSON::GetValue(CStringA strFieldName,string &value,Json::Value  * json_node)
{
	Json::Value json_value;
	if (FindValue(strFieldName,json_value,json_node))
	{
		if (!json_value.isObject() && !json_value.isArray())
		{
			CString sDecode = CHTMLUtils::UnEscapeJson(std::string(json_value.asCString()));
			value = string(ConvertUnicodeToUTF8(sDecode.GetBuffer()));
		}
	}
}