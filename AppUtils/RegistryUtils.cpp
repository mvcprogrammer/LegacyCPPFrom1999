#include "StdAfx.h"
#include "RegistryUtils.h"


CRegistryUtils::CRegistryUtils( CString strServiceName )
{
	m_sServiceName = strServiceName;
	m_bProfileWriteFlush = TRUE;
}

CRegistryUtils::~CRegistryUtils()
{
}

BOOL CRegistryUtils::WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
{
  return WriteServiceProfileString(m_sServiceName, lpszSection, lpszEntry, lpszValue, m_bProfileWriteFlush);
}

BOOL CRegistryUtils::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
{
  return WriteServiceProfileInt(m_sServiceName, lpszSection, lpszEntry, nValue, m_bProfileWriteFlush);
}

BOOL CRegistryUtils::WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes)
{
  return WriteServiceProfileBinary(m_sServiceName, lpszSection, lpszEntry, pData, nBytes, m_bProfileWriteFlush);
}

CString CRegistryUtils::GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault, DWORD* pLastError)
{
  return GetServiceProfileString(m_sServiceName, lpszSection, lpszEntry, lpszDefault, pLastError);
}

UINT CRegistryUtils::GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault, DWORD* pLastError)
{
  return GetServiceProfileInt(m_sServiceName, lpszSection, lpszEntry, nDefault, pLastError);
}

BOOL CRegistryUtils::GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes)
{
  return GetServiceProfileBinary(m_sServiceName, lpszSection, lpszEntry, ppData, pBytes);
}

BOOL CRegistryUtils::WriteServiceProfileString(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue, BOOL bFlush, DWORD* pLastError)
{
  ASSERT(lpszService != NULL);
  ASSERT(lpszSection != NULL);

  if (pLastError)
    *pLastError = ERROR_SUCCESS;

	LONG lResult;
	if (lpszEntry == NULL) //delete whole section
	{
		HKEY hAppKey = GetServiceRegistryKey(lpszService, FALSE, pLastError);
		if (hAppKey == NULL)
			return FALSE;
		lResult = ::RegDeleteKey(hAppKey, lpszSection);
    if ((lResult != ERROR_SUCCESS) && pLastError)
      *pLastError = lResult;
		if (bFlush)
		  RegFlushKey(hAppKey);
		RegCloseKey(hAppKey);
	}
	else if (lpszValue == NULL)
	{
		HKEY hSecKey = GetSectionKey(lpszService, lpszSection, FALSE, pLastError);
		if (hSecKey == NULL)
			return FALSE;
		// necessary to cast away const below
		lResult = ::RegDeleteValue(hSecKey, lpszEntry);
    if ((lResult != ERROR_SUCCESS) && pLastError)
      *pLastError = lResult;
		if (bFlush)
		  RegFlushKey(hSecKey);
		RegCloseKey(hSecKey);
	}
	else
	{
		HKEY hSecKey = GetSectionKey(lpszService, lpszSection, FALSE, pLastError);
		if (hSecKey == NULL)
			return FALSE;
		lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_SZ, reinterpret_cast<const BYTE*>(lpszValue), (lstrlen(lpszValue)+1)*sizeof(TCHAR));
    if ((lResult != ERROR_SUCCESS) && pLastError)
      *pLastError = lResult;
		if (bFlush)
		  RegFlushKey(hSecKey);
		RegCloseKey(hSecKey);
	}
	return lResult == ERROR_SUCCESS;
}

BOOL CRegistryUtils::WriteServiceProfileInt(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue, BOOL bFlush, DWORD* pLastError)
{
  ASSERT(lpszService != NULL);
  ASSERT(lpszSection != NULL);

  if (pLastError)
    *pLastError = ERROR_SUCCESS;

	HKEY hSecKey = GetSectionKey(lpszService, lpszSection, FALSE, pLastError);
	if (hSecKey == NULL)
		return FALSE;
	LONG lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_DWORD, reinterpret_cast<const BYTE*>(&nValue), sizeof(nValue));
  if ((lResult != ERROR_SUCCESS) && pLastError)
    *pLastError = lResult;
	if (bFlush)
		RegFlushKey(hSecKey);
	RegCloseKey(hSecKey);
	return lResult == ERROR_SUCCESS;
}

