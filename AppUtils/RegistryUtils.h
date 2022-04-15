#pragma once
#include "utils.h"

class AFX_EXT_CLASS CRegistryUtils : public CUtils
{
public:
	CRegistryUtils( CString strServiceName );
	virtual ~CRegistryUtils(void);

public:
	//Persistance support
	//Allows saving and restoring of a services settings to the 
	//"HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\ServiceName\Parameters"
	//location in the registry
	virtual BOOL    WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	virtual BOOL    WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
	virtual BOOL    WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes);

	virtual CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL, DWORD* pLastError = NULL);
	virtual UINT    GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault, DWORD* pLastError = NULL);
	virtual BOOL    GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes);

	//These versions of the persistence functions allow control over the "ServiceName" to write to as described above as well as control over the bFlush value 
	static BOOL     WriteServiceProfileString(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue, BOOL bFlush, DWORD* pLastError = NULL);
	static BOOL     WriteServiceProfileInt(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue, BOOL bFlush, DWORD* pLastError = NULL);
	static BOOL     WriteServiceProfileStringArray(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, const CStringArray& array, BOOL bFlush, DWORD* pLastError = NULL);
	static BOOL     WriteServiceProfileBinary(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes, BOOL bFlush, DWORD* pLastError = NULL);
	static CString  GetServiceProfileString(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL, DWORD* pLastError = NULL);
	static UINT     GetServiceProfileInt(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault, DWORD* pLastError = NULL);
	static BOOL     GetServiceProfileBinary(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes, DWORD* pLastError = NULL);

	//Used internally by the persistance functions
	static HKEY GetSectionKey(LPCTSTR lpszServiceName, LPCTSTR lpszSection, BOOL bReadOnly = FALSE, DWORD* pLastError = NULL);
	static HKEY GetServiceRegistryKey(LPCTSTR lpszServiceName, BOOL bReadOnly = FALSE, DWORD* pLastError = NULL);

private:
	 BOOL m_bProfileWriteFlush;      //Should calls to WriteProfile... be synchronous or left to be written by the lazy writer
	 CString m_sServiceName;
};

