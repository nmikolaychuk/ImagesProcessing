# pragma once
#include "pch.h"
#include "framework.h"
#include "ImagesProcessing.h"
#include "ImagesProcessingDlg.h"
#include "afxdialogex.h"
#include <afxtoolbarimages.h>

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
	, IMAGE_WIDTH(1100)
	, IMAGE_HEIGHT(1100)
	, NOISE_PERCENT(10)
	, FILTRATION_PERCENT(90)
	, DIFFERENCE_VALUE(0)
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
	DDX_Control(pDX, IDC_RADIO_DEFAULT_SPEC_RATIO, SPEC_RATIO_DEFAULT);
	DDX_Control(pDX, IDC_RADIO_LOG2_SPEC_RATIO, SPEC_RATIO_LOG2);
	DDX_Control(pDX, IDC_RADIO_LOG10_SPEC_RATIO, SPEC_RATIO_LOG10);
	DDX_Control(pDX, IDC_RADIO_RECTANGLE_WINDOW, RECT_WINDOW);
	DDX_Control(pDX, IDC_RADIO_CIRCLE_WINDOW, CIRCLE_WINDOW);
	DDX_Text(pDX, IDC_EDIT_FILTER_LEVEL, FILTRATION_PERCENT);
	DDX_Text(pDX, IDC_EDIT_SKO_INPUT_AND_NOISE, DIFFERENCE_VALUE);
	DDX_Control(pDX, IDC_RADIO_ZEROS_ADDITION, ZEROS_ADDITION);
	DDX_Control(pDX, IDC_RADIO_INTERPOL_ADDITION, INTERPOLATION_ADDITION);
	DDX_Control(pDX, IDC_BUTTON_DRAW_INPUT, INPUT_IMAGE_DRAW);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_INPUT, INPUT_IMAGE_CLEAR);
	DDX_Control(pDX, IDC_BUTTON_DRAW_INPUT_WITH_NOISE, NOISE_IMAGE_DRAW);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_INPUT_WITH_NOISE, NOISE_IMAGE_CLEAR);
	DDX_Control(pDX, IDC_BUTTON_DRAW_SPECTRUM_IMAGE, SPECTRUM_IMAGE_DRAW);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_SPECTRUM_IMAGE, SPECTRUM_IMAGE_CLEAR);
	DDX_Control(pDX, IDC_BUTTON_DRAW_FILTERED, FILTERED_IMAGE_DRAW);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_FILTERED, FILTERED_IMAGE_CLEAR);
	DDX_Control(pDX, IDC_EDIT_NOISE_LEVEL, NOISE_PERCENT_BUTTON);
	DDX_Control(pDX, IDC_EDIT_FILTER_LEVEL, FILTER_PERCENT_BUTTON);
}

