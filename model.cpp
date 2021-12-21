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
#include "setup.h"
#include "camera.h"
#include "shadow.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define MODEL_MOVE				(1.0f)
#define MODEL_ROT_ATTENUATION	(0.05f)

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
static Model s_model;							// モデルの構造体
static MODEL_STATE s_state;						// モデルのステータス
static int s_nShadowCnt;						// 影の割り当て

//=========================================
// 初期化
//=========================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファーへのポイント

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/bee_head.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	// モデルのサイズ計測
	ModelSize(&(s_model.MinVtx), &(s_model.MaxVtx), s_pMesh);

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

	// モデルのサイズ計測
	ModelSize(&s_model.MinVtx, &s_model.MaxVtx, s_pMesh);

	s_model.pos = ZERO_VECTOR;
	s_model.pos = D3DXVECTOR3(20.0f,s_model.MinVtx.y * -1.0f,0.0f);
	s_model.rot = ZERO_VECTOR;
	s_model.vec = ZERO_VECTOR;

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = s_model.pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = s_model.pos.z;
	s_nShadowCnt = SetShadow(ShadowPos, s_model.rot);
}

//=========================================
// 終了
//=========================================
void UninitModel(void)
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
void UpdateModel(void)
{
	Model* pModel = &(s_model);
	//MoveModel();

	pModel->pos += pModel->vec;
	pModel->rot.y += NormalizeRot(pModel->rotDest.y - pModel->rot.y) * MODEL_ROT_ATTENUATION;

	// 角度の正規化
	NormalizeRot(pModel->rot.y);

	D3DXVECTOR3 ShadowPos;
	ShadowPos.x = pModel->pos.x;
	ShadowPos.y = 0.01f;
	ShadowPos.z = pModel->pos.z;
	SetPositionShadow(s_nShadowCnt,ShadowPos);
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

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_model.rot.y, s_model.rot.x, s_model.rot.z);		// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxRot);										// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxTrans);								// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_mtxWorld);;

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
void MoveModel()
{
	Model* pModel = &(s_model);
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
		pModel->pos.y += MODEL_MOVE;
	}
	if (GetKeyboardPress(DIK_B))
	{
		pModel->pos.y += -(MODEL_MOVE);
	}

	if (D3DXVec3Length(&move) == 0.0f)
	{
		pModel->vec *= 0.95f;
	}
	else
	{
		D3DXVec3Normalize(&move, &move);	// 正規化する(大きさ１のベクトルにする)
		pModel->rotDest.y = atan2f(move.x, move.z);
	}

	pModel->vec = pModel->vec * 0.95f + move * MODEL_MOVE * 0.05f;
}

//=========================================
// 当たり判定
//=========================================
void CollisionModel(D3DXVECTOR3 * pos, D3DXVECTOR3 * pos_old, D3DXVECTOR3 min, D3DXVECTOR3 max)
{

}

//=========================================
// 取得
//=========================================
Model *GetModel(void)
{
	return &s_model;
}
