
// UDP_Internet_ClientDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CUDP_Internet_ClientDlg 对话框



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


// CUDP_Internet_ClientDlg 消息处理程序

BOOL CUDP_Internet_ClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//编辑框内添加预设值，未启用按钮设置不可用
	SetDlgItemText(IDC_IP, _T("172.16.1.137"));
	SetDlgItemText(IDC_PORT, _T("8000"));
	SetDlgItemText(IDC_NAME, _T("gw10"));
	SetDlgItemText(IDC_PW, _T("123"));
	this->GetDlgItem(IDC_BUTTONDISCONNECT)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONGET)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONSHOW)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONPUNCHING)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTONSEND)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUDP_Internet_ClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUDP_Internet_ClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUDP_Internet_ClientDlg::OnBnClickedconnect()
{
	// TODO:  在此添加控件通知处理程序代码
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
	str = "等待服务器验证用户...";
	this->SetDlgItemText(IDC_Showstatus, str);

	Connect_proc();

	this->GetDlgItem(IDC_BUTTONCONNECT)->EnableWindow(FALSE);
	SetTimer(TIMER_COUNT1, 300, NULL);
}


void CUDP_Internet_ClientDlg::OnBnClickedGet()
{
	// 获取服务器端在线数据列表
	CString str;
	str = "获取服务器端在线数据...";
	this->SetDlgItemText(IDC_Showstatus, str);
	GetUser_Online();

	GetDlgItem(IDC_BUTTONGET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONSHOW)->EnableWindow(TRUE);
}


void CUDP_Internet_ClientDlg::OnBnClickedShow()
{
	// TODO:  在此添加控件通知处理程序代码
	//Show_Online(this);
	GetDlgItem(IDC_BUTTONSHOW)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTONGET)->EnableWindow(TRUE);
}


void CUDP_Internet_ClientDlg::OnBnClickedPunching()
{
	// TODO:  在此添加控件通知处理程序代码
}
