#pragma once
class AFX_EXT_CLASS CParameter
{
public:
	CParameter(void);
	~CParameter(void);

	void SetProperty( CString strProperty );
	CString GetProperty();

	void SetValue(  CString strValue );
	CString GetValue();

private:
	CString m_strProperty;
	CString m_strValue;
};

