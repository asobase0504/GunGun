//=========================================
// 
// �|���S���̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _MESH_BUILD_H_
#define _MESH_BUILD_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitMeshBuild(void);				// �|���S���̏���������
void UninitMeshBuild(void);				// �|���S���̏I������
void UpdateMeshBuild(void);				// �|���S���̍X�V����
void DrawMeshBuild(void);				// �|���S���̕`�揈��
D3DXVECTOR3 GetMeshBuildPos(void);		// �|���S���̎擾����

#endif // !_MESH_BUILD_H_
