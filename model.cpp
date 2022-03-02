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

#include "load.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define MODEL_NUM				(100)
#define PLAYER_MOVE				(1.0f)
#define MODEL_ROT_ATTENUATION	(0.05f)
#define MODEL_LOAD_FILE			("data/model.txt")
#define MAP_LOAD_FILE			("data/map05.txt")

//------------------------------------
// 静的変数
//------------------------------------
static Model s_ModelType[MODEL_NUM];	// モデルの種類を保管
static Model s_Model[MODEL_MAX];		// モデルの構造体
static Model s_ModelUI;					// モデルUIの構造体
static int s_nShadowCnt;				// 影の割り当て
static LPD3DXFONT s_pFont = NULL;		// フォントへのポインタ

static bool bFlag;

//=========================================
// 初期化
//=========================================
void InitModel(void)
{
	bFlag = false;
	ZeroMemory(s_ModelType, sizeof(s_ModelType));
	ZeroMemory(&s_ModelUI, sizeof(s_ModelUI));
	ZeroMemory(s_Model, sizeof(s_Model));

	// 取得したモデル名用フォントの生成
	D3DXCreateFont(GetDevice(), 32, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07あかずきんポップ Heavy", &s_pFont);
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

	Model* model = &(s_ModelUI);

	if (model->pTexture != NULL)
	{
		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			if (model->pTexture[j] != NULL)
			{// テクスチャの解放
				model->pTexture[j] = NULL;
			}
		}
		model->pTexture = NULL;
	}

	// メッシュの解放
	if (model->pMesh != NULL)
	{
		model->pMesh = NULL;
	}
	// マテリアルの解放
	if (model->pBuffMat != NULL)
	{
		model->pBuffMat = NULL;
	}

	// 取得したモデル表示用フォントの破棄
	if (s_pFont != NULL)
	{
		s_pFont->Release();
		s_pFont = NULL;
	}
}

//=========================================
// 更新
//=========================================
void UpdateModel(void)
{
	// モデルの移動
	if (GetKeyboardTrigger(DIK_B))
	{
		bFlag = !bFlag;
	}

}

//=========================================
// 描画
//=========================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;						// マテリアルデータへのポインタ

	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* model = &(s_Model[i]);

		if (!model->bUse)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&model->mtxWorld);

		// スケールの反映
		D3DXMatrixScaling(&mtxScale, model->scale, model->scale, model->scale);
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxScale);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		
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

		// プレイヤーとくっついている状態のモデルはプレイヤーとの行列計算
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

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);

		// 現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		pDevice->SetRenderState(D3DRS_AMBIENT, 0xff808080);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			D3DMATERIAL9 matBak = pMat[j].MatD3D;
			
			// アンビエントライトの反映
			pMat[j].MatD3D.Ambient = pMat[j].MatD3D.Diffuse;

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, model->pTexture[j]);

			// モデルパーツの描写
			model->pMesh->DrawSubset(j);

			pMat[j].MatD3D = matBak;
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
	bool isType = false;
	char modelFile[255][255] = {};
	int nModelFileCnt = 0;

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
				fscanf(pFile, "%f %f %f", &modelType->size.x, &modelType->size.y, &modelType->size.z);

				modelType->typeCollision = COLLISION_SPHERE;
			}
			if (strcmp(&read[0], "COLLISION_POS") == 0)
			{
				Model* modelType = &(s_ModelType[nModelFileCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f %f %f", &modelType->pos_Collision.x, &modelType->pos_Collision.y, &modelType->pos_Collision.z);
			}
			if (strcmp(&read[0], "SCALE") == 0)
			{
				Model* modelType = &(s_ModelType[nModelFileCnt]);

				fscanf(pFile, "%s", &read);
				fscanf(pFile, "%f", &modelType->scale);
			}
		}
	}
	fclose(pFile);
}

//=========================================
// 読み込み処理
//=========================================
void LoadMap(void)
{
	FILE* pFile;
	bool isModel = false;
	bool isPlayer = false;
	int nModelCnt = 1;
	int nModelData;

	pFile = fopen(MAP_LOAD_FILE, "r");

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
				s_Model[nModelData].nType = nData;
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
				D3DXQuaternionRotationYawPitchRoll(&s_Model[nModelData].quaternion, rot.y, rot.x, rot.z);
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
	fclose(pFile);
}

