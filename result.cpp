//=========================================
//
// ���U���g����
// Author:Yuda Kaito
//
//=========================================
//------------------------------------
// include
//------------------------------------
#include "result.h"
#include "input.h"
#include "key_config.h"
#include "common.h"
#include "fade.h"
#include "sound.h"
// 2D
#include "result_ui.h"
// 3D
#include "camera.h"
#include "light.h"
#include "model.h"
#include "player.h"
#include "mesh_field.h"
#include "polygon.h"

//------------------------------------
// �}�N����`
//------------------------------------

//=========================================
// �ÓI�ϐ��ϐ�
//=========================================
static RESULT_MODE modeResult;	// ���U���g�̃��[�h
static Player* s_player;		// �v���C���[�f�[�^
static float upSpead;			// ���f���̏㏸�X�s�[�h

//=========================================
// ������
//=========================================
void InitResult(void)
{
	modeResult = RESULT_0;	// ���[�h�̃��Z�b�g

	// ����������
	InitResultUI();		// ���U���gUI
	InitCamera();		// �J����
	InitLight();		// ���C�g
	InitPolygon();		// �|���S��

	// �v���C���[�̐ݒ�
	s_player = GetPlayer();
	s_player->pos = ZERO_VECTOR;
	s_player->pos.y = 50.0f;

	// ���f���̐ݒ�
	for (int i = 0; i < sizeof(s_player->aModel) / sizeof(s_player->aModel[0]); i++)
	{
		if (s_player->aModel[i] == NULL || !s_player->aModel[i]->bUse)
		{
			continue;
		}
		if (s_player->aModel[i]->nIdxModelParent == -2)
		{
			s_player->aModel[i]->bUse = false;
		}

		SetModel(s_player->aModel[i]);	// �ݒ�
	}

	// �|���S���̐ݒ�
	SetPolygon(&D3DXVECTOR3(0.0f, (400.0f * GetCamera(0)->fDistance / 60.0f) - 10.0f, 0.0f), &D3DXVECTOR3(D3DX_PI*-0.5f, 0.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), "data/TEXTURE/sky.png", "wall");

	PlaySound(SOUND_LABEL_BGM_RESULT);
}

//=========================================
// ���U���g�̏I������
//=========================================
void UninitResult(void)
{
	StopSound();
	// �I������
	UninitModel();		// ���f��
	UninitPlayer();		// �v���C���[
	UninitResultUI();	// ���U���gUI
	UninitCamera();		// �J����
	UninitLight();		// ���C�g
	UninitPolygon();	// �|���S��
}

//=========================================
// ���U���g�̍X�V����
//=========================================
void UpdateResult(void)
{
	// �X�V����
	UpdateResultUI();		// ���U���gUI
	UpdateResultCamera();	// �J����
	UpdateLight();			// ���C�g
	UpdateModel();			// ���f��
	UpdatePolygon();		// �|���S��

	switch (modeResult)
	{
	case RESULT_0:

		if (DecisionKey())
		{
			modeResult = RESULT_1;
		}

		break;
	case RESULT_1:
	{
		break;
	}
	default:
		break;
	}

	D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// ��]��

	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &axis, 0.025f);	// ��]���Ɖ�]�p�x���w��

	s_player->aModel[0]->quaternion *= quaternion;
	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&s_player->aModel[0]->quaternion, &s_player->aModel[0]->quaternion);

	if (GetFade() == FADE_NONE && DecisionKey())
	{
		SetFade(MODE_TITLE);	 // �^�C�g����ʂɈڍs
	}
}

//=========================================
// ���U���g�̕`�揈��
//=========================================
void DrawResult(int cameraData)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	if (cameraData == 0)
	{
		// �r���[�{�[�h�̃N���A
		pDevice->SetViewport(&GetCamera(cameraData)->viewport);

		// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
		pDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
		
		// �`�揈��
		SetCamera(cameraData);		// �J����
		DrawModel();				// ���f��
		DrawPlayer();				// �v���C���[
		DrawResultUI();				// ���U���gUI
		DrawPolygon();				// �|���S��
	}
}

//=========================================
// ���U���g�̃��[�h���擾
//=========================================
RESULT_MODE* GetResult(void)
{
	return &modeResult;
}