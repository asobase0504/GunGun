//=========================================
// 
// ���ʏ���
// Author YudaKaito
// 
//=========================================
#include "colision.h"

///=========================================
/// �������m�̓����蔻��
///=========================================
///bool SegmentColision(Segment seg1, Segment seg2)
//{
//	// �x�N�g���̎n�_���m�̋����B
//	D3DXVECTOR3 v = seg2.s - seg1.s;
//
//	// �u���b�N�̃x�N�g���Ɣ�Ώۂ̃x�N�g�������s�����ׂ�
//	float Bv_Tv = D3DXVec2Cross(&(seg1.v), &(seg2.v));
//	if (Bv_Tv == 0.0f)
//	{
//		// ���s�ł���B
//		return false;
//	}
//
//	D3DXVec3Cross()
//
//	float v_Bv = D3DXVec2Cross(&(v), &(seg1.v));
//	float v_Tv = D3DXVec2Cross(&(v), &(seg2.v));
//
//	float hit1 = v_Tv / Bv_Tv;
//	float hit2 = v_Bv / Bv_Tv;
//
//	if ((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f))
//	{
//		return false;
//	}
//
//	if (Outpos != NULL)
//	{
//		*Outpos = seg1.s + seg1.v * hit1;
//	}
//	return true;
//}

//=========================================
//
// ���Ƌ��̓����蔻��
//
// ����1 pos1		�Ώ�1�̈ʒu
// ����2 fLength1	�Ώ�1�̒���
// ����3 pos2		�Ώ�2�̈ʒu
// ����4 fLength2	�Ώ�2�̒���
//=========================================
bool SphereColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2)
{
	D3DXVECTOR3 v = pos1 - pos2;
	return D3DXVec3LengthSq(&v) <= (fLength1 + fLength2)*(fLength1 + fLength2);
}

//=========================================
//
// ���Ɨ����̂̓����蔻��
//
// ����1 pos1		�Ώ�1�̈ʒu
// ����2 fLength1	�Ώ�1�̒���
// ����3 pos2		�Ώ�2�̈ʒu
// ����4 size		�Ώ�2�̑傫��
//=========================================
bool SphereCuboidColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, D3DXVECTOR3 size)
{
	D3DXVECTOR3 Vec(0, 0, 0);   // �ŏI�I�ɒ��������߂�x�N�g��

								// �e���ɂ��Ă͂ݏo�������̃x�N�g�����Z�o
	FLOAT L = size.x;
	if (!(L <= 0))
	{
		D3DXVECTOR3 x = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &x) / L;

		// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
		s = (float)fabs(s);
		if (s > 1)
		{
			Vec += (1 - s)*L*x;   // �͂ݏo���������̃x�N�g���Z�o
		}
	}
	L = size.y;
	if (!(L <= 0))
	{
		D3DXVECTOR3 y = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &y) / L;

		// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
		s = (float)fabs(s);
		if (s > 1)
		{
			Vec += (1 - s)*L*y;   // �͂ݏo���������̃x�N�g���Z�o
		}
	}
	L = size.z;
	if (!(L <= 0))
	{
		D3DXVECTOR3 z = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &z) / L;

		// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
		s = (float)fabs(s);
		if (s > 1)
		{
			Vec += (1 - s)*L*z;   // �͂ݏo���������̃x�N�g���Z�o
		}
	}
	return fLength1 < D3DXVec3Length(&Vec);
}

//=========================================
//
// ���ƃJ�v�Z���̓����蔻��
//
// ����1 pos1		�Ώ�1�̈ʒu
// ����2 fLength1	�Ώ�1�̒���
// ����3 pos2		�Ώ�2�̈ʒu
// ����4 fLength1	�Ώ�2�̒���
//=========================================
bool SphereCapsuleColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2)
{
	return false;
}

//=========================================
//
// ���Ɖ~���̓����蔻��
//
// ����1 pos1		�Ώ�1�̈ʒu
// ����2 fLength1	�Ώ�1�̒���
// ����3 pos2		�Ώ�2�̈ʒu
// ����4 fLength1	�Ώ�2�̒���
//=========================================
//bool SphereCylinderColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2)
//{
	//D3DXVECTOR3 Vec(0, 0, 0);   // �ŏI�I�ɒ��������߂�x�N�g��

	//// �e���ɂ��Ă͂ݏo�������̃x�N�g�����Z�o
	//FLOAT L = size.x;
	//if (!(L <= 0))
	//{
	//	D3DXVECTOR3 x = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	//	FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &x) / L;

	//	// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
	//	s = (float)fabs(s);
	//	if (s > 1)
	//	{
	//		Vec += (1 - s)*L*x;   // �͂ݏo���������̃x�N�g���Z�o
	//	}
	//}
	//L = size.y;
	//if (!(L <= 0))
	//{
	//	D3DXVECTOR3 y = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//	FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &y) / L;

	//	// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
	//	s = (float)fabs(s);
	//	if (s > 1)
	//	{
	//		Vec += (1 - s)*L*y;   // �͂ݏo���������̃x�N�g���Z�o
	//	}
	//}
	//L = size.z;
	//if (!(L <= 0))
	//{
	//	D3DXVECTOR3 z = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//	FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &z) / L;

	//	// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
	//	s = (float)fabs(s);
	//	if (s > 1)
	//	{
	//		Vec += (1 - s)*L*z;   // �͂ݏo���������̃x�N�g���Z�o
	//	}
	//}
	//return fLength1 < D3DXVec3Length(&Vec);
//}
