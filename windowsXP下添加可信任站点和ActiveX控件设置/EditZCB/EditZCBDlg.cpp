// EditZCBDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EditZCB.h"
#include "EditZCBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CEditZCBDlg �Ի���



CEditZCBDlg::CEditZCBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditZCBDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEditZCBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ip);
}

BEGIN_MESSAGE_MAP(CEditZCBDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CEditZCBDlg ��Ϣ�������

BOOL CEditZCBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO���ڴ���Ӷ���ĳ�ʼ������
	this->SetWindowText("��ӿ�����վ��");
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CEditZCBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEditZCBDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CEditZCBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEditZCBDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString buf_ip;
	m_ip.GetWindowText(buf_ip);
	if(50 <  strlen(buf_ip)) 
	{
		AfxMessageBox("�������ַ��ʽ����ȷ");
		return ;
	}
	char buf[100];
	memset(buf,0,100);
	memcpy(buf,buf_ip,strlen(buf_ip));
	/*if(0 != memcmp(buf,"http",4))
	{
		AfxMessageBox("�������ַ��ʽ����ȷ");
		return ;
	}*/
	int i=0;
	int count=0;
	while(buf[i])//�ҳ��������ַ��.�ĸ��������ж��ú��ַ�ʽ�޸�ע���
	{
		char mem_c='.';
		if(memcmp(&buf[i],&mem_c,1)==0) count++;
		i++;
	}
	char buf_item1[100];//�����������
	memset(buf_item1,0,100);
	char buf_item2[100];//�����������
	memset(buf_item2,0,100);
	char buf_value[100];//ֵ������
	memset(buf_value,0,100);
	if(2 == count)
	{
		if(3 != sscanf(buf,"%4s://%3s.%s",buf_value,buf_item1,buf_item2))
		{
			AfxMessageBox("�������ַ��ʽ����ȷ");
			return ;
		}
		HKEY Hkey;
		char itemname[256];
		memset(itemname,0,256);
		sprintf(itemname,"%s\\%s","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Domains",buf_item2);
		if( ERROR_SUCCESS != RegCreateKey(HKEY_CURRENT_USER,itemname,&Hkey))
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"��������%sʧ��",itemname);
			AfxMessageBox(errorbuf);
			return ;
		}
		RegCloseKey(Hkey);
		memset(itemname,0,256);
		sprintf(itemname,"%s\\%s\\%s","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Domains",buf_item2,buf_item1);
		if( ERROR_SUCCESS != RegCreateKey(HKEY_CURRENT_USER,itemname,&Hkey))
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"��������%sʧ��",itemname);
			AfxMessageBox(errorbuf);
			return ;
		}
		//RegCloseKey(Hkey);
		DWORD valuedata=2;
		if( ERROR_SUCCESS != RegSetValueEx(Hkey,buf_value,0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"����ֵ��%sʧ��",buf_value);
			AfxMessageBox(errorbuf);
			return ;
		}
		RegCloseKey(Hkey);
	}
	else if(3 == count)
	{
		if(2 != sscanf(buf,"%4s://%s",buf_value,buf_item1))
		{
			AfxMessageBox("�������ַ��ʽ����ȷ");
			return ;
		}
		HKEY Hkey;
		if( ERROR_SUCCESS != RegCreateKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Ranges\\Range100",&Hkey))
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"��������%sʧ��","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\ZoneMap\\Ranges\\Range100");
			AfxMessageBox(errorbuf);
			return ;
		}
		DWORD valuedata=2;
		if( ERROR_SUCCESS != RegSetValueEx(Hkey,buf_value,0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"����ֵ��%sʧ��",buf_value);
			AfxMessageBox(errorbuf);
			return ;
		}
		if( ERROR_SUCCESS != RegSetValueEx(Hkey,":Range",0,REG_SZ,(BYTE *)buf_item1,strlen(buf_item1)))
		{
			char errorbuf[256];
			memset(errorbuf,0,256);
			sprintf(errorbuf,"����ֵ��%sʧ��",buf_item1);
			AfxMessageBox(errorbuf);
			return ;
		}
		RegCloseKey(Hkey);
	}
	else
	{
		AfxMessageBox("�������ַ��ʽ����ȷ");
		return ;
	}

	HKEY Hkey;
	if( ERROR_SUCCESS != RegOpenKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\2",&Hkey))
	{
		char errorbuf[256];
		memset(errorbuf,0,256);
		sprintf(errorbuf,"������%sʧ��","Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\Zones\\2");
		AfxMessageBox(errorbuf);
		return ;
	}
	DWORD valuedata=1;
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1001",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("����ActiveX�ؼ�ʧ��");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1004",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("����ActiveX�ؼ�ʧ��");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1200",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("����ActiveX�ؼ�ʧ��");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1201",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("����ActiveX�ؼ�ʧ��");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"1405",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("����ActiveX�ؼ�ʧ��");
		return ;
	}
	if( ERROR_SUCCESS != RegSetValueEx(Hkey,"2201",0,REG_DWORD,(BYTE *)&valuedata,sizeof(DWORD)) )
	{
		AfxMessageBox("����ActiveX�ؼ�ʧ��");
		return ;
	}
	RegCloseKey(Hkey);
	AfxMessageBox("��ӳɹ�");
	return ;
}
