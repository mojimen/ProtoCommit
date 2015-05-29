// OpenGLRect.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "CtrlUnitPreviewAndEdit.h"
#include "OpenGLRect.h"


// OpenGLRect

OpenGLRect::OpenGLRect()
{

}

OpenGLRect::~OpenGLRect()
{
}

void OpenGLRect::CopyVert(const float(&fOriginalVert)[4][3], float(&fVert)[4][3])
{
	for (int i = 0; i < 4; ++i)

	{
		fVert[i][0] = fOriginalVert[i][0];
		fVert[i][1] = fOriginalVert[i][1];
		fVert[i][2] = fOriginalVert[i][2];
	}
	return;
}

void OpenGLRect::SetColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4, float(&fColor)[4][4])
{
	fColor[0][0] = fR1;
	fColor[0][1] = fG1;
	fColor[0][2] = fB1;
	fColor[0][3] = fA1;
	fColor[1][0] = fR2;
	fColor[1][1] = fG2;
	fColor[1][2] = fB2;
	fColor[1][3] = fA2;
	fColor[2][0] = fR3;
	fColor[2][1] = fG3;
	fColor[2][2] = fB3;
	fColor[2][3] = fA3;
	fColor[3][0] = fR4;
	fColor[3][1] = fG4;
	fColor[3][2] = fB4;
	fColor[3][3] = fA4;

	return;
}

void OpenGLRect::CopyColor(const float(&fOriginalColor)[4][4], float(&fColor)[4][4])
{
	for (int i = 0; i < 4; ++i)

	{
		fColor[i][0] = fOriginalColor[i][0];
		fColor[i][1] = fOriginalColor[i][1];
		fColor[i][2] = fOriginalColor[i][2];
		fColor[i][3] = fOriginalColor[i][3];
	}
	return;
}



void OpenGLRect::GetVert(float(&fVert)[4][3])
{
	CopyVert(m_fVert, fVert);
	return;
}

void OpenGLRect::GetBorderVert(float(&fVert)[4][3])
{
	CopyVert(m_fVert, fVert);
	fVert[0][0] += 1;
	fVert[2][1] += 1;
	fVert[3][0] += 1;
	fVert[3][1] += 1;
	return;
}

void OpenGLRect::GetLeftBorderVert(float(&fVert)[2][3])
{
	fVert[0][0] = m_fVert[0][0] + 1;
	fVert[0][1] = m_fVert[0][1];
	fVert[0][2] = m_fVert[0][2];
	fVert[1][0] = m_fVert[3][0] + 1;
	fVert[1][1] = m_fVert[3][1] + 1;
	fVert[1][2] = m_fVert[3][2];
}

void OpenGLRect::GetTopBorderVert(float(&fVert)[2][3])
{
	fVert[0][0] = m_fVert[0][0] + 1;
	fVert[0][1] = m_fVert[0][1];
	fVert[0][2] = m_fVert[0][2];
	fVert[1][0] = m_fVert[1][0];
	fVert[1][1] = m_fVert[1][1];
	fVert[1][2] = m_fVert[1][2];
}

void OpenGLRect::GetRightBorderVert(float(&fVert)[2][3])
{
	fVert[0][0] = m_fVert[1][0];
	fVert[0][1] = m_fVert[1][1];
	fVert[0][2] = m_fVert[1][2];
	fVert[1][0] = m_fVert[2][0];
	fVert[1][1] = m_fVert[2][1] + 1;
	fVert[1][2] = m_fVert[2][2];
}

void OpenGLRect::GetBottomBorderVert(float(&fVert)[2][3])
{
	fVert[0][0] = m_fVert[2][0];
	fVert[0][1] = m_fVert[2][1] + 1;
	fVert[0][2] = m_fVert[2][2];
	fVert[1][0] = m_fVert[3][0] + 1;
	fVert[1][1] = m_fVert[3][1] + 1;
	fVert[1][2] = m_fVert[3][2];
}

void OpenGLRect::GetLeftBorderColor(float(&fVert)[2][4])
{
	fVert[0][0] = m_fBorderColor[0][0];
	fVert[0][1] = m_fBorderColor[0][1];
	fVert[0][2] = m_fBorderColor[0][2];
	fVert[0][3] = m_fBorderColor[0][3];
	fVert[1][0] = m_fBorderColor[3][0];
	fVert[1][1] = m_fBorderColor[3][1];
	fVert[1][2] = m_fBorderColor[3][2];
	fVert[1][3] = m_fBorderColor[3][3];
}

void OpenGLRect::GetTopBorderColor(float(&fVert)[2][4])
{
	fVert[0][0] = m_fBorderColor[0][0];
	fVert[0][1] = m_fBorderColor[0][1];
	fVert[0][2] = m_fBorderColor[0][2];
	fVert[0][3] = m_fBorderColor[0][3];
	fVert[1][0] = m_fBorderColor[1][0];
	fVert[1][1] = m_fBorderColor[1][1];
	fVert[1][2] = m_fBorderColor[1][2];
	fVert[1][3] = m_fBorderColor[1][3];
}

void OpenGLRect::GetRightBorderColor(float(&fVert)[2][4])
{
	fVert[0][0] = m_fBorderColor[1][0];
	fVert[0][1] = m_fBorderColor[1][1];
	fVert[0][2] = m_fBorderColor[1][2];
	fVert[0][3] = m_fBorderColor[1][3];
	fVert[1][0] = m_fBorderColor[2][0];
	fVert[1][1] = m_fBorderColor[2][1];
	fVert[1][2] = m_fBorderColor[2][2];
	fVert[1][3] = m_fBorderColor[2][3];
}

