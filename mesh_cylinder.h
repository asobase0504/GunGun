//=========================================
// 
// ���b�V��(�~��)�̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _MESH_CYLINDER_H_
#define _MESH_CYLINDER_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitMeshCylinder(void);				// ���b�V���̏���������
void UninitMeshCylinder(void);				// ���b�V���̏I������
void UpdateMeshCylinder(void);				// ���b�V���̍X�V����
void DrawMeshCylinder(void);				// ���b�V���̕`�揈��

void SetMeshCylinder(void);			// ���b�V���ŉ~���̍쐬�ݒ�

#endif // !_MESH_CYLINDER_H_
