//=========================================
// 
// �|���S���̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXMATRIX mtxProjection;	// �v���W�F�N�g�}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
}Camera;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitCamera(void);		// �|���S���̏���������
void UninitCamera(void);	// �|���S���̏I������
void UpdateCamera(void);	// �|���S���̍X�V����
void SetCamera(void);		// �|���S���̕`�揈��

#endif // !_CAMERA_H_
