//=========================================
// 
// モデル処理関数
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
// マクロ定義
//------------------------------------
#define MODEL_NUM				(25)
#define MODEL_MAX				(255)
#define PLAYER_MOVE				(1.0f)
#define MODEL_ROT_ATTENUATION	(0.05f)
#define MODEL_LOAD_FILE			("data/model.txt")
//------------------------------------
// 静的変数
//------------------------------------
static Model s_ModelType[MODEL_NUM];	// モデルの種類を保管
static Model s_Model[MODEL_MAX];		// モデルの構造体
static Model s_ModelUI;					// モデルUIの構造体
static int s_nShadowCnt;				// 影の割り当て

//=========================================
// 初期化
//=========================================
void InitModel(void)
{
	ZeroMemory(s_ModelType, sizeof(s_ModelType));
	ZeroMemory(s_Model, sizeof(s_Model));
}

//=========================================
// 終了
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
				{// テクスチャの解放
					model->pTexture[j]->Release();
					model->pTexture[j] = NULL;
				}
			}

			delete[]model->pTexture;
			model->pTexture = NULL;
		}

		// メッシュの解放
		if (model->pMesh != NULL)
		{
			model->pMesh->Release();
			model->pMesh = NULL;
		}
		// マテリアルの解放
		if (model->pBuffMat != NULL)
		{
			model->pBuffMat->Release();
			model->pBuffMat = NULL;
		}
	}
}

//=========================================
// 更新
//=========================================
void UpdateModel(void)
{
}

//=========================================
// 描画
//=========================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* model = &(s_Model[i]);

		if (!model->bUse)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&model->mtxWorld);

		// 角度の反映
		if (model->isQuaternion)
		{
			// クォータニオンの使用した姿勢の設定
			D3DXMatrixRotationQuaternion(&mtxRot, &model->quaternion);			// クオータニオンによる行列回転
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		}
		else
		{
			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rot.y, model->rot.x, model->rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		}

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, model->pos.x, model->pos.y, model->pos.z);		// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTrans);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		D3DXMATRIX mtxParent;
		if (model->nIdxModelParent == -1)
		{
			mtxParent = GetPlayer()->mtxWorld;
		}
		else
		{
			mtxParent = s_Model[model->nIdxModelParent].mtxWorld;
		}

		// プレイヤーとくっついている状態のモデルはプレイヤーとの行列計算
		if (model->nIdxModelParent != -2)
		{
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxParent);

		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);;

		// 現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, model->pTexture[j]);

			// モデルパーツの描写
			model->pMesh->DrawSubset(j);
		}
		// 保持していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=========================================
// 当たり判定
//=========================================
void CollisionModel(D3DXVECTOR3* pos, D3DXVECTOR3* pos_old, D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	for (int i = 0; i < MODEL_NUM; i++)
	{
		Model* model = &(s_ModelType[i]);

		if (pos->x + max.x > model->pos.x + model->MinVtx.x && pos->x + min.x < model->pos.x + model->MaxVtx.x)
		{
			// 奥
			if (pos->z + max.z >= model->pos.z + model->MinVtx.z && pos_old->z + max.z <= model->pos.z + model->MinVtx.z)
			{
				pos->z = model->pos.z + model->MinVtx.z - max.z;
			}
			// 手前
			else if (pos->z + min.z <= model->pos.z + model->MaxVtx.z && pos_old->z + min.z >= model->pos.z + model->MaxVtx.z)
			{
				pos->z = model->pos.z + model->MaxVtx.z - min.z;
			}
		}
		if (pos->z + max.z > model->pos.z + model->MinVtx.z && pos->z + min.z < model->pos.z + model->MaxVtx.z)
		{
			// 左
			if (pos->x + max.x >= model->pos.x + model->MinVtx.x && pos_old->x + max.x <= model->pos.x + model->MinVtx.x)
			{
				pos->x = model->pos.x + model->MinVtx.x - max.x;
			}
			// 右
			else if (pos->x + min.x <= model->pos.x + model->MaxVtx.x && pos_old->x + min.x >= model->pos.x + model->MaxVtx.x)
			{
				pos->x = model->pos.x + model->MaxVtx.x - min.x;
			}
		}
	}
}

//=========================================
// 取得
//=========================================
Model *GetModel(void)
{
	return s_ModelType;
}

//=========================================
// 読み込み処理
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

		if (strcmp(&read[0], "MODEL_FILENAME") == 0)
		{
			fscanf(pFile, "%s", &read);
			fscanf(pFile, "%s", &modelFile[nModelFileCnt][0]);

			Model* modelType = &(s_ModelType[nModelFileCnt]);

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
// 設定
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
// モデルUIの描画
// ローカル座標のみで描画位置を決定する
//=========================================
void DrawModelUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;				// マテリアルデータへのポインタ

	Model* model = &(s_ModelUI);

	model->rot.y += 0.05f;

	if (model == NULL && model->bUse)
	{

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&model->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rot.y, model->rot.x, model->rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, model->pos.x, model->pos.y, model->pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTrans);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		D3DXMATRIX mtxParent;

		if (model->nIdxModelParent == -1)
		{
			mtxParent = GetPlayer()->mtxWorld;
		}
		else
		{
			mtxParent = s_Model[model->nIdxModelParent].mtxWorld;
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);

		// 現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, model->pTexture[j]);

			// モデルパーツの描写
			model->pMesh->DrawSubset(j);
		}

		// 保持していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=========================================
// モデルUIの設定
// ローカル座標のみで描画位置を決定する
//=========================================
void SetModelUI(Model * model)
{
	s_ModelUI = *model;
	s_ModelUI.pos = D3DXVECTOR3(0.0f, -200.0f, 0.0f);
}
