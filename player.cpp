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
#include "player.h"
#include "input.h"
#include "setup.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "mesh_field.h"
#include <stdio.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define MODEL_MOVE				(1.0f)
#define MODEL_ROT_ATTENUATION	(0.05f)	// ���Z

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
void LoadPlayer(void);		//�v���C���[�̓ǂݍ��ݏ���

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DTEXTURE9 *s_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
static Player s_player;							// ���f���̍\����
static char s_tex[255];							// ���f���t�@�C��
static MODEL_STATE s_state;						// ���f���̃X�e�[�^�X
static int s_nShadowCnt;						// �e�̊��蓖��

//=========================================
// ������
//=========================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ǂݍ��ݏ���
	LoadPlayer();

	for (int i = 0; i < PARTS_NUM; i++)
	{
		ModelParts* model = &(s_player.aModel[i]);

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(s_tex,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&model->pBuffMat,
			NULL,
			&model->nNumMat,
			&model->pMesh);

		// ���f���̃T�C�Y�v��
		ModelSize(&(s_player.MinVtx), &(s_player.MaxVtx), model->pMesh);

		// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
		s_pTexture = new LPDIRECT3DTEXTURE9[model->nNumMat];

		// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

		// �e���b�V���̃}�e���A�������擾����
		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			s_pTexture[j] = NULL;

			if (pMat[j].pTextureFilename != NULL)
			{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
				D3DXCreateTextureFromFileA(pDevice,
					pMat[j].pTextureFilename,
					&s_pTexture[j]);
			}
			else
			{
				s_pTexture[j] = NULL;
			}
		}
	}

	s_player.aModel[0].pos = ZERO_VECTOR;
	//s_player.aModel[0].pos.y = -s_player.MinVtx.y;
	s_player.aModel[0].rot = ZERO_VECTOR;
	s_player.aModel[0].nIdxModelParent = -1;

	s_player.aModel[1].pos = ZERO_VECTOR;
	s_player.aModel[1].pos.x = -10.0f;
	s_player.aModel[1].pos.y = -s_player.MinVtx.y;
	s_player.aModel[1].rot = ZERO_VECTOR;
	s_player.aModel[1].nIdxModelParent = 0;

	s_player.pos = ZERO_VECTOR;
	s_player.pos.y = -s_player.MinVtx.y;
	s_player.rot = ZERO_VECTOR;
	s_player.movevec = ZERO_VECTOR;
	s_player.quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// �N�H�[�^�j�I��

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = s_player.pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = s_player.pos.z;
	s_nShadowCnt = SetShadow(ShadowPos, s_player.rot);
}

//=========================================
// �I��
//=========================================
void UninitPlayer(void)
{
	if (s_pTexture != NULL)
	{
		for (int i = 0; i < PARTS_NUM; i++)
		{
			for (int j = 0; j < (int)s_player.aModel[i].nNumMat; j++)
			{
				if (s_pTexture[j] != NULL)
				{// �e�N�X�`���̉��
					s_pTexture[j]->Release();
					s_pTexture[j] = NULL;
				}
			}
		}

		delete[](s_pTexture);
		s_pTexture = NULL;
	}

	for (int i = 0; i < PARTS_NUM; i++)
	{
		// ���b�V���̉��
		if (s_player.aModel[i].pMesh != NULL)
		{
			s_player.aModel[i].pMesh->Release();
			s_player.aModel[i].pMesh = NULL;
		}
		// �}�e���A���̉��
		if (s_player.aModel[i].pBuffMat != NULL)
		{
			s_player.aModel[i].pBuffMat->Release();
			s_player.aModel[i].pBuffMat = NULL;
		}
	}
}

//=========================================
// �X�V
//=========================================
void UpdatePlayer(void)
{
	Player* pPlayer = &(s_player);

	pPlayer->pos_old = pPlayer->pos;

	// �ړ�����
	MovePlayer();

	pPlayer->pos += pPlayer->movevec;
	pPlayer->rot.y += NormalizeRot(pPlayer->rotDest.y - pPlayer->rot.y) * MODEL_ROT_ATTENUATION;

	// ���̓����蔻��
	CollisionMeshField(&pPlayer->pos);

	// ���f���̓����蔻��
	CollisionModel(&pPlayer->pos, &pPlayer->pos_old, pPlayer->MinVtx, pPlayer->MaxVtx);

	pPlayer->pos.y += -s_player.MinVtx.y;

	// �p�x�̐��K��
	NormalizeRot(pPlayer->rot.y);

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = pPlayer->pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = pPlayer->pos.z;
	SetPositionShadow(s_nShadowCnt,ShadowPos);
}

