
// UDP_Internet_ClientDlg.h : ͷ�ļ�
//

#pragma once


// CUDP_Internet_ClientDlg �Ի���
class CUDP_Internet_ClientDlg : public CDialogEx
{
// ����
public:
	CUDP_Internet_ClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UDP_INTERNET_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
