//=========================================
// 
// �J�����̍쐬
// Author YudaKaito
// 
//=========================================
#include "main.h"
#include "camera.h"
#include "input.h"

//------------------------------------
// static�ϐ�
//------------------------------------
static Camera s_camera;	// �J�������
static float rotY;

//=========================================
// ������
//=========================================
void InitCamera(void)
{
	// ���_�E�����_�E�������ݒ肷��
	s_camera.posV = D3DXVECTOR3(0.0f,10.0f,-30.0f);
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);
}

//=========================================
// �I��
//=========================================
void UninitCamera(void)
{
}

//=========================================
// �X�V
//=========================================
void UpdateCamera(void)
{
	if (GetKeyboardTrigger(DIK_W))
	{
		s_camera.posV.z += 5.0f;
	}

	if (GetKeyboardPress(DIK_A))
	{
		rotY -= 0.5f;
		float radY = rotY / (D3DX_PI * 2.0f);
		float c = cosf(radY);
		float s = sinf(radY);
		s_camera.posV = D3DXVECTOR3(30.0f * c, s_camera.posV.y, 30.0f * s);
	}

	if (GetKeyboardTrigger(DIK_S))
	{
		s_camera.posV.z -= 5.0f;
	}

	if (GetKeyboardPress(DIK_D))
	{
		rotY += 0.5f;
		float radY = rotY / (D3DX_PI * 2.0f);
		float c = cosf(radY);
		float s = sinf(radY);
		s_camera.posV = D3DXVECTOR3(30.0f * c, s_camera.posV.y, 30.0f * s);
	}

	if (GetKeyboardTrigger(DIK_Q))
	{
		s_camera.posV.y += 5.0f;
	}

	if (GetKeyboardTrigger(DIK_Z))
	{
		s_camera.posV.y -= 5.0f;
	}
}

//=========================================
// �ݒ�
//=========================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&s_camera.mtxView, &s_camera.posV, &s_camera.posR, &s_camera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &s_camera.mtxView);	// �r���[���W�s��̐ݒ�

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&s_camera.mtxProjection,
		D3DXToRadian(45.0f),						// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// �A�X�y�N�g��
		10.0f,100.0f);								// �ǂ�����(�j�A)(��T����)�ǂ��܂�(�t�@�[)(��U����)���J�����ŕ\�����邩�ݒ� 

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &s_camera.mtxProjection);
}
