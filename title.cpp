//=========================================
// 
// �^�C�g���̏���
// Author YudaKaito
// 
//=========================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "common.h"

#include "time.h"

#include <assert.h>

// �}�N����`
#define TITLEPRESS_WIDTH	(620)
#define TITLEPRESS_HEIGHT	(100)

#define SELECTBG			"data/TEXTURE/enemy000.png"
#define TITLE				"data/TEXTURE/�^�C�g��.png"
#define GAMESTART			"data/TEXTURE/GAMESTART.png"
#define TUTORIAL			"data/TEXTURE/TUTORIAL.png"
#define EXIT				"data/TEXTURE/EXIT.png"

// �v���X�G���^�[�̏�Ԃ̎��
typedef enum
{
	TYPE_NOME,	// �ω��Ȃ�
	TYPE_BLINK,	// �_�ŏ��
	TYPE_MAX
}PRESS_TYPE;

// �z�u����I�u�W�F�N�g�̎��
typedef enum
{
	OBJ_BG,				// �X�N���[���w�i
	OBJ_SELECTBG,		// �I����ʔw�i
	OBJ_TITLE,			// �^�C�g������
	OBJ_GAMESTART,		// GAMESTART����
	OBJ_TUTORIAL,		// TUTORIAL����
	OBJ_EXIT,			// EXIT����
	OBJ_SERECTCURSOR,	// �Z���N�g�J�[�\��
	OBJ_MAX				// 
}OBJ_TYPE;

// �I�����ꂽ�I�u�W�F�N�g
typedef enum
{
	SELECT_GAMESTART = OBJ_GAMESTART,
	SELECT_TUTORIAL,
	SELECT_EXIT,
}SELECT_MODE;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 Tex = NULL;				// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 pos;							// �ʒu
	D3DXVECTOR3 rot;							// �p�x
	D3DXCOLOR col;								// �F
	bool bUse;									// �g�p���Ă邩�ǂ���
	float Height;								// ����
	float Width;								// ��
}OBJECT;

// �O���[�o���ϐ�
static SELECT_MODE s_Select;
static OBJECT s_Object[OBJ_MAX] = {};
static bool	s_bFadeCheek;	// �t�F�[�h���u�Ɉڍs���邩�̏���
static int	s_nFadeCnt;		// �t�F�[�h�����ɍs���܂ł̊Ԋu
static bool s_bExit;		// Exit�̃t���O

//=========================================
// ����������
//=========================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�ւ̃|�C���g

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		SELECTBG,
		&s_Object[OBJ_SELECTBG].Tex);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		TITLE,
		&s_Object[OBJ_TITLE].Tex);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		GAMESTART,
		&s_Object[OBJ_GAMESTART].Tex);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL,
		&s_Object[OBJ_TUTORIAL].Tex);

	// �e�N�X�`���̓Ǎ�	   
	D3DXCreateTextureFromFile(pDevice,
		EXIT,
		&s_Object[OBJ_EXIT].Tex);

	s_bFadeCheek = false;	// �t�F�[�h�����Ɉڍs���邩�̕ϐ�
	s_nFadeCnt = 0;			// �t�F�[�h�����Ɉڍs����܂ł̊Ԋu
	s_Select = SELECT_GAMESTART;
	s_bExit = false;

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	for (int i = 0; i < OBJ_MAX; i++)
	{
		OBJECT *object = &(s_Object[i]);

		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&object->pVtxBuff,
			NULL);

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		object->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		switch (i)
		{
		case OBJ_BG:
			object->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(1.0f, 0.9f, 0.8f, 1.0f);	// �J���[�̐ݒ�
			object->Width = SCREEN_WIDTH;						// ���̐ݒ�
			object->Height = SCREEN_HEIGHT;						// �����̐ݒ�

			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_SELECTBG:
			object->pos = D3DXVECTOR3(SCREEN_WIDTH - 350.0f, SCREEN_HEIGHT, 0.0f);		// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.8f, 1.0f, 1.0f, 1.0f);							// �J���[�̐ݒ�
			object->Width = 1000.0f;													// ���̐ݒ�
			object->Height = 1000.0f;													// �����̐ݒ�
			
			// ���_���W�̐ݒ�
			SetRectCenterPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_TITLE:
			object->pos = D3DXVECTOR3(40.0f, 30.0f, 0.0f);		// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);	// �J���[�̐ݒ�
			object->Width = 1380.0f;							// ���̐ݒ�
			object->Height = 415.0f;							// �����̐ݒ�

			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_SERECTCURSOR:
		{
			object->pos = D3DXVECTOR3(1000.0f - 70.0f, 500.0f, 0.0f);	// ���S���W�̐ݒ�
			object->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.4f, 0.81f, 0.53f, 1.0f);	// �J���[�̐ݒ�
			object->Width = 50.0f;								// ���̐ݒ�
			object->Height = 50.0f;								// �����̐ݒ�

			// ���S���W�����̒������Z�o����B
			float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height);

			// ���S���W�����̒��_�̊p�x���Z�o����
			float fAngle = atan2f(object->Width, object->Height);

			// ���_���W�̐ݒ�
			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);
		}
			break;
		case OBJ_GAMESTART:
			object->pos = D3DXVECTOR3(1000.0f, 560.0f, 0.0f);	// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	// �J���[�̐ݒ�
			object->Width = 780.0f;								// ���̐ݒ�
			object->Height = 140.0f;							// �����̐ݒ�

			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_TUTORIAL:
			object->pos = D3DXVECTOR3(1000.0f, 720.0f, 0.0f);	// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	// �J���[�̐ݒ�
			object->Width = 615.0f;								// ���̐ݒ�
			object->Height = 140.0f;							// �����̐ݒ�

			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_EXIT:
			object->pos = D3DXVECTOR3(1000.0f, 880.0f, 0.0f);	// ���S���W�̐ݒ�
			object->col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);	// �J���[�̐ݒ�
			object->Width = 270.0f;								// ���̐ݒ�
			object->Height = 140.0f;							// �����̐ݒ�
			
			// ���_���W�̐ݒ�
			SetRectUpLeftPos(pVtx, object->pos, object->Width, object->Height);
			break;
		case OBJ_MAX:
		default:
			assert(false);
			break;
		}

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(object->col));

		// rhw�̐ݒ�
		InitRectRhw(pVtx);

		// �e�N�X�`�����W�̐ݒ�
		InitRectTex(pVtx);

		// ���_�o�b�t�@���A�����b�N����
		s_Object[i].pVtxBuff->Unlock();

		// �g�p�ɐ؂�ւ�
		object->bUse = true;
	}
}

