
// UDP_Internet_ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDP_Internet_Client.h"
#include "UDP_Internet_ClientDlg.h"
#include "afxdialogex.h"
#include "NetPunching.h"
#include <string>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUDP_Internet_ClientDlg �Ի���



CUDP_Internet_ClientDlg::CUDP_Internet_ClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDP_Internet_ClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUDP_Internet_ClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUDP_Internet_ClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONCONNECT, &CUDP_Internet_ClientDlg::OnBnClickedconnect)
	ON_BN_CLICKED(IDC_BUTTONGET, &CUDP_Internet_ClientDlg::OnBnClickedGet)
	ON_BN_CLICKED(IDC_BUTTONSHOW, &CUDP_Internet_ClientDlg::OnBnClickedShow)
	ON_BN_CLICKED(IDC_BUTTONPUNCHING, &CUDP_Internet_ClientDlg::OnBnClickedPunching)
END_MESSAGE_MAP()


// CUDP_Internet_ClientDlg ��Ϣ�������

BOOL CUDP_Internet_ClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//�༭�������Ԥ��ֵ��δ���ð�ť���ò�����
	SetDlgItemText(IDC_IP, _T("172.16.1.137"));
	SetDlgItemText(IDC_PORT, _T("8000"));
	SetDlgItemText(IDC_NAME, _T("gw10"));
	SetDlgItemText(IDC_PW, _T("123"));
	this->GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONGET)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONSHOW)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONPUNCHING)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONSEND)->EnableWindow(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CUDP_Internet_ClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUDP_Internet_ClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUDP_Internet_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDP_Internet_ClientDlg::OnBnClickedconnect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	this->GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(FALSE);

	if (SocketInit((LPVOID)this) == 0)
	{
		this->GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(TRUE);
		return;
	}

	char host_name[225];
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR)
	{

	}
	
	struct  hostent	*phe = gethostbyname(host_name);
	if (phe == 0)
	{

	}

	in_addr addr;
	memcpy(&addr, phe->h_addr_list[0], sizeof(in_addr));

	CString str1;
	CHAR *p = inet_ntoa(addr);
	str1 = p;
	this->SetDlgItemText(IDC_LOCALIP, str1);

	CreateThread(0, 0, MsgReceive, this, 0, NULL);
	Sleep(500);

	CString str;
	str = "�ȴ���������֤�û�...";
	this->SetDlgItemText(IDC_Showstatus, str);

	Connect_proc();

	this->GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(FALSE);
	SetTimer(TIMER_COUNT1, 300, NULL);
}


void CUDP_Internet_ClientDlg::OnBnClickedGet()
{
	// ��ȡ�����������������б�
	CString str;
	str = "��ȡ����������������...";
	this->SetDlgItemText(IDC_Showstatus, str);
	GetUser_Online();

	GetDlgItem(IDC_BUTTONGET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONSHOW)->EnableWindow(TRUE);
}


void CUDP_Internet_ClientDlg::OnBnClickedShow()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//Show_Online(this);
	GetDlgItem(IDC_BUTTONSHOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONGET)->EnableWindow(TRUE);
}


void CUDP_Internet_ClientDlg::OnBnClickedPunching()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
