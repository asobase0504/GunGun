//=========================================
// 
// win�v���W�F�N�g�̍쐬
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "debug.h"
#include <stdio.h>

//-----------------------------------------
// �}�N����`
//-----------------------------------------
#define CLASS_NAME	"windowclass"			// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME	"3D�A�N�V����"		// �E�C���h�E�N���X�̖��O�i�L���v�V�����ɕ\���j
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static HRESULT Init(HINSTANCE hInstance, HWND hWmd, BOOL bWindow);
static void Uninit(void);
static void Update(void);
static void Draw(void);
static MODE s_mode = MODE_TITLE;

//-----------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------
LPDIRECT3D9	g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT g_pFont = NULL;	// �t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;		// FPS�J�E���^

//=========================================
// ���C���֐�
//=========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExecLastTime;	// �Ō�ɏ�����������
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							// �E�B���h�E�̃X�^�C��
		WindowProc,							// �E�B���h�E�v���V�[�W��
		0,									// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,									// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,							// �C���X�^���X�o���h��
		LoadIcon(NULL,IDI_APPLICATION),		// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),			// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			// �C���X�^���X
		NULL,								// ���j���[�o�[
		CLASS_NAME,							// �E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)		// �t�@�C���̃A�C�R��
	};

	HWND hWnd;		// �E�B���h�E�n���h��(���ʎq)
	MSG msg;		// ���b�Z�[�W�Ɗi�[����ϐ�

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�B���h�E�̐���
	hWnd = CreateWindowEx(0,		// �g���E�C���h�E�X�^�C��
		CLASS_NAME,					// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,				// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		// �E�B���h�E�̃X�^�C��
		CW_USEDEFAULT,				// �E�B���h�E�̍���X���W
		CW_USEDEFAULT,				// �@�@""�@�@����Y���W
		(rect.right - rect.left),	// �@�@""�@�@��
		(rect.bottom - rect.top),	// �@�@""�@�@����
		NULL,						// �e�E�B���h�E�̃n���h��
		NULL,						// ���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,					// �C���X�^���X�n���h��
		NULL);						// �E�B���h�E�쐬�f�[�^

	if (FAILED(Init(hInstance, hWnd, true)))	// true = window,false = fullscreen
	{// ���������������s�����ꍇ
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// ����������
	dwExecLastTime = timeGetTime();		// ���ݎ������擾(�ۑ�)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);	// �E�B���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);			// �N���C�A���g�̈���X�V

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);	// ���z���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);	// �E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗐M
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();	// ���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
			 // FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;				// �t���[���J�E���g���N���A
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60����1�b�o��
				dwExecLastTime = dwCurrentTime;	// �����J�n�̎���[���ݎ���]��ۑ�

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;	// �t���[���J�E���g�����Z

			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=========================================
// �E�B���h�E�v���V�[�W��
//=========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (GetExit())
	{
		// �E�B���h�E��j������
		DestroyWindow(hWnd);
	}

	switch (uMsg)
	{
	case WM_DESTROY:	// �E�B���h�E�j���̃��b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]�L�[�������ꂽ
			// �E�B���h�E��j������(WM_DESTOROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:		// ����{�^�������̃��b�Z�[�W
		// �E�B���h�E��j������
		DestroyWindow(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// �K��̏�����Ԃ�
}

//=========================================
// ������
//=========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^
									// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));		// �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ��i�f���M���ɓ����j
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs�Ȃ�)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs�Ȃ�)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}

		}
	}

	// �����_�[�X�e�[�g�ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// �A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �A���t�@�u�����h�̐ݒ�

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���̑傫����ς��Ă��Y��ɓ\��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���̑傫����ς��Ă��Y��ɓ\��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`�����J��Ԃ��ē\��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`�����J��Ԃ��ē\��

	// �e�N�X�`���X�e�[�W�X�e�[�g
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �|���S���ƃe�N�X�`���̃A���t�@�l��������B
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �|���S���ƃe�N�X�`���̃A���t�@�l��������B�e�N�X�`���w��
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �|���S���ƃe�N�X�`���̃A���t�@�l��������B�|���S���w��

	InitFPS();

	//// �f�o�b�O�\���p�t�H���g�̐���
	//D3DXCreateFont(g_pD3DDevice, 32, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	// ���͏����̏�����
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ���[�h�̐ݒ�
	InitFade(s_mode);

	return S_OK;
}

//=========================================
// �I��
//=========================================
void Uninit(void)
{
	UninitFPS();

	//// �f�o�b�O�\���p�t�H���g�̔j��
	//if (g_pFont != NULL)
	//{
	//	g_pFont->Release();
	//	g_pFont = NULL;
	//}
	//// Direct3D�f�o�C�X�̔j��
	//if (g_pD3DDevice != NULL)
	//{
	//	g_pD3DDevice->Release();
	//	g_pD3DDevice = NULL;
	//}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// ���͏����̏I��
	UninitInput();

	// �Q�[���̏I������
	UninitGame();

	// �^�C�g����ʂ̏I������
	UninitTitle();
}

