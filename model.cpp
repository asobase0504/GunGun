//=========================================
// 
// モデル処理関数
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "model.h"
#include "main.h"
#include "input.h"
#include "camera.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define MODEL_MOVE	(1.0f)

//------------------------------------
// モデルステータス列挙型
//------------------------------------
typedef enum
{
	MODELSTATEL_NONE = 0,
	MODELSTATEL_MOVE,
	MODELSTATEL_MAX
}MODEL_STATE;


//------------------------------------
// モデルの構造体定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 rot;				// 向き
}Model;

//------------------------------------
// 静的変数
//------------------------------------
static LPD3DXMESH s_pMesh = NULL;		// メッシュ情報へのポインタ		// 頂点の集まりのこと
static LPD3DXBUFFER s_pBuffMat = NULL;	// マテリアル情報へのポインタ	// 1つのXファイルに複数のマテリアルが入っている
static DWORD s_nNumMat = 0;				// マテリアル情報の数
static D3DXMATRIX s_mtxWorld;			// ワールドマトリックス
static Model s_model;					// モデルの構造体
static MODEL_STATE s_state;				// モデルのステータス


//=========================================
// 初期化
//=========================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/bee_butt.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	s_model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//=========================================
// 終了
//=========================================
void UninitModel(void)
{
	// メッシュの解放
	if(s_pMesh != NULL)
	{
		s_pMesh->Release();
		s_pMesh = NULL;
	}
	// マテリアルの解放
	if (s_pBuffMat != NULL)
	{
		s_pBuffMat->Release();
		s_pBuffMat = NULL;
	}
}

//=========================================
// 更新
//=========================================
void UpdateModel(void)
{
	MoveModel();
}

//=========================================
// 描画
//=========================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;	// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;		// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&s_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_model.rot.y, s_model.rot.x, s_model.rot.z);		// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxRot);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxTrans);				// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &s_mtxWorld);;

	// 現在のマテリアルを保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// モデルパーツの描写
		s_pMesh->DrawSubset(i);
	}
	// 保持していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=========================================
// 移動
//=========================================
void MoveModel()
{
	D3DXVECTOR3 CameraRot = GetRotCamera();		//カメラの角度情報取得
	float fAngle = CameraRot.y;	// 最終角度
	Model* pModel = &(s_model);
	float fAngleK = 0.0f;
	int KeyCnt = 0;

	// モデルの移動
	if (GetKeyboardPress(DIK_UP))
	{
		KeyCnt++;
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		KeyCnt++;
		fAngleK += -0.5f;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		KeyCnt++;
		if (KeyCnt > 1)
		{
			fAngleK -= 1.0f;
		}
		else
		{
			fAngleK += 1.0f;
		}
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		KeyCnt++;
		fAngleK += 0.5f;
	}

	if (KeyCnt > 0)
	{
		s_state = MODELSTATEL_MOVE;
		fAngle = D3DX_PI * fAngleK / KeyCnt + CameraRot.y;
		pModel->pos.x += sinf(fAngle) * MODEL_MOVE;
		pModel->pos.z += cosf(fAngle) * MODEL_MOVE;
		pModel->rot.y = fAngle;
	}
	else
	{
		s_state = MODELSTATEL_NONE;
	}

	//// モデルの移動
	//if (GetKeyboardPress(DIK_UP) && !(GetKeyboardPress(DIK_DOWN)))
	//{
	//	s_state = MODELSTATEL_MOVE;
	//	if (GetKeyboardPress(DIK_LEFT))
	//	{
	//		fAngle = D3DX_PI * -0.25f + CameraRot.y;
	//	}
	//	else if (GetKeyboardPress(DIK_RIGHT))
	//	{
	//		fAngle = D3DX_PI * 0.25f + CameraRot.y;
	//	}
	//	else
	//	{
	//		fAngle = CameraRot.y;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_LEFT) && !(GetKeyboardPress(DIK_RIGHT)))
	//{
	//	s_state = MODELSTATEL_MOVE;
	//	if (GetKeyboardPress(DIK_DOWN))
	//	{
	//		fAngle = D3DX_PI * -0.75f + CameraRot.y;
	//	}
	//	else
	//	{
	//		fAngle = D3DX_PI * -0.5f + CameraRot.y;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_DOWN) && !(GetKeyboardPress(DIK_UP)))
	//{
	//	s_state = MODELSTATEL_MOVE;
	//	if (GetKeyboardPress(DIK_RIGHT))
	//	{
	//		fAngle = D3DX_PI * 0.75f + CameraRot.y;
	//	}
	//	else
	//	{
	//		fAngle = D3DX_PI + CameraRot.y;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_RIGHT) && !(GetKeyboardPress(DIK_LEFT)))
	//{
	//	s_state = MODELSTATEL_MOVE;
	//	fAngle = D3DX_PI * 0.5f + CameraRot.y;
	//}
	//else
	//{
	//	s_state = MODELSTATEL_NONE;
	//}
}
