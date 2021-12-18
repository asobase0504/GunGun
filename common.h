//=========================================
// 
// 共通処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _COMMON_H_
#define _COMMON_H_

#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------

// モデル系統
void ModelSize(D3DXVECTOR3 *Min, D3DXVECTOR3 *Max, LPD3DXMESH Mesh);	// モデルのサイズを算出

#endif // !_COMMON_H_