//=========================================
// 設定
//=========================================
Model* SetModel(int nType)
{
	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* pModel = &(s_Model[i]);

		if (pModel->bUse)
		{
			continue;
		}

		s_Model[i] = s_ModelType[nType];

		return pModel;
	}
	return NULL;
}

//=========================================
// 設定
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

		// Xファイルの読み込み
		D3DXLoadMeshFromX(file,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pModel->pBuffMat,
			NULL,
			&pModel->nNumMat,
			&pModel->pMesh);

		// メッシュに使用されているテクスチャ用の配列を用意する
		pModel->pTexture = new LPDIRECT3DTEXTURE9[pModel->nNumMat];

		// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

		// 各メッシュのマテリアル情報を取得する
		for (int j = 0; j < (int)pModel->nNumMat; j++)
		{
			pModel->pTexture[j] = NULL;

			if (pMat[j].pTextureFilename != NULL)
			{// マテリアルで設定されているテクスチャ読み込み
				D3DXCreateTextureFromFileA(pDevice,
					pMat[j].pTextureFilename,
					&pModel->pTexture[j]);
			}
		}

		// モデルのサイズ計測
		ModelSize(&pModel->MinVtx, &pModel->MaxVtx, pModel->pMesh);

		pModel->bUse = true;

		return pModel;
	}
	return NULL;
}

//=========================================
// モデルUIの描画
// ローカル座標のみで描画位置を決定する
//=========================================
void DrawModelUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;						// マテリアルデータへのポインタ

	Model* model = &(s_ModelUI);
	Camera* camera = GetCamera(0);
	D3DXMATRIX mtxCamera;
	s_ModelUI.rot.y += 0.01f;

	D3DXVECTOR3 size = model->MaxVtx - model->MinVtx;
	float scale = model->scale;

	while (size.x >= 3.0f || size.y >= 3.0f || size.z >= 3.0f)
	{
		size = model->MaxVtx - model->MinVtx;
		scale -= 0.01f;
		size *= scale;
	}

	pDevice->LightEnable(0, false);
	pDevice->LightEnable(1, false);
	pDevice->LightEnable(2, false);

	if (model != NULL && model->bUse)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&model->mtxWorld);

		// スケールの反映
		D3DXMatrixScaling(&mtxScale, scale, scale, scale);
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxScale);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, model->rot.y, model->rot.x, model->rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, model->pos.x, model->pos.y, model->pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxTrans);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		D3DXMatrixInverse(&mtxCamera, NULL, &camera->mtxView);
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxCamera);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &model->mtxWorld);

		// 現在のマテリアルを保持
		pDevice->GetMaterial(&matDef);

		pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			D3DMATERIAL9 matBak = pMat[j].MatD3D;

			// アンビエントライトの反映
			pMat[j].MatD3D.Ambient = pMat[j].MatD3D.Diffuse;

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[j].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, model->pTexture[j]);

			// モデルパーツの描写
			model->pMesh->DrawSubset(j);

			pMat[j].MatD3D = matBak;
		}

		// 保持していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

	pDevice->LightEnable(0, true);
	pDevice->LightEnable(1, true);
	pDevice->LightEnable(2, true);

	// 表示領域の作成
	RECT rect = { -1050,670,SCREEN_WIDTH,SCREEN_HEIGHT };

	// テキストの描画
	s_pFont->DrawText(NULL, &model->name[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(0, 255, 255, 255));

}

//=========================================
// モデルUIの設定
// ローカル座標のみで描画位置を決定する
//=========================================
void SetModelUI(Model* model)
{
	D3DXVECTOR3 rot_old = s_ModelUI.rot;
	s_ModelUI = *model;
	s_ModelUI.rot = rot_old;
	s_ModelUI.pos.y = -9.0f;
	s_ModelUI.pos.x = -17.0f;
	s_ModelUI.pos.z = 20.0f;
}

//=========================================
// 取れるモデルの中で最大のモデル
// 
//=========================================
Model* SetJustModel(void)
{
	float fPlayerLength = GetPlayer()->fLength;
	float fModelLength = 0;
	int idx = 0;

	for (int i = 0; i < MODEL_MAX; i++)
	{
		Model* pModel = &(s_Model[i]);

		if (!pModel->bUse)
		{
			continue;
		}

		if (fPlayerLength > pModel->sizeCriter && fModelLength < pModel->sizeCriter)
		{
			fModelLength = pModel->sizeCriter;
			idx = i;
		}
	}

	return &s_Model[idx];
}
