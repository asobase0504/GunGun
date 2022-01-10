//=========================================
// 
// デバッグ用ラインの作成
// Author YudaKaito
// 
//=========================================
#ifndef _LINE_H_
#define _LINE_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitLine(void);
void UninitLine(void);
void UpdateLine(void);
void DrawLine(void);
void SetLine(D3DXVECTOR3* pos, D3DXVECTOR3* rot, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col);

#endif // !_LINE_H_