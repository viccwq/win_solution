
// bunnyDlg.h : header file
//

#pragma once
#include "comm_def.h"
#include "subfun_cplus.h"
#include "img_proc.h"


// CbunnyDlg dialog
class CbunnyDlg : public CDialogEx
{
// Construction
public:
	CbunnyDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BUNNY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void DrawPicToHDC(cv::Mat img_src, UINT ID);
	afx_msg void OnBnClickedButtonOpen();
	CString m_file_get;
	// source image
	cv::Mat image_src;
	// //destinate image
	cv::Mat image_dst;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnDestroy();
private:
	int Mat2CImage(Mat *mat, CImage &img);
public:
	afx_msg void OnBnClickedButtonSave();
	CString m_file_dst;
	CString m_file_src;
	afx_msg void OnClickedButtonProc();
};
