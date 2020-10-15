#include "stdafx.h"
#include "Font.h"

Font::Font()
{
	m_spriteBatch = g_graphicsEngine->GetSpriteBatch();
	m_spriteFont = g_graphicsEngine->GetSpriteFont();

	m_screenSize.x = FRAME_BUFFER_W;
	m_screenSize.y = FRAME_BUFFER_H;
}

Font::~Font()
{
}

void Font::DrawScreenPos(wchar_t const* text,
	const CVector2& pos,
	const CVector4& color,
	const CVector2& scale,
	const CVector2& pivot,
	float rotation,
	DirectX::SpriteEffects effects)
{
	//ƒrƒMƒ“ŠÖ”‚ğƒhƒ[‚Ì‘O‚ÅŒÄ‚Ô‚±‚Æ!!
	m_spriteBatch->Begin();
	m_spriteFont->DrawString(
		m_spriteBatch,
		text,
		pos.vec,
		color,
		rotation,
		DirectX::XMFLOAT2(pivot.x, pivot.y),
		DirectX::XMFLOAT2(scale.x, scale.y),
		effects
	);
	//‚±‚±‚Å•`‰æ
	m_spriteBatch->End();
}