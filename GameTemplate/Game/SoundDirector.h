#pragma once
//�y���ɍ��܂����B
//���ASE�����Ȃ��Ă��邩���v�����āA����𗬂����ǂ��������߂�N���X
//���ꂾ���̂��߂ɍ��ꂽ
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
	//SE���ǂꂾ���Ȃ��Ă��邩���J�E���g����B
	int m_seRingCount = 0;
	//�ҋ@�҂���se���ǂꂾ�����邩���J�E���g����B
	int m_seStockCount = 0;
	const int	MAX_RING_SE = 1;						//se��炷�����			
};

