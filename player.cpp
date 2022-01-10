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
#define MODEL_ROT_ATTENUATION	(0.05f)	// 減算
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
void LoadPlayerModel(void);		// プレイヤーパーツの読み込み処理
void ColisionPartsModel(void);	// モデルパーツ同士の当たり判定
void LookUpSizePlayer(void);	// プレイヤーのサイズを調べる

//------------------------------------
// 静的変数
//------------------------------------
static Player s_player;							// モデルの構造体
static MODEL_STATE s_state;						// モデルのステータス
static int s_nShadowCnt;						// 影の割り当て

//=========================================
// 初期化
//=========================================
void InitPlayer(void)
{

	// 読み込み処理
	LoadPlayer();

	s_player.pos = ZERO_VECTOR;
	s_player.pos.y = -s_player.MinVtx.y;
	s_player.rot = ZERO_VECTOR;
	s_player.movevec = ZERO_VECTOR;
	s_player.aModel[0].quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// クォータニオン

	// プレイヤーにくっつくモデルの配置
	LoadPlayerModel();

	for (int i = 0; i < sizeof(s_player.aModel) / sizeof(s_player.aModel[0]); i++)
	{
		Model* model = &(s_player.aModel[i]);

		if (!(model->bUse))
		{
			continue;
		}

		if (model->nIdxModelParent == -2)
		{
			model->pos_world = model->pos;
		}
		else
		{
			model->pos_world = s_player.pos - model->pos;
		}

		D3DXVECTOR3 start;
		D3DXVECTOR3 end;

		// 線の作成
		start = D3DXVECTOR3(model->MinVtx.x, model->MaxVtx.y, model->MinVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MinVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MaxVtx.y, model->MinVtx.z);				// 線の始点
		end	= D3DXVECTOR3(model->MaxVtx.x, model->MinVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MinVtx.x, model->MaxVtx.y, model->MaxVtx.z);				// 線の始点
		end	= D3DXVECTOR3(model->MinVtx.x, model->MinVtx.y, model->MaxVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		/*	x軸		*/
		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MaxVtx.y, model->MaxVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MaxVtx.y, model->MaxVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MinVtx.y, model->MinVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MinVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MaxVtx.y, model->MinVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MaxVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MinVtx.y, model->MaxVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MinVtx.y, model->MaxVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MaxVtx.y, model->MaxVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MaxVtx.y, model->MaxVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		/*	y軸		*/
		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MaxVtx.y, model->MaxVtx.z);				// 線の始点
		end	= D3DXVECTOR3(model->MaxVtx.x, model->MinVtx.y, model->MaxVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MinVtx.y, model->MinVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MinVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MaxVtx.y, model->MinVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MaxVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MinVtx.y, model->MaxVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MinVtx.y, model->MaxVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		/*	x軸		*/
		// 線の作成
		start = D3DXVECTOR3(model->MinVtx.x, model->MinVtx.y, model->MaxVtx.z);				// 線の始点
		end	= D3DXVECTOR3(model->MinVtx.x, model->MinVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MinVtx.y, model->MaxVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MaxVtx.x, model->MinVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MinVtx.x, model->MaxVtx.y, model->MaxVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MinVtx.x, model->MaxVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定

		// 線の作成
		start = D3DXVECTOR3(model->MaxVtx.x, model->MaxVtx.y, model->MaxVtx.z);				// 線の始点
		end = D3DXVECTOR3(model->MaxVtx.x, model->MaxVtx.y, model->MinVtx.z);				// 線の終点
		SetLine(&model->pos_world, &model->quaternion, start, end, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 線の設定
	}
}

//=========================================
// 終了
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
void UpdatePlayer(void)
{
	Player* pPlayer = &(s_player);

	pPlayer->pos_old = pPlayer->pos;	// プレイヤー位置の保存

	// モデル位置の保存
	for (int i = 0; i < PARTS_NUM; i++)
	{
		pPlayer->aModel[i].pos_old = pPlayer->aModel[i].pos;

		if (pPlayer->aModel[i].nIdxModelParent == -2)
		{
			pPlayer->aModel[i].pos_world = pPlayer->aModel[i].pos;
		}
		else
		{
			pPlayer->aModel[i].pos_world = s_player.pos - pPlayer->aModel[i].pos;
		}
		pPlayer->aModel[i].rot = D3DXVECTOR3(pPlayer->aModel[i].quaternion.x, pPlayer->aModel[i].quaternion.y, pPlayer->aModel[i].quaternion.z);
	}

	// 移動処理
	MovePlayer();
	
	// プレイヤーと床の当たり判定
	CollisionMeshField(&pPlayer->pos);

	// モデルパーツと床の当たり判定
	//CollisionMeshField(&pPlayer->pos,);

	//// プレイヤーとモデルの当たり判定
	//CollisionModel(&pPlayer->pos, &pPlayer->pos_old, pPlayer->MinVtx, pPlayer->MaxVtx);

	// モデルパーツごとの当たり判定
	ColisionPartsModel();

	// プレイヤーの球の半径を求める
	LookUpSizePlayer();
	
	// プレイヤー位置とモデル回転する軸の位置の調整。
	//pPlayer->pos.y += s_player.fLength;	// 半径分床の当たり判定を底上げする
	pPlayer->pos.y += -s_player.MinVtx.y;

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
	if (GetKeyboardPress(DIK_O))
	{
		s_player.rot.y += 0.05f;
	}

	D3DXVECTOR3 axis;	// 回転軸

	D3DXVec3Normalize(&move, &move);								// 正規化する(大きさ１のベクトルにする)
	D3DXVec3Cross(&axis, &-move, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));	// 行列計算

	D3DXQUATERNION quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// クオータニオン

	D3DXQuaternionRotationAxis(&quaternion, &axis, 0.1f);	// 回転軸と回転角度を指定

	s_player.aModel[0].quaternion *= quaternion;

	// クオータニオンのノーマライズ
	D3DXQuaternionNormalize(&s_player.aModel[0].quaternion, &s_player.aModel[0].quaternion);

	s_player.movevec = move * PLAYER_MOVE;
	s_player.pos += s_player.movevec;
	//pPlayer->rot.y += NormalizeRot(pPlayer->rotDest.y - pPlayer->rot.y) * MODEL_ROT_ATTENUATION;
}

//=========================================
// モデルパーツ同士の当たり判定
//=========================================
void ColisionPartsModel(void)
{
	for (int i = 0; i < PARTS_NUM; i++)
	{
		Model* model = &(s_player.aModel[i]);

		if (!(model->bUse) || model->nIdxModelParent == -2)
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

			bool isHit = false;

			if ((pos.x + model->MaxVtx.x > hitMin.x) && (pos.x + model->MinVtx.x < hitMax.x))
			{
				// 奥
				if ((pos.z + model->MaxVtx.z >= hitMin.z) && (pos_old.z + model->MaxVtx.z <= hitMin.z))
				{
					isHit = true;
				}
				// 手前
				else if ((pos.z + model->MinVtx.z <= hitMax.z) && pos_old.z + model->MinVtx.z >= hitMax.z)
				{
					isHit = true;
				}
			}
			if (pos.z + model->MaxVtx.z > hitMin.z && pos.z + model->MinVtx.z < hitMax.z)
			{
				// 左
				if (pos.x + model->MaxVtx.x >= hitMin.x && pos_old.x + model->MaxVtx.x <= hitMin.x)
				{
					isHit = true;
				}
				// 右
				else if (pos.x + model->MinVtx.x <= hitMax.x && pos_old.x + model->MinVtx.x >= hitMax.x)
				{
					isHit = true;
				}
			}

			// 当たった場合
			if (isHit)
			{
				hitModel->pos -= s_player.pos;
				hitModel->quaternion = s_player.aModel[0].quaternion;
				hitModel->nIdxModelParent = 0;
				//s_player.pos.z = hitMin.z - model->MaxVtx.z;
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
		Model* model = &(s_player.aModel[i]);

		if (model->nIdxModelParent == -2)
		{
			continue;
		}

		D3DXVECTOR3 modelMaxVtx = model->pos + model->MaxVtx;
		D3DXVECTOR3 modelMinVtx = model->pos + model->MinVtx;

		if (modelMinVtx.x < s_player.MinVtx.x)
		{
			s_player.MinVtx.x = modelMaxVtx.x;
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

	// 一番値が大きい値を半径にする。
	if (s_player.fLength < fabsf(s_player.MinVtx.x))
	{
		s_player.fLength = fabsf(s_player.MinVtx.x);
	}
	if (s_player.fLength < fabsf(s_player.MinVtx.y))
	{
		s_player.fLength = fabsf(s_player.MinVtx.y);
	}
	if (s_player.fLength < fabsf(s_player.MinVtx.z))
	{
		s_player.fLength = fabsf(s_player.MinVtx.z);
	}
	if (s_player.fLength < fabsf(s_player.MaxVtx.x))
	{
		s_player.fLength = fabsf(s_player.MaxVtx.x);
	}
	if (s_player.fLength < fabsf(s_player.MaxVtx.y))
	{
		s_player.fLength = fabsf(s_player.MaxVtx.y);
	}
	if (s_player.fLength < fabsf(s_player.MaxVtx.z))
	{
		s_player.fLength = fabsf(s_player.MaxVtx.z);
	}

	//s_player.fLength = s_player.fLength > fabsf(s_player.MinVtx.x) ? s_player.fLength : fabsf(s_player.MinVtx.x);
	//s_player.fLength = s_player.fLength > fabsf(s_player.MinVtx.y) ? s_player.fLength : fabsf(s_player.MinVtx.y);
	//s_player.fLength = s_player.fLength > fabsf(s_player.MinVtx.z) ? s_player.fLength : fabsf(s_player.MinVtx.z);
	//s_player.fLength = s_player.fLength > fabsf(s_player.MaxVtx.x) ? s_player.fLength : fabsf(s_player.MaxVtx.x);
	//s_player.fLength = s_player.fLength > fabsf(s_player.MaxVtx.y) ? s_player.fLength : fabsf(s_player.MaxVtx.y);
	//s_player.fLength = s_player.fLength > fabsf(s_player.MaxVtx.z) ? s_player.fLength : fabsf(s_player.MaxVtx.z);
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

	// スケールの反映
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

	for (int i = 0; i < PARTS_NUM; i++)
	{
		Model* model = &(s_player.aModel[i]);

		if (!model->bUse)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&model->mtxWorld);

		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &s_player.aModel[i].quaternion);			// クオータニオンによる行列回転
		D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxRot);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

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

		// プレイヤーとくっついている状態のモデルはプレイヤーとの行列計算
		if (model->nIdxModelParent != -2)
		{
			D3DXMatrixMultiply(&model->mtxWorld, &model->mtxWorld, &mtxParent);

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
	Model* model = &(s_player.aModel[0]);

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

	s_player.aModel[0].pos = ZERO_VECTOR;
	//s_player.aModel[0].pos.y = -s_player.MinVtx.y;
	s_player.aModel[0].rot = ZERO_VECTOR;
	s_player.aModel[0].nIdxModelParent = -1;
	s_player.aModel[0].bUse = true;

}

//=========================================
// 読み込み処理
//=========================================
void LoadPlayerModel(void)
{
	FILE* pFile;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	bool isModel = false;
	char modelFile[255][255] = {};
	int nTexCnt;
	int nModelFileCnt = 0;
	int nModelCnt = 1;

	pFile = fopen(MODEL_LOAD_FILE, "r");

	char read[255] = {};
	while (1)
	{
		fscanf(pFile, "%s", &read);

		//# を検出すると一行読み込む
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

				// Xファイルの読み込み
				D3DXLoadMeshFromX(&modelFile[nData][0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&model->pBuffMat,
					NULL,
					&model->nNumMat,
					&model->pMesh);

				// メッシュに使用されているテクスチャ用の配列を用意する
				model->pTexture = new LPDIRECT3DTEXTURE9[model->nNumMat];

				// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
				D3DXMATERIAL *pMat = (D3DXMATERIAL*)model->pBuffMat->GetBufferPointer();

				// 各メッシュのマテリアル情報を取得する
				for (int i = 0; i < (int)model->nNumMat; i++)
				{
					model->pTexture[i] = NULL;

					if (pMat[i].pTextureFilename != NULL)
					{// マテリアルで設定されているテクスチャ読み込み
						D3DXCreateTextureFromFileA(pDevice,
							pMat[i].pTextureFilename,
							&model->pTexture[i]);
					}
				}

				// モデルのサイズ計測
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

//--------------------------------------------------
// 取得
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}