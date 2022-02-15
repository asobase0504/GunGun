//=========================================
// 
// �p�[�e�B�N���̏���
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "common.h"
#include "particle.h"
#include <assert.h>

//-----------------------------------------
// �}�N����`
//-----------------------------------------
#define MAX_PARTCLE	(1024)	// �p�[�e�B�N���̍ő吔
#define MAX_TEX		(MAX_PARTICLE)		// �e�N�X�`���̎��
#define JUMP_TEX	"data/TEXTURE/Circle_01.png"

//-----------------------------------------
// �p�[�e�B�N���\����
//-----------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// �ʒu
	D3DXVECTOR3 move;	// �ړ���
	D3DXVECTOR3 rot;	// �p�x
	D3DXCOLOR col;		// �F
	float fRaduus;		// ���a
	float fLength;		// ���S���W����̔��a
	float fAngle;		// �p�x�̎Z�o
	int nMaxLife;		// �ő����
	int nLife;			// ����
	PARTICLE_TYPE type;	// �p�[�e�B�N���̎��
	bool bUse;			// �g�p���Ă��邩�ǂ���e
}Particle;

//-----------------------------------------
// static�ϐ�
//-----------------------------------------
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_PARTICLE] = {};		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
static Particle s_aParticle[MAX_PARTCLE];
static float s_fAngle;

//-----------------------------------------
// �v���g�R�[��
//-----------------------------------------
#if 0
float angle = rand() / (float)RAND_MAX * 2.0f * D3DX_PI;
float length = 50.0f + rand() / (float)RAND_MAX * 50.0f;
float diffX = sinf(angle) * length;
float diffY = cosf(angle) * length;
float speed = ? ? ? ? ;
float life = length / speed;

pParticle->pos.x = pos.x + diffX;
pParticle->pos.y = pos.y + diffY;
pParticle->move.x = sinf(angle) * ((float)(rand() % 10) / 50 + 4.0f);
pParticle->move.y = cosf(angle) * ((float)(rand() % 10) / 50 + 4.0f);
pParticle->move.z = 0.0f;
pParticle->col = D3DXCOLOR(0.0f, 1.0f, 0.5f, 1.0f);
pParticle->fRaduus = 10.0f;
pParticle->nLife = (rand() % 20) + 10;
#endif

//=========================================
// �p�[�e�B�N���̏���������
//=========================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntParticle;
	VERTEX_2D *pVtx;
	Particle *pParticle;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓Ǎ�
	// �e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		JUMP_TEX,
		&s_pTexture[PARTICLE_PLAYER_GETMODEL]);

	ZeroMemory(s_aParticle, sizeof(s_aParticle));;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTCLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	VERTEX_2D* vtxBase = pVtx;

	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		assert(pVtx + 3 < vtxBase + (4 * MAX_PARTCLE));

		pParticle = &(s_aParticle[nCntParticle]);

		// ���_���W�̐ݒ�
		SetRectCenterPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(pParticle->col));

		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);
		
		// rhw�̐ݒ�
		InitRectRhw(pVtx);

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	s_pVtxBuff->Unlock();
}

//=========================================
// �p�[�e�B�N���̏I������
//=========================================
void UninitParticle(void)
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (s_pTexture[nCnt] != NULL)
		{
			s_pTexture[nCnt]->Release();
			s_pTexture[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}

}

//=========================================
// �p�[�e�B�N���̍X�V����
//=========================================
void UpdateParticle(void)
{
	Particle *pParticle;

	for (int i = 0; i < MAX_PARTCLE; i++)
	{
		pParticle = s_aParticle + i;

		if (!pParticle->bUse)
		{	// �p�[�e�B�N�����g�p����Ă��Ȃ�
			continue;
		}

		// �p�[�e�B�N�����g�p����Ă���
		// �ʒu�̍X�V
		pParticle->pos += pParticle->move;

		switch (pParticle->type)
		{
		case PARTICLE_PLAYER_GETMODEL:	// �v���C���[�̃W�����v�p�[�e�B�N��	// �񋓌^�ɕύX����B
			pParticle->nLife--;			// �̗͂̍X�V
			pParticle->col.a -= (float)1.0f / pParticle->nMaxLife;			// �����x�̍X�V
			pParticle->fRaduus += 0.75f;	// ���a�̊g��
			if (pParticle->nLife <= 0)
			{
				pParticle->bUse = false;
			}
			break;
		default:
			assert(false);
			break;
		}

		// ���S���W�����̒������Z�o����B
		pParticle->fLength = sqrtf(pParticle->fRaduus  * pParticle->fRaduus + pParticle->fRaduus * pParticle->fRaduus);

		// ���S���W�����̒��_�̊p�x���Z�o����
		pParticle->fAngle = atan2f(pParticle->fRaduus, pParticle->fRaduus);

		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += i * 4;

		// ���_���W�̐ݒ�
		SetRectCenterRotPos(pVtx, pParticle->pos, pParticle->rot, pParticle->fAngle, pParticle->fLength);
		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(pParticle->col));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

	}
}

//=========================================
// �p�[�e�B�N���̕`�揈��
//=========================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g
	Particle *pParticle;

	// �`��̂��߂̐ݒ�
	InitDraw(pDevice, s_pVtxBuff);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (!pParticle->bUse)
		{// �p�[�e�B�N�����g�p����Ă���
			continue;
		}

		switch (pParticle->type)
		{
		case PARTICLE_PLAYER_GETMODEL:	// �v���C���[�̃W�����v��
			// �e�N�X�`�������Z�����œ\��t���ĕ`�悷��
			RectDraw(pDevice, s_pTexture[pParticle->type], nCntParticle * 4);
			break;
		default:
			assert(false);
			break;
		}
	}
}

//=========================================
// �p�[�e�B�N���̐ݒ菈��
//=========================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLE_TYPE type)
{
	Particle *pParticle;
	int nCntParticle;


	for (nCntParticle = 0; nCntParticle < MAX_PARTCLE; nCntParticle++)
	{
		pParticle = &(s_aParticle[nCntParticle]);

		if (pParticle->bUse)
		{// �p�[�e�B�N�����g�p����Ă�
			continue;
		}

		ZeroMemory(pParticle, sizeof(s_aParticle[nCntParticle]));

		// �p�[�e�B�N�����g�p����Ă��Ȃ�
		pParticle->type = type;
		switch (type)
		{
		case PARTICLE_PLAYER_GETMODEL:	// �v���C���[�̃W�����v�p�[�e�B�N��
			pParticle->pos = pos;
			pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pParticle->move.x = 0.0f;
			pParticle->move.y = 0.0f;
			pParticle->move.z = 0.0f;
			pParticle->col = col;
			pParticle->fRaduus = 300.0f;
			pParticle->nMaxLife = 60;
			pParticle->nLife = pParticle->nMaxLife;
			break;
		default:
			assert(false);
			break;
		}

		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntParticle * 4;

		// ���S���W�����̒������Z�o����B
		pParticle->fLength = sqrtf(pParticle->fRaduus  * pParticle->fRaduus + pParticle->fRaduus * pParticle->fRaduus);

		// ���S���W�����̒��_�̊p�x���Z�o����
		pParticle->fAngle = atan2f(pParticle->fRaduus, pParticle->fRaduus);

		pParticle->bUse = true;

		SetRectCenterPos(pVtx, pParticle->pos, pParticle->fRaduus, pParticle->fRaduus);

		SetRectColor(pVtx, &(pParticle->col));

		// ���_�o�b�t�@���A�����b�N����
		s_pVtxBuff->Unlock();

		break;
	}
}
