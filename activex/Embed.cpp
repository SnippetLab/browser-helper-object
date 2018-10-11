// Embed.cpp : Implementation of CEmbed

#include "stdafx.h"
#include "Activex.h"
#include "Embed.h"


#import "C:\WINDOWS\system32\Macromed\Flash\Flash10b.ocx" \
	no_namespace raw_interfaces_only raw_native_types \
	named_guids exclude("IServiceProvider")
#import "C:\\WINDOWS\\system32\\wmp.dll" no_namespace \
	raw_interfaces_only raw_native_types named_guids

/////////////////////////////////////////////////////////////////////////////
// CEmbed

// CEmbed
CEmbed::CEmbed()
{
	m_bWindowOnly = TRUE;
	nElapse = 0;
}

LRESULT CEmbed::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	RECT rc;
	GetWindowRect(&rc);
	rc.right -= rc.left;
	rc.bottom -= rc.top;
	rc.top = rc.left = 0;
	//m_ctlStatic.Create(m_hWnd, rc);	
	
	//����ATL3.0 �������AtlAxWinInit������Create�����ʧ��
	AtlAxWinInit(); //��ʼ���������ڵ�ע����

	RECT rect = rc;
	//����Shock wave flash�ؼ�����Flash
	TCHAR* controlName = L"ShockwaveFlash.ShockwaveFlash";
	m_FlashWin.Create(m_hWnd, rect, controlName, WS_CHILD | WS_VISIBLE, 0, ID_FLASHCTRL );
	DWORD  error = ::GetLastError();
	CComPtr< IAxWinAmbientDispatch > ambient;
	m_FlashWin.QueryHost( &ambient );
	
	CComPtr<IAxWinHostWindow> hostwin;
	m_FlashWin.QueryHost(&hostwin);
	hostwin->QueryControl(__uuidof(IDispatch),(void**)&m_ptrFlash.p);
	CComQIPtr<IShockwaveFlash> ptrFlash = m_ptrFlash;
	ptrFlash->put_BackgroundColor(RGB( 0, 0, 0 ) );

	//����Windows Media Play�ؼ�����.wmv�ļ�
	controlName = L"WMPlayer.OCX";
	//CLSID cld;
	//::CLSIDFromProgID(controlName,&cld);
	m_WmvWin.Create(m_hWnd, rect,controlName,WS_CHILD, 0,ID_WMPCTRL);
	
	if(hostwin)
		hostwin.Release();
	m_WmvWin.QueryHost(&hostwin);
	hostwin->QueryControl(__uuidof(IDispatch),(void**)&m_ptrWmv.p);	

/*
	//for ATL3.0���������������
	//������������	
	HWND hWndCont = m_FlashWin.Create(m_hWnd, rect, 0, WS_CHILD | WS_VISIBLE);
	//Create & activate ActiveX control
	HRESULT hr = m_FlashWin.CreateControl(L"ShockwaveFlash.ShockwaveFlash");
	CComPtr<IAxWinHostWindow> hostwin;
	m_FlashWin.QueryHost(&hostwin);
	hostwin->QueryControl(__uuidof(IDispatch),(void**)&m_ptrFlash.p);
	CComQIPtr<IShockwaveFlash> ptrFlash = m_ptrFlash;
	ptrFlash->put_BackgroundColor(RGB( 0, 0, 0 ) );

	//������������
	hWndCont = m_WmvWin.Create(m_hWnd, rect, 0, WS_CHILD);
	//Create & activate ActiveX control
	hr = m_WmvWin.CreateControl(L"WMPlayer.OCX");
	if(hostwin)
		hostwin.Release();
	m_WmvWin.QueryHost(&hostwin);
	hostwin->QueryControl(__uuidof(IDispatch),(void**)&m_ptrWmv.p);	
*/
	return 0;
}

HRESULT CEmbed::SetObjectRects(LPCRECT prcPos,LPCRECT prcClip)
{
	IOleInPlaceObjectWindowlessImpl<CEmbed>::SetObjectRects(prcPos, prcClip);
	int cx, cy;
	cx = prcPos->right - prcPos->left;
	cy = prcPos->bottom - prcPos->top;

	RECT rect;
	GetClientRect( &rect );
	rect.top = 0;
	rect.left = 0;
	if(m_FlashWin)
		m_FlashWin.MoveWindow(&rect );
	if(m_FlashWin)
		m_WmvWin.MoveWindow(&rect );
	return S_OK;
}

LRESULT CEmbed::OnTimer(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL & bHandled)
{
	//��ʼ����.WMV��ʽ����Ƶ����
	if(m_FlashWin.m_hWnd != NULL && m_WmvWin.m_hWnd != NULL)
	{
		m_FlashWin.ShowWindow(SW_HIDE);
		m_WmvWin.ShowWindow(SW_SHOW);
	}
	if(m_ptrWmv)
	{

		//��ʼ����.wmv��Ƶ
		CComQIPtr<IWMPPlayer> ptrPlay = m_ptrWmv;		
		ptrPlay->put_URL(m_strWmv);
		IWMPControls* pctrl = NULL;

		ptrPlay->get_controls(&pctrl);
		ptrPlay->put_enableContextMenu(false);
		pctrl->play();
		pctrl->Release();
	}
	//�رն�ʱ��
	KillTimer(wparam);
    return 0;
}

STDMETHODIMP CEmbed::LoadFlashMovie(BSTR bstr)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strFlash = bstr;
	return S_OK;
}

STDMETHODIMP CEmbed::LoadWmvMovie(BSTR bstr)
{
	// TODO: �ڴ����ʵ�ִ���
	m_strWmv = bstr;
	return S_OK;
}

STDMETHODIMP CEmbed::SetJmpTimer(LONG tm)
{
	// TODO: �ڴ����ʵ�ִ���
	nElapse = tm;
	return S_OK;
}

STDMETHODIMP CEmbed::Play(void)
{
	// TODO: �ڴ����ʵ�ִ���
	if(m_FlashWin.m_hWnd != NULL && m_WmvWin.m_hWnd != NULL)
	{
		m_WmvWin.ShowWindow(SW_HIDE);
		m_FlashWin.ShowWindow(SW_SHOW);		
	}

	ATLASSERT(m_ptrFlash);
	if(m_ptrFlash)
	{
		//��ʼ����Flash
		IShockwaveFlash* pFalsh = NULL;
		m_ptrFlash->QueryInterface(__uuidof(IShockwaveFlash),(void**)&pFalsh);
		pFalsh->LoadMovie(0,m_strFlash);
		pFalsh->Play();
		pFalsh->Release();
		if(nElapse > 0)
		{
			ATLASSERT(nElapse<1000);
			SetTimer(1,nElapse*1000);
		}
	}

	return S_OK;
}