//=========================================
// 
// �J�����̍쐬
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "camera.h"
#include "input.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define CAMERA_MOVE			(1.0f)
#define CAMERA_ROT_VOLUME	(0.03f)

//-----------------------------------------
// �ÓI�ϐ�
//-----------------------------------------
static Camera s_camera;	// �J�������
static float rotY;

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
static void InputCamera(void);	// �J�����̓��͏���

//=========================================
// ������
//=========================================
void InitCamera(void)
{
	// ���_�E�����_�E�������ݒ肷��
	s_camera.posV = D3DXVECTOR3(0.0f,50.0f,-40.0f);	// ���_
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �����_
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);	// ������x�N�g��
	s_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//s_camera.rot.y = atan2f((s_camera.posV.x - s_camera.posR.x), (s_camera.posV.z - s_camera.posR.z));

	s_camera.fDistance = sqrtf((s_camera.posV.x - s_camera.posR.x) * (s_camera.posV.x - s_camera.posR.x) + (s_camera.posV.z - s_camera.posR.z) * (s_camera.posV.z - s_camera.posR.z));
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
	Camera* pCamara = &(s_camera);

	InputCamera();

	// �p�x�̐��K��
	if (pCamara->rot.y > D3DX_PI)
	{
		pCamara->rot.y -= D3DX_PI * 2;
	}
	if (pCamara->rot.y < -D3DX_PI)
	{
		pCamara->rot.y += D3DX_PI * 2;
	}

	pCamara->posV.x = pCamara->posR.x - sinf(pCamara->rot.y) * pCamara->fDistance;
	pCamara->posV.z = pCamara->posR.z - cosf(pCamara->rot.y) * pCamara->fDistance;

}

//=========================================
// �ݒ�
//=========================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Camera* camara = &(s_camera);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&camara->mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&camara->mtxView, &camara->posV, &camara->posR, &camara->vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &camara->mtxView);	// �r���[���W�s��̐ݒ�

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&camara->mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&camara->mtxProjection,
		D3DXToRadian(45.0f),						// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// �A�X�y�N�g��
		10.0f,100.0f);								// �ǂ�����(�j�A)(��T����)�ǂ��܂�(�t�@�[)(��U����)���J�����ŕ\�����邩�ݒ� 

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &camara->mtxProjection);
}

//=========================================
// ����
//=========================================
void InputCamera(void)
{
	Camera* pCamara = &(s_camera);
	float fAngle;

	// �J�����̈ړ�
	if (GetKeyboardPress(DIK_W))
	{
		if (GetKeyboardPress(DIK_A))
		{
			fAngle = D3DX_PI * -0.25f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
		else if (GetKeyboardPress(DIK_S))
		{
		}
		else if (GetKeyboardPress(DIK_D))
		{
			fAngle = D3DX_PI * 0.25f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
		else
		{
			fAngle = pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_A))
	{
		if (GetKeyboardPress(DIK_S))
		{
			fAngle = D3DX_PI * -0.75f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
		else if (GetKeyboardPress(DIK_D))
		{

		}
		else
		{
			fAngle = D3DX_PI * -0.5f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_S))
	{
		if (GetKeyboardPress(DIK_D))
		{
			fAngle = D3DX_PI * 0.75f + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
		else
		{
			fAngle = D3DX_PI + pCamara->rot.y;
			pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
			pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_D))
	{
		fAngle = D3DX_PI * 0.5f + pCamara->rot.y;
		pCamara->posV.x += sinf(fAngle) * CAMERA_MOVE;
		pCamara->posV.z += cosf(fAngle) * CAMERA_MOVE;
	}
	pCamara->posR.x = pCamara->posV.x + sinf(pCamara->rot.y) * pCamara->fDistance;
	pCamara->posR.z = pCamara->posV.z + cosf(pCamara->rot.y) * pCamara->fDistance;

	// �����_�p�x�̉�]
	if (GetKeyboardPress(DIK_Z))
	{
		pCamara->rot.y += -(CAMERA_ROT_VOLUME);	// ��]��
		pCamara->posV.x = pCamara->posR.x - sinf(pCamara->rot.y) * pCamara->fDistance;
		pCamara->posV.z = pCamara->posR.z - cosf(pCamara->rot.y) * pCamara->fDistance;
	}
	if (GetKeyboardPress(DIK_C))
	{
		pCamara->rot.y += CAMERA_ROT_VOLUME;	// ��]��
		pCamara->posV.x = pCamara->posR.x - sinf(pCamara->rot.y) * pCamara->fDistance;
		pCamara->posV.z = pCamara->posR.z - cosf(pCamara->rot.y) * pCamara->fDistance;
	}

	// ���_�p�x�̉�]
	if (GetKeyboardPress(DIK_Q))
	{
		pCamara->rot.y += -(CAMERA_ROT_VOLUME);	// ��]��
		pCamara->posR.x = pCamara->posV.x + sinf(pCamara->rot.y) * pCamara->fDistance;
		pCamara->posR.z = pCamara->posV.z + cosf(pCamara->rot.y) * pCamara->fDistance;
	}
	if (GetKeyboardPress(DIK_E))
	{
		pCamara->rot.y += CAMERA_ROT_VOLUME;	// ��]��
		pCamara->posR.x = pCamara->posV.x + sinf(pCamara->rot.y) * pCamara->fDistance;
		pCamara->posR.z = pCamara->posV.z + cosf(pCamara->rot.y) * pCamara->fDistance;
	}
}

//=========================================
// �J�����̊p�x���擾
//=========================================
D3DXVECTOR3 GetRotCamera(void)
{
	return s_camera.rot;
}