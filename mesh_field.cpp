//=========================================
// 
// メッシュの作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "mesh_field.h"
#include "polygon.h"
#include "common.h"
#include "input.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define SIZE	(50.0f)

//------------------------------------
// ポリゴンの種類の列挙型
//------------------------------------
typedef enum
{
	POLYGON_FLOOR = 0,
	POLYGON_MAX
}POLYGON_TYPE;

//------------------------------------
// メッシュの構造体を定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標
	D3DXVECTOR3 rot;		// 回転座標
	int nSurfaceWidth;		// 面の幅
	int nSurfaceHeight;		// 面の高さ
	float fLineWidth;		// 辺の幅
	float fLineHeight;		// 辺の高さ
	int vertexCnt;			// 頂点数
	int polygonCnt;			// ポリゴン数
	int IdxCnt;				// インデックス数
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
} Mesh;


//------------------------------------
// プロトタイプ宣言
//------------------------------------
float Vec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2);

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};		// 頂点バッファーへのポインタ
static LPDIRECT3DTEXTURE9 s_pTexture = {};			// テクスチャへのポインタ
static LPDIRECT3DINDEXBUFFER9 s_pIdxBuff = NULL;	// インデックスバッファへのポインタ
static Mesh s_aMesh = {};							// ポリゴンの構造体
static int* s_aIdx = {};							// インデックスの配列

//=========================================
// 初期化
//=========================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// nSurfaceWidth × nSurfaceHeight
	s_aMesh.nSurfaceWidth = 30;			// X軸の面の数
	s_aMesh.nSurfaceHeight = 30;		// Y軸の面の数
	s_aMesh.fLineWidth = 50.0f;			// X軸の面の数
	s_aMesh.fLineHeight = 50.0f;		// Y軸の面の数

	int nLineVtx = (s_aMesh.nSurfaceWidth + 1);				// X軸の頂点数

	s_aMesh.vertexCnt = nLineVtx * (s_aMesh.nSurfaceHeight + 1);	// 頂点数

	// ポリゴン数を求める計算
	s_aMesh.polygonCnt
		= 2 * s_aMesh.nSurfaceWidth * s_aMesh.nSurfaceHeight		// 一行分のポリゴン数
		+ 4 * (s_aMesh.nSurfaceHeight - 1);				// 縮退ポリゴン数

	s_aMesh.IdxCnt = s_aMesh.polygonCnt + 2;	// インデックス数

