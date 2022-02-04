//=========================================
// 
// 影の処理
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "load.h"
#include "model.h"
#include "common.h"
#include <stdio.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define MODEL_LOAD_FILE			("data/model.txt")
#define MAP_LOAD_FILE			("data/map.txt")
#define MODEL_NUM				(100)

//------------------------------------
// 静的変数
//------------------------------------
static Model s_ModelType[MODEL_NUM];	// モデルの種類を保管

//=========================================
// モデルのバイナリファイル化
//=========================================
void WritingBinaryModel(void)
{
	FILE* pFile;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	bool isModel = false;
	bool isType = false;
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

		//# を検出すると一行読み込む
		if (strncmp(&read[0], "#", 1) == 0)
		{
			fgets(read, 255, pFile);
			continue;
		}

		if (strcmp(&read[0], "END_SCRIPT") == 0)
		{
			break;
		}

		if (strcmp(&read[0], "TYPESET") == 0)
		{
			isType = true;
		}

		if (strcmp(&read[0], "END_TYPESET") == 0)
		{
			nModelFileCnt++;
			isType = false;
		}

		if (isType)
		{
			if (strcmp(&read[0], "FILENAME") == 0)
			{
				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%s", &modelFile[nModelFileCnt][0]);

				Model* modelType = &(s_ModelType[nModelFileCnt]);

				modelType->scale = 1.0f;

				// Xファイルの読み込み
				D3DXLoadMeshFromX(&modelFile[nModelFileCnt][0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&modelType->pBuffMat,
					NULL,
					&modelType->nNumMat,
					&modelType->pMesh);

				// メッシュに使用されているテクスチャ用の配列を用意する
				modelType->pTexture = new LPDIRECT3DTEXTURE9[modelType->nNumMat];

				// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
				D3DXMATERIAL *pMat = (D3DXMATERIAL*)modelType->pBuffMat->GetBufferPointer();

				// 各メッシュのマテリアル情報を取得する
				for (int i = 0; i < (int)modelType->nNumMat; i++)
				{
					modelType->pTexture[i] = NULL;

					if (pMat[i].pTextureFilename != NULL)
					{// マテリアルで設定されているテクスチャ読み込み
						D3DXCreateTextureFromFileA(pDevice,
							pMat[i].pTextureFilename,
							&modelType->pTexture[i]);
					}
				}

				// モデルのサイズ計測
				ModelSize(&modelType->MinVtx, &modelType->MaxVtx, modelType->pMesh);
			}
			if (strcmp(&read[0], "NAME") == 0)
			{
				Model* modelType = &(s_ModelType[nModelFileCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%s", &modelType->name[0]);
			}
			if (strcmp(&read[0], "SIZE_CRITERIA") == 0)
			{
				Model* modelType = &(s_ModelType[nModelFileCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f", &modelType->sizeCriter);
			}
			if (strcmp(&read[0], "SIZE_ADD") == 0)
			{
				Model* modelType = &(s_ModelType[nModelFileCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f", &modelType->sizeAdd);
			}
			if (strcmp(&read[0], "COLLISION_SPHERE") == 0)
			{
				Model* modelType = &(s_ModelType[nModelFileCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f", &modelType->fLength);

				modelType->typeCollision = COLLISION_SPHERE;
			}
			if (strcmp(&read[0], "COLLISION_SPHERE") == 0)
			{
				Model* modelType = &(s_ModelType[nModelFileCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f %f", &modelType->size.top, &modelType->size.bottom, &modelType->size.left, &modelType->size.right);

				modelType->typeCollision = COLLISION_SPHERE;
			}
			if (strcmp(&read[0], "COLLISION_POS") == 0)
			{
				Model* modelType = &(s_ModelType[nModelFileCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &modelType->pos_Collision.x, &modelType->pos_Collision.y, &modelType->pos_Collision.z);
			}
		}
	}
	fclose(pFile);

	pFile = fopen("data/model.bin", "wb");

	if (pFile != NULL)
	{// ファイルが開いた場合
	 // ファイルの書き込み
		fwrite(s_ModelType, sizeof(Model), MODEL_NUM, pFile);
		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けない場合
		printf("\n * * * ファイルが開けません * * * \n");
	}

}

//=========================================
// マップのバイナリファイル化
//=========================================
void WritingBinaryMap(void)
{
}
