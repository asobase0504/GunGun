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

// �����`����(2D)
void InitRect(VERTEX_2D *vtx);	// �����`�̏�����
void InitRectPos(VERTEX_2D *vtx);	// ���_���W������
void SetRectCenterPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);						// ���S���W�����S�ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�
void SetRectCenterRotPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAngle, float fLength);	// ���S���W�����S�ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�(�p�x�̕ω��ɑΉ�)
void SetRectUpLeftPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);						// ���S���W������ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�
void SetRectUpRightPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);						// ���S���W���E��ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�

void InitRectColor(VERTEX_2D *vtx);						// ���_�o�b�t�@�̐F�̏�����
void SetRectColor(VERTEX_2D *vtx, D3DXCOLOR *inColor);	// ���_�o�b�t�@�̐F�ݒ�

void InitRectTex(VERTEX_2D *vtx);															// ���_�o�b�t�@�̃e�N�X�`�����W�̏�����
void SetRectTex(VERTEX_2D *vtx, float top, float bottom, float left, float right);			// ���_�o�b�t�@�̃e�N�X�`�����W�̐ݒ�

void InitRectRhw(VERTEX_2D *vtx);

// ���f���n��
void ModelSize(D3DXVECTOR3* minOut, D3DXVECTOR3* maxOut, const LPD3DXMESH mesh);	// ���f���̃T�C�Y���Z�o

// ���K���n��
float NormalizeRot(float* rot);	// �p�x�̐��K��

// �`��n��
void InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff);		// �`�ʑO�̏���
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// �ʏ�̕`��
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// ���Z�����̕`��

// �����蔻��
bool SegmentColision(Segment seg1, Segment seg2);	// �������m�̓����蔻��
bool SphereColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2);		// �����m�̓����蔻��
bool SphereCuboidColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, D3DXVECTOR3 size);	// ���Ɨ����̂̓����蔻��
bool SphereCylinderColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2);	// ���Ɠ��̓����蔻��
#endif // !_COMMON_H_
