
// PsiScaleEditor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPsiScaleEditorApp: 
// �йش����ʵ�֣������ PsiScaleEditor.cpp
//

class CPsiScaleEditorApp : public CWinApp
{
public:
	CPsiScaleEditorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPsiScaleEditorApp theApp;