BEGIN_MESSAGE_MAP(CImagesProcessingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DRAW_INPUT, &CImagesProcessingDlg::OnBnClickedButtonDrawInput)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_INPUT, &CImagesProcessingDlg::OnBnClickedButtonClearInput)
	ON_BN_CLICKED(IDC_BUTTON_DRAW_INPUT_WITH_NOISE, &CImagesProcessingDlg::OnBnClickedButtonDrawInputWithNoise)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_INPUT_WITH_NOISE, &CImagesProcessingDlg::OnBnClickedButtonClearInputWithNoise)
	ON_BN_CLICKED(IDC_BUTTON_DRAW_SPECTRUM_IMAGE, &CImagesProcessingDlg::OnBnClickedButtonDrawSpectrumImage)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SPECTRUM_IMAGE, &CImagesProcessingDlg::OnBnClickedButtonClearSpectrumImage)
	ON_BN_CLICKED(IDC_RADIO_DEFAULT_SPEC_RATIO, &CImagesProcessingDlg::OnBnClickedRadioDefaultSpecRatio)
	ON_BN_CLICKED(IDC_RADIO_LOG2_SPEC_RATIO, &CImagesProcessingDlg::OnBnClickedRadioLog2SpecRatio)
	ON_BN_CLICKED(IDC_RADIO_LOG10_SPEC_RATIO, &CImagesProcessingDlg::OnBnClickedRadioLog10SpecRatio)
	ON_BN_CLICKED(IDC_RADIO_RECTANGLE_WINDOW, &CImagesProcessingDlg::OnBnClickedRadioRectangleWindow)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE_WINDOW, &CImagesProcessingDlg::OnBnClickedRadioCircleWindow)
	ON_BN_CLICKED(IDC_BUTTON_DRAW_FILTERED, &CImagesProcessingDlg::OnBnClickedButtonDrawFiltered)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_FILTERED, &CImagesProcessingDlg::OnBnClickedButtonClearFiltered)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL, &CImagesProcessingDlg::OnBnClickedButtonClearAll)
	ON_WM_GETMINMAXINFO()
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
	SPEC_RATIO_DEFAULT.SetCheck(TRUE);
	ZEROS_ADDITION.SetCheck(TRUE);

	NOISE_IMAGE_DRAW.EnableWindow(FALSE);
	NOISE_IMAGE_CLEAR.EnableWindow(FALSE);
	SPECTRUM_IMAGE_CLEAR.EnableWindow(FALSE);
	SPECTRUM_IMAGE_DRAW.EnableWindow(FALSE);
	SPEC_RATIO_DEFAULT.EnableWindow(FALSE);
	SPEC_RATIO_LOG2.EnableWindow(FALSE);
	SPEC_RATIO_LOG10.EnableWindow(FALSE);
	FILTERED_IMAGE_CLEAR.EnableWindow(FALSE);
	FILTERED_IMAGE_DRAW.EnableWindow(FALSE);
	RECT_WINDOW.EnableWindow(FALSE);
	CIRCLE_WINDOW.EnableWindow(FALSE);
	NOISE_PERCENT_BUTTON.EnableWindow(FALSE);
	FILTER_PERCENT_BUTTON.EnableWindow(FALSE);
	INPUT_IMAGE_CLEAR.EnableWindow(FALSE);

	PicWnd = GetDlgItem(IDC_INPUT_IMAGE);
	PicDc = PicWnd->GetDC();
	PicWnd->GetClientRect(&Pic);

	NoiseWnd = GetDlgItem(IDC_INPUT_IMAGE_WITH_NOISE);
	NoiseDc = NoiseWnd->GetDC();
	NoiseWnd->GetClientRect(&Noise);

	SpecWnd = GetDlgItem(IDC_SPECTRUM_IMAGE);
	SpecDc = SpecWnd->GetDC();
	SpecWnd->GetClientRect(&Spec);

	FiltWnd = GetDlgItem(IDC_FILTERED_IMAGE);
	FiltDc = FiltWnd->GetDC();
	FiltWnd->GetClientRect(&Filt);

	// Инициализируем объект изображения.
	DrawSettings dsetting_in(PicWnd, PicDc, Pic, IMAGE_WIDTH, IMAGE_HEIGHT,
							(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	DrawSettings dsetting_noise(NoiseWnd, NoiseDc, Noise, IMAGE_WIDTH, IMAGE_HEIGHT,
								(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	DrawSettings dsetting_spec(SpecWnd, SpecDc, Spec, IMAGE_WIDTH, IMAGE_HEIGHT,
								(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	DrawSettings dsetting_filtered(FiltWnd, FiltDc, Filt, IMAGE_WIDTH, IMAGE_HEIGHT,
								(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image = new Image(RADIO_IS_MODEL.GetCheck(), ZEROS_ADDITION.GetCheck(), IMAGE_WIDTH, IMAGE_HEIGHT, dsetting_in,
					  dsetting_noise, dsetting_spec, dsetting_filtered);

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

void CImagesProcessingDlg::OnUpdateWindowsSize()
{
	// Обновление информации о размерах окон.
	UpdateData(TRUE);
	PicWnd = GetDlgItem(IDC_INPUT_IMAGE);
	PicDc = PicWnd->GetDC();
	PicWnd->GetClientRect(&Pic);
	DrawSettings dsetting_inp(PicWnd, PicDc, Pic, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_input = dsetting_inp;

	NoiseWnd = GetDlgItem(IDC_INPUT_IMAGE_WITH_NOISE);
	NoiseDc = NoiseWnd->GetDC();
	NoiseWnd->GetClientRect(&Noise);
	DrawSettings dsetting_noise(NoiseWnd, NoiseDc, Noise, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_noise = dsetting_noise;

	SpecWnd = GetDlgItem(IDC_SPECTRUM_IMAGE);
	SpecDc = SpecWnd->GetDC();
	SpecWnd->GetClientRect(&Spec);
	DrawSettings dsetting_spec(SpecWnd, SpecDc, Spec, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_noise = dsetting_spec;

	FiltWnd = GetDlgItem(IDC_FILTERED_IMAGE);
	FiltDc = FiltWnd->GetDC();
	FiltWnd->GetClientRect(&Filt);
	DrawSettings dsetting_filt(FiltWnd, FiltDc, Filt, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_filtered = dsetting_filt;
}

void CImagesProcessingDlg::OnBnClickedButtonDrawInput()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	if (RADIO_IS_MODEL.GetCheck()) {
		image->get_input_image();
	}
	else if (RADIO_IS_PICTURE.GetCheck()) {
		OnBnClickedButtonLoadImage();
		
		IMAGE_WIDTH = image->input_image[0].size();
		IMAGE_HEIGHT = image->input_image.size();
		UpdateData(FALSE);
	}

	DrawSettings dsetting_inp(PicWnd, PicDc, Pic, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_input = dsetting_inp;
	OnUpdateWindowsSize();
	image->update_image_condition(RADIO_IS_MODEL.GetCheck(), ZEROS_ADDITION.GetCheck(), IMAGE_WIDTH, IMAGE_HEIGHT);
	image->draw_image(image->input_image, image->ds_input, false);

	INPUT_IMAGE_CLEAR.EnableWindow(TRUE);
	NOISE_IMAGE_DRAW.EnableWindow(TRUE);
	NOISE_PERCENT_BUTTON.EnableWindow(TRUE);
}


void CImagesProcessingDlg::OnBnClickedButtonClearInput()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	image->input_image = image->clear_selected_image(image->input_image);
	image->draw_image(image->input_image, image->ds_input, false);

	NOISE_IMAGE_DRAW.EnableWindow(FALSE);
	NOISE_IMAGE_CLEAR.EnableWindow(FALSE);
	SPECTRUM_IMAGE_CLEAR.EnableWindow(FALSE);
	SPECTRUM_IMAGE_DRAW.EnableWindow(FALSE);
	SPEC_RATIO_DEFAULT.EnableWindow(FALSE);
	SPEC_RATIO_LOG2.EnableWindow(FALSE);
	SPEC_RATIO_LOG10.EnableWindow(FALSE);
	FILTERED_IMAGE_CLEAR.EnableWindow(FALSE);
	FILTERED_IMAGE_DRAW.EnableWindow(FALSE);
	RECT_WINDOW.EnableWindow(FALSE);
	CIRCLE_WINDOW.EnableWindow(FALSE);
	NOISE_PERCENT_BUTTON.EnableWindow(FALSE);
	FILTER_PERCENT_BUTTON.EnableWindow(FALSE);
	INPUT_IMAGE_CLEAR.EnableWindow(FALSE);
}


void CImagesProcessingDlg::OnBnClickedButtonDrawInputWithNoise()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	OnUpdateWindowsSize();

	DrawSettings dsetting_noise(NoiseWnd, NoiseDc, Noise, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_noise = dsetting_noise;

	image->update_image_condition(RADIO_IS_MODEL.GetCheck(), ZEROS_ADDITION.GetCheck(), IMAGE_WIDTH, IMAGE_HEIGHT);
	image->get_noise_image(NOISE_PERCENT);
	image->draw_image(image->noise_image, image->ds_noise, false);

	NOISE_IMAGE_CLEAR.EnableWindow(TRUE);
	SPECTRUM_IMAGE_DRAW.EnableWindow(TRUE);
}


void CImagesProcessingDlg::OnBnClickedButtonClearInputWithNoise()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	image->noise_image = image->clear_selected_image(image->noise_image);
	image->draw_image(image->noise_image, image->ds_noise, false);

	NOISE_IMAGE_CLEAR.EnableWindow(FALSE);
	SPECTRUM_IMAGE_CLEAR.EnableWindow(FALSE);
	SPECTRUM_IMAGE_DRAW.EnableWindow(FALSE);
	SPEC_RATIO_DEFAULT.EnableWindow(FALSE);
	SPEC_RATIO_LOG2.EnableWindow(FALSE);
	SPEC_RATIO_LOG10.EnableWindow(FALSE);
	FILTERED_IMAGE_CLEAR.EnableWindow(FALSE);
	FILTERED_IMAGE_DRAW.EnableWindow(FALSE);
	RECT_WINDOW.EnableWindow(FALSE);
	CIRCLE_WINDOW.EnableWindow(FALSE);
	FILTER_PERCENT_BUTTON.EnableWindow(FALSE);
}


void CImagesProcessingDlg::OnBnClickedButtonDrawSpectrumImage()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	OnUpdateWindowsSize();

	DrawSettings dsetting_spec(SpecWnd, SpecDc, Spec, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_spec = dsetting_spec;

	image->update_image_condition(RADIO_IS_MODEL.GetCheck(), ZEROS_ADDITION.GetCheck(), IMAGE_WIDTH, IMAGE_HEIGHT);
	image->get_spectrum_image();
	if (SPEC_RATIO_DEFAULT.GetCheck() == BST_CHECKED) OnBnClickedRadioDefaultSpecRatio();
	else if (SPEC_RATIO_LOG2.GetCheck() == BST_CHECKED) OnBnClickedRadioLog2SpecRatio();
	else if (SPEC_RATIO_LOG10.GetCheck() == BST_CHECKED) OnBnClickedRadioLog10SpecRatio();

	SPECTRUM_IMAGE_CLEAR.EnableWindow(TRUE);
	SPEC_RATIO_DEFAULT.EnableWindow(TRUE);
	SPEC_RATIO_LOG2.EnableWindow(TRUE);
	SPEC_RATIO_LOG10.EnableWindow(TRUE);
	RECT_WINDOW.EnableWindow(TRUE);
	CIRCLE_WINDOW.EnableWindow(TRUE);
	FILTER_PERCENT_BUTTON.EnableWindow(TRUE);
}


void CImagesProcessingDlg::OnBnClickedButtonClearSpectrumImage()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	image->spectrum_image_calc = image->clear_selected_image(image->spectrum_image_calc);
	image->spectrum_image_vis = image->clear_selected_image(image->spectrum_image_vis);
	image->draw_image(image->spectrum_image_vis, image->ds_spec, false);

	SPECTRUM_IMAGE_CLEAR.EnableWindow(FALSE);
	SPEC_RATIO_DEFAULT.EnableWindow(FALSE);
	SPEC_RATIO_LOG2.EnableWindow(FALSE);
	SPEC_RATIO_LOG10.EnableWindow(FALSE);
	FILTERED_IMAGE_CLEAR.EnableWindow(FALSE);
	FILTERED_IMAGE_DRAW.EnableWindow(FALSE);
	RECT_WINDOW.EnableWindow(FALSE);
	CIRCLE_WINDOW.EnableWindow(FALSE);
	FILTER_PERCENT_BUTTON.EnableWindow(FALSE);
}


void CImagesProcessingDlg::OnBnClickedRadioDefaultSpecRatio()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!image->spectrum_image_vis.empty()) {
		UpdateData(TRUE);
		OnUpdateWindowsSize();

		DrawSettings dsetting_spec(SpecWnd, SpecDc, Spec, IMAGE_WIDTH, IMAGE_HEIGHT,
			(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
		image->ds_spec = dsetting_spec;

		image->draw_image(image->spectrum_image_vis, image->ds_spec, false);
	}
}


void CImagesProcessingDlg::OnBnClickedRadioLog2SpecRatio()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	if (!image->spectrum_image_vis.empty()) {
		UpdateData(TRUE);
		OnUpdateWindowsSize();

		DrawSettings dsetting_spec(SpecWnd, SpecDc, Spec, IMAGE_WIDTH, IMAGE_HEIGHT,
			(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
		image->ds_spec = dsetting_spec;

		vector<vector<Pixel>> spec_img_log2 = image->spectrum_image_vis;
		for (int i = 0; i < spec_img_log2.size(); i++) {
			for (int j = 0; j < spec_img_log2[i].size(); j++) {
				spec_img_log2[i][j].value.mod = log2(1+spec_img_log2[i][j].value.mod);
			}
		}

		spec_img_log2 = image->calculate_color_of_image(spec_img_log2, true);
		image->draw_image(spec_img_log2, image->ds_spec, false);
	}
}


void CImagesProcessingDlg::OnBnClickedRadioLog10SpecRatio()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!image->spectrum_image_vis.empty()) {
		UpdateData(TRUE);
		OnUpdateWindowsSize();

		DrawSettings dsetting_spec(SpecWnd, SpecDc, Spec, IMAGE_WIDTH, IMAGE_HEIGHT,
			(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
		image->ds_spec = dsetting_spec;

		vector<vector<Pixel>> spec_img_log10 = image->spectrum_image_vis;
		for (int i = 0; i < spec_img_log10.size(); i++) {
			for (int j = 0; j < spec_img_log10[i].size(); j++) {
				spec_img_log10[i][j].value.mod = log10(1+spec_img_log10[i][j].value.mod);
			}
		}

		spec_img_log10 = image->calculate_color_of_image(spec_img_log10, true);
		image->draw_image(spec_img_log10, image->ds_spec, false);
	}
}

void CImagesProcessingDlg::OnBnClickedRadioRectangleWindow()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	if (!image->spectrum_image_vis.empty()) {
		OnUpdateWindowsSize();
		DrawSettings dsetting_spec(SpecWnd, SpecDc, Spec, IMAGE_WIDTH, IMAGE_HEIGHT,
			(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
		image->ds_spec = dsetting_spec;

		image->draw_image(image->spectrum_image_vis, image->ds_spec, true);
	}

	FILTERED_IMAGE_DRAW.EnableWindow(TRUE);
}


void CImagesProcessingDlg::OnBnClickedRadioCircleWindow()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	if (!image->spectrum_image_vis.empty()) {
		OnUpdateWindowsSize();
		DrawSettings dsetting_spec(SpecWnd, SpecDc, Spec, IMAGE_WIDTH, IMAGE_HEIGHT,
			(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
		image->ds_spec = dsetting_spec;

		image->draw_image(image->spectrum_image_vis, image->ds_spec, true);
	}

	FILTERED_IMAGE_DRAW.EnableWindow(TRUE);
}


void CImagesProcessingDlg::OnBnClickedButtonDrawFiltered()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	OnUpdateWindowsSize();

	DrawSettings dsetting_filtered(FiltWnd, FiltDc, Filt, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_filtered = dsetting_filtered;

	if (!image->spectrum_image_calc.empty() && (image->ds_filtered.circle_window || image->ds_filtered.rect_window)) {
		image->get_filtered_image();

		DIFFERENCE_VALUE = image->calculate_difference();
		UpdateData(FALSE);
		image->draw_image(image->filtered_image, image->ds_filtered, false);
	}

	FILTERED_IMAGE_CLEAR.EnableWindow(TRUE);
}


void CImagesProcessingDlg::OnBnClickedButtonClearFiltered()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	image->filtered_image = image->clear_selected_image(image->filtered_image);
	image->draw_image(image->filtered_image, image->ds_filtered, false);

	FILTERED_IMAGE_CLEAR.EnableWindow(FALSE);
}


void CImagesProcessingDlg::OnBnClickedButtonClearAll()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	OnUpdateWindowsSize();

	// Обновление гиперпараметров.
	DrawSettings dsetting_input(PicWnd, PicDc, Pic, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_input = dsetting_input;

	DrawSettings dsetting_noise(NoiseWnd, NoiseDc, Noise, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_noise = dsetting_noise;

	DrawSettings dsetting_spec(SpecWnd, SpecDc, Spec, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_spec = dsetting_spec;

	DrawSettings dsetting_filtered(FiltWnd, FiltDc, Filt, IMAGE_WIDTH, IMAGE_HEIGHT,
		(bool)CIRCLE_WINDOW.GetCheck(), (bool)RECT_WINDOW.GetCheck(), FILTRATION_PERCENT);
	image->ds_filtered = dsetting_filtered;

	// Очистка данных.
	image->input_image = image->clear_selected_image(image->input_image);
	image->noise_image = image->clear_selected_image(image->noise_image);
	image->spectrum_image_calc = image->clear_selected_image(image->spectrum_image_calc);
	image->spectrum_image_vis = image->clear_selected_image(image->spectrum_image_vis);
	image->filtered_image = image->clear_selected_image(image->filtered_image);
	image->window_offset_x = 0;
	image->window_offset_y = 0;

	// Обновление окон рисования.
	image->draw_image(image->input_image, image->ds_input, false);
	image->draw_image(image->noise_image, image->ds_noise, false);
	image->draw_image(image->spectrum_image_vis, image->ds_spec, false);
	image->draw_image(image->filtered_image, image->ds_filtered, false);

	DIFFERENCE_VALUE = 0.0;
	UpdateData(FALSE);

	NOISE_IMAGE_DRAW.EnableWindow(FALSE);
	NOISE_IMAGE_CLEAR.EnableWindow(FALSE);
	SPECTRUM_IMAGE_CLEAR.EnableWindow(FALSE);
	SPECTRUM_IMAGE_DRAW.EnableWindow(FALSE);
	SPEC_RATIO_DEFAULT.EnableWindow(FALSE);
	SPEC_RATIO_LOG2.EnableWindow(FALSE);
	SPEC_RATIO_LOG10.EnableWindow(FALSE);
	FILTERED_IMAGE_CLEAR.EnableWindow(FALSE);
	FILTERED_IMAGE_DRAW.EnableWindow(FALSE);
	RECT_WINDOW.EnableWindow(FALSE);
	CIRCLE_WINDOW.EnableWindow(FALSE);
	NOISE_PERCENT_BUTTON.EnableWindow(FALSE);
	FILTER_PERCENT_BUTTON.EnableWindow(FALSE);
	INPUT_IMAGE_CLEAR.EnableWindow(FALSE);
}

void CImagesProcessingDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного

	CDialogEx::OnGetMinMaxInfo(lpMMI);
	lpMMI->ptMinTrackSize.x = 1335;
	lpMMI->ptMinTrackSize.y = 710;
}


void CImagesProcessingDlg::OnBnClickedButtonLoadImage()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	CFileDialog fileDialog(TRUE, NULL, L"*.png;*.bmp;*.jpg;*.jpeg");
	int res = fileDialog.DoModal();
	if (res != IDOK)
		return;
	
	CImage img;
	CString file_name = fileDialog.GetPathName();
	HRESULT read_img = img.Load(file_name);

	int width = img.GetWidth();
	int height = img.GetHeight();
	image->update_image_condition(RADIO_IS_MODEL.GetCheck(), ZEROS_ADDITION.GetCheck(), width, height);
	image->input_image.clear();
	image->input_image.resize(height);
	for (int i = 0; i < height; i++) {
		image->input_image[i].resize(width);
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			image->input_image[height-1-i][j].value.re = img.GetPixel(j, i);
		}
	}

	image->input_image = image->calculate_color_of_image(image->input_image, false);
}
