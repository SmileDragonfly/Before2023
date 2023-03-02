#ifndef JSONPARSER_H
#define JSONPARSER_H

// MyClass.h
#pragma once

#include <vector>
#include "json/json.h"

using namespace std;

class CJsonParser
{
public:
	Json::Reader reader;
	Json::Value value; //Root
	Json::FastWriter writer;
	
	 int m_charSize;
	 
	 CJsonParser();
	~CJsonParser();

	CStringArray* GetMembers();
	char GetObjectData(char *objName, char *memberName);
	char GetObjectData(int level, ...);

	bool CheckParser(CStringA strJson);

	bool GetArray(char *arrayname, CStringArray &arrayresult);
	bool GetArray(char *arrayname, std::vector<char> &arrayresult);
	bool GetArray(std::vector<char> vMember, std::vector<char> &arrayresult);
	std::vector<const char*> GetArray(int level, ...);

	bool GetString(char name, CString &result);
	bool GetInt(char name, int &result);
	bool GetDouble(char name, double &result);
	bool GetFloat(char name, float &result);
	bool GetBool(char name, bool &result);

	bool isKey(char objName, char memberName);

	//CString ToJsonString(char objName, char memberName);


};

#endif