BOOL CRegistryUtils::WriteServiceProfileBinary(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE pData, UINT nBytes, BOOL bFlush, DWORD* pLastError)
{
	ASSERT(lpszSection != NULL);
  ASSERT(lpszService != NULL);

  if (pLastError)
    *pLastError = ERROR_SUCCESS;

	HKEY hSecKey = GetSectionKey(lpszService, lpszSection, FALSE, pLastError);
	if (hSecKey == NULL)
		return FALSE;
	LONG lResult = RegSetValueEx(hSecKey, lpszEntry, NULL, REG_BINARY, pData, nBytes);
  if ((lResult != ERROR_SUCCESS) && pLastError)
    *pLastError = lResult;
	if (bFlush)
		RegFlushKey(hSecKey);
	RegCloseKey(hSecKey);
	return lResult == ERROR_SUCCESS;
}

CString CRegistryUtils::GetServiceProfileString(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault, DWORD* pLastError)
{
  ASSERT(lpszService != NULL);
  ASSERT(lpszSection != NULL);

  if (pLastError)
    *pLastError = ERROR_SUCCESS;

	HKEY hSecKey = GetSectionKey(lpszService, lpszSection, TRUE, pLastError);
	if (hSecKey == NULL)
		return lpszDefault;
	CString strValue;
	DWORD dwType, dwCount;
	LONG lResult = RegQueryValueEx(hSecKey, lpszEntry, NULL, &dwType,	NULL, &dwCount);
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		lResult = RegQueryValueEx(hSecKey, lpszEntry, NULL, &dwType, reinterpret_cast<LPBYTE>(strValue.GetBuffer(dwCount/sizeof(TCHAR))), &dwCount);
		strValue.ReleaseBuffer();
	}
	RegCloseKey(hSecKey);

  if ((lResult != ERROR_SUCCESS) && pLastError)
    *pLastError = lResult;

	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_SZ);
		return strValue;
	}

	return lpszDefault;
}

UINT CRegistryUtils::GetServiceProfileInt(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault, DWORD* pLastError)
{
  ASSERT(lpszService != NULL);
  ASSERT(lpszSection != NULL);

  if (pLastError)
    *pLastError = ERROR_SUCCESS;

	HKEY hSecKey = GetSectionKey(lpszService, lpszSection, TRUE, pLastError);
	if (hSecKey == NULL)
		return nDefault;
	DWORD dwValue;
	DWORD dwType;
	DWORD dwCount = sizeof(DWORD);
	LONG lResult = RegQueryValueEx(hSecKey, lpszEntry, NULL, &dwType,	reinterpret_cast<LPBYTE>(&dwValue), &dwCount);
	RegCloseKey(hSecKey);

  if ((lResult != ERROR_SUCCESS) && pLastError)
    *pLastError = lResult;

	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_DWORD);
		ASSERT(dwCount == sizeof(dwValue));
		return (UINT)dwValue;
	}

	return nDefault;
}

