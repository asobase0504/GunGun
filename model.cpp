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
#define MODEL_MAX				(255)
#define PLAYER_MOVE				(1.0f)
#define MODEL_ROT_ATTENUATION	(0.05f)
#define MODEL_LOAD_FILE			("data/model.txt")
//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static Model s_model[MODEL_MAX];	// ���f���̍\����
static int s_nShadowCnt;			// �e�̊��蓖��

//=========================================
// ������
//=========================================
void InitModel(void)
{
	ZeroMemory(s_model, sizeof(s_model));
}

//=========================================
// �I��
//=========================================
void UninitModel(void)
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* model = &(s_model[i]);

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
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	//D3DMATERIAL9 matDef;			// ���݂̃}�e���A���ۑ��p
	//D3DXMATERIAL *pMat;				// �}�e���A���f�[�^�ւ̃|�C���^

	//for (int i = 0; i < MODEL_MAX; i++)
	//{
	//	Model* model = &(s_model[i]);

	//	// ���[���h�}�g���b�N�X�̏�����
	//	D3DXMatrixIdentity(&model->mtxWorld);

	//	// �����𔽉f
	//	D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rot.y, model->rot.x, model->rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	//	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	//	// �ʒu�𔽉f
	//	D3DXMatrixTranslation(&mtxTrans, model->pos.x, model->pos.y, model->pos.z);		// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	//	D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	//	// ���[���h�}�g���b�N�X�̐ݒ�
	//	pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);;

	//	// ���݂̃}�e���A����ێ�
	//	pDevice->GetMaterial(&matDef);

	//	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	//	pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

	//	for (int j = 0; j < (int)model->nNumMat; j++)
	//	{
	//		// �}�e���A���̐ݒ�
	//		pDevice->SetMaterial(&pMat[j].MatD3D);

	//		// �e�N�X�`���̐ݒ�
	//		pDevice->SetTexture(0, model->pTexture[j]);

	//		// ���f���p�[�c�̕`��
	//		model->pMesh->DrawSubset(j);
	//	}
	//	// �ێ����Ă����}�e���A����߂�
	//	pDevice->SetMaterial(&matDef);
	//}
}

//=========================================
// �����蔻��
//=========================================
void CollisionModel(D3DXVECTOR3* pos, D3DXVECTOR3* pos_old, D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* model = &(s_model[i]);

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
	return s_model;
}

//=========================================
// �ǂݍ��ݏ���
//=========================================
void LoadModel(void)
{
	FILE* pFile;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	bool isModel = false;
	char modelFile[255][255] = {};
	int nModelFileCnt = 0;
	int nModelCnt = 1;

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

			Model* model = &(s_model[nModelFileCnt]);

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(&modelFile[nModelFileCnt][0],
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
			Model* model = GetPlayerModel();
			model += nModelCnt;

			if (strcmp(&read[0], "TYPE") == 0)
			{
				int nData;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%d", &nData);

				*model = s_model[nData];

				model->nIdxModelParent = -2;
				model->bUse = true;
				model->quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
			}
			if (strcmp(&read[0], "POS") == 0)
			{
				D3DXVECTOR3 pos;

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				model->pos = pos;
			}
			if (strcmp(&read[0], "ROT") == 0)
			{
				D3DXVECTOR3 rot;
	
				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
				model->rot = rot;
			}
		}
	}
}

//=========================================
// �ǂݍ��ݏ���
//=========================================
void SaveModel(void)
{

}
