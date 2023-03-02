#pragma once

class CriticalSection
{
public:
	CriticalSection()
	{ ::InitializeCriticalSection(&m_rep); }
	~CriticalSection()
	{ ::DeleteCriticalSection(&m_rep); }

	void Enter()
	{ 
		::EnterCriticalSection(&m_rep); 
	}
	void Leave()
	{
		try
		{
			::LeaveCriticalSection(&m_rep); 
		}
		catch (...)
		{
			//CBkavLog::WriteLog(_T("LeaveCriticalSection failed!!!\r\n"));
		}		
	}

private:
	// copy ops are private to prevent copying
	CriticalSection(const CriticalSection&);
	CriticalSection& operator=(const CriticalSection&);

	CRITICAL_SECTION m_rep;
};

class CSLock
{
public:
	explicit CSLock(CriticalSection& a_section)
		: m_section(a_section) { m_section.Enter(); }	
	~CSLock()
	{ m_section.Leave(); }
	
private:
	// copy ops are private to prevent copying
	CSLock(const CSLock&);
	CSLock& operator=(const CSLock&);

	CriticalSection& m_section;
};



template <typename T> class property {
public:
	T value;
	bool IsObject;
	bool IsJson;
	bool IsHTML;
	bool IsUpdated;
	bool IsPrimaryKey;
	CStringA ColName;
	
public:
	property()
	{
		IsHTML = false;
		IsUpdated = false;
		IsPrimaryKey = false;
		IsObject = false;
		IsJson = false;
		ColName =_T("");
	}
	T & operator= (const T &i) 
	{	
		IsUpdated = true;
		return value = i;

		/*value = i;
		T& retValue = value;
		return retValue;*/
	}

	T get()
	{
		return value;
	}

	T* getPointer()
	{
		return &value;
	}

// 	template <typename T2> 
// 	T & operator = (const T2 &i) 
// 	{		
// 		IsUpdated = true;
// 		return value = (T)i;
// 	}
	
	operator T& () {
		return (T&)value;
		//T& retValue = value;
		//return (T&)retValue;
	}

	template <typename T2> operator T()
	{
		//T& retValue = value;
		//return (T2)retValue;
		return (T2)value;
	}


};

/*
template <typename T> class propertyid: public property<T> {
public:
	propertyid()
	{
		IsUpdated = false;
		IsPrimaryKey = true;
		IsHTML = false;
		IsObject = false;
		IsJson = false;
	}
	propertyid(const T &i)
	{
		IsUpdated = false;
		IsPrimaryKey = true;
		IsHTML = false;
		IsObject = false;
		value = i;
	}	
};


template <typename T> class propertyhtml: public property<T> {
public:	
	propertyhtml()
	{
		IsUpdated = false;
		IsPrimaryKey = false;
		IsHTML = true;
		IsObject = false;
		IsJson = false;
	}
	T & operator = (const T &i) {	
		IsUpdated = true;
		return value = i;
	}

	template <typename T2> T & operator = (const property<T2> &i) 
	{		
		IsUpdated = true;
		return value = (T)i.value;
	}

	template <typename T2> T & operator = (const T2 &i) 
	{		
		IsUpdated = true;
		return value = (T)i;
	}
	operator T& () {
		return value;
	}

	template <typename T2> operator T()
	{
		return (T2)value;
	}
};

template <typename T> class propertyobject: public property<T> {
public:	
	propertyobject()
	{
		IsUpdated = false;
		IsPrimaryKey = false;
		IsHTML = false;
		IsObject = true;
		IsJson = false;
	}
	T & operator = (const T &i) {	
		IsUpdated = true;
		return value = i;
	}

	template <typename T2> T & operator = (const T2 &i) 
	{		
		IsUpdated = true;
		return value = (T)i;
	}
	operator T& () {
		return value;
	}

	template <typename T2> operator T()
	{
		return (T2)value;
	}
};

template <typename T> class propertyjson: public property<T> {
public:	
	propertyjson()
	{
		IsUpdated = false;
		IsPrimaryKey = false;
		IsHTML = false;
		IsObject = false;
		IsJson = true;
	}
	T & operator = (const T &i) {	
		IsUpdated = true;
		return value = i;
	}

	template <typename T2> T & operator = (const T2 &i) 
	{		
		IsUpdated = true;
		return value = (T)i;
	}
	operator T& () {
		return value;
	}

	template <typename T2> operator T()
	{
		return (T2)value;
	}
};

*/

#define ORM_COL(name)                              \
	make_col(#name, name)

template<class T> inline
	property<T>* make_col(const char * name, property<T> &t)
{
	t.ColName  = name;
	return &t;
}

/*
template<class T> inline
	property<T>* make_col(const char * name, propertyid<T> &t)
{
	t.ColName  = name;
	return (property<T>*)&t;
}
*/