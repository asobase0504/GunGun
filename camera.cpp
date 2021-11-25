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
	Camera* pCamera = &(s_camera);

	InputCamera();

	// �p�x�̐��K��
	if (pCamera->rot.y > D3DX_PI)
	{
		pCamera->rot.y -= D3DX_PI * 2;
	}
	if (pCamera->rot.y < -D3DX_PI)
	{
		pCamera->rot.y += D3DX_PI * 2;
	}

	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;

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
	Camera* pCamera = &(s_camera);
	float fAngle;

	pCamera->vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���f���̈ړ�
	if (GetKeyboardPress(DIK_W))
	{
		pCamera->vec.x += sinf(pCamera->rot.y);
		pCamera->vec.z += cosf(pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_A))
	{
		pCamera->vec.x += sinf(D3DX_PI * -0.5f + pCamera->rot.y);
		pCamera->vec.z += cosf(D3DX_PI * -0.5f + pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_S))
	{
		pCamera->vec.x += sinf(D3DX_PI + pCamera->rot.y);
		pCamera->vec.z += cosf(D3DX_PI + pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_D))
	{
		pCamera->vec.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y);
		pCamera->vec.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y);
	}

	// �����_�p�x�̉�]
	if (GetKeyboardPress(DIK_Z))
	{
		pCamera->rot.y += CAMERA_ROT_VOLUME;	// ��]��
		pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
		pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
	}
	if (GetKeyboardPress(DIK_C))
	{
		pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// ��]��
		pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
		pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
	}

	// ���_�p�x�̉�]
	if (GetKeyboardPress(DIK_Q))
	{
		pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// ��]��
		pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
		pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;
	}
	if (GetKeyboardPress(DIK_E))
	{
		pCamera->rot.y += CAMERA_ROT_VOLUME;	// ��]��
		pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
		pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;
	}

	// ���_�p�x�̉�]
	if (GetKeyboardPress(DIK_R))
	{
		pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// ��]��
		pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
		pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;
	}
	if (GetKeyboardPress(DIK_V))
	{
		pCamera->rot.y += CAMERA_ROT_VOLUME;	// ��]��
		pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
		pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;
	}

	D3DXVec3Normalize(&pCamera->vec, &pCamera->vec);	// ���K������(�傫���P�̃x�N�g���ɂ���)
	pCamera->vec *= CAMERA_MOVE;
	pCamera->posV += pCamera->vec;
	pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;

	// z = r sin�� cos?
	// x = r sin�� sin(?)
	// y = r cos��
}

//=========================================
// �J�����̊p�x���擾
//=========================================
D3DXVECTOR3 GetRotCamera(void)
{
	return s_camera.rot;
}