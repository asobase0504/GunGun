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
static Camera s_camera[2];	// �J�������

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
	s_camera[0].posV = D3DXVECTOR3(0.0f,50.0f,-60.0f);	// ���_
	s_camera[0].posR = D3DXVECTOR3(0.0f,0.0f,0.0f);	// �����_
	s_camera[0].vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);	// ������x�N�g��
	s_camera[0].rot = ZERO_VECTOR;	// ����

	s_camera[0].rot.y = atan2f((s_camera[0].posR.x - s_camera[0].posV.x), (s_camera[0].posR.z - s_camera[0].posV.z));
	s_camera[0].rot.x = atan2f((s_camera[0].posR.z - s_camera[0].posV.z), (s_camera[0].posR.y - s_camera[0].posV.y));

	D3DXVECTOR3 vec = s_camera[0].posV - s_camera[0].posR;
	s_camera[0].fDistance = D3DXVec3Length(&vec);

	s_camera[0].viewport.X = (DWORD)0.0f;
	s_camera[0].viewport.Y = (DWORD)0.0f;
	s_camera[0].viewport.Width = SCREEN_WIDTH;
	s_camera[0].viewport.Height = SCREEN_HEIGHT;

	s_camera[1].posV = D3DXVECTOR3(0.0f, -200.0f, -60.0f);	// ���_
	s_camera[1].posR = D3DXVECTOR3(0.0f, -200.0f, 0.0f);	// �����_
	s_camera[1].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		// ������x�N�g��
	s_camera[1].rot = ZERO_VECTOR;	// ����
			 
	s_camera[1].rot.y = atan2f((s_camera[1].posR.x - s_camera[1].posV.x), (s_camera[1].posR.z - s_camera[1].posV.z));
	s_camera[1].rot.x = atan2f((s_camera[1].posR.z - s_camera[1].posV.z), (s_camera[1].posR.y - s_camera[1].posV.y));

	vec = s_camera[1].posV - s_camera[1].posR;
	s_camera[1].fDistance = D3DXVec3Length(&vec);

	s_camera[1].viewport.X = (DWORD)0.0f;
	s_camera[1].viewport.Y = (DWORD)0.0f;
	s_camera[1].viewport.Width = (DWORD)80.0f * 8.0f;
	s_camera[1].viewport.Height = (DWORD)80.0f * 4.5f;

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
	Camera* pCamera = &(s_camera[0]);

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
	Camera* pCamera = &(s_camera[0]);

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
void SetCamera(int nData)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Camera* camara = &(s_camera[nData]);

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
		D3DXToRadian(60.0f),						// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// �A�X�y�N�g��
		10.0f,400.0f * camara->fDistance / 60.0f);								// �ǂ�����(�j�A)(��T����)�ǂ��܂�(�t�@�[)(��U����)���J�����ŕ\�����邩�ݒ� 

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &camara->mtxProjection);
}

//=========================================
// ����
//=========================================
void InputCamera(void)
{
	Camera* pCamera = &(s_camera[0]);

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
Camera* GetCamera(int nData)
{
	return &s_camera[nData];
}

//=========================================
// �J�����̊p�x���擾
//=========================================
D3DXVECTOR3 GetRotCamera(void)
{
	return s_camera[0].rot;
}