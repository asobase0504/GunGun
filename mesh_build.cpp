//=========================================
// 
// ポリゴンの作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "mesh_build.h"
#include "polygon.h"
#include "setup.h"

#define SIZE	(50)

//------------------------------------
// ポリゴンの種類の列挙型
//------------------------------------
typedef enum
{
	POLYGON_FLOOR = 0,
	POLYGON_MAX
}POLYGON_TYPE;

//------------------------------------
// ポリゴンの構造体を定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 rot;	// 回転座標
	D3DXMATRIX mtxWorld;// ワールドマトリックス
} Mesh;

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};	// 頂点バッファーへのポインタ
static LPDIRECT3DTEXTURE9 s_pTexture = {};		// テクスチャへのポインタ
static Mesh s_abillboard;					// ポリゴンの構造体

//=========================================
// 初期化
//=========================================
void InitMeshBuild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 初期化処理
	s_abillboard.pos = ZERO_VECTOR;	// 頂点座標
	s_abillboard.rot = ZERO_VECTOR;	// 回転座標

									// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/07.彼方への君に捧ぐ.png",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 14,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// 頂点座標をロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-SIZE, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-SIZE, 0.0f, SIZE);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, SIZE);
	pVtx[4].pos = D3DXVECTOR3(SIZE, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SIZE, 0.0f, SIZE);
	pVtx[6].pos = D3DXVECTOR3(SIZE, 0.0f, SIZE);
	pVtx[7].pos = D3DXVECTOR3(-SIZE, 0.0f, -SIZE);
	pVtx[8].pos = D3DXVECTOR3(-SIZE, 0.0f, -SIZE);
	pVtx[9].pos = D3DXVECTOR3(-SIZE, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, -SIZE);
	pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[12].pos = D3DXVECTOR3(SIZE, 0.0f, -SIZE);
	pVtx[13].pos = D3DXVECTOR3(SIZE, 0.0f, 0.0f);

	// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[11].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[12].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.5f);;

	pVtx += 4;

	// 頂点座標をアンロック
	s_pVtxBuff->Unlock();

}

//=========================================
// 終了
//=========================================
void UninitMeshBuild(void)
{
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
}

//=========================================
// 更新
//=========================================
void UpdateMeshBuild(void)
{
}

//=========================================
// 描画
//=========================================
void DrawMeshBuild(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_abillboard.mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_abillboard.rot.y, s_abillboard.rot.x, s_abillboard.rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&s_abillboard.mtxWorld, &s_abillboard.mtxWorld, &mtxRot);							// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_abillboard.pos.x, s_abillboard.pos.y, s_abillboard.pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&s_abillboard.mtxWorld, &s_abillboard.mtxWorld, &mtxTrans);							// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_abillboard.mtxWorld);	// ワールド座標行列の設定

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, s_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 12);

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

}

D3DXVECTOR3 GetMeshBuildPos(void)
{
	return s_abillboard.pos;
}
