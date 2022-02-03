//=========================================
// 
// ポリゴンの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//------------------------------------
// カメラの構造体
//------------------------------------
typedef struct
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 posRDest;		// 視点の目的値
	D3DXVECTOR3 posVDest;		// 注視点の目的値
	D3DXVECTOR3 vec;			// 移動量
	D3DXVECTOR3 rot;			// 向き
	D3DXVECTOR3 rotDest;		// 向きの目的地
	D3DXVECTOR3 vecU;			// 上方向ベクトル
	D3DXMATRIX mtxProjection;	// プロジェクトマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
	D3DVIEWPORT9 viewport;		// ビューポート
	float fDistance;			// 視点から注視点の距離
}Camera;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitCamera(void);			// カメラの初期化処理
void UninitCamera(void);		// カメラの終了処理
void UpdateCamera(void);		// カメラの更新処理
void UpdateGameCamera(void);	// ゲーム中の更新処理
void UpdateResultCamera(void);	// リザルト中の更新処理
void SetCamera(int nData);		// カメラの設定処理
Camera* GetCamera(int nData);	// カメラの情報取得
D3DXVECTOR3* GetRotCamera(void);	// カメラの角度情報取得

#endif // !_CAMERA_H_