//別解
/*
	// インデックス数を求める計算
	s_aMesh.IdxCnt
	= 2 * (s_aMesh.nSurfaceWidth + 1) * s_aMesh.nSurfaceHeight	// 一行分のインデックス数
	+ 2 * (s_aMesh.nSurfaceHeight - 1);					// 改行時に発生する重複しているインデックス数

	s_aMesh.polygonCnt = s_aMesh.IdxCnt - 2;	// ポリゴン数
*/

	s_aMesh.pos = ZERO_VECTOR;	// 頂点座標
	s_aMesh.rot = ZERO_VECTOR;	// 回転座標

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/暗転.jpg",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * s_aMesh.polygonCnt,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * s_aMesh.IdxCnt,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&s_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// 頂点座標をロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	for (int Z = 0; Z <= s_aMesh.nSurfaceHeight;Z++)
	{
		for (int X = 0; X <= s_aMesh.nSurfaceWidth; X++)
		{
			pVtx[X + Z * nLineVtx].pos = D3DXVECTOR3((X - s_aMesh.nSurfaceWidth * 0.5f) *  s_aMesh.fLineWidth, 0.0f, (Z - s_aMesh.nSurfaceHeight * 0.5f) * -s_aMesh.fLineHeight);

			pVtx[X + Z * nLineVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[X + Z * nLineVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[X + Z * nLineVtx].tex = D3DXVECTOR2((float)X, (float)Z);
		}
	}

	// 頂点座標をアンロック
	s_pVtxBuff->Unlock();

	// メッシュに使用されているテクスチャ用の配列を用意する
	s_aIdx = new int[s_aMesh.IdxCnt];

	WORD* pIdx;
	s_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスを求める
	for (int Y = 0; Y < s_aMesh.nSurfaceHeight; Y++)
	{
		int nlineTop = Y * (nLineVtx * 2 + 2);
		for (int X = 0; X <= s_aMesh.nSurfaceWidth; X++)
		{
			int nIdxData = X * 2 + nlineTop;
			pIdx[nIdxData + 1] = (WORD)(X + nLineVtx * Y);
			pIdx[nIdxData] = (WORD)(pIdx[nIdxData + 1] + nLineVtx);
			s_aIdx[nIdxData + 1] = (WORD)pIdx[nIdxData + 1];
			s_aIdx[nIdxData] = (WORD)pIdx[nIdxData];
		}

		if (Y < s_aMesh.nSurfaceHeight - 1)
		{
			pIdx[nLineVtx * 2 + nlineTop] = (WORD)(s_aMesh.nSurfaceWidth + nLineVtx * Y);
			pIdx[nLineVtx * 2 + 1 + nlineTop] = (WORD)(nLineVtx * 2 + nLineVtx * Y);
			s_aIdx[nLineVtx * 2 + nlineTop] = (WORD)pIdx[nLineVtx * 2 + nlineTop];
			s_aIdx[nLineVtx * 2 + 1 + nlineTop] = (WORD)pIdx[nLineVtx * 2 + 1 + nlineTop];

		}
	}

	s_pIdxBuff->Unlock();
}

//=========================================
// 終了
//=========================================
void UninitMeshField(void)
{
	// インデックス数の保存
	if (s_aIdx != NULL)
	{
		delete[](s_aIdx);
		s_aIdx = NULL;
	}

	// テクスチャの破棄
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// 頂点バッファーの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

	// インデックスバッファの破棄
	if (s_pIdxBuff != NULL)
	{
		s_pIdxBuff->Release();
		s_pIdxBuff = NULL;
	}
}

//=========================================
// 更新
//=========================================
void UpdateMeshField(void)
{
}

//=========================================
// 描画
//=========================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_aMesh.mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_aMesh.rot.y, s_aMesh.rot.x, s_aMesh.rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&s_aMesh.mtxWorld, &s_aMesh.mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_aMesh.pos.x, s_aMesh.pos.y, s_aMesh.pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&s_aMesh.mtxWorld, &s_aMesh.mtxWorld, &mtxTrans);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_aMesh.mtxWorld);	// ワールド座標行列の設定

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(s_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,0,0,s_aMesh.vertexCnt,0,s_aMesh.polygonCnt);

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);
}

//=========================================
// メッシュの座標を取得
//=========================================
D3DXVECTOR3 GetMeshFieldPos(void)
{
	return s_aMesh.pos;
}

//=========================================
// メッシュフィールドの当たり判定
// 引数 当たり判定を行う位置
//=========================================
void CollisionMeshField(D3DXVECTOR3 * pos)
{
	VERTEX_3D* pVtx = NULL;
	D3DXVECTOR3 vecField[3];	// ポリゴンの線分
	D3DXVECTOR3 vecModel[3];	// モデルからポリゴンの線分

	// 頂点座標をロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の反映
	for (int i = 0; i <= (s_aMesh.IdxCnt - 3); i++)
	{
		if (s_aIdx[i] == s_aIdx[i + 1] || s_aIdx[i + 1] == s_aIdx[i + 2] || s_aIdx[i] == s_aIdx[i + 2])
		{
			continue;
		}

		vecField[0] = pVtx[s_aIdx[i + 1]].pos - pVtx[s_aIdx[i]].pos;
		vecField[1] = pVtx[s_aIdx[i + 2]].pos - pVtx[s_aIdx[i + 1]].pos;
		vecField[2] = pVtx[s_aIdx[i]].pos - pVtx[s_aIdx[i + 2]].pos;

		vecModel[0] = *pos - pVtx[s_aIdx[i]].pos;
		vecModel[1] = *pos - pVtx[s_aIdx[i + 1]].pos;
		vecModel[2] = *pos - pVtx[s_aIdx[i + 2]].pos;

		float crs_v1 = Vec2Cross(&vecModel[0], &vecField[0]);
		float crs_v2 = Vec2Cross(&vecModel[1], &vecField[1]);
		float crs_v3 = Vec2Cross(&vecModel[2], &vecField[2]);

		// 乗ってるメッシュかチェック
		if (i % 2 == 0)
		{
			if (crs_v1 >= 0.0f && crs_v2 >= 0.0f && crs_v3 >= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);
				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}
				D3DXVec3Normalize(&N, &N);

				pVtx[s_aIdx[i + 0]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[s_aIdx[i + 1]].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[s_aIdx[i + 2]].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
				pos->y = pVtx[s_aIdx[i]].pos.y - 1.0f / N.y * (N.x * (pos->x - pVtx[s_aIdx[i]].pos.x) + N.z * (pos->z - pVtx[s_aIdx[i]].pos.z));
			}
		}
		else
		{
			if (crs_v1 <= 0.0f && crs_v2 <= 0.0f && crs_v3 <= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);
				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}
				D3DXVec3Normalize(&N, &N);

				pVtx[s_aIdx[i + 0]].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[s_aIdx[i + 1]].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				pVtx[s_aIdx[i + 2]].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
				pos->y = pVtx[s_aIdx[i]].pos.y - 1.0f / N.y * (N.x * (pos->x - pVtx[s_aIdx[i]].pos.x) + N.z * (pos->z - pVtx[s_aIdx[i]].pos.z));
			}
		}
	}

	// 頂点座標をアンロック
	s_pVtxBuff->Unlock();

}

