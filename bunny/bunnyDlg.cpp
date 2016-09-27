
// bunnyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "bunny.h"
#include "bunnyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CbunnyDlg dialog




CbunnyDlg::CbunnyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CbunnyDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_file_src = _T("");
	m_file_dst = _T("");
}

void CbunnyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRC, m_file_src);
	DDX_Text(pDX, IDC_TEXT_DST, m_file_dst);
}

BEGIN_MESSAGE_MAP(CbunnyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CbunnyDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_START, &CbunnyDlg::OnBnClickedButtonStart)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CbunnyDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CbunnyDlg message handlers

BOOL CbunnyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.
	cout<<"hello world"<<endl;
	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CbunnyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CbunnyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CbunnyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//http://blog.csdn.net/u014682691/article/details/51296219
void CbunnyDlg::DrawPicToHDC(cv::Mat img_src, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC= pDC->GetSafeHdc();

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	cv::Size winSize(rect.right, rect.bottom);

	// Resize the source to the size of the destination image if necessary
	cv::Mat img_temp;
	if (img_src.channels() == 1)
		img_temp.create(winSize, CV_8UC1);
	else
		img_temp.create(winSize, CV_8UC3);

	float ratio_h = img_temp.rows * 1.0 / img_src.rows;
	float ratio_w = img_temp.cols * 1.0 / img_src.cols;

	cv::Size size_2;
	if (ratio_h < ratio_w)
	{
		size_2.height = img_src.rows * ratio_h + 0.5;
		size_2.width = img_src.cols * ratio_h + 0.5;
	}
	else
	{
		size_2.height = img_src.rows * ratio_w + 0.5;
		size_2.width = img_src.cols * ratio_w + 0.5;
	}
	cv::Mat img_resize;
	if (img_src.channels() == 1)
		img_resize.create(size_2, CV_8UC1);
	else
		img_resize.create(size_2, CV_8UC3);

	cv::resize(img_src, img_resize, size_2);
	img_resize.copyTo(img_temp(cv::Rect(0, 0, size_2.width, size_2.height)));

	CImage img_dst;
	Mat2CImage(&img_temp, img_dst);


// 	img_dst.Draw(GetDlgItem(ID)->GetDC()->GetSafeHdc(), rect);
	img_dst.Draw(hDC, rect);
	ReleaseDC( pDC );
}


int CbunnyDlg::Mat2CImage(Mat *mat, CImage &img) {
	if (!mat || mat->empty())
		return -1;
	int nBPP = mat->channels() * 8;
	img.Create(mat->cols, mat->rows, nBPP);
	if (nBPP == 8)
	{
		static RGBQUAD pRGB[256];
		for (int i = 0; i < 256; i++)
			pRGB[i].rgbBlue = pRGB[i].rgbGreen = pRGB[i].rgbRed = i;
		img.SetColorTable(0, 256, pRGB);
	}
	uchar* psrc = mat->data;
	uchar* pdst = (uchar*)img.GetBits();
	int imgPitch = img.GetPitch();
	for (int y = 0; y < mat->rows; y++)
	{
		memcpy(pdst, psrc, mat->cols*mat->channels());//mat->step is incorrect for those images created by roi (sub-images!)
		psrc += mat->step;
		pdst += imgPitch;
	}

	return 0;
}



void CbunnyDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	BOOL isOpen = TRUE;		//是否打开(否则为保存)
	CString defaultDir = ".\\";	//默认打开的文件路径
	CString fileName = "";			//默认打开的文件名
//	CString filter = L"文件 (*.doc; *.ppt; *.xls)|*.doc;*.ppt;*.xls||";	//文件过虑的类型
	CString filter = "文件 (*.jpg; *.bmp; *.jpeg; *.png)|*.jpg;*.bmp;*.jpeg;*png|所有文件|*.*||";	//文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY|OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = ".\\test.doc";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\test.doc";
	if(result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}
	CWnd::SetDlgItemTextA(IDC_EDIT_SRC, filePath);
	UpdateData(TRUE);
}


void CbunnyDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	std::string str = m_file_src.GetBuffer(0); 
	/************************************************************************/
/*
	image_src = cv::imread(str ,1); //显示图片	
	image_src.copyTo(image_dst);
	vic::salt(image_dst, 5000);
	vic::getHarris(image_src, image_dst);
*/
	/************************************************************************/
	getBunny(str.data(), image_src, image_dst);

	DrawPicToHDC(image_src, IDC_Image);
	DrawPicToHDC(image_dst, IDC_Image2);
}


void CbunnyDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}


void CbunnyDlg::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	m_file_dst = m_file_src + ".bmp";
	cv::imwrite(m_file_dst.GetBuffer(0), image_src);
}
