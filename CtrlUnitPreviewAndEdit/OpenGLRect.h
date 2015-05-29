#pragma once

#include <GL/gl.h>
#include <GL/glu.h>

#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "opengl32.lib")


// OpenGLRect

class OpenGLRect : public CRect
{

protected:
	// OpenGL��ł̍��W
	float m_fVert[4][3];
	float m_fColor[4][4];
	float m_fBorderColor[4][4];
	// �`�掞�Ɏg�p���钸�_�z��i�`��̓s�x���������āI�j
	float m_fLineVert[2][3];	// ���`�掞���W�̒��_�z��
	float m_fLineColor[2][4];	// ���`�掞�F�̒��_�z��
	float m_fRectVert[4][3];	// �l�p�`�`�掞���W�̒��_�z��
	float m_fRectColor[4][4];	// �l�p�`�`�掞���W�̒��_�z��

	void CopyVert(const float(&fOriginalVert)[4][3], float(&fVert)[4][3]);
	void SetColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4, float(&fColor)[4][4]);
	void CopyColor(const float(&fOriginalColor)[4][4], float(&fColor)[4][4]);

	void DrawMyRect(const float& fLineWidth, GLenum glMode = GL_LINE_LOOP);
	void DrawMyLine(void);



public:
	OpenGLRect();
	virtual ~OpenGLRect();

	float m_fInterfaceVert[4][3];
	float m_fInterfaceColor[4][4];

	float(*GetVert(void))[3] { return m_fVert; }
	float(*GetColor(void))[4] { return m_fColor; }
	float(*GetBorderColor(void))[4] { return m_fBorderColor; }
	void GetVert(float(&fVert)[4][3]);
	void GetBorderVert(float(&fVert)[4][3]);
	void GetLeftBorderVert(float(&fVert)[2][3]);
	void GetTopBorderVert(float(&fVert)[2][3]);
	void GetRightBorderVert(float(&fVert)[2][3]);
	void GetBottomBorderVert(float(&fVert)[2][3]);
	void GetColor(float(&fcolor)[4][4]);
	void GetBorderColor(float(&fcolor)[4][4]);
	void GetLeftBorderColor(float(&fVert)[2][4]);
	void GetTopBorderColor(float(&fVert)[2][4]);
	void GetRightBorderColor(float(&fVert)[2][4]);
	void GetBottomBorderColor(float(&fVert)[2][4]);

	void SetVert(const float fLeft, const float fTop, const float fRight, const float fBottom);
	void SetVert(const int iHeight);
	void SetColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4);
	void SetBorderColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4);
	void SetBothColor(const float fR1, const float fG1, const float fB1, const float fA1,
		const float fR2, const float fG2, const float fB2, const float fA2,
		const float fR3, const float fG3, const float fB3, const float fA3,
		const float fR4, const float fG4, const float fB4, const float fA4);

	void DrawMyFillRect(void);
	void DrawMyBorderRect(const float& fLineWidth);
	void DrawMyBothRect(const float& fLineWidth);
	void DrawMyLeftLine(const float& fLineWidth);
	void DrawMyTopLine(const float& fLineWidth);
	void DrawMyRightLine(const float& fLineWidth);
	void DrawMyBottomLine(const float& fLineWidth);

};


