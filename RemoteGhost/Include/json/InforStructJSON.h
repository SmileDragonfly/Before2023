#ifndef MICROTEC_INFORSTRUCT_JSON_H_
#define MICROTEC_INFORSTRUCT_JSON_H_

#include <vector>

#include "model/BusinessLayer/DataObject.h"
#include "model/HTMLUtils.h"
#include "json/value.h"
#include "JsonParser.h"


class CInforStructJSON
{
public:
	
	//////////////////////////////////////////////////////////////////////////
	
	void SetValue(CStringA strFieldName,int value);
	void SetValue(CStringA strFieldName,long value);
	void SetValue(CStringA strFieldName,DWORD value);
	void SetValue(CStringA strFieldName,double value);
	void SetValue(CStringA strFieldName,CString value);
	void SetValue(CStringA strFieldName,COleDateTime value);
	void SetValue(CStringA strFieldName,DWORDLONG value); //DWORDLONG = unsigned long long
	void SetValue(CStringA strFieldName,string value);

	//////////////////////////////////////////////////////////////////////////
	
	void GetValue(CStringA strFieldName,int &value,Json::Value  * json_node = NULL);
	void GetValue(CStringA strFieldName,long &value,Json::Value  * json_node = NULL);
	void GetValue(CStringA strFieldName,DWORD &value,Json::Value  * json_node = NULL);
	void GetValue(CStringA strFieldName,double &value,Json::Value  * json_node = NULL);
	void GetValue(CStringA strFieldName,CString &value,Json::Value  * json_node = NULL);
	void GetValue(CStringA strFieldName,COleDateTime &value,Json::Value  * json_node = NULL);
	void GetValue(CStringA strFieldName,DWORDLONG &value,Json::Value  * json_node = NULL);
	void GetValue(CStringA strFieldName,string &value,Json::Value  * json_node = NULL);

	//////////////////////////////////////////////////////////////////////////

	template<typename T>
	CInforStructJSON& operator&(property<T> * col)
	{
		if (!m_bUpdate)
		{
			if (m_curNode->isObject())
			{
// 				Json::Value* test = &(*m_curNode)["Files"];
// 				if (test->isArray())
// 				{
// 					CString sLog;
// 					sLog.Format(_T("Test data Files - counter=%d "), (int)test->size());
// 					LOGNULL(sLog);
// 				}
// 
// 				Json::Value* test2 = &(*m_curNode)["Results"];
// 				if (test2->isArray())
// 				{
// 					CString sLog;
// 					sLog.Format(_T("Test data Results - counter=%d "), (int)test2->size());
// 					LOGNULL(sLog);
// 				}

				

//				GetValue(col->ColName,col->value,&(*m_curNode)[col->ColName]);
				GetValue(col->ColName,col->value,m_curNode);

				col->IsUpdated = true;
			}
		}
		else
		{
			if (col->IsUpdated)
			{
				SetValue(col->ColName,col->value);		
			}	
		}
			
		return *this;
	}
	
	/*
	CInforStructJSON& operator&(property<CString> * col)
	{
		if(!bUpdate)
		{
			if (m_curNode->isObject())
			{
				CString value;
				GetValue(col->ColName,value,&(*m_curNode)[col->ColName]);	
				col->value = CHTMLUtils::Decode(value);
				col->IsUpdated = true;
			}
		}
		else
		{
			if(col->IsUpdated)
			{		
				if (col->IsHTML)
				{
					SetValue(col->ColName,CHTMLUtils::Encode(col->value));
				}
				else
				{
					SetValue(col->ColName,col->value);
				}	

				// Encode all
				//SetValue(col->ColName,CHTMLUtils::Encode(col->value));
			}
		}
		
		return *this;
	}
	*/

