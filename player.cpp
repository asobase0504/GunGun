//=========================================
// 
// �v���C���[�����֐�
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "common.h"
#include "colision.h"
#include "player.h"
#include "input.h"
#include "line.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "mesh_field.h"
#include "sound.h"
#include <stdio.h>
#include <math.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define PLAYER_MOVE				(1.5f)	// �v���C���[�̈ړ���
#define MODEL_ROT_ATTENUATION	(0.125f)	// ���Z
#define MODEL_LOAD_FILE			("data/model.txt")

//------------------------------------
// ���f���X�e�[�^�X�񋓌^
//------------------------------------
typedef enum
{
	MODELSTATEL_NONE = 0,
	MODELSTATEL_MOVE,
	MODELSTATEL_MAX
}MODEL_STATE;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void ColisionPartsModel(void);	// ���f���p�[�c���m�̓����蔻��
void LookUpSizePlayer(void);	// �v���C���[�̃T�C�Y�𒲂ׂ�

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static Player s_player;			// ���f���̍\����
static MODEL_STATE s_state;		// ���f���̃X�e�[�^�X
static int s_nShadowCnt;		// �e�̊��蓖��
static Model s_getModel;		// �����������f��

//=========================================
// ������
//=========================================
void InitPlayer(void)
{
	ZeroMemory(&s_player, sizeof(s_player));
	ZeroMemory(&s_getModel, sizeof(s_getModel));

	// �v���C���[�ɂ��������f���̔z�u
	LoadModel();
	LoadMap();

	s_player.fLength += s_player.aModel[0]->fLength;
	s_player.pos.y = -s_player.MinVtx.y;
	s_player.aModel[0]->quaternion = ZERO_QUATERNION;	// �N�H�[�^�j�I��

	if (s_player.pos.y - s_player.fLength <= 0.0f)
	{
		s_player.pos.y = s_player.fLength;
	}

	for (int i = 0; i < sizeof(s_player.aModel) / sizeof(s_player.aModel[0]); i++)
	{
		Model* model = s_player.aModel[i];

		if (model == NULL || !(model->bUse))
		{
			continue;
		}
		model->pos_world = D3DXVECTOR3(model->mtxWorld._41, model->mtxWorld._42, model->mtxWorld._43);
	}

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_player.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);	// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);
}

//=========================================
// �I��
//=========================================
void UninitPlayer(void)
{
}

//=========================================
// �X�V
//=========================================
void UpdatePlayer(void)
{
	Player* pPlayer = &(s_player);

	pPlayer->pos_old = pPlayer->pos;	// �v���C���[�ʒu�̕ۑ�

	// ���f���ʒu�̕ۑ�
	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* model = pPlayer->aModel[i];

		if (model == NULL)
		{
			continue;
		}
		model->pos_old = model->pos;

		model->pos_world = D3DXVECTOR3(model->mtxWorld._41, model->mtxWorld._42, model->mtxWorld._43);
	}

	// �ړ�����
	MovePlayer();
	
	// �v���C���[�Ə��̓����蔻��
	//CollisionMeshField(&pPlayer->pos);

	// ���f���p�[�c���Ƃ̓����蔻��
	ColisionPartsModel();

	// �v���C���[�̋��̔��a�����߂�
	LookUpSizePlayer();
	
	// �n�ʂ܂ł̗�������
	pPlayer->pos.y -= 1.5f;
	if (pPlayer->pos.y - pPlayer->fLength <= 0.0f)
	{
		pPlayer->pos.y = pPlayer->fLength;
	}

	// �p�x�̐��K��
	NormalizeRot(&pPlayer->rot.y);

	SetModelUI(&s_getModel);
}

