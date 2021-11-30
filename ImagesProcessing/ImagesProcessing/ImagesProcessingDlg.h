#pragma once
#include <iostream>
#include "panel_logic.h"


// Диалоговое окно CImagesProcessingDlg
class CImagesProcessingDlg : public CDialogEx
{
// Создание
public:
	CImagesProcessingDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGESPROCESSING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Переменные, связываемые с окном исходного изображения.
	CWnd* PicWnd;
	CDC* PicDc;
	CRect Pic;

	// Переменные, связываемые с окном для зашумленного изображения.
	CWnd* NoiseWnd;
	CDC* NoiseDc;
	CRect Noise;

	// Переменные, связываемые с окном для спектра.
	CWnd* SpecWnd;
	CDC* SpecDc;
	CRect Spec;

	// Переменные, связываемые с окном для фильтрованного изображения.
	CWnd* FiltWnd;
	CDC* FiltDc;
	CRect Filt;

	// Глобальные объекты для обращения из интерфейса.
	Image* image;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int IMAGE_WIDTH;
	double FILTRATION_PERCENT;
	double DIFFERENCE_VALUE;
	int IMAGE_HEIGHT;
	double NOISE_PERCENT;

	CButton RADIO_IS_MODEL;
	CButton RADIO_IS_PICTURE;
	CButton SPEC_RATIO_DEFAULT;
	CButton SPEC_RATIO_LOG2;
	CButton SPEC_RATIO_LOG10;
	CButton RECT_WINDOW;
	CButton CIRCLE_WINDOW;
	CButton ZEROS_ADDITION;
	CButton INTERPOLATION_ADDITION;
	CButton INPUT_IMAGE_DRAW;
	CButton INPUT_IMAGE_CLEAR;
	CButton NOISE_IMAGE_DRAW;
	CButton NOISE_IMAGE_CLEAR;
	CButton SPECTRUM_IMAGE_DRAW;
	CButton SPECTRUM_IMAGE_CLEAR;
	CButton FILTERED_IMAGE_DRAW;
	CButton FILTERED_IMAGE_CLEAR;
	CEdit NOISE_PERCENT_BUTTON;
	CEdit FILTER_PERCENT_BUTTON;

	afx_msg void OnBnClickedButtonDrawInput();
	afx_msg void OnBnClickedButtonClearInput();
	afx_msg void OnBnClickedButtonDrawInputWithNoise();
	afx_msg void OnBnClickedButtonClearInputWithNoise();
	afx_msg void OnBnClickedButtonDrawSpectrumImage();
	afx_msg void OnBnClickedButtonClearSpectrumImage();
	afx_msg void OnBnClickedRadioDefaultSpecRatio();
	afx_msg void OnBnClickedRadioLog2SpecRatio();
	afx_msg void OnBnClickedRadioLog10SpecRatio();
	afx_msg void OnBnClickedRadioRectangleWindow();
	afx_msg void OnBnClickedButtonClearAll();
	afx_msg void OnBnClickedRadioCircleWindow();
	afx_msg void OnBnClickedButtonDrawFiltered();
	afx_msg void OnBnClickedButtonClearFiltered();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	void OnUpdateWindowsSize();
	void OnBnClickedButtonLoadImage();
};
