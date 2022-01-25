//=========================================
// 
// タイトルの処理
// Author YudaKaito
// 
//=========================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "common.h"

#include "time.h"

#include <assert.h>

// マクロ定義
#define TITLEPRESS_WIDTH	(620)
#define TITLEPRESS_HEIGHT	(100)

#define SELECTBG			"data/TEXTURE/enemy000.png"
#define TITLE				"data/TEXTURE/title.png"
#define GAMESTART			"data/TEXTURE/GAMESTART.png"
#define TUTORIAL			"data/TEXTURE/TUTORIAL.png"
#define EXIT				"data/TEXTURE/EXIT.png"

// プレスエンターの状態の種類
typedef enum
{
	TYPE_NOME,	// 変化なし
	TYPE_BLINK,	// 点滅状態
	TYPE_MAX
}PRESS_TYPE;

// 配置するオブジェクトの種類
typedef enum
{
	OBJ_BG,				// スクリーン背景
	OBJ_SELECTBG,		// 選択画面背景
	OBJ_TITLE,			// タイトル文字
	OBJ_GAMESTART,		// GAMESTART文字
	OBJ_TUTORIAL,		// TUTORIAL文字
	OBJ_EXIT,			// EXIT文字
	OBJ_SERECTCURSOR,	// セレクトカーソル
	OBJ_MAX				// 
}OBJ_TYPE;

// 選択されたオブジェクト
typedef enum
{
	SELECT_GAMESTART = OBJ_GAMESTART,
	SELECT_TUTORIAL,
	SELECT_EXIT,
}SELECT_MODE;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 Tex = NULL;				// テクスチャへのポインタ
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 角度
	D3DXCOLOR col;								// 色
	bool bUse;									// 使用してるかどうか
	float Height;								// 高さ
	float Width;								// 幅
}OBJECT;

// グローバル変数
static SELECT_MODE s_Select;
static OBJECT s_Object[OBJ_MAX] = {};
static bool	s_bFadeCheek;	// フェード処置に移行するかの処理
static int	s_nFadeCnt;		// フェード処理に行くまでの間隔
static bool s_bExit;		// Exitのフラグ

//=========================================
// 初期化処理
//=========================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポイント

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTBG,
		&s_Object[OBJ_SELECTBG].Tex);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		TITLE,
		&s_Object[OBJ_TITLE].Tex);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		GAMESTART,
		&s_Object[OBJ_GAMESTART].Tex);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL,
		&s_Object[OBJ_TUTORIAL].Tex);

	// テクスチャの読込	   
	D3DXCreateTextureFromFile(pDevice,
		EXIT,
		&s_Object[OBJ_EXIT].Tex);

	s_bFadeCheek = false;	// フェード処理に移行するかの変数
	s_nFadeCnt = 0;			// フェード処理に移行するまでの間隔
	s_Select = SELECT_GAMESTART;
	s_bExit = false;

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	for (int i = 0; i < OBJ_MAX; i++)
	{
		OBJECT *object = &(s_Object[i]);

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&object->pVtxBuff,
			NULL);

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		object->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		switch (i)
		{
		case OBJ_BG:
			object->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 中心座標の設定
			object->col = D3DXCOLOR(1.0f, 0.9f, 0.8f, 1.0f);	// カラーの設定
			object->Width = SCREEN_WIDTH;						// 幅の設定
			object->Height = SCREEN_HEIGHT;						// 高さの設定

			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_SELECTBG:
			object->pos = D3DXVECTOR3(SCREEN_WIDTH - 350.0f, SCREEN_HEIGHT, 0.0f);		// 中心座標の設定
			object->col = D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f);							// カラーの設定
			object->Width = 1000.0f;													// 幅の設定
			object->Height = 1000.0f;													// 高さの設定
			
			// 頂点座標の設定
			SetRectCenterPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_TITLE:
			object->pos = D3DXVECTOR3(40.0f, 30.0f, 0.0f);		// 中心座標の設定
			object->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// カラーの設定
			object->Width = 962.0f * 0.5f;								// 幅の設定
			object->Height = 526.0f * 0.5f;								// 高さの設定

			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_SERECTCURSOR:
		{
			object->pos = D3DXVECTOR3(800.0f - 70.0f, 460.0f, 0.0f);	// 中心座標の設定
			object->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 中心座標の設定
			object->col = D3DXCOLOR(0.4f, 0.81f, 0.53f, 1.0f);	// カラーの設定
			object->Width = 25.0f;								// 幅の設定
			object->Height = 25.0f;								// 高さの設定

			// 中心座標から上の長さを算出する。
			float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height);

			// 中心座標から上の頂点の角度を算出する
			float fAngle = atan2f(object->Width, object->Height);

			// 頂点座標の設定
			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);
		}
			break;
		case OBJ_GAMESTART:
			object->pos = D3DXVECTOR3(800.0f, 460.0f, 0.0f);	// 中心座標の設定
			object->col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	// カラーの設定
			object->Width = 780.0f * 0.5f;								// 幅の設定
			object->Height = 140.0f * 0.5f;							// 高さの設定

			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_TUTORIAL:
			object->pos = D3DXVECTOR3(800.0f, 460.0f + 70.0f, 0.0f);	// 中心座標の設定
			object->col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	// カラーの設定
			object->Width = 615.0f * 0.5f;								// 幅の設定
			object->Height = 140.0f * 0.5f;							// 高さの設定

			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_EXIT:
			object->pos = D3DXVECTOR3(800.0f, 460.0f + 140.0f, 0.0f);	// 中心座標の設定
			object->col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	// カラーの設定
			object->Width = 270.0f * 0.5f;								// 幅の設定
			object->Height = 140.0f * 0.5f;							// 高さの設定
			
			// 頂点座標の設定
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_MAX:
		default:
			assert(false);
			break;
		}

		// 頂点カラーの設定
		SetRectColor(pVtx, &(object->col));

		// rhwの設定
		InitRectRhw(pVtx);

		// テクスチャ座標の設定
		InitRectTex(pVtx);

		// 頂点バッファをアンロックする
		s_Object[i].pVtxBuff->Unlock();

		// 使用に切り替え
		object->bUse = true;
	}
}

