//=========================================
// 
// ���ʏ���(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _COMMON_H_
#define _COMMON_H_

#include "main.h"

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------

// �������n��

// ���f���n��
void ModelSize(D3DXVECTOR3 *Min, D3DXVECTOR3 *Max, LPD3DXMESH Mesh);	// ���f���̃T�C�Y���Z�o

// ���K���n��
float NormalizeRot(float rot);	// �p�x�̐��K��

// �`��n��
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// �ʏ�̕`��
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// ���Z�����̕`��

#endif // !_COMMON_H_