	template<typename T> void SetValue(CStringA strClassName, T object)
	{	
		m_bUpdate = TRUE;
		CStringA strTmp="";
		if(!strClassName.IsEmpty())
		{
			strTmp.Format("\"%s\":{",strClassName);				
		}
		else
		{
			strTmp= "{";				
		}
		m_strDoc+= strTmp;
		object.orm(*this);
		m_strDoc.Delete(m_strDoc.GetLength()-1);		
		strTmp = "},";
		m_strDoc+= strTmp;
	}

	template<typename T> void SetValue(CStringA strListName,std::vector<T> lsObject)
	{		
		CStringA strTmp("");
		strTmp.Format("\"%s\":[",strListName);
		m_strDoc+= strTmp;
		for (int i=0;i<(int)lsObject.size();i++)
		{			
			SetValue("",lsObject.at(i));
		}		
		if (lsObject.size()>0)
		{
			m_strDoc.Delete(m_strDoc.GetLength()-1);
		}
		m_strDoc += "],";
	}

	//////////////////////////////////////////////////////////////////////////
	bool FindValue(CStringA strClassName,Json::Value &json_node,Json::Value * json_parentnode)
	{
		if (!m_bCheckParser)
		{
			return false;
		}
		m_bUpdate = FALSE;

		std::vector<std::string> members;
		if (json_parentnode!= NULL)
		{
			if (json_parentnode->isArray())
			{
				json_node = *json_parentnode;
				return true;
			}
			else if (json_parentnode->isObject())
			{
				members = json_parentnode->getMemberNames();

				int iMemCount = (int)members.size();
				if (iMemCount<1)
					return false;
				for (int i=0; i<iMemCount; i++)
				{
					CStringA sFieldName = CStringA(members.at(i).c_str(), (int)members.at(i).length());
					if (strClassName.CompareNoCase(sFieldName) == 0)
					{				
						json_node = (*json_parentnode)[sFieldName];						
						return true;
					}			
				}				
			}
		}
		else
		{
			// Only accept root is object
			if (!(jsonParser.value.isNull() || jsonParser.value.isObject()))
				return false;
			members = jsonParser.value.getMemberNames();
		
			int iMemCount = (int)members.size();
			if (iMemCount<1)
				return false;
			int i=0;
			bool bRet = false;
			for (i=0; i<iMemCount; i++)
			{
				CStringA sFieldName = CStringA(members.at(i).c_str(), (int)members.at(i).length());
				if (strClassName.CompareNoCase(sFieldName) == 0)
				{				
					json_node = jsonParser.value[sFieldName];			
					bRet = true;
					break;
				}			
			}
			return bRet;
		}

		return false;
	}

	template<typename T> void GetValue(CStringA strClassName, 
									   T &object,
									   Json::Value* json_node = NULL)
	{	
		Json::Value  curNode;
		if (FindValue(strClassName,curNode,json_node))
		{			
			if (curNode.isObject())
			{
				m_curNode = &curNode;
				object.orm(*this);
			}
			m_curNode = json_node;
		}
		
	}	
	
	template<typename T> bool GetValue(CStringA strListName,
									   std::vector<T> &lsObject,
									   Json::Value* json_node = NULL)
	{	
		m_bUpdate = FALSE;

		Json::Value arrNode;
		if (FindValue(strListName,arrNode,json_node))
		{
			if (arrNode.isArray())
			{
				for (int i=0;i<(int)arrNode.size();i++)
				{						
					m_curNode = &arrNode[i];
					T object;
					object.orm(*this);
					lsObject.push_back(object);	
				}
			}

			m_curNode = json_node;
			return true;
		}
		else
		{
			return false;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	CString GetJsonDoc();
	CStringA GetJsonDocA();

	bool SetJSONDoc(CString strDoc);
	bool SetJSONDocA(CStringA strDocA);

	CInforStructJSON(void);
	~CInforStructJSON(void);
	//////////////////////////////////////////////////////////////////////////

	CJsonParser jsonParser;
private:
	Json::Value  *m_curNode;
	bool m_bCheckParser;
	BOOL m_bUpdate;
	CStringA m_strDoc;
	
};

#endif // MICROTEC_INFORSTRUCT_JSON_H_