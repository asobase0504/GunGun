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
#include "player.h"
#include "input.h"
#include "setup.h"
#include "camera.h"
#include "shadow.h"

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
// 静的変数
//------------------------------------
static LPD3DXMESH s_pMesh = NULL;				// メッシュ情報へのポインタ		// 頂点の集まりのこと
static LPD3DXBUFFER s_pBuffMat = NULL;			// マテリアル情報へのポインタ	// 1つのXファイルに複数のマテリアルが入っている
static LPDIRECT3DTEXTURE9 *s_pTexture = NULL;	// テクスチャへのポインタ
static DWORD s_nNumMat = 0;						// マテリアル情報の数
static D3DXMATRIX s_mtxWorld;					// ワールドマトリックス
static Player s_player;							// モデルの構造体
static MODEL_STATE s_state;						// モデルのステータス
static int s_nShadowCnt;						// 影の割り当て

//=========================================
// 初期化
//=========================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/player.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	// メッシュに使用されているテクスチャ用の配列を用意する
	s_pTexture = new LPDIRECT3DTEXTURE9[s_nNumMat];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		s_pTexture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&s_pTexture[i]);
		}
		else
		{
			s_pTexture[i] = NULL;
		}
	}

	s_player.pos = ZERO_VECTOR;
	s_player.pos.y = 10.0f;
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
		for (int i = 0; i < (int)s_nNumMat; i++)
		{
			if (s_pTexture[i] != NULL)
			{// テクスチャの解放
				s_pTexture[i]->Release();
				s_pTexture[i] = NULL;
			}
		}

		delete[](s_pTexture);
		s_pTexture = NULL;
	}

	// メッシュの解放
	if(s_pMesh != NULL)
	{
		s_pMesh->Release();
		s_pMesh = NULL;
	}
	// マテリアルの解放
	if (s_pBuffMat != NULL)
	{
		s_pBuffMat->Release();
		s_pBuffMat = NULL;
	}
}

//=========================================
// 更新
//=========================================
void UpdatePlayer(void)
{
	Player* pPlayer = &(s_player);
	// 移動処理
	MovePlayer();

	pPlayer->pos += pPlayer->movevec;
	pPlayer->rot.y += NormalizeRot(pPlayer->rotDest.y - pPlayer->rot.y) * MODEL_ROT_ATTENUATION;

	// 角度の正規化
	NormalizeRot(pPlayer->rot.y);

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = pPlayer->pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = pPlayer->pos.z;
	SetPositionShadow(s_nShadowCnt,ShadowPos);
}

//=========================================
// 描画
//=========================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ
	D3DXVECTOR3 scale(1.0f, 1.0f, 1.0f);

	//s_player.quaternion = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);	// クォータニオン

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_mtxWorld);

	D3DXQuaternionRotationAxis(&s_player.quaternion, &s_player.axis, s_player.moverot);	// 回転軸と回転角度を指定

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

	D3DXMatrixRotationQuaternion(
		&mtxRot,
		&s_player.quaternion
	);

	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxRot);								// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_player.pos.x, s_player.pos.y, s_player.pos.z);	// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxTrans);							// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
																								
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_mtxWorld);

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[i]);

		// モデルパーツの描写
		s_pMesh->DrawSubset(i);
	}
	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 移動
//=========================================
void MovePlayer()
{
	Player* pPlayer = &(s_player);
	D3DXVECTOR3 CameraRot = GetRotCamera();	// カメラの角度情報取得
	D3DXVECTOR3 move = ZERO_VECTOR;			// 移動量の初期化
	D3DXVECTOR3 oldaxis = s_player.axis;	// 変更前の情報を取得

	// モデルの移動
	if (GetKeyboardPress(DIK_UP))
	{
		move.x += sinf(CameraRot.y);
		move.z += cosf(CameraRot.y);
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		move.x += sinf(D3DX_PI * -0.5f + CameraRot.y);
		move.z += cosf(D3DX_PI * -0.5f + CameraRot.y);
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		move.x += sinf(D3DX_PI + CameraRot.y);
		move.z += cosf(D3DX_PI + CameraRot.y);
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

	if (D3DXVec3Length(&move) == 0.0f)
	{
		pPlayer->movevec *= 0.95f;
	}
	else
	{
		D3DXVec3Normalize(&move, &move);	// 正規化する(大きさ１のベクトルにする)
		//s_player.axis = 
		D3DXVec3Cross(&s_player.axis, &-move, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		pPlayer->moverot += 0.05f;
		pPlayer->moverot = NormalizeRot(pPlayer->moverot);
	}

	pPlayer->movevec =  move * MODEL_MOVE;
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
Player *GetPlayer(void)
{
	return &s_player;
}
