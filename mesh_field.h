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
// �v���g�^�C�v�錾
//------------------------------------
void InitMeshBuild(void);				// ���b�V���̏���������
void UninitMeshBuild(void);				// ���b�V���̏I������
void UpdateMeshBuild(void);				// ���b�V���̍X�V����
void DrawMeshBuild(void);				// ���b�V���̕`�揈��
D3DXVECTOR3 GetMeshBuildPos(void);		// ���b�V���̎擾����

void SetMeshRect(void);				// ���b�V���Œ����`�̍쐬�ݒ�
void SetMeshCylinder(void);			// ���b�V���ŉ~���̍쐬�ݒ�

#endif // !_MESH_FIELD_H_