//=========================================
// �ړ�
//=========================================
void MovePlayer()
{
	D3DXVECTOR3 move = ZERO_VECTOR;			// �ړ��ʂ̏�����
	float moveLength = 0.0f;
	D3DXVECTOR2 moveInput;

	if (IsJoyPadUse(0))
	{// �W���C�p�b�h�̎g�p
		moveInput.x = GetJoypadStick(JOYKEY_LEFT_STICK, 0).x;
		moveInput.y = -GetJoypadStick(JOYKEY_LEFT_STICK, 0).y;

		if (moveInput.x != 0.0f || moveInput.y != 0.0f)
		{
			moveLength = D3DXVec2Length(&moveInput);

			if (moveLength > 1.0f)
			{
				moveLength = 1.0f;
			}
		}
	}
	else
	{
		moveInput.x = 0.0f;
		moveInput.y = 0.0f;

		// ���f���̈ړ�
		if (GetKeyboardPress(DIK_W))
		{
			moveInput.y += 1.0f;
			moveLength = 1.0f;
		}
		if (GetKeyboardPress(DIK_A))
		{
			moveInput.x -= 1.0f;
			moveLength = 1.0f;
		}
		if (GetKeyboardPress(DIK_S))
		{
			moveInput.y -= 1.0f;
			moveLength = 1.0f;
		}
		if (GetKeyboardPress(DIK_D))
		{
			moveInput.x += 1.0f;
			moveLength = 1.0f;
		}
	}

	if (moveLength > 0.0f)
	{
		// �J�����̊p�x���擾
		D3DXVECTOR3* CameraRot = GetRotCamera();

		D3DXVec2Normalize(&moveInput, &moveInput);

		float c = cosf(-CameraRot->y);
		float s = sinf(-CameraRot->y);

		// move �̒����� 1 �ɂȂ�B
		move.x = moveInput.x * c - moveInput.y * s;
		move.z = moveInput.x * s + moveInput.y * c;
	}
	else
	{ // ���͂���Ă��Ȃ��B
		return;
	}

	D3DXVECTOR3 axis;	// ��]��
	D3DXVECTOR3 inverseVec = -move;		// move�l�𔽑΂ɂ���
	D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&axis, &inverseVec, &vecY);	// �O�ςŉ�]�����Z�o�B

	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &axis, moveLength * MODEL_ROT_ATTENUATION);	// ��]���Ɖ�]�p�x���w��

	s_player.aModel[0]->quaternion *= quaternion;
	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&s_player.aModel[0]->quaternion, &s_player.aModel[0]->quaternion);

	// �����x�N�g���|����ړ���
	s_player.movevec = move * moveLength * PLAYER_MOVE * 2.0f * ((s_player.fLength / 24.0f + 1) * 0.5f);
	s_player.pos += s_player.movevec;
}

//=========================================
// ���f���p�[�c�̏���
//=========================================
void DeleteModel(void)
{
	for (int j = 0; j < MODEL_MAX; j++)
	{
		Model* model = s_player.aModel[j];

		if (model == NULL)
		{
			continue;
		}

		if (model->nIdxModelParent == -2)
		{
			model->bUse = false;
		}
	}
}

//=========================================
// ���f���p�[�c���m�̓����蔻��
//=========================================
void ColisionPartsModel(void)
{
	for (int j = 0; j < MODEL_MAX; j++)
	{
		Model* model = s_player.aModel[j];

		if (model == NULL || !(model->bUse) || model->nIdxModelParent != -2)
		{
			continue;
		}

		// ���������ꍇ
		if ((SphereColision(s_player.aModel[0]->pos_world, s_player.fLength, model->pos_world, model->fLength) && model->typeCollision == COLLISION_SPHERE))
		{
			if (s_player.fLength >= model->sizeCriter)
			{	// ��荞�߂�T�C�Y�̏ꍇ
				PlaySound(SOUND_LABEL_SE_GETITEM);
				D3DXMATRIX mtxRot;
				D3DXVECTOR3 pos_local = model->pos_world - s_player.pos_old;
				D3DXVECTOR3 v = ZERO_VECTOR;

				D3DXQUATERNION quaternionHit = s_player.aModel[0]->quaternion;
				quaternionHit.w *= -1;

				// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
				D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
				D3DXVec3TransformCoord(&model->pos, &pos_local, &mtxRot);

				model->quaternion *= quaternionHit;
				model->nIdxModelParent = 0;

				// �v���C���[�̒������K��l���Z
				s_player.fLength += model->sizeAdd;

				s_player.nModelCnt++;
				s_getModel = *model;
			}
			else
			{	// ��荞�߂Ȃ��T�C�Y�̏ꍇ
				D3DXVECTOR3 vec = (model->pos_world - s_player.pos);	// �����x�N�g��
				D3DXVec3Normalize(&vec, &vec);						// ���������������擾

				// ���x��0�ɂ���
				s_player.movevec = ZERO_VECTOR;

				s_player.pos -= vec;

				// ��鏈��
				s_player.pos.y += 1.0f;
			}
		}
	}
}

