//=========================================
// 
// �����蔻��(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _COLISION_H_
#define _COLISION_H_

#include "main.h"

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
bool SegmentColision(Segment seg1, Segment seg2);	// �������m�̓����蔻��
bool SphereColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2);			// �����m�̓����蔻��
bool SphereCuboidColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, D3DXVECTOR3 size);	// ���Ɨ����̂̓����蔻��
bool SphereCylinderColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2);	// ���Ɖ~���̓����蔻��
bool SphereCapsuleColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2);	// ���Ɖ~���̓����蔻��
#endif // !_COLISION_H_
