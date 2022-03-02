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
// �}�N����`
//------------------------------------
#define RADIAN(degree)			(degree * D3DX_PI / 180.0f)	// Degree��Radian�ɕϊ�

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------

// �������n��

// �����`����(2D)
void SetVtxBuff2D(LPDIRECT3DVERTEXBUFFER9 *vtxBuff,int nData);	// ���_�o�b�t�@�̐���
void InitRect(VERTEX_2D *vtx);									// �����`�̏�����
void InitRectPos(VERTEX_2D *vtx);								// ���_���W������
void SetPosRectCenter(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);						// ���S���W�����S�ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�
void SetPosRectCenterRot(VERTEX_2D *vtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAngle, float fLength);	// ���S���W�����S�ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�(�p�x�̕ω��ɑΉ�)
void SetPosRectUpLeft(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);						// ���S���W������ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�
void SetPosRectUpRight(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth);						// ���S���W���E��ɂ������ꍇ�̒��_�o�b�t�@�̍��W�ݒ�

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
void InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff);			// �`�ʑO�̏���
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);		// �����`�̕`��
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// ���Z�����̕`��

// �Z�o�n��
bool isSharpAngle(D3DXVECTOR3* pos1, D3DXVECTOR3* pos2, D3DXVECTOR3* pos3);	// �s�p���ۂ�
float CalculateDistPointLine(D3DXVECTOR3* posPoint, D3DXVECTOR3* posLine, D3DXVECTOR3* vecLine, D3DXVECTOR3* posDist, float* fLength);	// �_�Ɛ����̍ŒZ�������Z�o
float CalculateDistPointSegment(D3DXVECTOR3* posPoint, Segment* segment, D3DXVECTOR3* vecLine, D3DXVECTOR3* posDist, float* fLength);	// �_�Ɛ����̍ŒZ�������Z�o

#endif // !_COMMON_H_
