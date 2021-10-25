# pragma once
#include "pch.h"
#include "framework.h"
#include "ImagesProcessing.h"
#include "ImagesProcessingDlg.h"
#include "afxdialogex.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно CImagesProcessingDlg



CImagesProcessingDlg::CImagesProcessingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGESPROCESSING_DIALOG, pParent)
	, IMAGE_WIDTH(600)
	, IMAGE_HEIGHT(600)
	, NOISE_PERCENT(15)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImagesProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IMAGE_WIDTH, IMAGE_WIDTH);
	DDX_Text(pDX, IDC_EDIT_IMAGE_HEIGHT, IMAGE_HEIGHT);
	DDX_Control(pDX, IDC_RADIO_IMAGE_IS_MODEL, RADIO_IS_MODEL);
	DDX_Control(pDX, IDC_RADIO_IMAGE_IS_PICTURE, RADIO_IS_PICTURE);
	DDX_Text(pDX, IDC_EDIT_NOISE_LEVEL, NOISE_PERCENT);
}

BEGIN_MESSAGE_MAP(CImagesProcessingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DRAW_INPUT, &CImagesProcessingDlg::OnBnClickedButtonDrawInput)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_INPUT, &CImagesProcessingDlg::OnBnClickedButtonClearInput)
	ON_BN_CLICKED(IDC_BUTTON_DRAW_INPUT_WITH_NOISE, &CImagesProcessingDlg::OnBnClickedButtonDrawInputWithNoise)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_INPUT_WITH_NOISE, &CImagesProcessingDlg::OnBnClickedButtonClearInputWithNoise)
END_MESSAGE_MAP()


// Обработчики сообщений CImagesProcessingDlg

BOOL CImagesProcessingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	RADIO_IS_MODEL.SetCheck(TRUE);
	RADIO_IS_PICTURE.SetCheck(FALSE);

	PicWnd = GetDlgItem(IDC_INPUT_IMAGE);
	PicDc = PicWnd->GetDC();
	PicWnd->GetClientRect(&Pic);

	NoiseWnd = GetDlgItem(IDC_INPUT_IMAGE_WITH_NOISE);
	NoiseDc = NoiseWnd->GetDC();
	NoiseWnd->GetClientRect(&Noise);

	// Инициализируем объект изображения.
	DrawSettings dsetting_in(PicWnd, PicDc, Pic, IMAGE_WIDTH, IMAGE_HEIGHT);
	DrawSettings dsetting_noise(NoiseWnd, NoiseDc, Noise, IMAGE_WIDTH, IMAGE_HEIGHT);
	image = new Image(RADIO_IS_MODEL, IMAGE_WIDTH, IMAGE_HEIGHT, dsetting_in, dsetting_noise);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CImagesProcessingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CImagesProcessingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CImagesProcessingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImagesProcessingDlg::OnBnClickedButtonDrawInput()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	image->update_image_condition(RADIO_IS_MODEL, IMAGE_WIDTH, IMAGE_HEIGHT);
	image->get_input_image();
	image->draw_image(image->input_image, image->ds_input);
}


void CImagesProcessingDlg::OnBnClickedButtonClearInput()
{
	// TODO: добавьте свой код обработчика уведомлений
	image->input_image = image->clear_selected_image(image->input_image);
	image->draw_image(image->input_image, image->ds_input);
}


void CImagesProcessingDlg::OnBnClickedButtonDrawInputWithNoise()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	image->update_image_condition(RADIO_IS_MODEL, IMAGE_WIDTH, IMAGE_HEIGHT);
	image->get_noise_image(NOISE_PERCENT);
	image->draw_image(image->noise_image, image->ds_noise);
}


void CImagesProcessingDlg::OnBnClickedButtonClearInputWithNoise()
{
	// TODO: добавьте свой код обработчика уведомлений
	image->noise_image = image->clear_selected_image(image->noise_image);
	image->draw_image(image->noise_image, image->ds_noise);
}
