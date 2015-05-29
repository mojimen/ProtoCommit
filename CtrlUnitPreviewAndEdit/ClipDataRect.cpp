// ClipDataRect.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "ClipDataRect.h"


// ClipDataRect

ClipDataRect::ClipDataRect()
{
}

ClipDataRect::~ClipDataRect()
{
}


// ClipDataRect メンバー関数
// とりあえずの初期値設定
BOOL ClipDataRect::InitClipData(void)
{
	SetColor(CLIPCOLOR_BRUSH_FLOAT, CLIPCOLOR_BRUSH_FLOAT, CLIPCOLOR_BRUSH_FLOAT, CLIPCOLOR_BRUSH_FLOAT);
	SetBorderColor(CLIPBORDERCOLOR_BRUSH_FLOAT, CLIPBORDERCOLOR_BRUSH_FLOAT, CLIPBORDERCOLOR_BRUSH_FLOAT, CLIPBORDERCOLOR_BRUSH_FLOAT);
	SetSingleInTrimColor(CLIPCOLOR_SINGLEINTRIMING_BRUSH_FLOAT, CLIPCOLOR_SINGLEINTRIMING_BRUSH_FLOAT, CLIPCOLOR_SINGLEINTRIMING_BRUSH_FLOAT, CLIPCOLOR_SINGLEINTRIMING_BRUSH_FLOAT);
	SetSingleOutTrimColor(CLIPCOLOR_SINGLEOUTTRIMING_BRUSH_FLOAT, CLIPCOLOR_SINGLEOUTTRIMING_BRUSH_FLOAT, CLIPCOLOR_SINGLEOUTTRIMING_BRUSH_FLOAT, CLIPCOLOR_SINGLEOUTTRIMING_BRUSH_FLOAT);
	SetMovingColor(CLIPCOLOR_MOVING_BRUSH_FLOAT, CLIPCOLOR_MOVING_BRUSH_FLOAT, CLIPCOLOR_MOVING_BRUSH_FLOAT, CLIPCOLOR_MOVING_BRUSH_FLOAT);
	SetOperatingOldColor(CLIPCOLOR_OPERATING_BRUSH_FLOAT, CLIPCOLOR_OPERATING_BRUSH_FLOAT, CLIPCOLOR_OPERATING_BRUSH_FLOAT, CLIPCOLOR_OPERATING_BRUSH_FLOAT);
	SetOverlappingColor(CLIPCOLOR_OVERLAPPING_BRUSH_FLOAT, CLIPCOLOR_OVERLAPPING_BRUSH_FLOAT, CLIPCOLOR_OVERLAPPING_BRUSH_FLOAT, CLIPCOLOR_OVERLAPPING_BRUSH_FLOAT);
	SetSingleTrimBorderColor(CLIPCOLOR_SINGLETRIMINGBORDER_BRUSH_FLOAT, CLIPCOLOR_SINGLETRIMINGBORDER_BRUSH_FLOAT, CLIPCOLOR_SINGLETRIMINGBORDER_BRUSH_FLOAT, CLIPCOLOR_SINGLETRIMINGBORDER_BRUSH_FLOAT);

	return TRUE;
}


BOOL ClipDataRect::InitializeClipRectId(UUID& uiClipRectId)
{
	if (RPC_S_OK == UuidCreate(&m_uiClipRectId))
	{
		uiClipRectId = m_uiClipRectId;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void ClipDataRect::GetOverlappingVert(float(&fVert)[4][3])
{
	CopyVert(m_fOverlappingVert, fVert);
	return;
}

void ClipDataRect::SetOverlappingVert(const float fLeft, const float fTop, const float fRight, const float fBottom)
{

	m_fOverlappingVert[0][0] = fLeft;
	m_fOverlappingVert[0][1] = fTop;
	m_fOverlappingVert[0][2] = 0.0f;
	m_fOverlappingVert[1][0] = fRight;
	m_fOverlappingVert[1][1] = fTop;
	m_fOverlappingVert[1][2] = 0.0f;
	m_fOverlappingVert[2][0] = fRight;
	m_fOverlappingVert[2][1] = fBottom;
	m_fOverlappingVert[2][2] = 0.0f;
	m_fOverlappingVert[3][0] = fLeft;
	m_fOverlappingVert[3][1] = fBottom;
	m_fOverlappingVert[3][2] = 0.0f;

	return;
}

void ClipDataRect::SetMovingColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4)
{
	SetColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3,
		fR4, fG4, fB4, fA4, m_fMovingColor);
	return;
}

