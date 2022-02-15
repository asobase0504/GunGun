//=========================================
// 
// パーティクルのヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//-----------------------------------------
// include
//-----------------------------------------

//------------------------------------
// 判定位置の列挙型の定義
//------------------------------------
typedef enum
{
	PARTICLE_PLAYER_GETMODEL = 0,		// プレイヤーがモデルをゲットするときに出るエフェクト
	MAX_PARTICLE
}PARTICLE_TYPE;

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLE_TYPE type);

#endif // _PARTICLE_H_
