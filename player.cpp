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
#include "line.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "mesh_field.h"
#include <stdio.h>
#include <math.h>

//------------------------------------
// マクロ定義
//------------------------------------
#define PLAYER_MOVE				(1.5f)	// プレイヤーの移動量
#define MODEL_ROT_ATTENUATION	(0.125f)	// 減算
#define MODEL_LOAD_FILE			("data/model.txt")

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
void LoadPlayer(void);			// プレイヤーの読み込み処理
void ColisionPartsModel(void);	// モデルパーツ同士の当たり判定
void LookUpSizePlayer(void);	// プレイヤーのサイズを調べる

//------------------------------------
// 静的変数
//------------------------------------
static Player s_player;			// モデルの構造体
static MODEL_STATE s_state;		// モデルのステータス
static int s_nShadowCnt;		// 影の割り当て

//=========================================
// 初期化
//=========================================
void InitPlayer(void)
{
	ZeroMemory(&s_player, sizeof(s_player));
	//for (int i = 0; i < sizeof(s_player.aModel) / sizeof(s_player.aModel[0]); i++)
	//{
	//	Model* model = s_player.aModel[i];

	//	model = &ZeroModel;
	//}

	// プレイヤーにくっつくモデルの配置
	LoadModel();
	// 読み込み処理
	//LoadPlayer();

	s_player.pos = ZERO_VECTOR;
	s_player.pos.y = -s_player.MinVtx.y;
	s_player.rot = ZERO_VECTOR;
	s_player.movevec = ZERO_VECTOR;
	s_player.aModel[0]->quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// クォータニオン


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
		// 線の設定
		//SetModelLine(&model->pos_world, &model->quaternion, model->MaxVtx, model->MinVtx, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	D3DXMATRIX /*mtxScale,*/ mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_player.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);
}

//=========================================
// 終了
//=========================================
void UninitPlayer(void)
{
}

//=========================================
// 更新
//=========================================
void UpdatePlayer(void)
{
	Player* pPlayer = &(s_player);

	pPlayer->pos_old = pPlayer->pos;	// プレイヤー位置の保存

	// モデル位置の保存
	for (int i = 0; i < PARTS_NUM; i++)
	{
		Model* model = pPlayer->aModel[i];

		if (model == NULL)
		{
			continue;
		}
		model->pos_old = model->pos;
		model->quaternion_old = model->quaternion;

		model->pos_world = D3DXVECTOR3(model->mtxWorld._41, model->mtxWorld._42, model->mtxWorld._43);
	}

	// 移動処理
	MovePlayer();
	
	// プレイヤーと床の当たり判定
	CollisionMeshField(&pPlayer->pos);

	/// 球の凸凹を表現するための当たり判定
	//for (int i = 0; i < PARTS_NUM; i++)
	//{
	//	Model* model = &(pPlayer->aModel[i]);
	//
	//	if (!model->bUse || model->nIdxModelParent != 0)
	//	{
	//		continue;
	//	}
	//
	//	if (model->pos_world.x + model->MinVtx.x <= s_player.fLength ||
	//		model->pos_world.y + model->MinVtx.y <= s_player.fLength ||
	//		model->pos_world.z + model->MinVtx.z <= s_player.fLength)
	//	{
	//		// モデルパーツと床の当たり判定
	//		CollisionMeshField(&pPlayer->pos, &(model->pos_world + model->MinVtx));
	//	}
	//	if (model->pos.x + model->MaxVtx.x == s_player.MaxVtx.x ||
	//		model->pos.y + model->MaxVtx.y == s_player.MaxVtx.y ||
	//		model->pos.z + model->MaxVtx.z == s_player.MaxVtx.z)
	//	{
	//		// モデルパーツと床の当たり判定
	//		CollisionMeshField(&pPlayer->pos, &(model->pos_world + model->MaxVtx));
	//	}
	//
	//}

	//// プレイヤーとモデルの当たり判定
	//CollisionModel(&pPlayer->pos, &pPlayer->pos_old, pPlayer->MinVtx, pPlayer->MaxVtx);

	// モデルパーツごとの当たり判定
	ColisionPartsModel();

	// プレイヤーの球の半径を求める
	LookUpSizePlayer();
	
	// プレイヤー位置とモデル回転する軸の位置の調整。
	//pPlayer->pos.y += s_player.fLength;	// 半径分床の当たり判定を底上げする

	// 角度の正規化
	NormalizeRot(pPlayer->rot.y);
}

