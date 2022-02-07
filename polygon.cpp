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
#include "polygon.h"
#include "common.h"
#include "camera.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define MAX_POLYGON	(64)

//------------------------------------
// ポリゴンの構造体を定義
//------------------------------------
typedef struct
{
	char* name;
	LPDIRECT3DVERTEXBUFFER9	VtxBuff;	// 頂点バッファーへのポインタ
	LPDIRECT3DTEXTURE9		Tex;		// テクスチャへのポインタ
	D3DXVECTOR3				pos;		// 頂点座標
	D3DXVECTOR3				size;		// 頂点座標
	D3DXVECTOR3				rot;		// 回転座標
	D3DXMATRIX				mtxWorld;	// ワールドマトリックス
	bool					bUse;		// 使用しているか
} ObjectPolygon;

//------------------------------------
// 静的変数
//------------------------------------
static ObjectPolygon s_aPolygon[MAX_POLYGON];		// ポリゴンの構造体
static ObjectPolygon s_aPolygonUI[MAX_POLYGON];		// ポリゴンの構造体

//=========================================
// 初期化
//=========================================
void InitPolygon(void)
{
	ZeroMemory(s_aPolygon, sizeof(s_aPolygon));	// 初期化
}

//=========================================
// 終了
//=========================================
void UninitPolygon(void)
{
	for (int i = 0; i < MAX_POLYGON; i++)
	{
		// テクスチャの破棄
		if (s_aPolygon[i].Tex != NULL)
		{
			s_aPolygon[i].Tex->Release();
			s_aPolygon[i].Tex = NULL;
		}

		// 頂点バッファーの破棄
		if (s_aPolygon[i].VtxBuff != NULL)
		{
			s_aPolygon[i].VtxBuff->Release();
			s_aPolygon[i].VtxBuff = NULL;
		}
	}

}

//=========================================
// 更新
//=========================================
void UpdatePolygon(void)
{
}

//=========================================
// 描画
//=========================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	for (int i = 0; i < MAX_POLYGON; i++)
	{
		ObjectPolygon *polygon = &s_aPolygon[i];

		if (!polygon->bUse)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&polygon->mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, polygon->rot.y, polygon->rot.x, polygon->rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixMultiply(&polygon->mtxWorld, &polygon->mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, polygon->pos.x, polygon->pos.y, polygon->pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixMultiply(&polygon->mtxWorld, &polygon->mtxWorld, &mtxTrans);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &polygon->mtxWorld);	// ワールド座標行列の設定

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, polygon->VtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, polygon->Tex);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);

	}
}

//=========================================
// 設定
//=========================================
void SetPolygon(D3DXVECTOR3* pos, D3DXVECTOR3* rot, D3DXVECTOR3 size, char* texFile, char* name)
{
	for (int i = 0; i <= MAX_POLYGON; i++)
	{
		ObjectPolygon* polygon = &s_aPolygon[i];

		if (polygon->bUse)
		{
			continue;
		}

		polygon->name = name;
		polygon->pos = *pos;
		polygon->size = size;
		polygon->rot = *rot;
		polygon->bUse = true;

		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// テクスチャの読込
		D3DXCreateTextureFromFile(pDevice, texFile,&polygon->Tex);

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&polygon->VtxBuff,
			NULL);

		VERTEX_3D* pVtx = NULL;

		// 頂点座標をロック
		polygon->VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-size.x, size.y,  size.z);
		pVtx[1].pos = D3DXVECTOR3( size.x, size.y,  size.z);
		pVtx[2].pos = D3DXVECTOR3(-size.x, size.y, -size.z);
		pVtx[3].pos = D3DXVECTOR3( size.x, size.y, -size.z);

		// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点座標をアンロック
		polygon->VtxBuff->Unlock();

		break;
	}
}

//=========================================
// ポリゴンUIの描画
//=========================================
void DrawPolygonUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	for (int i = 0; i < MAX_POLYGON; i++)
	{
		ObjectPolygon *polygon = &s_aPolygonUI[i];

		if (!polygon->bUse)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&polygon->mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, polygon->rot.y, polygon->rot.x, polygon->rot.z);	// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixMultiply(&polygon->mtxWorld, &polygon->mtxWorld, &mtxRot);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, polygon->pos.x, polygon->pos.y, polygon->pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixMultiply(&polygon->mtxWorld, &polygon->mtxWorld, &mtxTrans);						// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		Camera* camera = GetCamera(0);
		D3DXMATRIX mtxCamera;

		D3DXMatrixInverse(&mtxCamera, NULL, &camera->mtxView);
		D3DXMatrixMultiply(&polygon->mtxWorld, &polygon->mtxWorld, &mtxCamera);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &polygon->mtxWorld);	// ワールド座標行列の設定

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, polygon->VtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetTexture(0, polygon->Tex);

		// ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, false);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// ライトを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, true);

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);

	}
}

//=========================================
// ポリゴンUIの設定
//=========================================
void SetPolygonUI(D3DXVECTOR3 * pos, D3DXVECTOR3 * rot, D3DXVECTOR3 size, char * texFile)
{
	for (int i = 0; i <= MAX_POLYGON; i++)
	{
		ObjectPolygon* polygon = &s_aPolygonUI[i];

		if (polygon->bUse)
		{
			continue;
		}

		polygon->pos = *pos;
		polygon->size = size;
		polygon->rot = *rot;
		polygon->bUse = true;

		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// テクスチャの読込
		D3DXCreateTextureFromFile(pDevice, texFile, &polygon->Tex);

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&polygon->VtxBuff,
			NULL);

		VERTEX_3D* pVtx = NULL;

		// 頂点座標をロック
		polygon->VtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-size.x, size.y, size.z);
		pVtx[1].pos = D3DXVECTOR3(size.x, size.y, size.z);
		pVtx[2].pos = D3DXVECTOR3(-size.x, size.y, -size.z);
		pVtx[3].pos = D3DXVECTOR3(size.x, size.y, -size.z);

		// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点座標をアンロック
		polygon->VtxBuff->Unlock();

		break;
	}
}

//=========================================
// ポリゴンの当たり判定
//=========================================
bool CollisionPolygon(D3DXVECTOR3* pos, char* name)
{

	for (int i = 0; i < MAX_POLYGON; i++)
	{
		ObjectPolygon* polygon = &s_aPolygon[i];

		if (s_aPolygon[i].name == NULL)
		{
			continue;
		}

		if(strcmp(s_aPolygon[i].name, name) == 0 && polygon->bUse)
		{
			if (((polygon->pos.z - polygon->size.z) < pos->z) && ((polygon->pos.z + polygon->size.z) >  pos->z) &&
				((polygon->pos.x - polygon->size.x) < pos->x) && ((polygon->pos.x + polygon->size.x) > pos->x))
			{//(PL手前側 < モデル奥側) かつ (PL奥側 > モデル手前側)
				return true;
			}
		}
	}
	return false;
}

//=========================================
// ポリゴンの位置を取得
//=========================================
D3DXVECTOR3 GetPolygonPos(void)
{
	return s_aPolygon[0].pos;
}