//=========================================
// メッシュフィールドの当たり判定
// 引数１ 実際に位置をずらす値
// 引数２ 当たり判定を行う位置
//=========================================
void CollisionMeshField(D3DXVECTOR3* pos, D3DXVECTOR3* HitPos)
{
	VERTEX_3D* pVtx = NULL;
	D3DXVECTOR3 vecField[3];						// ポリゴンの線分
	D3DXVECTOR3 vecModel[3];						// モデルからポリゴンの線分

	// 頂点座標をロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の反映
	for (int i = 0; i <= (s_aMesh.IdxCnt - 3); i++)
	{
		if (s_aIdx[i] == s_aIdx[i + 1] || s_aIdx[i + 1] == s_aIdx[i + 2] || s_aIdx[i] == s_aIdx[i + 2])
		{
			continue;
		}

		vecField[0] = pVtx[s_aIdx[i + 1]].pos - pVtx[s_aIdx[i]].pos;
		vecField[1] = pVtx[s_aIdx[i + 2]].pos - pVtx[s_aIdx[i + 1]].pos;
		vecField[2] = pVtx[s_aIdx[i]].pos - pVtx[s_aIdx[i + 2]].pos;

		vecModel[0] = *HitPos - pVtx[s_aIdx[i]].pos;
		vecModel[1] = *HitPos - pVtx[s_aIdx[i + 1]].pos;
		vecModel[2] = *HitPos - pVtx[s_aIdx[i + 2]].pos;

		float crs_v[3];
		crs_v[0] = Vec2Cross(&vecModel[0], &vecField[0]);
		crs_v[1] = Vec2Cross(&vecModel[1], &vecField[1]);
		crs_v[2] = Vec2Cross(&vecModel[2], &vecField[2]);

		// 乗ってるメッシュかチェック
		if (i % 2 == 0)
		{
			if (crs_v[0] >= 0.0f && crs_v[1] >= 0.0f && crs_v[2] >= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);
				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}
				D3DXVec3Normalize(&N, &N);

				pVtx[s_aIdx[i + 0]].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[s_aIdx[i + 1]].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				pVtx[s_aIdx[i + 2]].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

				float fLength;
				fLength = pVtx[s_aIdx[i]].pos.y - 1.0f / N.y * (N.x * (HitPos->x - pVtx[s_aIdx[i]].pos.x) + N.z * (HitPos->z - pVtx[s_aIdx[i]].pos.z));

				pos->y = -HitPos->y;
			}
		}
		else
		{
			if (crs_v[0] >= 0.0f && crs_v[1] >= 0.0f && crs_v[2] >= 0.0f)
			{
				D3DXVECTOR3 N;
				D3DXVec3Cross(&N, &vecField[0], &vecField[1]);
				if (N.y < 0.0f)
				{
					N *= -1.0f;
				}
				D3DXVec3Normalize(&N, &N);

				pVtx[s_aIdx[i + 0]].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				pVtx[s_aIdx[i + 1]].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
				pVtx[s_aIdx[i + 2]].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

				float fLength;
				fLength = pVtx[s_aIdx[i]].pos.y - 1.0f / N.y * (N.x * (HitPos->x - pVtx[s_aIdx[i]].pos.x) + N.z * (HitPos->z - pVtx[s_aIdx[i]].pos.z));

				pos->y = -HitPos->y;
			}
		}
	}

	// 頂点座標をアンロック
	s_pVtxBuff->Unlock();

}

//=========================================
// 2Dベクトルの外積
//=========================================
float Vec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->z - v1->z * v2->x;
}