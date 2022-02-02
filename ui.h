//=========================================
// 
// UI処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _UI_H_
#define _UI_H_

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitUI(void);		// 初期化処理
void UninitUI(void);	// 終了処理
void UpdateUI(void);	// 更新処理
void DrawUI(void);		// 描画処理
void SetUI(char* pFile, D3DXVECTOR3* pos, D3DXCOLOR* col, D3DXVECTOR3* size);	// 設定処理
void LoadTitleUI(void);		// 読み込み処理(タイトル)
void LoadGameUI(void);		// 読み込み処理(ゲーム)
void LoadResultUI(void);	// 読み込み処理(リザルト)

#endif // !_UI_H_