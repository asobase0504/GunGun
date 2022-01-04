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
void LoadPlayer(void);		//�v���C���[�̓ǂݍ��ݏ���

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
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
		Model* model = &(s_player.aModel[i]);

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
	}

	s_player.aModel[0].pos = ZERO_VECTOR;
	//s_player.aModel[0].pos.y = -s_player.MinVtx.y;
	s_player.aModel[0].rot = ZERO_VECTOR;
	s_player.aModel[0].nIdxModelParent = -1;
	s_player.aModel[0].bUse = true;
	s_player.aModel[1].bUse = true;

	s_player.aModel[1].pos = ZERO_VECTOR;
	s_player.aModel[1].pos.x = -10.0f;
	s_player.aModel[1].pos.y = -s_player.MinVtx.y;
	s_player.aModel[1].rot = ZERO_VECTOR;
	s_player.aModel[1].nIdxModelParent = 0;

	s_player.pos = ZERO_VECTOR;
	s_player.pos.y = -s_player.MinVtx.y;
	s_player.rot = ZERO_VECTOR;
	s_player.movevec = ZERO_VECTOR;
	s_player.aModel[0].quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// �N�H�[�^�j�I��

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = s_player.pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = s_player.pos.z;
	s_nShadowCnt = SetShadow(ShadowPos, s_player.rot);

	LoadPlayerModel();

}

