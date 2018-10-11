/*
* Copyright (c) 2003,GDC
* All rights reserved.
*
* �ļ����ƣ�ICCardCtl.C
* �ļ���ʶ�������ù���ƻ���
* ժҪ����IC��OCX�ؼ�����ʵ���ļ�
*
* ��ǰ�汾��0.1 beta
* ���ߣ�����ƽ
* ������ڣ�2003��06��10��
*/
#include "stdafx.h"
#include "ICCard.h"
#include "ICCardCtl.h"
#include "ICCardPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ���ö�̬���ӿ���,���ڹ����ڴ��
#pragma data_seg("mydata") 
static HINSTANCE DLLInst = NULL; //��̬����
#pragma data_seg()
// ��̬���ӿ�������������
typedef  int (__stdcall READ_HOST_S)(int ComPort, char* buf);
READ_HOST_S* Read_Host_S;

IMPLEMENT_DYNCREATE(CICCardCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CICCardCtrl, COleControl)
	//{{AFX_MSG_MAP(CICCardCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CICCardCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CICCardCtrl)
	DISP_PROPERTY_NOTIFY(CICCardCtrl, "ComPort", m_comPort, OnComPortChanged, VT_I2)
	DISP_FUNCTION(CICCardCtrl, "ReadICCard", ReadICCard, VT_BSTR, VTS_NONE)
	DISP_STOCKPROP_ENABLED()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CICCardCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CICCardCtrl, COleControl)
	//{{AFX_EVENT_MAP(CICCardCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

BEGIN_PROPPAGEIDS(CICCardCtrl, 1)
	PROPPAGEID(CICCardPropPage::guid)
END_PROPPAGEIDS(CICCardCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CICCardCtrl, "ICCARD.ICCardCtrl.1",
	0x7ae7497b, 0xcad8, 0x4e66, 0xa5, 0x8b, 0xdd, 0xe9, 0xbc, 0xaf, 0x6b, 0x61)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CICCardCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DICCard =
		{ 0x7ee8b159, 0x980f, 0x4380, { 0x84, 0x5a, 0x28, 0xd7, 0xd, 0x1b, 0xf7, 0x5c } };
const IID BASED_CODE IID_DICCardEvents =
		{ 0xcef07ba8, 0x3493, 0x47f6, { 0x8e, 0x24, 0x24, 0xda, 0x79, 0x14, 0xa0, 0x84 } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwICCardOleMisc =
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CICCardCtrl, IDS_ICCARD, _dwICCardOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::CICCardCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CICCardCtrl

BOOL CICCardCtrl::CICCardCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ICCARD,
			IDB_ICCARD,
			afxRegApartmentThreading,
			_dwICCardOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::CICCardCtrl - Constructor

CICCardCtrl::CICCardCtrl()
{
	InitializeIIDs(&IID_DICCard, &IID_DICCardEvents);  
	// ���ÿؼ��ߴ�
	SetInitialSize(24, 22);
	// ���ڶ˿ںţ�Ĭ��Ϊ0x01
	//m_comPort = DEFAULT_COM_PORT;
	// ���ض�̬���ӿ�
	LoadDll();
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::~CICCardCtrl - Destructor

CICCardCtrl::~CICCardCtrl()
{
    // ж��DLL
	if (DLLInst!=NULL)
		FreeLibrary(DLLInst);
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::OnDraw - Drawing function

void CICCardCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
	pdc->TextOut(rcBounds.left+6,rcBounds.top+4, "IC");
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::DoPropExchange - Persistence support

void CICCardCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
	// �־�����
	//PX_Short(pPX, _T("ComPort"), (short)m_comPort, (short)DEFAULT_COM_PORT);
}


/////////////////////////////////////////////////////////////////////////////
// CICCardCtrl::OnResetState - Reset control to default state

void CICCardCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/*****************************************************************
* ��������: ���ù��ڶԻ�����
* �������: ��
* �������: ��
* �� �� ֵ: ��
*****************************************************************/
void CICCardCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_ICCARD);
	dlgAbout.DoModal();
}