void OpenGLRect::GetBottomBorderColor(float(&fVert)[2][4])
{
	fVert[0][0] = m_fBorderColor[2][0];
	fVert[0][1] = m_fBorderColor[2][1];
	fVert[0][2] = m_fBorderColor[2][2];
	fVert[0][3] = m_fBorderColor[2][3];
	fVert[1][0] = m_fBorderColor[3][0];
	fVert[1][1] = m_fBorderColor[3][1];
	fVert[1][2] = m_fBorderColor[3][2];
	fVert[1][3] = m_fBorderColor[3][3];
}


void OpenGLRect::GetColor(float(&fColor)[4][4])
{
	CopyColor(m_fColor, fColor);
	return;
}

void OpenGLRect::GetBorderColor(float(&fColor)[4][4])
{
	CopyColor(m_fBorderColor, fColor);
	return;
}

void OpenGLRect::SetVert(const float fLeft, const float fTop, const float fRight, const float fBottom)
{

	m_fVert[0][0] = fLeft;
	m_fVert[0][1] = fTop;
	m_fVert[0][2] = 0.0f;
	m_fVert[1][0] = fRight;
	m_fVert[1][1] = fTop;
	m_fVert[1][2] = 0.0f;
	m_fVert[2][0] = fRight;
	m_fVert[2][1] = fBottom;
	m_fVert[2][2] = 0.0f;
	m_fVert[3][0] = fLeft;
	m_fVert[3][1] = fBottom;
	m_fVert[3][2] = 0.0f;

	return;
}

void OpenGLRect::SetVert(const int iHeight)
{

	m_fVert[0][0] = static_cast<float>(left);
	m_fVert[0][1] = iHeight - static_cast<float>(top);
	m_fVert[0][2] = 0.0f;
	m_fVert[1][0] = static_cast<float>(right);
	m_fVert[1][1] = iHeight - static_cast<float>(top);
	m_fVert[1][2] = 0.0f;
	m_fVert[2][0] = static_cast<float>(right);
	m_fVert[2][1] = iHeight - static_cast<float>(bottom);
	m_fVert[2][2] = 0.0f;
	m_fVert[3][0] = static_cast<float>(left);
	m_fVert[3][1] = iHeight - static_cast<float>(bottom);
	m_fVert[3][2] = 0.0f;

	return;
}

void OpenGLRect::SetColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4)
{
	SetColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3,
		fR4, fG4, fB4, fA4, m_fColor);
	return;
}

void OpenGLRect::SetBorderColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4)
{
	SetColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3,
		fR4, fG4, fB4, fA4, m_fBorderColor);
	return;
}

void OpenGLRect::SetBothColor(const float fR1, const float fG1, const float fB1, const float fA1,
	const float fR2, const float fG2, const float fB2, const float fA2,
	const float fR3, const float fG3, const float fB3, const float fA3,
	const float fR4, const float fG4, const float fB4, const float fA4)
{
	SetColor(fR1, fG1, fB1, fA1, fR2, fG2, fB2, fA2, fR3, fG3, fB3, fA3,
		fR4, fG4, fB4, fA4, m_fColor);
	CopyColor(m_fColor, m_fBorderColor);
	return;
}

void OpenGLRect::DrawMyRect(const float& fLineWidth, GLenum glMode /*= GL_LINE_LOOP*/)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glLineWidth(fLineWidth);
	glVertexPointer(3, GL_FLOAT, 0, m_fRectVert);
	glColorPointer(4, GL_FLOAT, 0, m_fRectColor);
	glDrawArrays(glMode, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}


void OpenGLRect::DrawMyFillRect(void)
{
	GetVert(m_fRectVert);
	GetColor(m_fRectColor);
	DrawMyRect(1.0f, GL_QUADS);
}
void OpenGLRect::DrawMyBorderRect(const float& fLineWidth)
{
	GetBorderVert(m_fRectVert);
	GetBorderColor(m_fRectColor);
	DrawMyRect(fLineWidth);
}
void OpenGLRect::DrawMyBothRect(const float& fLineWidth)
{
	GetVert(m_fRectVert);
	GetColor(m_fRectColor);
	DrawMyRect(1.0f, GL_QUADS);

	GetBorderVert(m_fRectVert);
	GetBorderColor(m_fRectColor);
	DrawMyRect(fLineWidth);
}



void OpenGLRect::DrawMyLine(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, m_fLineVert);
	glColorPointer(4, GL_FLOAT, 0, m_fLineColor);
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void OpenGLRect::DrawMyLeftLine(const float& fLineWidth)
{
	GetLeftBorderVert(m_fLineVert);
	GetLeftBorderColor(m_fLineColor);
	glLineWidth(fLineWidth);
	DrawMyLine();
}

void OpenGLRect::DrawMyTopLine(const float& fLineWidth)
{
	GetTopBorderVert(m_fLineVert);
	GetTopBorderColor(m_fLineColor);
	glLineWidth(fLineWidth);
	DrawMyLine();
}

void OpenGLRect::DrawMyRightLine(const float& fLineWidth)
{
	GetRightBorderVert(m_fLineVert);
	GetRightBorderColor(m_fLineColor);
	glLineWidth(fLineWidth);
	DrawMyLine();
}

void OpenGLRect::DrawMyBottomLine(const float& fLineWidth)
{
	GetBottomBorderVert(m_fLineVert);
	GetBottomBorderColor(m_fLineColor);
	glLineWidth(fLineWidth);
	DrawMyLine();
}

