//=========================================
// 
// ���f�������֐�
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "model.h"
#include "common.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "player.h"
#include <stdio.h>

//------------------------------------
// �}�N����`
//------------------------------------
#define MODEL_NUM				(25)
#define MODEL_MAX				(255)
#define PLAYER_MOVE				(1.0f)
#define MODEL_ROT_ATTENUATION	(0.05f)
#define MODEL_LOAD_FILE			("data/model.txt")
//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static Model s_ModelType[MODEL_NUM];	// ���f���̎�ނ�ۊ�
static Model s_Model[MODEL_MAX];		// ���f���̍\����
static Model s_ModelUI;					// ���f��UI�̍\����
static int s_nShadowCnt;				// �e�̊��蓖��

//=========================================
// ������
//=========================================
void InitModel(void)
{
	ZeroMemory(s_ModelType, sizeof(s_ModelType));
	ZeroMemory(&s_ModelUI, sizeof(s_ModelUI));
	ZeroMemory(s_Model, sizeof(s_Model));
}

//=========================================
// �I��
//=========================================
void UninitModel(void)
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		Model* model = &(s_ModelType[i]);

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
void UpdateModel(void)
{
}

//=========================================
// �`��
//=========================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* model = &(s_Model[i]);

		if (!model->bUse)
		{
			continue;
		}

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&model->mtxWorld);

		// �p�x�̔��f
		if (model->isQuaternion)
		{
			// �N�H�[�^�j�I���̎g�p�����p���̐ݒ�
			D3DXMatrixRotationQuaternion(&mtxRot, &model->quaternion);			// �N�I�[�^�j�I���ɂ��s���]
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);	// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
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

		// �v���C���[�Ƃ������Ă����Ԃ̃��f���̓v���C���[�Ƃ̍s��v�Z
		if (model->nIdxModelParent != -2)
		{
			D3DXMATRIX mtxParent;
		
			if (model->nIdxModelParent == -1)
			{
				mtxParent = GetPlayer()->mtxWorld;
			}
			else
			{
				mtxParent = s_Model[model->nIdxModelParent].mtxWorld;
			}

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

//=========================================
// �����蔻��
//=========================================
void CollisionModel(D3DXVECTOR3* pos, D3DXVECTOR3* pos_old, D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		Model* model = &(s_ModelType[i]);

		if (pos->x + max.x > model->pos.x + model->MinVtx.x && pos->x + min.x < model->pos.x + model->MaxVtx.x)
		{
			// ��
			if (pos->z + max.z >= model->pos.z + model->MinVtx.z && pos_old->z + max.z <= model->pos.z + model->MinVtx.z)
			{
				pos->z = model->pos.z + model->MinVtx.z - max.z;
			}
			// ��O
			else if (pos->z + min.z <= model->pos.z + model->MaxVtx.z && pos_old->z + min.z >= model->pos.z + model->MaxVtx.z)
			{
				pos->z = model->pos.z + model->MaxVtx.z - min.z;
			}
		}
		if (pos->z + max.z > model->pos.z + model->MinVtx.z && pos->z + min.z < model->pos.z + model->MaxVtx.z)
		{
			// ��
			if (pos->x + max.x >= model->pos.x + model->MinVtx.x && pos_old->x + max.x <= model->pos.x + model->MinVtx.x)
			{
				pos->x = model->pos.x + model->MinVtx.x - max.x;
			}
			// �E
			else if (pos->x + min.x <= model->pos.x + model->MaxVtx.x && pos_old->x + min.x >= model->pos.x + model->MaxVtx.x)
			{
				pos->x = model->pos.x + model->MaxVtx.x - min.x;
			}
		}
	}
}

//=========================================
// �擾
//=========================================
Model *GetModel(void)
{
	return s_ModelType;
}