//--------------------------------------------------
// �v���C���[�̃T�C�Y�𒲂ׂ�
//--------------------------------------------------
void LookUpSizePlayer(void)
{
	for (int i = 0; i < sizeof(s_player.aModel) / sizeof(s_player.aModel[0]); i++)
	{
		Model* model = s_player.aModel[i];

		if (model == NULL)
		{
			continue;
		}
		if (model->nIdxModelParent == -2)
		{
			continue;
		}

		D3DXVECTOR3 modelMaxVtx = model->pos + model->MaxVtx;
		D3DXVECTOR3 modelMinVtx = model->pos + model->MinVtx;

		if (modelMinVtx.x < s_player.MinVtx.x)
		{
			s_player.MinVtx.x = modelMinVtx.x;
		}
		if (modelMinVtx.y < s_player.MinVtx.y)
		{
			s_player.MinVtx.y = modelMinVtx.y;
		}
		if (modelMinVtx.z < s_player.MinVtx.z)
		{
			s_player.MinVtx.z = modelMinVtx.z;
		}
		if (modelMinVtx.x > s_player.MaxVtx.x)
		{
			s_player.MaxVtx.x = modelMinVtx.x;
		}
		if (modelMinVtx.y > s_player.MaxVtx.y)
		{
			s_player.MaxVtx.y = modelMinVtx.y;
		}
		if (modelMinVtx.z > s_player.MaxVtx.z)
		{
			s_player.MaxVtx.z = modelMinVtx.z;
		}

		if (modelMaxVtx.x < s_player.MinVtx.x)
		{
			s_player.MinVtx.x = modelMaxVtx.x;
		}
		if (modelMaxVtx.y < s_player.MinVtx.y)
		{
			s_player.MinVtx.y = modelMaxVtx.y;
		}
		if (modelMaxVtx.z < s_player.MinVtx.z)
		{
			s_player.MinVtx.z = modelMaxVtx.z;
		}
		if (modelMaxVtx.x > s_player.MaxVtx.x)
		{
			s_player.MaxVtx.x = modelMaxVtx.x;
		}
		if (modelMaxVtx.y > s_player.MaxVtx.y)
		{
			s_player.MaxVtx.y = modelMaxVtx.y;
		}
		if (modelMaxVtx.z > s_player.MaxVtx.z)
		{
			s_player.MaxVtx.z = modelMaxVtx.z;
		}
	}

	D3DXVECTOR3 v = s_player.MaxVtx - s_player.MinVtx;
	//s_player.fLength = sqrtf((v.x + v.y + v.z) / 6.0f - s_player.aModel[0]->MaxVtx.x) + s_player.aModel[0]->MaxVtx.x;
}

//=========================================
// �`��
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX /*mtxScale,*/ mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_player.mtxWorld);
	
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);	// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}

//--------------------------------------------------
// ���f���擾
//--------------------------------------------------
Model** GetPlayerModel(void)
{
	return s_player.aModel;
}