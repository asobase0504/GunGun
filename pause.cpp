//============================================================================================================
//
// �|�[�Y����
// Author : Kishimoto Eiji
// Author : Yuda Kaito
//
//============================================================================================================
#include "pause.h"
#include "input.h"
#include "game.h"
#include "common.h"
#include "fade.h"
#include "sound.h"

//============================================================================================================
//�}�N����`
//============================================================================================================
#define NUM_PAUSE				(6)			//�|�[�Y�̎��
#define PAUSEMENU_HALF_WIDTH	(200)		//�|�[�Y���j���[�̕�(����)
#define PAUSEMENU_HALF_HEIGHT	(50)		//�|�[�Y���j���[�̍���(����)
#define PAUSEMENU_TITLE_WIDTH	(300)		//�|�[�Y���j���[�̕�(����)
#define PAUSEMENU_TITLE_HEIGHT	(150)		//�|�[�Y���j���[�̍���(����)
#define PAUSEMENU_SPACE			(200)		//�|�[�Y���j���[�̊Ԋu
#define PAUSE_HALF_WIDTH		(350)		//�|�[�Y�̕�(����)
#define PAUSE_HALF_HEIGHT		(400)		//�|�[�Y�̍���(����)

//============================================================================================================
//�X�^�e�B�b�N�ϐ�
//============================================================================================================
static LPDIRECT3DTEXTURE9	   s_apTexture[NUM_PAUSE] = { NULL, NULL, NULL, NULL };		//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPause		  = NULL;							//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPauseTitle = NULL;								//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPauseCursol = NULL;							//���_�o�b�t�@�ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPauseMenu	  = NULL;							//���_�o�b�t�@�ւ̃|�C���^
static PAUSE_MENU			   s_pauseMenu;												//�|�[�Y���j���[
static PauseMenu			   s_aPauseMenu[PAUSE_MENU_MAX];							//�|�[�Y���j���[�̏��
static D3DXVECTOR3			   s_posPause;												//�|�[�Y�̈ʒu
static D3DXVECTOR3			   s_posPauseCusol;											//�|�[�Y�̈ʒu
static D3DXVECTOR3			   s_rotPauseCusol;											//�|�[�Y�̈ʒu

