// ClipDataInfo.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "ClipDataInfo.h"

#include "ClipDataRect.h"


// ClipDataInfo

ClipDataInfo::ClipDataInfo()
{
	m_eClipDataInfoTag = CLIPDATAINFO;
	m_eClipKind = VIDEO;
	m_iInPoint = 0;
	m_iOutPoint = 0;
	m_iDuration = 0;
	m_iInOffset = 0;
}

ClipDataInfo::ClipDataInfo(InfoKind eKind)
{
	ClipDataInfo();
	m_eClipKind = eKind;
}

ClipDataInfo::~ClipDataInfo()
{
}


// ClipDataInfo �����o�[�֐�
BOOL ClipDataInfo::InitializeClipId(UUID& uiClipId)
{
	if (RPC_S_OK == UuidCreate(&m_uiClipId))
	{
		uiClipId = m_uiClipId;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// �N���b�v�\���f�[�^�폜
BOOL ClipDataInfo::DeleteClipData(void)
{
	if (m_pClipDataRect)
	{
		delete m_pClipDataRect;
		return TRUE;
	}
	return FALSE;
}