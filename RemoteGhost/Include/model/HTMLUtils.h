#pragma once

class CHTMLUtils
{
public:	
	//static CString Encode(const CString& strHTML);
	//static CString Decode(const CString& strInput);
	CHTMLUtils(void);
	~CHTMLUtils(void);

	static CStringA EscapeJson(const CString &s);
	static CString UnEscapeJson(const std::string &s);

	static std::string escape_json(const std::string& input);
	//static std::string unescapeJSON(const std::string& input);

};

