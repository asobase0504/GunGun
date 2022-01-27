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
#include "player.h"
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

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
static void InputCamera(void);	// �J�����̓��͏���

//=========================================
// ������
//=========================================
void InitCamera(void)
{
	ZeroMemory(&s_camera, sizeof(s_camera));

	// ���_�E�����_�E�������ݒ肷��
	s_camera.posV = D3DXVECTOR3(0.0f,50.0f,-40.0f);	// ���_
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �����_
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);	// ������x�N�g��
	s_camera.rot = ZERO_VECTOR;	// ����

	s_camera.rot.y = atan2f((s_camera.posR.x - s_camera.posV.x),(s_camera.posR.z - s_camera.posV.z));
	s_camera.rot.x = atan2f((s_camera.posR.z - s_camera.posV.z),(s_camera.posR.y - s_camera.posV.y));

	D3DXVECTOR3 vec = s_camera.posV - s_camera.posR;
	s_camera.fDistance = D3DXVec3Length(&vec);
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

}

//=========================================
// �Q�[�����̍X�V
//=========================================
void UpdateGameCamera(void)
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

	// �Ǐ]����
	Player *player = GetPlayer();

	// �J�����̒Ǐ]����
	pCamera->posRDest.x = player->pos.x + sinf(player->movevec.x) * 40.0f;
	pCamera->posRDest.y = player->pos.y;
	pCamera->posRDest.z = player->pos.z + sinf(player->movevec.z) * 40.0f;

	pCamera->posR += (pCamera->posRDest - pCamera->posR) * 0.05f;
	pCamera->posR.y = player->pos.y;

	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;

	if (GetKeyboardPress(DIK_9))
	{
		pCamera->fDistance++;
	}
	if (GetKeyboardPress(DIK_0))
	{
		pCamera->fDistance--;
	}
}

//=========================================
// ���U���g���̍X�V
//=========================================
void UpdateResultCamera(void)
{
	Camera* pCamera = &(s_camera);

	Player *player = GetPlayer();

	// �J�����̒Ǐ]����
	pCamera->posR.x += (player->pos.x - pCamera->posR.x) * 0.05f;
	pCamera->posR.z += (player->pos.z - pCamera->posR.z) * 0.05f;

	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;

	pCamera->posR.y = player->pos.y;
	//// ���X�ɏ�ɏオ��
	//if (pCamera->posR.y <= 75.0f)
	//{
	//	pCamera->posR.y += 0.5f;	// �����_
	//}

	// �p�x�̐��K��
	if (pCamera->rot.y > D3DX_PI)
	{
		pCamera->rot.y -= D3DX_PI * 2;
	}
	if (pCamera->rot.y < -D3DX_PI)
	{
		pCamera->rot.y += D3DX_PI * 2;
	}
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
		10.0f,400.0f);								// �ǂ�����(�j�A)(��T����)�ǂ��܂�(�t�@�[)(��U����)���J�����ŕ\�����邩�ݒ� 

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &camara->mtxProjection);
}

//=========================================
// ����
//=========================================
void InputCamera(void)
{
	Camera* pCamera = &(s_camera);

	pCamera->vec = ZERO_VECTOR;

	if (IsJoyPadUse(0))
	{// �W���C�p�b�h�̎g�p
		if (GetJoypadPress(JOYKEY_RIGHT_SHOULDER))
		{
			pCamera->rot.y += CAMERA_ROT_VOLUME;	// ��]��
			pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
			pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
		}
		if (GetJoypadPress(JOYKEY_LEFT_SHOULDER))
		{
			pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// ��]��
			pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
			pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
		}
	}
	else
	{
		// �J�����̈ړ�
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
		}
		if (GetKeyboardPress(DIK_E))
		{
			pCamera->rot.y += CAMERA_ROT_VOLUME;	// ��]��
		}
		if (GetKeyboardPress(DIK_R))
		{
			pCamera->rot.x += -(CAMERA_ROT_VOLUME);	// ��]��
		}
		if (GetKeyboardPress(DIK_V))
		{
			pCamera->rot.x += CAMERA_ROT_VOLUME;	// ��]��
		}
	}

	D3DXVec3Normalize(&pCamera->vec, &pCamera->vec);	// ���K������(�傫���P�̃x�N�g���ɂ���)
	pCamera->vec *= CAMERA_MOVE;
	pCamera->posV += pCamera->vec;

	pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.y = pCamera->posV.y + tanf(-pCamera->rot.x + (D3DX_PI * 0.5f)) * pCamera->fDistance;
}

//=========================================
// �J�����̏��擾
//=========================================
Camera* GetCamera(void)
{
	return &(s_camera);
}

//=========================================
// �J�����̊p�x���擾
//=========================================
D3DXVECTOR3 GetRotCamera(void)
{
	return s_camera.rot;
}