BOOL CRegistryUtils::GetServiceProfileBinary(LPCTSTR lpszService, LPCTSTR lpszSection, LPCTSTR lpszEntry, LPBYTE* ppData, UINT* pBytes, DWORD* pLastError)
{
  //Validate our parameters
  ASSERT(lpszService != NULL);
	ASSERT(lpszSection != NULL);
	ASSERT(lpszEntry != NULL);
	ASSERT(ppData != NULL);
	ASSERT(pBytes != NULL);

  if (pLastError)
    *pLastError = ERROR_SUCCESS;

	*ppData = NULL;
	*pBytes = 0;
	HKEY hSecKey = GetSectionKey(lpszService, lpszSection, TRUE, pLastError);
	if (hSecKey == NULL)
		return FALSE;

	DWORD dwType, dwCount;
	LONG lResult = RegQueryValueEx(hSecKey, lpszEntry, NULL, &dwType,	NULL, &dwCount);
	*pBytes = dwCount;
	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_BINARY);
		*ppData = new BYTE[*pBytes];
		lResult = RegQueryValueEx(hSecKey, lpszEntry, NULL, &dwType, *ppData, &dwCount);
    if ((lResult != ERROR_SUCCESS) && pLastError)
      *pLastError = lResult;
	}
  else
  {
    if (pLastError)
      *pLastError = lResult;
  }
	RegCloseKey(hSecKey);

	if (lResult == ERROR_SUCCESS)
	{
		ASSERT(dwType == REG_BINARY);
		return TRUE;
	}
	else
	{
		delete [] *ppData;
		*ppData = NULL;
	}

	return FALSE;
}

// returns key for:
//      HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\ServiceName\Parameters\lpszSection.
// creating it if it doesn't exist.
// responsibility of the caller to call RegCloseKey() on the returned HKEY
HKEY CRegistryUtils::GetSectionKey(LPCTSTR lpszServiceName, LPCTSTR lpszSection, BOOL bReadOnly, DWORD* pLastError)
{
	ASSERT(lpszServiceName != NULL);
	ASSERT(lpszSection != NULL);

	if (pLastError)
		*pLastError = ERROR_SUCCESS;

	HKEY hSectionKey = NULL;
	HKEY hServiceKey = GetServiceRegistryKey(lpszServiceName, bReadOnly, pLastError);
	if (hServiceKey == NULL)
		return NULL;

	DWORD dw = 0;
	LONG lResult = RegCreateKeyEx(hServiceKey, lpszSection, 0, REG_NONE,	REG_OPTION_NON_VOLATILE, bReadOnly ? KEY_READ : KEY_WRITE | KEY_READ, NULL, &hSectionKey, &dw);
	if ((lResult != ERROR_SUCCESS) && pLastError)
		*pLastError = lResult;

	RegCloseKey(hServiceKey);
	return hSectionKey;
}

// returns key for:
//      HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\ServiceName\Parameters
// creating it if it doesn't exist
// responsibility of the caller to call RegCloseKey() on the returned HKEY
HKEY CRegistryUtils::GetServiceRegistryKey(LPCTSTR lpszServiceName, BOOL bReadOnly, DWORD* pLastError)
{
  ASSERT(lpszServiceName != NULL);

  if (pLastError)
    *pLastError = ERROR_SUCCESS;

	HKEY hServicesKey = NULL;
	HKEY hParametersKey = NULL;
	HKEY hServiceKey = NULL;
  LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services"), 0, bReadOnly ? KEY_READ : KEY_WRITE | KEY_READ, &hServicesKey);
	if (lResult == ERROR_SUCCESS)
	{
		DWORD dw = 0;
    lResult = RegCreateKeyEx(hServicesKey, lpszServiceName, 0, REG_NONE,	REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hServiceKey, &dw);
		if (lResult == ERROR_SUCCESS)
		{
			lResult = RegCreateKeyEx(hServiceKey, _T("Parameters"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,	&hParametersKey, &dw);
      if ((lResult != ERROR_SUCCESS) && pLastError)
        *pLastError = lResult;
		}
    else
    {
      if (pLastError)
        *pLastError = lResult;
    }
	}
  else
  {
    if (pLastError)
      *pLastError = lResult;
  }

  //Close down the unused registry keys before returning
	if (hServicesKey != NULL)
		RegCloseKey(hServicesKey);
	if (hServiceKey != NULL)
		RegCloseKey(hServiceKey);

	return hParametersKey;
}