//=========================================
// 
// �f�o�b�O�p�̃��C���̍쐬
// Author YudaKaito
// 
//=========================================
#include "Line.h"
#include "main.h"

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};		// ���_�o�b�t�@�[�ւ̃|�C���^
static LPDIRECT3DTEXTURE9 *s_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
static int s_nShadowCnt;						// �e�̊��蓖��

void InitLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 40,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

}

void UninitLine(void)
{
}

void UpdateLine(void)
{
}

void DrawLine(void)
{

}
