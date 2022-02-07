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
//void LoadPlayer(void);			// �v���C���[�̓ǂݍ��ݏ���
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

	// �v���C���[�ɂ��������f���̔z�u
	LoadModel();
	LoadMap();
	// �ǂݍ��ݏ���
	//LoadPlayer();

	s_player.fLength += s_player.aModel[0]->fLength;
	s_player.pos = ZERO_VECTOR;
	s_player.pos.y = -s_player.MinVtx.y;
	s_player.rot = ZERO_VECTOR;
	s_player.movevec = ZERO_VECTOR;
	s_player.aModel[0]->quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// �N�H�[�^�j�I��

	for (int i = 0; i < sizeof(s_player.aModel) / sizeof(s_player.aModel[0]); i++)
	{
		Model* model = s_player.aModel[i];

		if (model == NULL)
		{
			continue;
		}
		if (!(model->bUse))
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
	for (int i = 0; i < PARTS_NUM; i++)
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
	
	/// ���̓ʉ���\�����邽�߂̓����蔻��
	//float fData = 0.0f;
	//for (int i = 0; i < PARTS_NUM; i++)
	//{
	//	Model* model = pPlayer->aModel[i];
	//
	//	if (model == NULL || !model->bUse || model->nIdxModelParent != 0)
	//	{
	//		continue;
	//	}
	//
	//	if (model->pos_world.y + pPlayer->fLengthLand * 2.0f < fData)
	//	{
	//		fData = model->pos_world.y + pPlayer->fLengthLand * 2.0f;
	//	}
	//}
	//pPlayer->fLengthLand = fData;

	//pPlayer->pos.y = pPlayer->fLength;	// �v���C���[�̈ʒu���グ�B
	
	//// ��n�܂ł̗�������
	pPlayer->pos.y -= 0.5f;
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
			if (GetKeyboardPress(DIK_UP))
			{
				moveInput.y += 1.0f;
				moveLength = 1.0f;
			}
			if (GetKeyboardPress(DIK_LEFT))
			{
				moveInput.x -= 1.0f;
				moveLength = 1.0f;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{
				moveInput.y -= 1.0f;
				moveLength = 1.0f;
			}
			if (GetKeyboardPress(DIK_RIGHT))
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
	D3DXQuaternionRotationAxis(&quaternion, &axis,  moveLength * MODEL_ROT_ATTENUATION);	// ��]���Ɖ�]�p�x���w��

	s_player.aModel[0]->quaternion *= quaternion;
	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&s_player.aModel[0]->quaternion, &s_player.aModel[0]->quaternion);

	// �����x�N�g���|����ړ���
	s_player.movevec = move * moveLength * PLAYER_MOVE;
	s_player.pos += s_player.movevec;
}

//=========================================
// ���f���p�[�c�̏���
//=========================================
void DeleteModel(void)
{
	for (int j = 0; j < PARTS_NUM; j++)
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
//	Model* model = &(s_player.aModel[0]);

	for (int j = 0; j < PARTS_NUM; j++)
	{
		Model* model = s_player.aModel[j];

		if (model == NULL)
		{
			continue;
		}
		if (!(model->bUse) || model->nIdxModelParent != -2)
		{
			continue;
		}

		// ���������ꍇ
		if ((SphereColision(s_player.aModel[0]->pos_world, s_player.fLength, model->pos_world, model->fLength) && model->typeCollision == COLLISION_SPHERE) ||
			(SphereCuboidColision(s_player.aModel[0]->pos_world, s_player.fLength, model->pos_world, model->size) && model->typeCollision == COLLISION_CUBOID))
		{
			if (s_player.fLength >= model->sizeCriter)
			{	// ��荞�߂�T�C�Y�̏ꍇ
				D3DXMATRIX mtxRot;
				D3DXVECTOR3 pos_local = model->pos_world - s_player.pos_old;
				D3DXVECTOR3 v = ZERO_VECTOR;

				D3DXQUATERNION quaternionHit = s_player.aModel[0]->quaternion;
				quaternionHit.w *= -1;

				// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
				D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// �N�I�[�^�j�I���ɂ��s���]
				D3DXVec3TransformCoord(&model->pos, &pos_local, &mtxRot);

				model->quaternion = quaternionHit;
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
				//s_player.pos.y += 1.0f;
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

	//// �X�P�[���̔��f
	//D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
	//D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxScale);		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
	
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);	// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);
}

#if 0
//--------------------------------------------------
// �ǂݍ��ݏ���
//--------------------------------------------------
void LoadPlayer(void)
{
	FILE* pFile = fopen("data/FILE/Player.txt", "r");
	char modelFile[255] = {};	// ���f���t�@�C��

	fscanf(pFile, "%s", &modelFile);
	if (strcmp(modelFile, "PlayerModel") == 0)
	{
		fscanf(pFile, "%s", &modelFile);	// = �̏���
		fscanf(pFile, "%s", &modelFile);	// �l������
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Model* model = s_player.aModel[0];

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(modelFile,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&model->pBuffMat,
		NULL,
		&model->nNumMat,
		&model->pMesh);

	// ���f���̃T�C�Y�v��
	ModelSize(&(s_player.MinVtx), &(s_player.MaxVtx), model->pMesh);
	// ���f���̃T�C�Y�v��
	ModelSize(&model->MinVtx, &model->MaxVtx, model->pMesh);

	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
	model->pTexture = new LPDIRECT3DTEXTURE9[model->nNumMat];

	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

	// �e���b�V���̃}�e���A�������擾����
	for (int j = 0; j < (int)model->nNumMat; j++)
	{
		model->pTexture[j] = NULL;

		if (pMat[j].pTextureFilename != NULL)
		{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFileA(pDevice,
				pMat[j].pTextureFilename,
				&model->pTexture[j]);
		}
	}

	s_player.aModel[0]->pos = ZERO_VECTOR;
	//s_player.aModel[0].pos.y = -s_player.MinVtx.y;
	s_player.aModel[0]->rot = ZERO_VECTOR;
	s_player.aModel[0]->nIdxModelParent = -1;
	s_player.aModel[0]->bUse = true;
}
#endif

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
