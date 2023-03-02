#pragma once
#include "Log.h"

#define LOGINFO(msg) \
	if (CLog::Default) \
	CLog::Default->Log(NULL, CString(__FUNCTION__), msg); \

#define LOGNULL(msg) \
	if (CLog::Default) \
	CLog::Default->Log(NULL, CString(__FUNCTION__), msg); \

#define LOGERROR(msg) \
	if (CLog::Default) \
	CLog::Default->LogError(NULL, CString(__FUNCTION__), msg); \

#define LOGWARNING(msg) \
	if (CLog::Default) \
	CLog::Default->LogError(NULL, CString(__FUNCTION__), msg); \

#define LOGEXCEPTION(ex,msg) \
	if (CLog::Default) \
	CLog::Default->Log(NULL, CString(__FUNCTION__), ex ,msg); \

#define LOGINFO_FUNC(msg) \
	if (CLog::Default) \
	CLog::Default->Log(this, CString(__FUNCTION__), msg); \

#define LOGERROR_FUNC(msg) \
	if (CLog::Default) \
	CLog::Default->LogError(this, CString(__FUNCTION__), msg); \

#define LOGWARNING_FUNC(msg) \
	if (CLog::Default) \
	CLog::Default->LogError(this, CString(__FUNCTION__), msg); \

#define LOGEXCEPTION_FUNC(ex,msg) \
	if (CLog::Default) \
	CLog::Default->Log(this, CString(__FUNCTION__), ex ,msg);