//=========================================
// �X�V
//=========================================
void Update(void)
{	
	// ���͏����̍X�V
	UpdateInput();

	// ���݂̉��(���[�h)�̍X�V����
	switch (s_mode)
	{
	case MODE_TITLE:	// �^�C�g�����
		UpdateTitle();
		break;

	case MODE_GAME:		// �Q�[�����
		UpdateGame();
		break;

	case MODE_TUTORIAL:	// �`���[�g���A�����
		//UpdateTutorial();
		break;

	case MODE_RESULT:	// �����L���O���
		UpdateResult();
		break;
	}

	UpdateFade();

	if (GetKeyboardTrigger(DIK_F1))
	{
		// ���C���[�t���[�����[�h�̐ݒ�
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else if (GetKeyboardTrigger(DIK_F2))
	{
		// ���C���[�t���[�����[�h�̐ݒ�����Ƃɖ߂�
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

//=========================================
// �`��
//=========================================
void Draw(void)
{
	// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 	�`��J�n�����������ꍇ

		switch (s_mode)
		{
		case MODE_TITLE:	// �^�C�g�����
			DrawTitle();
			break;

		case MODE_GAME:		// �Q�[�����
			DrawGame();
#ifdef _DEBUG
			// FPS�̕\��
			DrawFPS();
#endif // _DEBUG
			break;

		case MODE_TUTORIAL:	// �`���[�g���A�����
			//DrawTutorial();
			break;

		case MODE_RESULT:	// �����L���O���
			DrawResult();
			break;
		}

		// �t�F�[�h����
		DrawFade();

		// �`��I��
		g_pD3DDevice->EndScene();
	}
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


//=========================================
// �f�o�C�X�̎擾
//=========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

////=========================================
//// FPS�̕\��
////=========================================
//void DrawFPS(void)
//{
//	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[15][256];	// �\������
//	D3DXVECTOR3 camerarot = GetRotCamera();
//	Player* player = GetPlayer();
//	Model* model = GetModel();
//	Camera* camera = GetCamera();
//	Shadow* shadow = GetShadow();
//	MODE mode = GetMode();
//
//	D3DXVECTOR3 stickL = GetJoypadStick(JOYKEY_LEFT_STICK, 0);
//	D3DXVECTOR3 stickR = GetJoypadStick(JOYKEY_RIGHT_STICK, 0);
//
//	// ������ɑ��
//	wsprintf(&aStr[0][0], "FPS: %d\n", g_nCountFPS);
//
//	// ������ɑ��
//	sprintf(&aStr[1][0], "rot: %f\n", camera->rot.y);
//	// ������ɑ��
//	sprintf(&aStr[2][0], "Player.pos     : %.3f|%.3f|%.3f\n", player->pos.x, player->pos.y, player->pos.z);
//	// ������ɑ��
//	sprintf(&aStr[3][0], "Player.fLength : %.3f\n", player->fLength);
//	// ������ɑ��
//	sprintf(&aStr[4][0], "Player.MinVtx  : %.3f|%.3f|%.3f\n", player->MinVtx.x, player->MinVtx.y, player->MinVtx.z);
//	// ������ɑ��
//	sprintf(&aStr[5][0], "Player.MaxVtx  : %.3f|%.3f|%.3f\n", player->MaxVtx.x, player->MaxVtx.y, player->MaxVtx.z);
//	// ������ɑ��
//	sprintf(&aStr[6][0], "quaternion     : %.3f|%.3f|%.3f|%.3f\n", player->aModel[0].quaternion.x, player->aModel[0].quaternion.y, player->aModel[0].quaternion.z, player->aModel[0].quaternion.w);
//	// ������ɑ��
//	sprintf(&aStr[7][0], "posV: %.3f|%.3f|%.3f\n", camera->posV.x, camera->posV.y, camera->posV.z);
//	// ������ɑ��
//	sprintf(&aStr[8][0], "posR: %.3f|%.3f|%.3f\n", camera->posR.x, camera->posR.y, camera->posR.z);
//	// ������ɑ��
//	sprintf(&aStr[9][0], "posVDest: %.3f|%.3f|%.3f\n", camera->posVDest.x, camera->posVDest.y, camera->posVDest.z);
//	// ������ɑ��
//	sprintf(&aStr[10][0], "posRDest: %.3f|%.3f|%.3f\n", camera->posRDest.x, camera->posRDest.y, camera->posRDest.z);
//	// ������ɑ��
//	sprintf(&aStr[11][0], "MODE: %d\n", mode);
//	// ������ɑ��
//	sprintf(&aStr[12][0], "stickL: %.3f|%.3f|%.3f\n", stickL.x, stickL.y, stickL.z);
//	// ������ɑ��
//	sprintf(&aStr[13][0], "stickR: %.3f|%.3f|%.3f\n", stickR.x, stickR.y, stickR.z);
//	// ������ɑ��
//	sprintf(&aStr[14][0], "move: %.3f|%.3f|%.3f\n", player->movevec.x, player->movevec.y, player->movevec.z);
//
//	for (int i = 0; i < 15; i++)
//	{
//		// �e�L�X�g�̕`��
//		rect = { 0,i * 30,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//		g_pFont->DrawText(NULL, &aStr[i][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 255, 255));
//
//	}
//}

//=========================================
// ���[�h�̐ݒ�
//=========================================
void SetMode(MODE mode)
{
	// ���݂̉��(���[�h)�̏I������
	switch (s_mode)
	{
	case MODE_TITLE:	// �^�C�g�����
		UninitTitle();
		break;

	case MODE_GAME:		// �Q�[�����
		UninitGame();
		break;

	case MODE_TUTORIAL:	// �`���[�g���A�����
//		UninitTutorial();
		break;

	case MODE_RESULT:	// �����L���O���
		UninitResult();
		break;
	}

	// �V�������(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:	// �^�C�g�����
		InitTitle();
		break;

	case MODE_GAME:		// �Q�[�����
		InitGame();
		break;

	case MODE_TUTORIAL:	// �`���[�g���A�����
//		InitTutorial();
		break;

	case MODE_RESULT:	// �����L���O���
		InitResult();
		break;
	}

	s_mode = mode;	// ���݂̉��(���[�h)��؂�ւ���

}

//=========================================
// ���[�h�̎擾
//=========================================
MODE GetMode(void)
{
	return s_mode;
}

int GetFPS(void)
{
	return g_nCountFPS;
}