//=========================================
// 終了処理
//=========================================
void UninitTitle(void)
{
	// テクスチャの破棄
	for (int i = 0; i < OBJ_MAX; i++)
	{
		if (s_Object[i].Tex != NULL)
		{
			s_Object[i].Tex->Release();
			s_Object[i].Tex = NULL;
		}

	}

	// 頂点バッファの破棄
	for (int i = 0; i < OBJ_MAX; i++)
	{
		if (s_Object[i].pVtxBuff != NULL)
		{
			s_Object[i].pVtxBuff->Release();
			s_Object[i].pVtxBuff = NULL;
		}
	}
}

//=========================================
// 更新処理
//=========================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	if (!(s_bFadeCheek))
	{
		// 選択処理
		SelectTitle();

		// ジョイパッドの使用情報の取得
		bool bUseJoyPad = IsJoyPadUse(0);
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
			{// EnterキーかパッドのAを押された時
			 // 決定音の再生
				s_bFadeCheek = true;	// フェード処理に入る
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_RETURN))
			{// EnterキーかパッドのAを押された時
			 // 決定音の再生
				s_bFadeCheek = true;	// フェード処理に入る
			}
		}
	}

	for (int i = 0; i < OBJ_MAX; i++)
	{
		OBJECT *object = &(s_Object[i]);

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		object->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		if (i == OBJ_SERECTCURSOR)
		{
			object->rot.z += 0.025f;
			object->pos.x = s_Object[s_Select].pos.x - 50.0f;
			object->pos.y = s_Object[s_Select].pos.y + 70.0f / 2.0f;
			object->pos.z = s_Object[s_Select].pos.z;

			// 中心座標から上の長さを算出する。
			float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;

			// 中心座標から上の頂点の角度を算出する
			float fAngle = atan2f(object->Width, object->Height);

			// 頂点座標の設定
			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);
		}

		// 頂点カラーの設定
		SetRectColor(pVtx, &(object->col));

		// 頂点バッファをアンロックする
		object->pVtxBuff->Unlock();
	}

	if (s_bFadeCheek)
	{
		if (s_nFadeCnt >= 5)
		{
			switch (s_Select)
			{
			case SELECT_GAMESTART:
				SetFade(MODE_GAME);	// ゲームモードに移行
				break;
			case SELECT_TUTORIAL:
				SetFade(MODE_TUTORIAL);	// チュートリアル画面に移行
				break;
			case SELECT_EXIT:
				s_bExit = true;
				break;
			default:
				break;
			}
		}
		else
		{
			s_nFadeCnt++;
			D3DXVECTOR3 ObjPos;
			ObjPos.x = s_Object[s_Select].pos.x + (float)s_nFadeCnt / 70.0f * (s_Object[s_Select].Width);
			ObjPos.y = s_Object[s_Select].pos.y + s_Object[s_Select].Height;
			ObjPos.z = 0.0f;
		}
	}
}

//=========================================
// 選択処理
//=========================================
void SelectTitle(void)
{
	// ジョイパッドの使用情報の取得
	bool bUseJoyPad = IsJoyPadUse(0);

	switch (s_Select)
	{
	case SELECT_GAMESTART:
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_UP))
			{
				s_Select = SELECT_EXIT;
			}
			if (GetJoypadTrigger(JOYKEY_DOWN))
			{
				s_Select = SELECT_TUTORIAL;
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				s_Select = SELECT_EXIT;
			}
			if (GetKeyboardTrigger(DIK_S))
			{
				s_Select = SELECT_TUTORIAL;
			}
		}
		break;
	case SELECT_TUTORIAL:
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_UP))
			{
				s_Select = SELECT_GAMESTART;
			}
			if (GetJoypadTrigger(JOYKEY_DOWN))
			{
				s_Select = SELECT_EXIT;
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				s_Select = SELECT_GAMESTART;
			}
			if (GetKeyboardTrigger(DIK_S))
			{
				s_Select = SELECT_EXIT;
			}
		}
		break;
	case SELECT_EXIT:
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_UP))
			{
				s_Select = SELECT_TUTORIAL;
			}
			if (GetJoypadTrigger(JOYKEY_DOWN))
			{
				s_Select = SELECT_GAMESTART;
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_W))
			{
				s_Select = SELECT_TUTORIAL;
			}
			if (GetKeyboardTrigger(DIK_S))
			{
				s_Select = SELECT_GAMESTART;
			}
		}
		break;
	default:
		break;
	}
}

//=========================================
// 描画処理
//=========================================
void DrawTitle(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < OBJ_MAX; i++)
	{
		// 頂点バッファをデータストリーム設定
		pDevice->SetStreamSource(0, s_Object[i].pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		if (s_Object[i].bUse)
		{
			// ポリゴン描画
			// テクスチャの設定
			RectDraw(pDevice, s_Object[i].Tex, 0);
		}
	}
}

//
bool GetExit(void)
{
	return s_bExit;
}
