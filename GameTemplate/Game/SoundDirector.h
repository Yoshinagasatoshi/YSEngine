#pragma once
//軽率に作りました。
//今、SEが何個なっているかを計測して、それを流すかどうかを決めるクラス
//それだけのために作られた
class SoundDirector
{
public:
	bool voicenagasiteiindesyouka()
	{
		if (m_seRingCount < MAX_RING_SE)
		{
			m_seRingCount++;
			return true;
		}
		else {
			m_seStockCount;
			return false;
		}
	}
private:
	//SEがどれだけなっているかをカウントする。
	int m_seRingCount = 0;
	//待機待ちのseがどれだけあるかをカウントする。
	int m_seStockCount = 0;
	const int	MAX_RING_SE = 1;						//seを鳴らす上限数			
};

