#pragma once
class kunai
{
public:
	kunai();
	~kunai();

	void Update();
	void GhostInit();
private:
	SkinModel m_kunaiModel;							//スキンモデル。
};

