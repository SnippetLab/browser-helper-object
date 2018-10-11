// ICCard.cpp : Implementation of CICCardApp and DLL registration.

#include "stdafx.h"
#include "ICCard.h"
#include "comcat.h"
#include "Objsafe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CICCardApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x9d14d2ca, 0xa3db, 0x4f72, { 0xbf, 0xce, 0x61, 0x36, 0xad, 0x89, 0x5e, 0xeb } };

// ���ؼ���CLSID,ע�����
const GUID CDECL CLSID_SafeItem =
		{ 0x7AE7497B, 0xCAD8, 0x4E66, { 0xA5,0x8B,0xDD,0xE9,0xBC,0xAF,0x6B,0x61 } };
// ���汾��gggggggmmmmm
const WORD _wVerMajor = 1;
// �ΰ汾��lkkkk
const WORD _wVerMinor = 0;

////////////////////////////////////////////////////////////////////////////
// CICCardApp::InitInstance - DLL initialization

BOOL CICCardApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CICCardApp::ExitInstance - DLL termination

int CICCardApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////

// �����������
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
{
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
            NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (FAILED(hr))
        return hr;

    // Make sure the HKCR\Component Categories\{..catid...}
    // key is registered.
    CATEGORYINFO catinfo;
    catinfo.catid = catid;
    catinfo.lcid = 0x0409 ; // english

    // Make sure the provided description is not too long.
    // Only copy the first 127 characters if it is.
    int len = wcslen(catDescription);
    if (len>127)
        len = 127;
    wcsncpy(catinfo.szDescription, catDescription, len);
    // Make sure the description is null terminated.
    catinfo.szDescription[len] = '\0';

    hr = pcr->RegisterCategories(1, &catinfo);
        pcr->Release();

    return hr;
}

// ע���������
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    // Register your component categories information.
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
                NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
       // Register this category as being "implemented" by the class.
       CATID rgcatid[1] ;
       rgcatid[0] = catid;
       hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
    }
    if (pcr != NULL)
        pcr->Release();
    return hr;
}
// ж���������
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;

    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
            NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
       // Unregister this category as being "implemented" by the class.
       CATID rgcatid[1] ;
       rgcatid[0] = catid;
       hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
    }

    if (pcr != NULL)
        pcr->Release();

    return hr;
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
	HRESULT hr;

	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

    // ��ǿؼ���ʼ����ȫ.
    // ������ʼ����ȫ�������
    hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data!");
    if (FAILED(hr))
        return hr;
    // ע���ʼ����ȫ
    hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
    if (FAILED(hr))
        return hr;

    // ��ǿؼ��ű���ȫ
    // �����ű���ȫ������� 
    hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls safely scriptable!");
    if (FAILED(hr))
        return hr;
    // ע��ű���ȫ�������
    hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
    if (FAILED(hr))
        return hr;

	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	HRESULT hr;

	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

    // ɾ���ؼ���ʼ����ȫ���.
    hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
    if (FAILED(hr))
        return hr;
    // ɾ���ؼ��ű���ȫ���
    hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
    if (FAILED(hr))
        return hr;

//////////////////////////
	return NOERROR;
}
