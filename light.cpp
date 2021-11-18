//=========================================
// 
// ライトの作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "light.h"

//------------------------------------
// 静的変数
//------------------------------------
static D3DLIGHT9 s_light;	//ライト情報

void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;	// ライトの方向ベクトル

	// ライトをクリアする
	ZeroMemory(&s_light, sizeof(D3DLIGHT9));	// 構造体変数をゼロクリアできる関数

	// ライトの種類を設定
	s_light.Type = D3DLIGHT_DIRECTIONAL;					// 平行光源
	
	// ライトの拡散光を設定
	s_light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ライトの色

	// ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	// 正規化する(大きさ１のベクトルにする)
	D3DXVec3Normalize(&vecDir, &vecDir);
	s_light.Direction = vecDir;

	// ライトを設定する
	pDevice->SetLight(0, &s_light);

	// ライトを有効にする
	pDevice->LightEnable(0, true);
}

void UninitLight(void)
{
}

void UpdateLight(void)
{
}