//=========================================
// �I������
//=========================================
void UninitTitle(void)
{
	// �e�N�X�`���̔j��
	for (int i = 0; i < OBJ_MAX; i++)
	{
		if (s_Object[i].Tex != NULL)
		{
			s_Object[i].Tex->Release();
			s_Object[i].Tex = NULL;
		}

	}

	// ���_�o�b�t�@�̔j��
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
// �X�V����
//=========================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	if (!(s_bFadeCheek))
	{
		// �I������
		SelectTitle();

		// �W���C�p�b�h�̎g�p���̎擾
		bool bUseJoyPad = IsJoyPadUse(0);
		if (bUseJoyPad)
		{
			if (GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B))
			{// Enter�L�[���p�b�h��A�������ꂽ��
			 // ���艹�̍Đ�
				s_bFadeCheek = true;	// �t�F�[�h�����ɓ���
			}
		}
		else
		{
			if (GetKeyboardTrigger(DIK_RETURN))
			{// Enter�L�[���p�b�h��A�������ꂽ��
			 // ���艹�̍Đ�
				s_bFadeCheek = true;	// �t�F�[�h�����ɓ���
			}
		}
	}

	for (int i = 0; i < OBJ_MAX; i++)
	{
		OBJECT *object = &(s_Object[i]);

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		object->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		if (i == OBJ_SERECTCURSOR)
		{
			object->rot.z += 0.025f;
			object->pos.x = s_Object[s_Select].pos.x - 50.0f;
			object->pos.y = s_Object[s_Select].pos.y + 140.0f / 2.0f;
			object->pos.z = s_Object[s_Select].pos.z;

			// ���S���W�����̒������Z�o����B
			float fLength = sqrtf(object->Width  * object->Width + object->Height * object->Height) / 2.0f;

			// ���S���W�����̒��_�̊p�x���Z�o����
			float fAngle = atan2f(object->Width, object->Height);

			// ���_���W�̐ݒ�
			SetRectCenterRotPos(pVtx, object->pos, object->rot, fAngle, fLength);
		}

		// ���_�J���[�̐ݒ�
		SetRectColor(pVtx, &(object->col));

		// ���_�o�b�t�@���A�����b�N����
		object->pVtxBuff->Unlock();
	}

	if (s_bFadeCheek)
	{
		if (s_nFadeCnt >= 5)
		{
			switch (s_Select)
			{
			case SELECT_GAMESTART:
				SetFade(MODE_GAME);	// �Q�[�����[�h�Ɉڍs
				break;
			case SELECT_TUTORIAL:
				SetFade(MODE_TUTORIAL);	// �`���[�g���A����ʂɈڍs
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
// �I������
//=========================================
void SelectTitle(void)
{
	// �W���C�p�b�h�̎g�p���̎擾
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
// �`�揈��
//=========================================
void DrawTitle(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int i = 0; i < OBJ_MAX; i++)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
		pDevice->SetStreamSource(0, s_Object[i].pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		if (s_Object[i].bUse == true)
		{
			// �|���S���`��
			// �e�N�X�`���̐ݒ�
			RectDraw(pDevice, s_Object[i].Tex, 0);
		}
	}
}

//
bool GetExit(void)
{
	return s_bExit;
}