/*****************************************************************
* ��������: ����������Enabled�ı�ʱ����
* �������: ��
* �������: ��
* �� �� ֵ: ��
*****************************************************************/
void CICCardCtrl::OnEnabledChanged() 
{
	COleControl::OnEnabledChanged();
}

/*****************************************************************
* ��������: ������SetInitialSize()ʱ����
* �������: ��
* �������: ��
* �� �� ֵ: ��
*****************************************************************/
void CICCardCtrl::OnSetClientSite() 
{
	// ����OnCreate�ĵ���
	RecreateControlWindow();	
	COleControl::OnSetClientSite();
}

/*****************************************************************
* ��������: �ഴ������
* �������: LPCREATESTRUCT lpCreateStruct - ���������ṹ
* �������: ��
* �� �� ֵ: int                           - �ɹ�����0,����-1
*****************************************************************/
int CICCardCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ���ض˿ں�
	CWinApp* pApp = AfxGetApp();
    m_comPort = pApp->GetProfileInt("ComPort", "Port", DEFAULT_COM_PORT);

	return 0;
}

/*****************************************************************
* ��������: �����ٺ���
* �������: ��
* �������: ��
* �� �� ֵ: ��
*****************************************************************/
void CICCardCtrl::OnDestroy() 
{
	// ����˿ں�
	CWinApp* pApp = AfxGetApp();
    pApp->WriteProfileInt("ComPort", "Port", m_comPort);
	
	COleControl::OnDestroy();
}

/*****************************************************************
* ��������: ��IC������, ������һ����Ϣ��������Ϣ���������¼�
* �������: ��
* �������: ��
* �� �� ֵ: �����Ĵ�
*****************************************************************/
BSTR CICCardCtrl::ReadICCard() 
{
	CString strResult;
	LPTSTR p;
	p = strResult.GetBuffer(1120);
    int nRet = Read_Host_S(m_comPort, p);
	strResult.ReleaseBuffer();
	// ����ɹ�
	if (nRet==0)
	{
		return strResult.AllocSysString();
	}
	else
	{
		return NULL;
	}
}

//===================�Զ��岿��=================================//

/*****************************************************************
* ��������: ����DLL�ļ�����������
* �������: ��
* �������: ��
* �� �� ֵ: BOOL
*****************************************************************/
BOOL CICCardCtrl::LoadDll()
{
    HRSRC hResInfo;
	HGLOBAL hRes;
	HINSTANCE hInst;
    // ����DLL��Դ 
	hInst = AfxGetInstanceHandle();
	hResInfo = FindResource(hInst, (LPCTSTR)IDR_DLL1,"DLL" );
    // ������Դ
	hRes = LoadResource(hInst,hResInfo );
	// ����DLL�ļ���С
	DWORD dFileLength = SizeofResource(hInst, hResInfo );
	// ��ȡϵͳ·��
	char pPath[MAX_PATH];
    GetSystemDirectory(pPath, MAX_PATH);
    // �����ļ���
	strcpy(g_pFile, pPath);
	strcat(g_pFile, "\\HGIC.Dll");
    // ����DLL�ļ�
	HANDLE hFile = CreateFile(g_pFile, GENERIC_WRITE | GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, 
                            FILE_ATTRIBUTE_NORMAL,NULL);
    DWORD dwNumberOfBytesWritten = 0;
	// дDLL�ļ�
    if(hFile > 0)
        WriteFile(hFile, (LPCVOID)hRes, dFileLength, &dwNumberOfBytesWritten,   NULL);
    else
	    MessageBox("����Res���ɹ�","��ʾ��Ϣ",MB_OK);
	// �ͷ��ļ����
    CloseHandle(hFile);

    // ��̬����DLL
	DLLInst = LoadLibrary(g_pFile);
	// ������سɹ����ҳ�������ַ
	if (DLLInst!=NULL)
	{
	    Read_Host_S = (READ_HOST_S*)GetProcAddress(DLLInst, "Read_Host_S");
	}
	else
	{
		::MessageBox(NULL,"���ض�̬��ʧ�ܣ�", "��ʾ��Ϣ", MB_OK | MB_ICONINFORMATION);
        return false;
	}
    return true; 
}

void CICCardCtrl::OnComPortChanged() 
{
	// TODO: Add notification handler code

	SetModifiedFlag();
}
