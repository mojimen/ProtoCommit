// TransitionData.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "TransitionData.h"


// TransitionData

TransitionData::TransitionData()
{
	m_iTransitionDuration = 0;
	m_fReverse = FALSE;
}

TransitionData::~TransitionData()
{
}


// TransitionData �����o�[�֐�

// �����ݒ�
BOOL TransitionData::InitializeInTransition(CString& strTransitionDataUUID)
{
	if (InitializeData(TRANSITIONDATAIN))
	{
		strTransitionDataUUID = m_strUUID;
		return TRUE;
	}
	m_eTransitionType = DISSOLVE;
	return FALSE;
}

BOOL TransitionData::InitializeOutTransition(CString& strTransitionDataUUID)
{
	if (InitializeData(TRANSITIONDATAOUT))
	{
		strTransitionDataUUID = m_strUUID;
		return TRUE;
	}
	m_eTransitionType = DISSOLVE;
	return FALSE;
}

