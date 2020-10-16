#pragma once

class Font
{
public:
	Font();
	~Font();
	/// <summary>
	/// 描画、座標をスクリーン座標で指定
	/// </summary>
	/// <param name="text">文字</param>
	/// <param name="pos">座標</param>
	/// <param name="color">色</param>
	/// <param name="scale">大きさ</param>
	/// <param name="pivot">ピボット</param>
	/// <param name="rotation">回転</param>
	/// <param name="effects">デフォルトはDirectX::SpriteEffects_None</param>
	//引数が代入されていれば書かなくても大丈夫
	//逆に台数が書いてある場合、そっちが優先される
	//スケール、基準点
	void DrawScreenPos(
		wchar_t const* text,
		const CVector2& pos,
		const CVector4& color = CVector4::White(),
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2::Zero(),
		float rotation = 0.0f,
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None
	);
private:
	DirectX::SpriteBatch*	m_spriteBatch = nullptr;	//!<スプライトバッチ。
	DirectX::SpriteFont*	m_spriteFont = nullptr;		//ふぉんと
	CVector2 m_screenSize;								//画面サイズ
};

