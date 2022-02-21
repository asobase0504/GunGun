//=========================================
// 
// �^�C�g���̏���
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "title.h"
#include "input.h"
#include "key_config.h"
#include "fade.h"
#include "common.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "polygon.h"
#include "mesh_field.h"
#include "player.h"
#include "model.h"
#include "sound.h"
#include <assert.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define TITLEPRESS_WIDTH		(620)
#define TITLEPRESS_HEIGHT		(100)
#define TIMEUP_FADE				(90)	// �t�F�[�h�܂ł̎���

//------------------------------------
// �z�u����I�u�W�F�N�g�̎��
//------------------------------------
typedef enum
{
	OBJ_BG,				// �X�N���[���w�i
	OBJ_SELECTBG,		// �I����ʔw�i
	OBJ_TITLE,			// �^�C�g������
	OBJ_GAMESTART,		// GAMESTART����
	OBJ_TUTORIAL,		// TUTORIAL����
	OBJ_EXIT,			// EXIT����
	OBJ_SERECTCURSOR,	// �Z���N�g�J�[�\��
	OBJ_MAX				// 
}OBJ_TYPE;

//------------------------------------
// �I�����ꂽ�I�u�W�F�N�g
//------------------------------------
typedef enum
{
	SELECT_GAMESTART = OBJ_GAMESTART,
	SELECT_TUTORIAL,
	SELECT_EXIT,
}SELECT_MODE;

//------------------------------------
// �ÓI�ϐ��ϐ�
//------------------------------------
static SELECT_MODE s_Select;
static int	s_nCnt;
static bool	s_bFadeCheek;	// �t�F�[�h���u�Ɉڍs���邩�̏���
static int	s_nFadeCnt;		// �t�F�[�h�����ɍs���܂ł̊Ԋu
static bool s_bExit;		// Exit�̃t���O
static bool s_bSelectSE;

