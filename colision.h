//=========================================
// 
// 当たり判定(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _COLISION_H_
#define _COLISION_H_

#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
bool SegmentColision(Segment seg1, Segment seg2);	// 線分同士の当たり判定
bool SphereColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2);			// 球同士の当たり判定
bool SphereCuboidColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, D3DXVECTOR3 size);	// 球と立方体の当たり判定
bool SphereCylinderColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2);	// 球と円柱の当たり判定
bool SphereCapsuleColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2);	// 球と円柱の当たり判定
#endif // !_COLISION_H_
