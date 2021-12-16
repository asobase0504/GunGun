//=========================================
// 
// �|���S���̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//------------------------------------
// �J�����̍\����
//------------------------------------
typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 posRDest;		// ���_�̖ړI�l
	D3DXVECTOR3 posVDest;		// �����_�̖ړI�l
	D3DXVECTOR3 vec;			// �ړ���
	D3DXVECTOR3 rot;			// ����
	D3DXVECTOR3 rotDest;		// �����̖ړI�n
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXMATRIX mtxProjection;	// �v���W�F�N�g�}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
	float fDistance;			// ���_���璍���_�̋���
}Camera;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitCamera(void);			// �J�����̏���������
void UninitCamera(void);		// �J�����̏I������
void UpdateCamera(void);		// �J�����̍X�V����
void SetCamera(void);			// �J�����̐ݒ菈��
Camera* GetCamera(void);			// �J�����̊p�x���擾
D3DXVECTOR3 GetRotCamera(void);	// �J�����̊p�x���擾

#endif // !_CAMERA_H_