//============================================================================================================
//�|�[�Y�̏���������
//============================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;							//���_���ւ̃|�C���^
	int nCntPause;								//�J�E���g
	s_pauseMenu = PAUSE_MENU_CONTINUE;			//�|�[�Y���j���[

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/WORD/back.png",
		&s_apTexture[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/WORD/Restart.png",
		&s_apTexture[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/WORD/back_Title.png",
		&s_apTexture[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/pause100.png",
		&s_apTexture[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Pause_Menu.png",
		&s_apTexture[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/cursor.png",
		&s_apTexture[5]);


	s_posPause = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);	//�|�[�Y�̈ʒu��������
	s_rotPauseCusol = ZERO_VECTOR;

	//���̏�����
	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		s_aPauseMenu[nCntPause].pos = D3DXVECTOR3(s_posPause.x, s_posPause.y + ((nCntPause - 1) * PAUSEMENU_SPACE), 0.0f);
		s_aPauseMenu[nCntPause].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	}

	s_pauseMenu = PAUSE_MENU_CONTINUE;					//�|�[�Y���j���[��������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPause,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPauseTitle,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPauseMenu,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuffPauseCursol,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < 2; nCntPause++)
	{
		if (nCntPause == 0)
		{//�|�[�Y�̔������|���S��
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		}
		else if (nCntPause == 1)
		{//�|�[�Y
			//���_���W�̐ݒ�
			pVtx[0].pos.x = s_posPause.x - PAUSE_HALF_WIDTH;
			pVtx[0].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = s_posPause.x + PAUSE_HALF_WIDTH;
			pVtx[1].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = s_posPause.x - PAUSE_HALF_WIDTH;
			pVtx[2].pos.y = s_posPause.y + PAUSE_HALF_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = s_posPause.x + PAUSE_HALF_WIDTH;
			pVtx[3].pos.y = s_posPause.y + PAUSE_HALF_HEIGHT;
			pVtx[3].pos.z = 0.0f;
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		if (nCntPause == 0)
		{//�|�[�Y�̔������|���S��
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		}
		else if (nCntPause == 1)
		{//�|�[�Y
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.3f, 0.5f, 0.2f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.3f, 0.5f, 0.2f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.3f, 0.5f, 0.2f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.3f, 0.5f, 0.2f, 1.0f);
		}

		//�e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPause->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//�|�[�Y
		 //���_���W�̐ݒ�
		pVtx[0].pos.x = s_aPauseMenu[nCntPause].pos.x - PAUSEMENU_HALF_WIDTH;
		pVtx[0].pos.y = s_aPauseMenu[nCntPause].pos.y - PAUSEMENU_HALF_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = s_aPauseMenu[nCntPause].pos.x + PAUSEMENU_HALF_WIDTH;
		pVtx[1].pos.y = s_aPauseMenu[nCntPause].pos.y - PAUSEMENU_HALF_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = s_aPauseMenu[nCntPause].pos.x - PAUSEMENU_HALF_WIDTH;
		pVtx[2].pos.y = s_aPauseMenu[nCntPause].pos.y + PAUSEMENU_HALF_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = s_aPauseMenu[nCntPause].pos.x + PAUSEMENU_HALF_WIDTH;
		pVtx[3].pos.y = s_aPauseMenu[nCntPause].pos.y + PAUSEMENU_HALF_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);

		pVtx += 4;		//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPauseMenu->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPauseTitle->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = s_posPause.x - PAUSEMENU_TITLE_WIDTH;
	pVtx[0].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT - 70.0f - PAUSEMENU_TITLE_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = s_posPause.x + PAUSEMENU_TITLE_WIDTH;
	pVtx[1].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT - 70.0f - PAUSEMENU_TITLE_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = s_posPause.x - PAUSEMENU_TITLE_WIDTH;
	pVtx[2].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT - 70.0f + PAUSEMENU_TITLE_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = s_posPause.x + PAUSEMENU_TITLE_WIDTH;
	pVtx[3].pos.y = s_posPause.y - PAUSE_HALF_HEIGHT - 70.0f + PAUSEMENU_TITLE_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	InitRectTex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPauseTitle->Unlock();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuffPauseCursol->Lock(0, 0, (void**)&pVtx, 0);

	s_posPauseCusol.x = s_aPauseMenu[s_pauseMenu].pos.x - 250.0f;
	s_posPauseCusol.y = s_aPauseMenu[s_pauseMenu].pos.y;
	s_posPauseCusol.z = s_aPauseMenu[s_pauseMenu].pos.z;

	// ���_���W�̐ݒ�
	SetPosRectUpLeft(pVtx, s_posPauseCusol, 90.0f, 90.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	InitRectTex(pVtx);

	//���_�o�b�t�@���A�����b�N����
	s_pVtxBuffPauseCursol->Unlock();

}

//============================================================================================================
//�|�[�Y�̏I������
//============================================================================================================
void UninitPause(void)
{
	int nCntPause;		//�J�E���g

	//�e�N�X�`���̔j��
	for (nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (s_apTexture[nCntPause] != NULL)
		{
			s_apTexture[nCntPause]->Release();
			s_apTexture[nCntPause] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffPause != NULL)
	{
		s_pVtxBuffPause->Release();
		s_pVtxBuffPause = NULL;
	}

	if (s_pVtxBuffPauseCursol != NULL)
	{
		s_pVtxBuffPauseCursol->Release();
		s_pVtxBuffPauseCursol = NULL;
	}

	if (s_pVtxBuffPauseMenu != NULL)
	{
		s_pVtxBuffPauseMenu->Release();
		s_pVtxBuffPauseMenu = NULL;
	}

	if (s_pVtxBuffPauseTitle != NULL)
	{
		s_pVtxBuffPauseTitle->Release();
		s_pVtxBuffPauseTitle = NULL;
	}
}

//============================================================================================================
//�|�[�Y�̍X�V����
//============================================================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	if (GetFade() == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ��Ƃ�

		if (GetJoypadTrigger(JOYKEY_UP) || GetKeyboardTrigger(DIK_W))
		{//W�L�[�������ꂽ

			//1���I��
			s_pauseMenu = (PAUSE_MENU)(((s_pauseMenu - 1) + 3) % 3);
			//���ɂ���
			s_posPauseCusol.x = s_aPauseMenu[s_pauseMenu].pos.x - 250.0f;
			s_posPauseCusol.y = s_aPauseMenu[s_pauseMenu].pos.y;
			s_posPauseCusol.z = s_aPauseMenu[s_pauseMenu].pos.z;
			PlaySound(SOUND_LABEL_SE_SERECT_POUSE);
		}

		if (GetJoypadTrigger(JOYKEY_DOWN) || GetKeyboardTrigger(DIK_S))
		{//S�L�[�������ꂽ
			 //1����I��
			s_pauseMenu = (PAUSE_MENU)(((s_pauseMenu + 1) + 3) % 3);

			//���ɂ���
			s_posPauseCusol.x = s_aPauseMenu[s_pauseMenu].pos.x - 250.0f;
			s_posPauseCusol.y = s_aPauseMenu[s_pauseMenu].pos.y;
			s_posPauseCusol.z = s_aPauseMenu[s_pauseMenu].pos.z;
			PlaySound(SOUND_LABEL_SE_SERECT_POUSE);
		}
		s_rotPauseCusol.z += 0.01f;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuffPauseCursol->Lock(0, 0, (void**)&pVtx, 0);

		float fLength = sqrtf(90.0f  * 90.0f + 90.0f * 90.0f) / 2.0f;	// ���S���W�����̒������Z�o����B
		float fAngle = atan2f(90.0f, 90.0f);							// ���S���W�����̒��_�̊p�x���Z�o����

		// ���_���W�̐ݒ�
		SetPosRectCenterRot(pVtx, s_posPauseCusol, s_rotPauseCusol, fAngle, fLength);

		//���_�o�b�t�@���A�����b�N����
		s_pVtxBuffPauseCursol->Unlock();

		if (GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN))
		{//Enter�L�[�������ꂽ
			PlaySound(SOUND_LABEL_SE_SERECT);
			switch (s_pauseMenu)
			{
			case PAUSE_MENU_CONTINUE:	//�Q�[���ɖ߂�

				//�|�[�Y����
				SetEnablePause(false);
				break;

			case PAUSE_MENU_RETRY:		//�Q�[������蒼��

				//�Q�[����ʂɈڍs
				ResetFade(MODE_GAME);
				break;

			case PAUSE_MENU_QUIT:		//�^�C�g����ʂɖ߂�

				//�^�C�g����ʂɈڍs
				SetFade(MODE_TITLE);
				break;
			}
		}
	}
}

//============================================================================================================
//�|�[�Y�̕`�揈��
//============================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	int nCntPause;								//�J�E���g

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < 2; nCntPause++)
	{
		if (nCntPause == 0)
		{//�|�[�Y�̔������|���S��
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);
		}
		else if (nCntPause == 1)
		{//�|�[�Y
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, s_apTexture[3]);
		}

		//�|�[�Y�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCntPause * 4,			//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);						//�`�悷��v���~�e�B�u��
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPauseMenu, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_apTexture[nCntPause]);

		//�|�[�Y�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
			nCntPause * 4,								//�`�悷��ŏ��̒��_�C���f�b�N�X
			2);											//�`�悷��v���~�e�B�u��
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPauseTitle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_apTexture[4]);

	//�|�[�Y�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,			//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);						//�`�悷��v���~�e�B�u��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffPauseCursol, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_apTexture[5]);

	//�|�[�Y�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,			//�`�悷��ŏ��̒��_�C���f�b�N�X
		2);						//�`�悷��v���~�e�B�u��

}