//=========================================
// 移動
//=========================================
void MovePlayer()
{
	D3DXVECTOR3 CameraRot = GetRotCamera();	// カメラの角度情報取得
	D3DXVECTOR3 move = ZERO_VECTOR;			// 移動量の初期化
	float move_max = 0.0f;

	if (IsJoyPadUse(0))
	{// ジョイパッドの使用
		if (GetJoypadStick(JOYKEY_LEFT_STICK, 0).x != 0.0f || GetJoypadStick(JOYKEY_LEFT_STICK, 0).y != 0.0f)
		{
			float rot;
			move.x = GetJoypadStick(JOYKEY_LEFT_STICK, 0).x;
			move.z = GetJoypadStick(JOYKEY_LEFT_STICK, 0).y * -1.0f;

			rot = atan2f(move.x, move.z);

			move.x = sinf(rot + CameraRot.y);
			move.z = cosf(rot + CameraRot.y);

			move_max = fabsf(move.x) + fabsf(move.z);
			if (move_max >= 1.0f)
			{
				move_max = 1.0f;
			}
		}
	}
	else
	{
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
	}

	D3DXVECTOR3 axis;	// 回転軸

	D3DXVec3Normalize(&move, &move);	// 正規化する(大きさ１のベクトルにする)

	D3DXVECTOR3 inverseVec = move * -1;		// move値を反対にする
	D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXVec3Cross(&axis, &inverseVec, &vecY);	// 行列計算
	D3DXQUATERNION quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// クオータニオン

	D3DXQuaternionRotationAxis(&quaternion, &axis, MODEL_ROT_ATTENUATION * move_max);	// 回転軸と回転角度を指定

	s_player.aModel[0]->quaternion *= quaternion;

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&s_player.aModel[0]->quaternion, &s_player.aModel[0]->quaternion);

	// 方向ベクトル掛ける移動量
	s_player.movevec = move * PLAYER_MOVE * move_max;
	s_player.pos += s_player.movevec;
}

//=========================================
// モデルパーツ同士の当たり判定
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

		// 当たった場合
		if (SphereColision(s_player.aModel[0]->pos_world, s_player.fLength, model->pos_world, model->MaxVtx.x))
		{
			if (s_player.fLength >= model->MaxVtx.x * 1.5f)
			{	// 取り込めるサイズの場合
				D3DXMATRIX mtxRot;
				D3DXVECTOR3 pos_local = model->pos_world - s_player.pos_old;
				D3DXVECTOR3 v = ZERO_VECTOR;

				D3DXQUATERNION quaternionHit = s_player.aModel[0]->quaternion;
				quaternionHit.w *= -1;

				// クォータニオンの使用した姿勢の設定
				D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
				D3DXVec3TransformCoord(&model->pos, &pos_local, &mtxRot);

				D3DXQUATERNION quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// クオータニオン
				SetLine(&v, &quaternion, model->pos_world, s_player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				model->quaternion = quaternionHit;
				model->nIdxModelParent = 0;
			}
			else
			{	// 取り込めないサイズの場合
				D3DXVECTOR3 vec(model->pos_world - s_player.pos);	// 方向ベクトル
				D3DXVec3Normalize(&vec, &vec);						// 当たった方向を取得

				float fLength = s_player.fLength + model->MaxVtx.x;	// 長さ

				// 方向ベクトルに長さを掛ける
				vec *= fLength;
				
				// 速度を0にする
				s_player.movevec = ZERO_VECTOR;

				s_player.pos = model->pos_world - vec;
			}
		}
	}
}

//--------------------------------------------------
// プレイヤーのサイズを調べる
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
	s_player.fLength = sqrtf((v.x + v.y + v.z) / 6.0f - s_player.aModel[0]->MaxVtx.x) + s_player.aModel[0]->MaxVtx.x;
	s_player.pos.y += s_player.fLength;	// プレイヤーの位置を底上げ。
}

//=========================================r
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

	//// スケールの反映
	//D3DXMatrixScaling(&mtxScale, 1.0f, 1.0f, 1.0f);
	//D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxScale);							// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	
	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_player.rot.y, s_player.rot.x, s_player.rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&s_player.mtxWorld, &s_player.mtxWorld, &mtxTrans);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_player.mtxWorld);
}


//--------------------------------------------------
// 読み込み処理
//--------------------------------------------------
void LoadPlayer(void)
{
	FILE* pFile;
	char modelFile[255] = {};	// モデルファイル

	pFile = fopen("data/FILE/Player.txt", "r");

	char read[255] = {};

	fscanf(pFile, "%s", &read);
	if (strcmp(&read[0], "PlayerModel") == 0)
	{
		fscanf(pFile, "%s", &read);			// = の除去
		fscanf(pFile, "%s", &modelFile);		// 値を入れる
	}

	read[0] = *modelFile;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Model* model = s_player.aModel[0];

	// Xファイルの読み込み
	D3DXLoadMeshFromX(modelFile,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&model->pBuffMat,
		NULL,
		&model->nNumMat,
		&model->pMesh);

	// モデルのサイズ計測
	ModelSize(&(s_player.MinVtx), &(s_player.MaxVtx), model->pMesh);
	// モデルのサイズ計測
	ModelSize(&model->MinVtx, &model->MaxVtx, model->pMesh);

	// メッシュに使用されているテクスチャ用の配列を用意する
	model->pTexture = new LPDIRECT3DTEXTURE9[model->nNumMat];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int j = 0; j < (int)model->nNumMat; j++)
	{
		model->pTexture[j] = NULL;

		if (pMat[j].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
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

//--------------------------------------------------
// 取得
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}

//--------------------------------------------------
// モデル取得
//--------------------------------------------------
Model** GetPlayerModel(void)
{
	return s_player.aModel;
}