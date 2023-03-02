#include "stdafx.h"
#include "HTMLUtils.h"
#include <sstream>
#include <iomanip>


CHTMLUtils::CHTMLUtils(void)
{
}


CHTMLUtils::~CHTMLUtils(void)
{
}

// CString CHTMLUtils::Decode(const CString& strInput)
// {
// 	CString htmlCode[][2] =
// 	{
// 		{ _T("&lt;"), _T("<") },
// 		{ _T("&gt;"),_T(">") },
// 		{ _T("&frasl;"),_T("/") },		
// 		{ _T("&quot;"),_T("\"") },
// 		{ _T("&lt"), _T("<") },
// 		{ _T("&gt"),_T(">") },
// 		{ _T("&frasl"),_T("/") },		
// 		{ _T("&quot"),_T("\"") },
// 		{ _T("\\\\"),_T("\\") }
// 	};
// 
// 	CString strOut = strInput;
// 	for (int i=0;i < 9;i++)
// 	{
// 		strOut.Replace(htmlCode[i][0],htmlCode[i][1]);
// 	}
// 	return strOut;
// }
// 
// CString CHTMLUtils::Encode(const CString& strHTML)
// {
// 	CString htmlCode[][2] =
// 	{
// 		{ _T("&lt;"), _T("<") },
// 		{ _T("&gt;"),_T(">") },
// 		{ _T("&frasl;"),_T("/") },		
// 		{ _T("&quot;"),_T("\"") },
// 		{ _T("\\\\"),_T("\\") }
// 	};
// 	CString strOut = strHTML;
// 	for (int i=0;i < 5;i++)
// 	{
// 		strOut.Replace(htmlCode[i][1],htmlCode[i][0]);
// 	}
// 	return strOut;
// }

CStringA CHTMLUtils::EscapeJson(const CString &s) 
{
	CString str = s;
	CStringA sUtf8Value = ConvertUnicodeToUTF8(str.GetBuffer(str.GetLength()));
	return escape_json(sUtf8Value.GetBuffer()).c_str();
}

CString CHTMLUtils::UnEscapeJson(const std::string &s)
{
	std::string sUtf8Value = s;// unescapeJSON(s);
	USES_CONVERSION;
	CString sUnicodeValue = ConvertUTF8ToUnicode(CStringA(sUtf8Value.c_str()).GetBuffer());
	return sUnicodeValue;
}

std::string CHTMLUtils::escape_json(const std::string &s) 
{
	std::ostringstream o;
	for (std::string::const_iterator c = s.begin(); c != s.end(); c++) {
		if (*c == '"' || *c == '\\' || ('\x00' <= *c && *c <= '\x1f')) {
			o << "\\u"
				<< std::hex << std::setw(4) << std::setfill('0') << (int)*c;
		}
		else {
			o << *c;
		}
	}
	return o.str();
}