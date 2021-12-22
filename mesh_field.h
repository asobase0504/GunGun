//=========================================
// 
// �|���S���̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _MESH_FIELD_H_
#define _MESH_FIELD_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// �����\����
//------------------------------------
typedef struct
{
	D3DXVECTOR3 s; // �n�_
	D3DXVECTOR3 v; // �����x�N�g���i�����̒������S���̂Ő��K�����Ȃ��悤�ɁI�j
}Segment;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitMeshBuild(void);				// ���b�V���̏���������
void UninitMeshBuild(void);				// ���b�V���̏I������
void UpdateMeshBuild(void);				// ���b�V���̍X�V����
void DrawMeshBuild(void);				// ���b�V���̕`�揈��
D3DXVECTOR3 GetMeshBuildPos(void);		// ���b�V���̎擾����
void CollisionMeshField(D3DXVECTOR3* pos);			// ���b�V���t�B�[���h�̓����蔻��
#endif // !_MESH_FIELD_H_
