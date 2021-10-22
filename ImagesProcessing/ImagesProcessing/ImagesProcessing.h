
// ImagesProcessing.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CImagesProcessingApp:
// Сведения о реализации этого класса: ImagesProcessing.cpp
//

class CImagesProcessingApp : public CWinApp
{
public:
	CImagesProcessingApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CImagesProcessingApp theApp;
