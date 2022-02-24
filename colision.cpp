//=========================================
// 
// 共通処理
// Author YudaKaito
// 
//=========================================
#include "colision.h"

///=========================================
/// 線分同士の当たり判定
///=========================================
///bool SegmentColision(Segment seg1, Segment seg2)
//{
//	// ベクトルの始点同士の距離。
//	D3DXVECTOR3 v = seg2.s - seg1.s;
//
//	// ブロックのベクトルと被対象のベクトルが平行か調べる
//	float Bv_Tv = D3DXVec2Cross(&(seg1.v), &(seg2.v));
//	if (Bv_Tv == 0.0f)
//	{
//		// 並行である。
//		return false;
//	}
//
//	D3DXVec3Cross()
//
//	float v_Bv = D3DXVec2Cross(&(v), &(seg1.v));
//	float v_Tv = D3DXVec2Cross(&(v), &(seg2.v));
//
//	float hit1 = v_Tv / Bv_Tv;
//	float hit2 = v_Bv / Bv_Tv;
//
//	if ((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f))
//	{
//		return false;
//	}
//
//	if (Outpos != NULL)
//	{
//		*Outpos = seg1.s + seg1.v * hit1;
//	}
//	return true;
//}

//=========================================
//
// 球と球の当たり判定
//
// 引数1 pos1		対象1の位置
// 引数2 fLength1	対象1の長さ
// 引数3 pos2		対象2の位置
// 引数4 fLength2	対象2の長さ
//=========================================
bool SphereColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2)
{
	D3DXVECTOR3 v = pos1 - pos2;
	return D3DXVec3LengthSq(&v) <= (fLength1 + fLength2)*(fLength1 + fLength2);
}

//=========================================
//
// 球と立方体の当たり判定
//
// 引数1 pos1		対象1の位置
// 引数2 fLength1	対象1の長さ
// 引数3 pos2		対象2の位置
// 引数4 size		対象2の大きさ
//=========================================
bool SphereCuboidColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, D3DXVECTOR3 size)
{
	D3DXVECTOR3 Vec(0, 0, 0);   // 最終的に長さを求めるベクトル

								// 各軸についてはみ出た部分のベクトルを算出
	FLOAT L = size.x;
	if (!(L <= 0))
	{
		D3DXVECTOR3 x = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &x) / L;

		// sの値から、はみ出した部分があればそのベクトルを加算
		s = (float)fabs(s);
		if (s > 1)
		{
			Vec += (1 - s)*L*x;   // はみ出した部分のベクトル算出
		}
	}
	L = size.y;
	if (!(L <= 0))
	{
		D3DXVECTOR3 y = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &y) / L;

		// sの値から、はみ出した部分があればそのベクトルを加算
		s = (float)fabs(s);
		if (s > 1)
		{
			Vec += (1 - s)*L*y;   // はみ出した部分のベクトル算出
		}
	}
	L = size.z;
	if (!(L <= 0))
	{
		D3DXVECTOR3 z = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &z) / L;

		// sの値から、はみ出した部分があればそのベクトルを加算
		s = (float)fabs(s);
		if (s > 1)
		{
			Vec += (1 - s)*L*z;   // はみ出した部分のベクトル算出
		}
	}
	return fLength1 < D3DXVec3Length(&Vec);
}

//=========================================
//
// 球とカプセルの当たり判定
//
// 引数1 pos1		対象1の位置
// 引数2 fLength1	対象1の長さ
// 引数3 pos2		対象2の位置
// 引数4 fLength1	対象2の長さ
//=========================================
//bool SphereCapsuleColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2)
//{
//	return false;
//}

//=========================================
//
// 球と円柱の当たり判定
//
// 引数1 pos1		対象1の位置
// 引数2 fLength1	対象1の長さ
// 引数3 pos2		対象2の位置
// 引数4 fLength1	対象2の長さ
//=========================================
//bool SphereCylinderColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2)
//{
	//D3DXVECTOR3 Vec(0, 0, 0);   // 最終的に長さを求めるベクトル

	//// 各軸についてはみ出た部分のベクトルを算出
	//FLOAT L = size.x;
	//if (!(L <= 0))
	//{
	//	D3DXVECTOR3 x = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	//	FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &x) / L;

	//	// sの値から、はみ出した部分があればそのベクトルを加算
	//	s = (float)fabs(s);
	//	if (s > 1)
	//	{
	//		Vec += (1 - s)*L*x;   // はみ出した部分のベクトル算出
	//	}
	//}
	//L = size.y;
	//if (!(L <= 0))
	//{
	//	D3DXVECTOR3 y = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//	FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &y) / L;

	//	// sの値から、はみ出した部分があればそのベクトルを加算
	//	s = (float)fabs(s);
	//	if (s > 1)
	//	{
	//		Vec += (1 - s)*L*y;   // はみ出した部分のベクトル算出
	//	}
	//}
	//L = size.z;
	//if (!(L <= 0))
	//{
	//	D3DXVECTOR3 z = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//	FLOAT s = D3DXVec3Dot(&(pos1 - pos2), &z) / L;

	//	// sの値から、はみ出した部分があればそのベクトルを加算
	//	s = (float)fabs(s);
	//	if (s > 1)
	//	{
	//		Vec += (1 - s)*L*z;   // はみ出した部分のベクトル算出
	//	}
	//}
	//return fLength1 < D3DXVec3Length(&Vec);
//}
