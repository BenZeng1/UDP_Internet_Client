
// UDP_Internet_ClientDlg.h : 头文件
//

#pragma once


// CUDP_Internet_ClientDlg 对话框
class CUDP_Internet_ClientDlg : public CDialogEx
{
// 构造
public:
	CUDP_Internet_ClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UDP_INTERNET_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedconnect();
	afx_msg void OnBnClickedGet();
	afx_msg void OnBnClickedShow();
	afx_msg void OnBnClickedPunching();
	afx_msg void OnBnClickeDisconnect();
	afx_msg void OnBnClickedSend();
};
