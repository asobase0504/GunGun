//=========================================
// 
// プレイヤー処理関数
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
#include "shadow.h"
#include "mesh_field.h"
#include <stdio.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define MODEL_MOVE				(1.0f)
#define MODEL_ROT_ATTENUATION	(0.05f)	// 減算

//------------------------------------
// モデルステータス列挙型
//------------------------------------
typedef enum
{
	MODELSTATEL_NONE = 0,
	MODELSTATEL_MOVE,
	MODELSTATEL_MAX
}MODEL_STATE;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void LoadPlayer(void);		//プレイヤーの読み込み処理

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DTEXTURE9 *s_pTexture = NULL;	// テクスチャへのポインタ
static Player s_player;							// モデルの構造体
static char s_tex[255];							// モデルファイル
static MODEL_STATE s_state;						// モデルのステータス
static int s_nShadowCnt;						// 影の割り当て

//=========================================
// 初期化
//=========================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 読み込み処理
	LoadPlayer();

	for (int i = 0; i < PARTS_NUM; i++)
	{
		ModelParts* model = &(s_player.aModel[i]);

		// Xファイルの読み込み
		D3DXLoadMeshFromX(s_tex,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&model->pBuffMat,
			NULL,
			&model->nNumMat,
			&model->pMesh);

		// モデルのサイズ計測
		ModelSize(&(s_player.MinVtx), &(s_player.MaxVtx), model->pMesh);

		// メッシュに使用されているテクスチャ用の配列を用意する
		s_pTexture = new LPDIRECT3DTEXTURE9[model->nNumMat];

		// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

		// 各メッシュのマテリアル情報を取得する
		for (int j = 0; j < (int)model->nNumMat; j++)
		{
			s_pTexture[j] = NULL;

			if (pMat[j].pTextureFilename != NULL)
			{// マテリアルで設定されているテクスチャ読み込み
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
	s_player.quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// クォータニオン

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = s_player.pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = s_player.pos.z;
	s_nShadowCnt = SetShadow(ShadowPos, s_player.rot);
}

//=========================================
// 終了
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
				{// テクスチャの解放
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
		// メッシュの解放
		if (s_player.aModel[i].pMesh != NULL)
		{
			s_player.aModel[i].pMesh->Release();
			s_player.aModel[i].pMesh = NULL;
		}
		// マテリアルの解放
		if (s_player.aModel[i].pBuffMat != NULL)
		{
			s_player.aModel[i].pBuffMat->Release();
			s_player.aModel[i].pBuffMat = NULL;
		}
	}
}

//=========================================
// 更新
//=========================================
void UpdatePlayer(void)
{
	Player* pPlayer = &(s_player);

	pPlayer->pos_old = pPlayer->pos;

	// 移動処理
	MovePlayer();

	pPlayer->pos += pPlayer->movevec;
	pPlayer->rot.y += NormalizeRot(pPlayer->rotDest.y - pPlayer->rot.y) * MODEL_ROT_ATTENUATION;

	// 床の当たり判定
	CollisionMeshField(&pPlayer->pos);

	pPlayer->pos.y += -s_player.MinVtx.y;

	// 角度の正規化
	NormalizeRot(pPlayer->rot.y);

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = pPlayer->pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = pPlayer->pos.z;
	SetPositionShadow(s_nShadowCnt,ShadowPos);
}

//=========================================
// 移動
//=========================================
void MovePlayer()
{
	Player* pPlayer = &(s_player);
	D3DXVECTOR3 CameraRot = GetRotCamera();	// カメラの角度情報取得
	D3DXVECTOR3 move = ZERO_VECTOR;			// 移動量の初期化

											// モデルの移動
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

	// モデルの上下の移動
	if (GetKeyboardPress(DIK_T))
	{
		pPlayer->pos.y += MODEL_MOVE;
	}
	if (GetKeyboardPress(DIK_B))
	{
		pPlayer->pos.y += -(MODEL_MOVE);
	}

	D3DXVECTOR3 axis;

	D3DXVec3Normalize(&move, &move);	// 正規化する(大きさ１のベクトルにする)
	D3DXVec3Cross(&axis, &-move, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	D3DXQUATERNION quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);

	D3DXQuaternionRotationAxis(&quaternion, &axis, 0.1f);	// 回転軸と回転角度を指定

	s_player.quaternion *= quaternion;

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&s_player.quaternion, &s_player.quaternion);

	pPlayer->movevec = move * MODEL_MOVE;
}

//=========================================
// 描画
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX /*mtxScale,*/ mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_player.mtxWorld);

	//// クォータニオンの使用した姿勢の設定
	//D3DXMatrixTransformation(
	//	&s_mtxWorld,		// 取得したいワールド変換行列
	//	NULL,				// スケーリングの中心点
	//	NULL,				// スケーリングの回転
	//	&scale,				// 軸ごとのスケール値
	//	NULL,				// 回転の中心を指定
	//	&s_player.quaternion,	// 回転軸と回転角度を指定
	//	&s_player.pos		// スケーリングと回転が終わった頂点をさらにオフセット（平行移動）
	//);

	// スケールの反映
	//D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
	//D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxScale);							// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	
	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	
	//// クォータニオンの使用した姿勢の設定
	//D3DXMatrixRotationQuaternion(&mtxRot,&s_player.quaternion);							// クオータニオンによる行列回転
	//D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);

	for (int i = 0; i < PARTS_NUM; i++)
	{
		ModelParts* model = &(s_player.aModel[i]);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&model->mtxWorld);

		if ( i == 0)
		{
			// クォータニオンの使用した姿勢の設定
			D3DXMatrixRotationQuaternion(&mtxRot, &s_player.quaternion);							// クオータニオンによる行列回転
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
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
			mtxParent = s_player.mtxWorld;
		}
		else
		{
			mtxParent = s_player.aModel[model->nIdxModelParent].mtxWorld;
		}
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxParent);

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
			pDevice->SetTexture(0, s_pTexture[j]);

			// モデルパーツの描写
			model->pMesh->DrawSubset(j);
		}

		// 保持していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}

//--------------------------------------------------
// 読み込み処理
//--------------------------------------------------
void LoadPlayer(void)
{
	FILE* pFile;

	pFile = fopen("data/FILE/Player.txt", "r");

	char read[255] = {};

	fscanf(pFile, "%s", &read);
	if (strcmp(&read[0], "PlayerModel") == 0)
	{
		fscanf(pFile, "%s", &read);			// = の除去
		fscanf(pFile, "%s", &s_tex);		// 値を入れる
	}

	fclose(pFile);
}
