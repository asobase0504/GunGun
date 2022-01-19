//=========================================
// 
// デバッグ用ラインの作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "line.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define MAX_LINE		(512)		//線の最大数

//--------------------------------------------------
// ライン情報の構造体
//--------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 rot;			// 向き
	D3DXQUATERNION quaternion;		// クオータニオン
	D3DXCOLOR col;			// 色
	D3DXMATRIX mtxWorld;		// ワールドマトリックス
	bool bUse;			// 使用しているかどうか
}Line;

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9	s_pVtxBuff = NULL;	// 頂点バッファへのポインタ
static Line s_Line[MAX_LINE];						// 線の情報

//=========================================
// 初期化
//=========================================
void InitLine(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_LINE) * 2 * MAX_LINE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_LINE,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	//メモリのクリア
	memset(s_Line, NULL, sizeof(s_Line));

	VERTEX_LINE *pVtx = NULL;		// 頂点情報へのポインタ

									// 頂点情報をロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_LINE; i++)
	{
		// 全ての初期化
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 2;
	}

	// 頂点バッファをアンロックする
	s_pVtxBuff->Unlock();
}

//=========================================
// 終了
//=========================================
void UninitLine(void)
{
	if (s_pVtxBuff != NULL)
	{// 頂点バッファの解放
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//=========================================
// 更新
//=========================================
void UpdateLine(void)
{
}

//=========================================
// 描画
//=========================================
void DrawLine(void)
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

										// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ライトを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int i = 0; i < MAX_LINE; i++)
	{
		Line *pLine = &s_Line[i];

		if (!pLine->bUse)
		{//使用されていない
			continue;
		}

		/*↓ 使用されている ↓*/

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pLine->mtxWorld);

		//// 向きを反映
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, pLine->rot->y, pLine->rot->x, pLine->rot->z);
		//D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxRot);
		// クォータニオンの使用した姿勢の設定
		D3DXMatrixRotationQuaternion(&mtxRot, &pLine->quaternion);			// クオータニオンによる行列回転
		D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxRot);	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pLine->pos.x, pLine->pos.y, pLine->pos.z);
		D3DXMatrixMultiply(&pLine->mtxWorld, &pLine->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pLine->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_LINE));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_LINE);

		// ポリゴンの描画
		pDevice->DrawPrimitive(
			D3DPT_LINELIST,		// プリミティブの種類
			i * 2,				// 描画する最初の頂点インデックス
			1);					// プリミティブ(ポリゴン)数
	}

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// ライトを有効に戻す
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=========================================
// 設定(クオータニオンの設定)
//=========================================
void SetLine(D3DXVECTOR3* pos, D3DXQUATERNION* quaternion, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col)
{
	VERTEX_LINE *pVtx = NULL;		// 頂点情報へのポインタ

	for (int i = 0; i < MAX_LINE; i++)
	{
		Line *pLine = &s_Line[i];

		if (pLine->bUse)
		{//使用されている
			continue;
		}

		/*↓ 使用されていない ↓*/

		pLine->pos = *pos;
		pLine->quaternion = *quaternion;
		pLine->col = col;
		pLine->bUse = true;

		// 頂点情報をロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (i * 2);	//該当の位置まで進める

							// 頂点座標の設定
		pVtx[0].pos = start;
		pVtx[1].pos = end;

		pVtx[0].col = pLine->col;
		pVtx[1].col = pLine->col;

		// 頂点バッファをアンロックする
		s_pVtxBuff->Unlock();

		break;
	}
}

//=========================================
// モデルの大きさに沿うように線の設定(クオータニオンの設定)
//=========================================
void SetModelLine(D3DXVECTOR3 * pos, D3DXQUATERNION * quaternion, D3DXVECTOR3 Max, D3DXVECTOR3 Min, D3DXCOLOR col)
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;

	// 線の作成
	start = D3DXVECTOR3(Min.x, Max.y, Min.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Min.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Max.y, Min.z);	// 線の始点
	end = D3DXVECTOR3(Max.x, Min.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Min.x, Max.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Min.y, Max.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	/*	x軸		*/
	// 線の作成
	start = D3DXVECTOR3(Max.x, Max.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Max.y, Max.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Min.y, Min.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Min.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Max.y, Min.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Max.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Min.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Min.y, Max.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Max.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Max.y, Max.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	/*	y軸		*/
	// 線の作成
	start = D3DXVECTOR3(Max.x, Max.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Max.x, Min.y, Max.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Min.y, Min.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Min.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Max.y, Min.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Max.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Min.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Min.y, Max.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	/*	z軸		*/
	// 線の作成
	start = D3DXVECTOR3(Min.x, Min.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Min.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Min.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Max.x, Min.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Min.x, Max.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Min.x, Max.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

	// 線の作成
	start = D3DXVECTOR3(Max.x, Max.y, Max.z);	// 線の始点
	end = D3DXVECTOR3(Max.x, Max.y, Min.z);		// 線の終点
	SetLine(pos, quaternion, start, end, col);	// 線の設定

}