//=========================================
// �ǂݍ��ݏ���
//=========================================
void LoadModel(void)
{
	FILE* pFile;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	bool isModel = false;
	bool isPlayer = false;
	char modelFile[255][255] = {};
	int nModelFileCnt = 0;
	int nModelCnt = 1;
	int nModelData;

	pFile = fopen(MODEL_LOAD_FILE, "r");

	char read[255] = {};
	while (1)
	{
		fscanf(pFile, "%s", &read);

		//# �����o����ƈ�s�ǂݍ���
		if (strncmp(&read[0], "#", 1) == 0)
		{
			fgets(read, 255, pFile);
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

			Model* modelType = &(s_ModelType[nModelFileCnt]);

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(&modelFile[nModelFileCnt][0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&modelType->pBuffMat,
				NULL,
				&modelType->nNumMat,
				&modelType->pMesh);

			// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
			modelType->pTexture = new LPDIRECT3DTEXTURE9[modelType->nNumMat];

			// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
			D3DXMATERIAL *pMat = (D3DXMATERIAL*)modelType->pBuffMat->GetBufferPointer();

			// �e���b�V���̃}�e���A�������擾����
			for (int i = 0; i < (int)modelType->nNumMat; i++)
			{
				modelType->pTexture[i] = NULL;

				if (pMat[i].pTextureFilename != NULL)
				{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
					D3DXCreateTextureFromFileA(pDevice,
						pMat[i].pTextureFilename,
						&modelType->pTexture[i]);
				}
			}

			// ���f���̃T�C�Y�v��
			ModelSize(&modelType->MinVtx, &modelType->MaxVtx, modelType->pMesh);

			nModelFileCnt++;
		}

		if (strcmp(&read[0], "MODELSET") == 0)
		{
			for (nModelData = 0; nModelData < MODEL_MAX; nModelData++)
			{
				if (!s_Model[nModelData].bUse)
				{
					break;
				}
			}
			isModel = true;
		}
		else if (strcmp(&read[0], "PLAYERSET") == 0)
		{
			for (nModelData = 0; nModelData < MODEL_MAX; nModelData++)
			{
				if (!s_Model[nModelData].bUse)
				{
					break;
				}
			}
			isPlayer = true;
		}
		
		if (strcmp(&read[0], "END_MODELSET") == 0)
		{
			Model** parts = GetPlayerModel();
			parts += nModelCnt;
			*parts = &s_Model[nModelData];
			nModelCnt++;
			isModel = false;
		}
		else if (strcmp(&read[0], "END_PLAYERSET") == 0)
		{
			Model** player = GetPlayerModel();
			*player = &s_Model[nModelData];
			isPlayer = false;
		}

		if (isModel)
		{
			if (strcmp(&read[0], "TYPE") == 0)
			{
				int nData;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%d", &nData);

				s_Model[nModelData] = s_ModelType[nData];

				s_Model[nModelData].nIdxModelParent = -2;
				s_Model[nModelData].quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
				s_Model[nModelData].isQuaternion = true;
				s_Model[nModelData].bUse = true;
			}
			if (strcmp(&read[0], "POS") == 0)
			{
				D3DXVECTOR3 pos;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				s_Model[nModelData].pos = pos;
			}
			if (strcmp(&read[0], "ROT") == 0)
			{
				D3DXVECTOR3 rot;
	
				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
				s_Model[nModelData].rot = rot;
			}
		}
		if (isPlayer)
		{
			if (strcmp(&read[0], "TYPE") == 0)
			{
				int nData;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%d", &nData);

				s_Model[nModelData] = s_ModelType[nData];

				s_Model[nModelData].nIdxModelParent = -1;
				s_Model[nModelData].quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
				s_Model[nModelData].isQuaternion = true;
				s_Model[nModelData].bUse = true;
			}
			if (strcmp(&read[0], "POS") == 0)
			{
				D3DXVECTOR3 pos;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				s_Model[nModelData].pos = pos;
			}

		}
	}
}

//=========================================
// �ݒ�
//=========================================
void SetModel(Model* model)
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* pModel = &(s_Model[i]);

		if (!pModel->bUse)
		{
			continue;
		}

		pModel = model;
	}
}

//=========================================
// �ݒ�
//=========================================
Model* SetModel(char* file)
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* pModel = &(s_Model[i]);

		if (pModel->bUse)
		{
			continue;
		}

		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(file,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pModel->pBuffMat,
			NULL,
			&pModel->nNumMat,
			&pModel->pMesh);

		// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
		pModel->pTexture = new LPDIRECT3DTEXTURE9[pModel->nNumMat];

		// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

		// �e���b�V���̃}�e���A�������擾����
		for (int j = 0; j < (int)pModel->nNumMat; j++)
		{
			pModel->pTexture[j] = NULL;

			if (pMat[j].pTextureFilename != NULL)
			{// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
				D3DXCreateTextureFromFileA(pDevice,
					pMat[j].pTextureFilename,
					&pModel->pTexture[j]);
			}
		}

		// ���f���̃T�C�Y�v��
		ModelSize(&pModel->MinVtx, &pModel->MaxVtx, pModel->pMesh);

		pModel->bUse = true;

		return pModel;
	}
	return NULL;
}

//=========================================
// ���f��UI�̕`��
// ���[�J�����W�݂̂ŕ`��ʒu�����肷��
//=========================================
void DrawModelUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	Model* model = &(s_ModelUI);
	Camera* camera = GetCamera(0);
	D3DXMATRIX mtxCamera;
	s_ModelUI.rot.y += 0.01f;

	D3DXVECTOR3 size = model->MaxVtx - model->MinVtx;
	float scale = 1.0f;
	while (size.x >= 3.0f || size.y >= 3.0f || size.z >= 3.0f)
	{
		size = model->MaxVtx - model->MinVtx;
		scale -= 0.01f;
		size *= scale;
	}

	if (model != NULL && model->bUse)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&model->mtxWorld);

		// �X�P�[���̔��f
		D3DXMatrixScaling(&mtxScale, scale, scale, scale);
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxScale);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rot.y, model->rot.x, model->rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, model->pos.x, model->pos.y, model->pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTrans);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		D3DXMatrixInverse(&mtxCamera, NULL, &camera->mtxView);
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxCamera);

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

//=========================================
// ���f��UI�̐ݒ�
// ���[�J�����W�݂̂ŕ`��ʒu�����肷��
//=========================================
void SetModelUI(Model * model)
{
	D3DXVECTOR3 rot_old = s_ModelUI.rot;
	s_ModelUI = *model;
	s_ModelUI.rot = rot_old;
	s_ModelUI.pos.y = -5.85f;
	s_ModelUI.pos.x = -17.0f;
	s_ModelUI.pos.z = 20.0f;
}