//=========================================
// �I��
//=========================================
void UninitPlayer(void)
{
	for (int i = 0; i < PARTS_NUM; i++)
	{
		Model* model = &(s_player.aModel[i]);

		if (model->pTexture != NULL)
		{
			for (int j = 0; j < (int)model->nNumMat; j++)
			{
				if (model->pTexture[j] != NULL)
				{// �e�N�X�`���̉��
					model->pTexture[j]->Release();
					model->pTexture[j] = NULL;
				}
			}

			delete[]model->pTexture;
			model->pTexture = NULL;
		}

		// ���b�V���̉��
		if (model->pMesh != NULL)
		{
			model->pMesh->Release();
			model->pMesh = NULL;
		}
		// �}�e���A���̉��
		if (model->pBuffMat != NULL)
		{
			model->pBuffMat->Release();
			model->pBuffMat = NULL;
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

	// ���f���p�[�c���Ƃ̓����蔻��
	ColisionPartsModel();

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

	s_player.aModel[0].quaternion *= quaternion;

	// �N�I�[�^�j�I���̃m�[�}���C�Y
	D3DXQuaternionNormalize(&s_player.aModel[0].quaternion, &s_player.aModel[0].quaternion);

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

	// �X�P�[���̔��f
	//D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
	//D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxScale);							// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
	
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);	// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);

	for (int i = 0; i < PARTS_NUM; i++)
	{
		Model* model = &(s_player.aModel[i]);

		if (!model->bUse)
		{
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&model->mtxWorld);

		//if ( i == 0)
		{
			// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
			D3DXMatrixRotationQuaternion(&mtxRot, &s_player.aModel[i].quaternion);							// �N�I�[�^�j�I���ɂ��s���]
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		}
		//else
		//{
		//	// �����𔽉f
		//	D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rot.y, model->rot.x, model->rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
		//	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		//}

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

		if (model->nIdxModelParent != -2)
		{
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxParent);

		}

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
			pDevice->SetTexture(0, model->pTexture[j]);

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

//=========================================
// �ǂݍ��ݏ���
//=========================================
void LoadPlayerModel(void)
{
	FILE* pFile;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	bool isModel = false;
	char modelFile[255][255] = {};
	int nTexCnt;
	int nModelFileCnt = 0;
	int nModelCnt = 2;

	pFile = fopen(MODEL_LOAD_FILE, "r");

	char read[255] = {};
	while (1)
	{
		fscanf(pFile, "%s", &read);

		//# �����o����ƈ�s�ǂݍ���
		if (strncmp(&read[0], "#", 1) == 0)
		{
			fgets(read, sizeof(read), pFile);
			continue;
		}

		if (strcmp(&read[0], "END_SCRIPT") == 0)
		{
			break;
		}

		if (strcmp(&read[0], "MODEL_FILENAME") == 0)
		{
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%s", &modelFile[nModelFileCnt][0]);
			nModelFileCnt++;

		}
		if (strcmp(&read[0], "MODELSET") == 0)
		{
			isModel = true;
		}
		else if (strcmp(&read[0], "END_MODELSET") == 0)
		{
			nModelCnt++;
			isModel = false;
		}

		if (isModel)
		{
			if (strcmp(&read[0], "TYPE") == 0)
			{
				int nData;
				Model* model = &(s_player.aModel[nModelCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%d", &nData);

				// X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(&modelFile[nData][0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&model->pBuffMat,
					NULL,
					&model->nNumMat,
					&model->pMesh);

				// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
				model->pTexture = new LPDIRECT3DTEXTURE9[model->nNumMat];

				// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
				D3DXMATERIAL *pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

				// �e���b�V���̃}�e���A�������擾����
				for (int i = 0; i < (int)model->nNumMat; i++)
				{
					model->pTexture[i] = NULL;

					if (pMat[i].pTextureFilename != NULL)
					{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
						D3DXCreateTextureFromFileA(pDevice,
							pMat[i].pTextureFilename,
							&model->pTexture[i]);
					}
				}

				// ���f���̃T�C�Y�v��
				ModelSize(&model->MinVtx, &model->MaxVtx, model->pMesh);

				model->nIdxModelParent = -2;
				model->bUse = true;
			}
			if (strcmp(&read[0], "POS") == 0)
			{
				D3DXVECTOR3 pos;
				Model* model = &(s_player.aModel[nModelCnt]);
				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				model->pos = pos;

			}
			if (strcmp(&read[0], "ROT") == 0)
			{
				D3DXVECTOR3 rot;
				Model* model = &(s_player.aModel[nModelCnt]);
				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
				model->rot = rot;

			}
		}
	}
}

//=========================================
// ���f���p�[�c���m�̓����蔻��
//=========================================
void ColisionPartsModel(void)
{
	for (int i = 0; i < PARTS_NUM; i++)
	{
		Model* model = &(s_player.aModel[i]);

		if (!model->bUse || model->nIdxModelParent == -2)
		{
			continue;
		}

		for (int j = 0; j < PARTS_NUM; j++)
		{
			Model* hitModel = &(s_player.aModel[j]);

			if (!(hitModel->bUse) || hitModel->nIdxModelParent != -2 || i == j)
			{
				continue;
			}
			D3DXVECTOR3 hitMax = hitModel->pos + hitModel->MaxVtx;
			D3DXVECTOR3 hitMin = hitModel->pos + hitModel->MinVtx;
			D3DXVECTOR3 pos = s_player.pos + model->pos;
			D3DXVECTOR3 pos_old = s_player.pos_old + model->pos_old;

			if ((pos.y + model->MaxVtx.y > hitMin.y) && (pos.y + model->MinVtx.y < hitMax.y))
			{
				if ((pos.x + model->MaxVtx.x > hitMin.x) && (pos.x + model->MinVtx.x < hitMax.x))
				{
					// ��
					if ((pos.z + model->MaxVtx.z >= hitMin.z) && (pos_old.z + model->MaxVtx.z <= hitMin.z))
					{
						hitModel->pos -= s_player.pos;
						//hitModel->quaternion = s_player.aModel[0].quaternion;
						hitModel->nIdxModelParent = 0;
						//s_player.pos.z = hitMin.z - model->MaxVtx.z;

					}
					// ��O
					else if ((pos.z + model->MinVtx.z <= hitMax.z) && pos_old.z + model->MinVtx.z >= hitMax.z)
					{
						hitModel->pos -= s_player.pos;
						//hitModel->quaternion = s_player.aModel[0].quaternion;
						hitModel->nIdxModelParent = 0;
						//	s_player.pos.z = hitMax.z - model->MinVtx.z;
					}
				}
				if (pos.z + model->MaxVtx.z > hitMin.z && pos.z + model->MinVtx.z < hitMax.z)
				{
					// ��
					if (pos.x + model->MaxVtx.x >= hitMin.x && pos_old.x + model->MaxVtx.x <= hitMin.x)
					{
						hitModel->pos -= s_player.pos;
						//hitModel->quaternion = s_player.aModel[0].quaternion;
						hitModel->nIdxModelParent = 0;
						//s_player.pos.x = hitMin.x - model->MaxVtx.x;
					}
					// �E
					else if (pos.x + model->MinVtx.x <= hitMax.x && pos_old.x + model->MinVtx.x >= hitMax.x)
					{
						hitModel->pos -= s_player.pos;
						//hitModel->quaternion = s_player.aModel[0].quaternion;
						hitModel->nIdxModelParent = 0;
						//s_player.pos.x = hitMax.x - model->MinVtx.x;
					}
				}
			}

		}
	}
}