//=========================================
// �ړ�
//=========================================
void MovePlayer()
{
	Player* pPlayer = &(s_player);
	D3DXVECTOR3 CameraRot = GetRotCamera();	// �J�����̊p�x���擾
	D3DXVECTOR3 move = ZERO_VECTOR;			// �ړ��ʂ̏�����

											// ���f���̈ړ�
	if (GetKeyboardPress(DIK_UP))
	{
		move.x += sinf(CameraRot.y);
		move.z += cosf(CameraRot.y);
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		move.x -= sinf(D3DX_PI * 0.5f + CameraRot.y);
		move.z -= cosf(D3DX_PI * 0.5f + CameraRot.y);
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		move.x -= sinf(CameraRot.y);
		move.z -= cosf(CameraRot.y);
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		move.x += sinf(D3DX_PI * 0.5f + CameraRot.y);
		move.z += cosf(D3DX_PI * 0.5f + CameraRot.y);
	}

	// ���f���̏㉺�̈ړ�
	if (GetKeyboardPress(DIK_T))
	{
		pPlayer->pos.y += MODEL_MOVE;
	}
	if (GetKeyboardPress(DIK_B))
	{
		pPlayer->pos.y += -(MODEL_MOVE);
	}

	D3DXVECTOR3 axis;

	D3DXVec3Normalize(&move, &move);	// ���K������(�傫���P�̃x�N�g���ɂ���)
	D3DXVec3Cross(&axis, &-move, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	D3DXQUATERNION quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	D3DXQuaternionRotationAxis(&quaternion, &axis, 0.1f);	// ��]���Ɖ�]�p�x���w��

	s_player.quaternion *= quaternion;

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&s_player.quaternion, &s_player.quaternion);

	pPlayer->movevec = move * MODEL_MOVE;
}

//=========================================
// �`��
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX /*mtxScale,*/ mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_player.mtxWorld);

	//// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
	//D3DXMatrixTransformation(
	//	&s_mtxWorld,		// �擾���������[���h�ϊ��s��
	//	NULL,				// �X�P�[�����O�̒��S�_
	//	NULL,				// �X�P�[�����O�̉�]
	//	&scale,				// �����Ƃ̃X�P�[���l
	//	NULL,				// ��]�̒��S���w��
	//	&s_player.quaternion,	// ��]���Ɖ�]�p�x���w��
	//	&s_player.pos		// �X�P�[�����O�Ɖ�]���I��������_������ɃI�t�Z�b�g�i���s�ړ��j
	//);

	// �X�P�[���̔��f
	//D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
	//D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxScale);							// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
	
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
	
	//// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
	//D3DXMatrixRotationQuaternion(&mtxRot,&s_player.quaternion);							// �N�I�[�^�j�I���ɂ��s���]
	//D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);	// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);

	for (int i = 0; i < PARTS_NUM; i++)
	{
		ModelParts* model = &(s_player.aModel[i]);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&model->mtxWorld);

		if ( i == 0)
		{
			// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
			D3DXMatrixRotationQuaternion(&mtxRot, &s_player.quaternion);							// �N�I�[�^�j�I���ɂ��s���]
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		}
		else
		{
			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rot.y, model->rot.x, model->rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		}

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, model->pos.x, model->pos.y, model->pos.z);		// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		D3DXMATRIX mtxParent;
		if (model->nIdxModelParent == -1)
		{
			mtxParent = s_player.mtxWorld;
		}
		else
		{
			mtxParent = s_player.aModel[model->nIdxModelParent].mtxWorld;
		}
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);

		// ���݂̃}�e���A����ێ�
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_pTexture[j]);

			// ���f���p�[�c�̕`��
			model->pMesh->DrawSubset(j);
		}

		// �ێ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//--------------------------------------------------
// �擾
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}

//--------------------------------------------------
// �ǂݍ��ݏ���
//--------------------------------------------------
void LoadPlayer(void)
{
	FILE* pFile;

	pFile = fopen("data/FILE/Player.txt", "r");

	char read[255] = {};

	fscanf(pFile, "%s", &read);
	if (strcmp(&read[0], "PlayerModel") == 0)
	{
		fscanf(pFile, "%s", &read);			// = �̏���
		fscanf(pFile, "%s", &s_tex);		// �l������
	}

	fclose(pFile);
}