//=========================================
// ����������
//=========================================
void InitTitle(void)
{
	s_nCnt = 0;

	InitCamera();		// �J����
	InitLight();		// ���C�g
	InitPolygon();		// �|���S��
	InitMeshField();	// ���b�V���t�B�[���h
	InitModel();		// ���f��
	InitPlayer();		// �v���C���[
	DeleteModel();		// �v���C���[�ȊO�̃��f���̏���

	// �|���S���̐ݒ菈��
	SetPolygon(&D3DXVECTOR3(-40.0f, 1.0f, 25.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f),&D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), "data/TEXTURE/TITLE/Title_00.png", "Title1");
	SetPolygon(&D3DXVECTOR3(-15.0f, 1.0f, 25.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), "data/TEXTURE/TITLE/Title_01.png", "Title2");
	SetPolygon(&D3DXVECTOR3(15.0f, 1.0f, 25.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), "data/TEXTURE/TITLE/Title_00.png", "Title3");
	SetPolygon(&D3DXVECTOR3(40.0f, 1.0f, 25.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), "data/TEXTURE/TITLE/Title_01.png", "Title4");
	if (IsJoyPadUse(0))
	{
		SetPolygon(&D3DXVECTOR3(70.0f, 1.0f, 10.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/LeftStick.png", "operation1");
		SetPolygon(&D3DXVECTOR3(70.0f, 1.0f, 30.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/RightStick.png", "operation2");
	}
	else
	{
		SetPolygon(&D3DXVECTOR3(70.0f, 1.0f, 10.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/QE.png", "operation1");
		SetPolygon(&D3DXVECTOR3(70.0f, 1.0f, 30.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/WASD.png", "operation2");

	}
	SetPolygon(&D3DXVECTOR3(90.0f, 1.0f, 10.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/move.png", "operation3");
	SetPolygon(&D3DXVECTOR3(95.0f, 1.0f, 30.0f), &ZERO_VECTOR, &D3DXVECTOR3(12.5f, 0.0f, 12.5f), &D3DXCOLOR(0.75f, 0.3f, 0.4f, 1.0f), "data/TEXTURE/WORD/CameraMove.png", "operation4");
	SetPolygon(&D3DXVECTOR3(40.0f, 1.0f, -25.0f), &ZERO_VECTOR, &D3DXVECTOR3(25.0f, 0.0f, 9.0f), &D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), "data/TEXTURE/WORD/Exit.png", "exit");
	SetPolygon(&D3DXVECTOR3(-40.0f, 1.0f, -25.0f), &ZERO_VECTOR, &D3DXVECTOR3(25.0f, 0.0f, 9.0f), &D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), "data/TEXTURE/WORD/Start.png", "start");

	// �v���C���[�̐ݒ菈��
	Player* player = GetPlayer();
	player->pos = D3DXVECTOR3(0.0f, 0.0f, -25.0f);

	player->aModel[0]->quaternion = ZERO_QUATERNION;
	D3DXQuaternionRotationYawPitchRoll(&player->aModel[0]->quaternion, D3DX_PI * 0.25f, D3DX_PI * 0.25f, 0.0f);

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&player->aModel[0]->quaternion, &player->aModel[0]->quaternion);

	// ���b�V���t�B�[���h�̐ݒ菈��
	SetMesh setMesh;
	setMesh.file = NULL;
	setMesh.fLineHeight = 50.0f;
	setMesh.fLineWidth = 50.0f;
	setMesh.nSurfaceHeight = 30;
	setMesh.nSurfaceWidth = 30;
	setMesh.pos = ZERO_VECTOR;
	setMesh.rot = ZERO_VECTOR;
	SetMeshField(&setMesh);

	PlaySound(SOUND_LABEL_BGM_TITLE);
}

//=========================================
// �I������
//=========================================
void UninitTitle(void)
{
	StopSound();
	UninitCamera();		// �J����
	UninitLight();		// ���C�g
	UninitPolygon();	// �|���S��
	UninitMeshField();	// ���b�V���t�B�[���h
	UninitModel();		// ���f��
	UninitPlayer();		// �v���C���[
}

//=========================================
// �X�V����
//=========================================
void UpdateTitle(void)
{

	if (CollisionPolygon(&GetPlayer()->pos, "Title1") && !GetPolygon("Title1")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("Title1");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "Title2") && !GetPolygon("Title2")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("Title2");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "Title3") && !GetPolygon("Title3")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("Title3");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "Title4") && !GetPolygon("Title4")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("Title4");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "operation1") && !GetPolygon("operation1")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("operation1");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "operation2") && !GetPolygon("operation2")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("operation2");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "operation3") && !GetPolygon("operation3")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("operation3");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "operation4") && !GetPolygon("operation4")->HitPlayer)
	{
		ObjectPolygon* polygon = GetPolygon("operation4");
		D3DXMATRIX mtxRot;
		D3DXVECTOR3 pos_local = polygon->pos - GetPlayer()->pos_old;
		D3DXVECTOR3 v = ZERO_VECTOR;

		D3DXQUATERNION quaternionHit = GetPlayer()->aModel[0]->quaternion;
		quaternionHit.w *= -1;

		// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
		D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
		D3DXVec3TransformCoord(&polygon->pos, &pos_local, &mtxRot);

		polygon->quaternion *= quaternionHit;
		polygon->HitPlayer = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "start") && s_nCnt < TIMEUP_FADE)
	{	// �v���C���[���w�肳�ꂽ�|���S���ɏ���Ă�����J�E���g��i�߂�

		s_Select = SELECT_GAMESTART;
		s_nCnt++;
		GetPolygon("start")->col = D3DXCOLOR((float)s_nCnt / (float)TIMEUP_FADE, 0.0f, 0.0f, 1.0f);
		s_bSelectSE = true;
	}
	else if (CollisionPolygon(&GetPlayer()->pos, "exit") && s_nCnt < TIMEUP_FADE)
	{
		s_Select = SELECT_EXIT;
		s_nCnt++;
		GetPolygon("exit")->col = D3DXCOLOR((float)s_nCnt / (float)TIMEUP_FADE, 0.0f, 0.0f, 1.0f);
		s_bSelectSE = true;
	}
	else if(s_nCnt < TIMEUP_FADE)
	{
		s_nCnt = 0;
		GetPolygon("start")->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		GetPolygon("exit")->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		StopSound(SOUND_LABEL_SE_CHARGE);
		s_bSelectSE = false;
	}

	if (s_bSelectSE && s_nCnt == 1)
	{
		PlaySound(SOUND_LABEL_SE_CHARGE);
	}

	// ���ȏ�̒l�ɂȂ��������Ă������ڂɈȍ~����
	if (s_nCnt >= TIMEUP_FADE)
	{
		StopSound(SOUND_LABEL_SE_CHARGE);

		if (s_nCnt == TIMEUP_FADE)
		{
			// ���̂���Ȃ��B
			PlaySound(SOUND_LABEL_SE_SERECT);
			s_nCnt++;
		}
		
		switch (s_Select)
		{
		case SELECT_GAMESTART:
			SetFade(MODE_GAME);	// �Q�[�����[�h�Ɉڍs
			break;
		case SELECT_EXIT:
			s_bExit = true;
			break;
		default:
			break;
		}
	}
	else
	{
		UpdateCamera();		// �J����
		UpdateLight();		// ���C�g
		UpdatePolygon();	// �|���S��
		UpdateMeshField();	// ���b�V���t�B�[���h
		UpdateModel();		// ���f��
		UpdatePlayer();		// �v���C���[

		// �v���C���[�̈ړ�����
		Player* player = GetPlayer();
		if (player->pos.x < -130.0f)
		{
			player->pos.x = 120.0f;
		}
		else if (player->pos.x > 130.0f)
		{
			player->pos.x = -120.0f;
		}
		if (player->pos.z < -80.0f)
		{
			player->pos.z = 70.0f;
		}
		else if (player->pos.z > 80.0f)
		{
			player->pos.z = -70.0f;
		}
	}
}

//=========================================
// �I������
//=========================================
void SelectTitle(void)
{

}

//=========================================
// �`�揈��
//=========================================
void DrawTitle(int cameraData)
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

		SetCamera(cameraData);	// �J����
		DrawMeshField();		// ���b�V���t�B�[���h
		DrawPolygon();			// �|���S��
		DrawModel();		// ���f��
		DrawPlayer();			// �v���C���[
	}
}

//=========================================
// �I�������擾
//=========================================
bool GetExit(void)
{
	return s_bExit;
}
