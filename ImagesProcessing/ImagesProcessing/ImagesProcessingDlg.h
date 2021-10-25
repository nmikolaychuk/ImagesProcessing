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

	// Глобальные объекты для обращения из интерфейса.
	Image* image;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDrawInput();
	afx_msg void OnBnClickedButtonClearInput();
	int IMAGE_WIDTH;
	int IMAGE_HEIGHT;
	CButton RADIO_IS_MODEL;
	CButton RADIO_IS_PICTURE;
	double NOISE_PERCENT;
	afx_msg void OnBnClickedButtonDrawInputWithNoise();
	afx_msg void OnBnClickedButtonClearInputWithNoise();
};
