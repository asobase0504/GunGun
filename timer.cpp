//=========================================
// 
// �^�C������
// Author KOZUNA HIROHITO
// Author YudaKaito
// 
//=========================================
#include "timer.h"
#include <stdio.h>

//�}�N����`
#define MAX_TIMER_DITIT		(256)		// �^�C�}�[(1��)�̍ő吔
#define MAX_TIMER			(64)		// �^�C�}�[(�S��)�̍ő吔
#define TIME_TEX			"data/TEXTURE/number003.png"	// �^�C�}�[�̃e�N�X�`��

//�O���[�o���ϐ�
static LPDIRECT3DTEXTURE9 g_pTextureTimer = { NULL };					//�e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = { NULL };				//���_�o�b�t�@�ւ̃|�C���^
static TimerDigit g_TimerDigit[MAX_TIMER_DITIT];						//�^�C���\���̂̕ϐ�
static Timer g_Timer[MAX_TIMER];

//�v���g�^�C�v�錾
static void TimerDataInit(void);
static void SetTimer(int nNumber, int nSecond);

//=========================================
//�^�C���̏�����
//=========================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		TIME_TEX,
		&g_pTextureTimer);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMER_DITIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	//�^�C���̏��̏�����
	TimerDataInit();


	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TIMER_DITIT; nCnt++)
	{
		TimerDigit* timer = &(g_TimerDigit[nCnt]);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(timer->pos.x - timer->fWidth, timer->pos.y - timer->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(timer->pos.x + timer->fWidth, timer->pos.y - timer->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(timer->pos.x - timer->fWidth, timer->pos.y + timer->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(timer->pos.x + timer->fWidth, timer->pos.y + timer->fHeight, 0.0f);
		
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4; //�f�[�^���S���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTimer->Unlock();
}

//=========================================
//�^�C���̏I������
//=========================================
void UninitTimer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}

	for (int i = 0; i < MAX_TIMER; i++)
	{
		Timer* timer = &(g_Timer[i]);

		// �f�o�b�O�\���p�t�H���g�̔j��
		if (g_Timer[i].font != NULL)
		{
			g_Timer[i].font->Release();
			g_Timer[i].font = NULL;
		}
	}
}

//=========================================
//�^�C���̍X�V����
//=========================================
void UpdateTimer(void)
{
	for (int i = 0; i < MAX_TIMER; i++)
	{
		Timer* timer = &(g_Timer[i]);

		if (!timer->bUse && !timer->bCount)
		{
			continue;
		}

		timer->nFps++;
		if (timer->nFps >= 55)
		{
			timer->nFps = 0;

			if (timer->nSecond >= 0)
			{
				//�^�C���̃Z�b�g
				SetTimer(timer->nNumber, timer->nSecond);
			}

			//�^�C�}�[��i�߂�
			timer->nSecond--;
		}
	}

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_TIMER_DITIT; i++)
	{
		TimerDigit* timer = &(g_TimerDigit[i]);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(timer->pos.x - timer->fWidth, timer->pos.y - timer->fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(timer->pos.x + timer->fWidth, timer->pos.y - timer->fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(timer->pos.x - timer->fWidth, timer->pos.y + timer->fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(timer->pos.x + timer->fWidth, timer->pos.y + timer->fHeight, 0.0f);

		pVtx += 4;  //���_�f�[�^�̃|�C���^���S* nCntDigit���i�߂�
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTimer->Unlock();
}

//=========================================
//�^�C���̕`�揈��
//=========================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTimer);

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (!g_Timer[nCnt].bUse)
		{
			continue;
		}

		// �\���̈�̍쐬
		RECT rect = { (LONG)g_Timer[nCnt].pos.x,g_Timer[nCnt].pos.y,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };

		char aLength[5];

		sprintf(aLength, "%d", g_Timer[nCnt].nSecond);

		g_Timer[nCnt].font->DrawText(NULL, aLength, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
	}
	//for (int nCnt = 0; nCnt < MAX_TIMER_DITIT; nCnt++)
	//{
	//	if (!g_TimerDigit[nCnt].bUse)
	//	{
	//		continue;
	//	}

	//	s_pFont->DrawText(NULL, s_aLength, -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
	//	//�|���S���̕`��
	//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
	//		4 * nCnt,						//�`�悷��ŏ��̒��_�C���f�b�N�X
	//		2);		//�v���~�e�B�u�i�|���S���j��
	//}
}

//=========================================
// �^�C���̎擾����
//=========================================
Timer* GetTimer(int nNumber)
{
	return &g_Timer[nNumber];
}

//=========================================
// �^�C���̊J�n����
//=========================================
void StartTimer(int nSecond, int nDigit, float fWidth, float fHeight, D3DXVECTOR3 pos, int nPatn)
{
	int nCntTimer;
	for (nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{//�^�C�}�[�̃O���[�v�̃��[�v

		if (g_Timer[nCntTimer].bUse)
		{
			continue;
		}

		g_Timer[nCntTimer].nSecond = nSecond;	//�\������
		
		//�^�C�}�[�̔ԍ�
		if (nCntTimer - 1 < 0)
		{
			g_Timer[nCntTimer].nNumber = 0;
		}
		else
		{
			g_Timer[nCntTimer].nNumber = g_Timer[nCntTimer - 1].nNumber + 1;
		}
		g_Timer[nCntTimer].bUse = true;//�g�p���Ă邩�ǂ���
		g_Timer[nCntTimer].bCount = false;//�J�E���g���~�܂��Ă�����

		g_Timer[nCntTimer].pos = pos;//�ʒu
		D3DXCreateFont(GetDevice(), fHeight, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "07����������|�b�v Heavy", &g_Timer[nCntTimer].font);
		break;
	}

	for (int nCnt = 0; nCnt < MAX_TIMER_DITIT; nCnt++)
	{//�^�C�}�[�̂P���̃��[�v
		TimerDigit *timerDigit = &(g_TimerDigit[nCnt]);

		if (timerDigit->bUse)
		{
			continue;
		}

		timerDigit->fWidth = fWidth;	//����
		timerDigit->fHeight = fHeight;	//�c��
		timerDigit->pos = D3DXVECTOR3(pos.x - g_Timer[nCntTimer].nDigit * fWidth * 2, pos.y, 0.0f);//�ʒu
		timerDigit->nPatn = nPatn;//�\���̂�����
		timerDigit->nNumber = g_Timer[nCntTimer].nNumber;//�^�C�}�[�̔ԍ�
		timerDigit->bUse = true;//�g�p���Ă邩�ǂ���
		g_Timer[nCntTimer].nDigit++;//�K�v�Ȍ����J�E���g

		if (g_Timer[nCntTimer].nDigit == nDigit + 1)
		{
			break;
		}
	}

	//�^�C���Z�b�g����
	SetTimer(g_Timer[nCntTimer].nNumber, nSecond);
}

//=========================================
//�^�C���̃J�E���g���w��̒l�ɂȂ������ǂ���
//=========================================
bool TimerUp(int nNumber)
{
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (g_Timer[nCnt].bUse && g_Timer[nCnt].nNumber == nNumber && g_Timer[nCnt].nSecond <= 0)
		{
				return true;
		}
	}
	return false;
}

//=========================================
//�^�C���̏��̏�����
//=========================================
static void TimerDataInit(void)
{
	ZeroMemory(g_Timer,sizeof(g_Timer));

	ZeroMemory(g_TimerDigit, sizeof(g_TimerDigit));
}

//=========================================
//�^�C�}�[������
//=========================================
void BreakTimer(int nNumber)
{
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (g_Timer[nCnt].bUse && g_Timer[nCnt].nNumber == nNumber)
		{
			g_Timer[nCnt].nSecond = 0;//�\������
			g_Timer[nCnt].nFps = 0;//���݂̃t���[��
			g_Timer[nCnt].nNumber = 0;//�^�C�}�[�̔ԍ�
			g_Timer[nCnt].nDigit = 0;//������
			g_Timer[nCnt].bUse = false;//�g�p���Ă邩�ǂ���
			g_Timer[nCnt].bCount= false;//�J�E���g���X�g�b�v���Ă���
		}
	}

	for (int nCnt = 0; nCnt < MAX_TIMER_DITIT; nCnt++)
	{
		if (g_TimerDigit[nCnt].bUse && g_TimerDigit[nCnt].nNumber == nNumber)
		{
			g_TimerDigit[nCnt].fWidth = 0.0f;//����
			g_TimerDigit[nCnt].fHeight = 0.0f;//�c��
			g_TimerDigit[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ʒu
			g_TimerDigit[nCnt].nPatn = 0;//�\���̂�����
			g_TimerDigit[nCnt].nNumber = 0;//�^�C�}�[�̔ԍ�
			g_TimerDigit[nCnt].bUse = false;//�g�p���Ă邩�ǂ���
		}
	}

}

//=========================================
//�^�C���̃Z�b�g
//=========================================
static void SetTimer(int nNumber, int nSecond)
{
	int aPosTexU = 0;

	for (int nCntTimer = 0; nCntTimer < MAX_TIMER; nCntTimer++)
	{
		Timer* timer = &(g_Timer[nCntTimer]);

		if (!timer->bUse || timer->nNumber != nNumber)
		{
			continue;
		}

		for (int nCntDigit = 0; nCntDigit < MAX_TIMER_DITIT; nCntDigit++)
		{
			TimerDigit *timerDigit = &(g_TimerDigit[nCntDigit]);

			if (!timerDigit->bUse || timerDigit->nNumber != nNumber)
			{
				continue;
			}

			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
			
			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntDigit;  //���_�f�[�^�̃|�C���^���S* nCntDigit���i�߂�

			for (int nCntTexU = 0, nCnt = 1; nCntTexU < timer->nDigit; nCntTexU++, nCnt *= 10)
			{
				aPosTexU = nSecond % (nCnt * 10) / nCnt;	//�e�ʂ̐������i�[

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.1f * aPosTexU, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f * (aPosTexU + 1), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.1f * aPosTexU, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f * (aPosTexU + 1), 1.0f);

				pVtx += 4;  //���_�f�[�^�̃|�C���^���S���i�߂�

			}
			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffTimer->Unlock();

			return;
		}
	}
}

//=========================================
//�J�E���g�̃��X�^�[�g�ƃX�g�b�v����
//=========================================
void CountRestartStop(bool bCount, int nNumber)
{
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (!g_Timer[nCnt].bUse && g_Timer[nCnt].nNumber != nNumber)
		{
			continue;
		}

		g_Timer[nCnt].bCount = bCount;//�J�E���g�����X�^�[�g���X�g�b�v������
		break;
	}
}