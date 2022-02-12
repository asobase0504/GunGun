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
void ColisionPartsModel(void);	// モデルパーツ同士の当たり判定
void LookUpSizePlayer(void);	// プレイヤーのサイズを調べる

//------------------------------------
// 静的変数
//------------------------------------
static Player s_player;			// モデルの構造体
static MODEL_STATE s_state;		// モデルのステータス
static int s_nShadowCnt;		// 影の割り当て
static Model s_getModel;		// くっついたモデル

//=========================================
// 初期化
//=========================================
void InitPlayer(void)
{
	ZeroMemory(&s_player, sizeof(s_player));
	ZeroMemory(&s_getModel, sizeof(s_getModel));

	// プレイヤーにくっつくモデルの配置
	LoadModel();
	LoadMap();
	// 読み込み処理
	//LoadPlayer();

	s_player.fLength += s_player.aModel[0]->fLength;
	s_player.pos = ZERO_VECTOR;
	s_player.pos.y = -s_player.MinVtx.y;
	s_player.rot = ZERO_VECTOR;
	s_player.movevec = ZERO_VECTOR;
	s_player.aModel[0]->quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// クォータニオン

	if (s_player.pos.y - s_player.fLength <= 0.0f)
	{
		s_player.pos.y = s_player.fLength;
	}

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

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

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

		model->pos_world = D3DXVECTOR3(model->mtxWorld._41, model->mtxWorld._42, model->mtxWorld._43);
	}

	// 移動処理
	MovePlayer();
	
	// プレイヤーと床の当たり判定
	CollisionMeshField(&pPlayer->pos);

	// モデルパーツごとの当たり判定
	ColisionPartsModel();

	// プレイヤーの球の半径を求める
	LookUpSizePlayer();
	
	// 地面までの離す距離
	pPlayer->pos.y -= 0.5f;
	if (pPlayer->pos.y - pPlayer->fLength <= 0.0f)
	{
		pPlayer->pos.y = pPlayer->fLength;
	}

	// 角度の正規化
	NormalizeRot(&pPlayer->rot.y);

	SetModelUI(&s_getModel);
}

//=========================================
// 移動
//=========================================
void MovePlayer()
{
	D3DXVECTOR3 move = ZERO_VECTOR;			// 移動量の初期化
	float moveLength = 0.0f;


	D3DXVECTOR2 moveInput;

	if (IsJoyPadUse(0))
	{// ジョイパッドの使用
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

		// モデルの移動
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
		// カメラの角度情報取得
		D3DXVECTOR3* CameraRot = GetRotCamera();

		D3DXVec2Normalize(&moveInput, &moveInput);

		float c = cosf(-CameraRot->y);
		float s = sinf(-CameraRot->y);

		// move の長さは 1 になる。
		move.x = moveInput.x * c - moveInput.y * s;
		move.z = moveInput.x * s + moveInput.y * c;
	}
	else
	{ // 入力されていない。
		return;
	}

	D3DXVECTOR3 axis;	// 回転軸
	D3DXVECTOR3 inverseVec = -move;		// move値を反対にする
	D3DXVECTOR3 vecY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVec3Cross(&axis, &inverseVec, &vecY);	// 外積で回転軸を算出。

	D3DXQUATERNION quaternion;
	D3DXQuaternionRotationAxis(&quaternion, &axis, moveLength * MODEL_ROT_ATTENUATION);	// 回転軸と回転角度を指定

	s_player.aModel[0]->quaternion *= quaternion;
	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&s_player.aModel[0]->quaternion, &s_player.aModel[0]->quaternion);

	// 方向ベクトル掛ける移動量
	s_player.movevec = move * moveLength * PLAYER_MOVE;
	s_player.pos += s_player.movevec;
}

//=========================================
// モデルパーツの消失
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
// モデルパーツ同士の当たり判定
//=========================================
void ColisionPartsModel(void)
{
//	Model* model = &(s_player.aModel[0]);

	for (int j = 0; j < PARTS_NUM; j++)
	{
		Model* model = s_player.aModel[j];

		if (model == NULL || !(model->bUse) || model->nIdxModelParent != -2)
		{
			continue;
		}

		// 当たった場合
		if ((SphereColision(s_player.aModel[0]->pos_world, s_player.fLength, model->pos_world, model->fLength) && model->typeCollision == COLLISION_SPHERE) ||
			(SphereCuboidColision(s_player.aModel[0]->pos_world, s_player.fLength, model->pos_world, model->size) && model->typeCollision == COLLISION_CUBOID))
		{
			if (s_player.fLength >= model->sizeCriter)
			{	// 取り込めるサイズの場合
				D3DXMATRIX mtxRot;
				D3DXVECTOR3 pos_local = model->pos_world - s_player.pos_old;
				D3DXVECTOR3 v = ZERO_VECTOR;

				D3DXQUATERNION quaternionHit = s_player.aModel[0]->quaternion;
				quaternionHit.w *= -1;

				// クォータニオンの使用した姿勢の設定
				D3DXMatrixRotationQuaternion(&mtxRot, &quaternionHit);			// クオータニオンによる行列回転
				D3DXVec3TransformCoord(&model->pos, &pos_local, &mtxRot);

				model->quaternion = quaternionHit;
				model->nIdxModelParent = 0;

				// プレイヤーの長さを規定値加算
				s_player.fLength += model->sizeAdd;

				s_player.nModelCnt++;
				s_getModel = *model;
			}
			else
			{	// 取り込めないサイズの場合
				D3DXVECTOR3 vec = (model->pos_world - s_player.pos);	// 方向ベクトル
				D3DXVec3Normalize(&vec, &vec);						// 当たった方向を取得

				// 速度を0にする
				s_player.movevec = ZERO_VECTOR;

				s_player.pos -= vec;

				// 上る処理
				s_player.pos.y += 1.0f;
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
	//s_player.fLength = sqrtf((v.x + v.y + v.z) / 6.0f - s_player.aModel[0]->MaxVtx.x) + s_player.aModel[0]->MaxVtx.x;
}

//=========================================
// 描画
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX /*mtxScale,*/ mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_player.mtxWorld);
	
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