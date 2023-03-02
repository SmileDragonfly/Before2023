#pragma message("COMPILING WIN32")
#include "stdafx.h"
#include "OSSpecific.h"
#include <process.h>

bool Windows32Specific::CreateMutex(MutexT* pMutex) {
	InitializeCriticalSection(pMutex);
	return (pMutex!=NULL);
}

void Windows32Specific::LockMutex(MutexT& mutex) {
	EnterCriticalSection(&mutex);
}

void Windows32Specific::UnlockMutex(MutexT& mutex) {
	LeaveCriticalSection(&mutex);
}

void Windows32Specific::DestroyMutex(MutexT& mutex) {
	DeleteCriticalSection(&mutex);
}

void Windows32Specific::BeginThread(ThreadFunctionT pFunction, void* arg, ThreadIdentifierT* pThreadId ) {
	_beginthreadex (NULL, 0,
		(unsigned int(__stdcall*) ( void*)) pFunction, 
		(void*) arg, 0, pThreadId);
}

void Windows32Specific::CreateCondition(ConditionT* pCondition) {
	*pCondition = CreateEvent(NULL,0,0,NULL);
}

void Windows32Specific::WaitForCondition(ConditionT& condition) {
	WaitForSingleObject(condition, INFINITE);
}

void Windows32Specific::SignalCondition(ConditionT& condition) {
	SetEvent(condition);
}

void Windows32Specific::DestroyCondition(ConditionT& condition) {
	CloseHandle(condition);
}