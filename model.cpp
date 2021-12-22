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
static LPDIRECT3DTEXTURE9 *s_pTexture = NULL;	// テクスチャへのポインタ
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
		&s_model.pBuffMat,
		NULL,
		&s_model.nNumMat,
		&s_model.pMesh);

	// モデルのサイズ計測
	ModelSize(&(s_model.MinVtx), &(s_model.MaxVtx), s_model.pMesh);

	// メッシュに使用されているテクスチャ用の配列を用意する
	s_pTexture = new LPDIRECT3DTEXTURE9[s_model.nNumMat];

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)s_model.nNumMat; i++)
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
	ModelSize(&s_model.MinVtx, &s_model.MaxVtx, s_model.pMesh);

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
		for (int i = 0; i < (int)s_model.nNumMat; i++)
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
	if(s_model.pMesh != NULL)
	{
		s_model.pMesh->Release();
		s_model.pMesh = NULL;
	}
	// マテリアルの解放
	if (s_model.pBuffMat != NULL)
	{
		s_model.pBuffMat->Release();
		s_model.pBuffMat = NULL;
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

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_model.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_model.rot.y, s_model.rot.x, s_model.rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);		// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&s_model.mtxWorld, &s_model.mtxWorld, &mtxTrans);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_model.mtxWorld);;

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)s_model.pBuffMat->GetBufferPointer();
	for (int i = 0; i < (int)s_model.nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, s_pTexture[i]);

		// モデルパーツの描写
		s_model.pMesh->DrawSubset(i);
	}
	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 当たり判定
//=========================================
void CollisionModel(D3DXVECTOR3 * pos, D3DXVECTOR3 * pos_old, D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	if (pos->x + max.x > s_model.pos.x + s_model.MinVtx.x && pos->x + min.x < s_model.pos.x + s_model.MaxVtx.x)
	{
		// 奥
		if (pos->z + max.z >= s_model.pos.z + s_model.MinVtx.z && pos_old->z + max.z <= s_model.pos.z + s_model.MinVtx.z)
		{
			pos->z = s_model.pos.z + s_model.MinVtx.z - max.z;
		}
		// 手前
		else if (pos->z + min.z <= s_model.pos.z + s_model.MaxVtx.z && pos_old->z + min.z >= s_model.pos.z + s_model.MaxVtx.z)
		{
			pos->z = s_model.pos.z + s_model.MaxVtx.z - min.z;
		}
	}
	if (pos->z + max.z > s_model.pos.z + s_model.MinVtx.z && pos->z + min.z < s_model.pos.z + s_model.MaxVtx.z)
	{
		// 左
		if (pos->x + max.x >= s_model.pos.x + s_model.MinVtx.x && pos_old->x + max.x <= s_model.pos.x + s_model.MinVtx.x)
		{
			pos->x = s_model.pos.x + s_model.MinVtx.x - max.x;
		}
		// 右
		else if (pos->x + min.x <= s_model.pos.x + s_model.MaxVtx.x && pos_old->x + min.x >= s_model.pos.x + s_model.MaxVtx.x)
		{
			pos->x = s_model.pos.x + s_model.MaxVtx.x - min.x;
		}
	}
}

//=========================================
// 取得
//=========================================
Model *GetModel(void)
{
	return &s_model;
}
