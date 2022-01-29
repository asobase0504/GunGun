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
// 2D
#include "result_ui.h"
// 3D
#include "camera.h"
#include "light.h"
#include "model.h"
#include "player.h"
#include "mesh_sky.h"
#include "mesh_field.h"

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
	InitMeshSky();		// ���b�V��(��)
	SetMeshSky();		// ���b�V��(��)�̐ݒ�


	// �v���C���[�̃Q�b�g
	s_player = GetPlayer();
	s_player->pos = ZERO_VECTOR;
	s_player->pos.y = 50.0f;

	// ���f��
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

	upSpead = 0.01f;
}

//=========================================
// ���U���g�̏I������
//=========================================
void UninitResult(void)
{
	// �I������
	UninitModel();		// ���f��
	UninitPlayer();		// �v���C���[
	UninitResultUI();	// ���U���gUI
	UninitCamera();		// �J����
	UninitLight();		// ���C�g
	UninitMeshSky();	// ���b�V��(��)
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
	UpdateMeshSky();		// ���b�V��(��)

	switch (modeResult)
	{
	case RESULT_0:

		if (DecisionKey())
		{
			modeResult = RESULT_1;
		}

		break;
	case RESULT_1:

		if (s_player->pos.y <= 500.0f)
		{
			upSpead += 0.01f;	// ���x�㏸
			s_player->pos.y += upSpead * upSpead;
		}

		if (GetFade() == FADE_NONE && DecisionKey())
		{
			SetFade(MODE_TITLE);	 // �^�C�g����ʂɈڍs
		}
		break;
	default:
		break;
	}

}

//=========================================
// ���U���g�̕`�揈��
//=========================================
void DrawResult(void)
{
	// �`�揈��
	SetCamera();		// �J����
	DrawModel();		// ���f��
	DrawPlayer();		// �v���C���[
	DrawMeshSky();		// ���b�V��(��)
	DrawResultUI();		// ���U���gUI
}

//=========================================
// ���U���g�̃��[�h���擾
//=========================================
RESULT_MODE* GetResult(void)
{
	return &modeResult;
}