void ClipDataRect::SetSingleInTrimColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4)
{
	SetColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3,
		fR4, fG4, fB4, fA4, m_fSingleInTrimColor);
	return;
}

void ClipDataRect::SetSingleOutTrimColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4)
{
	SetColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3,
		fR4, fG4, fB4, fA4, m_fSingleOutTrimColor);
	return;
}

void ClipDataRect::SetOperatingOldColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4)
{
	SetColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3,
		fR4, fG4, fB4, fA4, m_fOperatingOldColor);
	return;
}

void ClipDataRect::SetOverlappingColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4)
{
	SetColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3,
		fR4, fG4, fB4, fA4, m_fOverlappingColor);
	return;
}

void ClipDataRect::SetSingleTrimBorderColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4)
{
	SetColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3,
		fR4, fG4, fB4, fA4, m_fSingleTrimBorderColor);
	return;
}

void ClipDataRect::GetMovingColor(float(&fColor)[4][4])
{
	CopyColor(m_fMovingColor, fColor);
	return;
}

void ClipDataRect::GetSingleInTrimColor(float(&fColor)[4][4])
{
	CopyColor(m_fSingleInTrimColor, fColor);
	return;
}

void ClipDataRect::GetSingleOutTrimColor(float(&fColor)[4][4])
{
	CopyColor(m_fSingleOutTrimColor, fColor);
	return;
}

void ClipDataRect::GetOperatingOldColor(float(&fColor)[4][4])
{
	CopyColor(m_fOperatingOldColor, fColor);
	return;
}

void ClipDataRect::GetOverlappingColor(float(&fColor)[4][4])
{
	CopyColor(m_fOverlappingColor, fColor);
	return;
}

void ClipDataRect::GetSingleTrimBorderColor(float(&fColor)[4][4])
{
	CopyColor(m_fSingleTrimBorderColor, fColor);
	return;
}


void ClipDataRect::DrawSingleTrimRect(int iHeight, BOOL fInTrim)
{
	m_rcOperatingRect.SetVert(iHeight);
	m_rcOperatingRect.GetVert(m_fRectVert);
	if (fInTrim)
	{
		GetSingleInTrimColor(m_fRectColor);
	}
	else
	{
		GetSingleOutTrimColor(m_fRectColor);
	}
	DrawMyRect(1.0f, GL_QUADS);

	m_rcOperatingBorderRect.CopyRect(m_rcOperatingRect);
	GetSingleTrimBorderColor(m_fRectColor);
	if (fInTrim)
	{
		m_rcOperatingBorderRect.right = m_rcOperatingBorderRect.left + SINGLETRIMINGBORDER_WIDTH;
		if (m_rcOperatingBorderRect.right > m_rcOperatingRect.right)
		{
			m_rcOperatingBorderRect.right = m_rcOperatingRect.right;
		}
		m_rcOperatingBorderRect.SetVert(iHeight);
		m_rcOperatingBorderRect.GetVert(m_fRectVert);
		DrawMyRect(1.0f, GL_QUADS);
	}
	else
	{
		m_rcOperatingBorderRect.left = m_rcOperatingBorderRect.right - SINGLETRIMINGBORDER_WIDTH;
		if (m_rcOperatingBorderRect.left < m_rcOperatingRect.left)
		{
			m_rcOperatingBorderRect.left = m_rcOperatingRect.left;
		}
		m_rcOperatingBorderRect.SetVert(iHeight);
		m_rcOperatingBorderRect.GetVert(m_fRectVert);
		DrawMyRect(1.0f, GL_QUADS);
	}
	m_rcOperatingBorderRect.SetRectEmpty();
}

void ClipDataRect::DrawMovingRect(int iHeight)
{
	m_rcOperatingRect.SetVert(iHeight);
	m_rcOperatingRect.GetVert(m_fRectVert);
	GetMovingColor(m_fRectColor);
	DrawMyRect(1.0f, GL_QUADS);
}

void ClipDataRect::DrawOperatingOldRect(int iHeight)
{
	GetVert(m_fRectVert);
	GetOperatingOldColor(m_fRectColor);
	DrawMyRect(1.0f, GL_QUADS);
}

void ClipDataRect::DrawOverlappingRect(int iHeight)
{
	GetOverlappingVert(m_fRectVert);
	GetOverlappingColor(m_fRectColor);
	DrawMyRect(1.0f, GL_QUADS);
}

