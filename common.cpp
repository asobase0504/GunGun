//=========================================
// 
// ���ʏ���
// Author YudaKaito
// 
//=========================================
#include "common.h"

//=========================================
// �p�x�̐��K��
//=========================================
float NormalizeRot(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2;
	}
	if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2;
	}

	return rot;
}

//=========================================
// ���Z�����Ȃ��̕`��(�����`)
//=========================================
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt, 2);
}

//=========================================
// ���Z�����L��̕`��(�����`)
//=========================================
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	RectDraw(pDevice, Texture, nCnt);

	// a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=========================================
// ���f���̃T�C�Y���Z�o
//=========================================
void ModelSize(D3DXVECTOR3* Min, D3DXVECTOR3* Max, LPD3DXMESH Mesh)
{
	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�[�ւ̃|�C���g

	nNumVtx = Mesh->GetNumVertices();	// ���_���̎擾

	sizeFVF = D3DXGetFVFVertexSize(Mesh->GetFVF());	// ���_�t�H�[�}�b�g�̃T�C�Y���擾

	// ������
	*Min = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	*Max = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	// ���_�o�b�t�@�[�̃��b�N
	Mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x < Min->x)
		{
			Min->x = vtx.x;
		}
		if (vtx.y < Min->y)
		{
			Min->y = vtx.y;
		}
		if (vtx.z < Min->z)
		{
			Min->z = vtx.z;
		}
		if (vtx.x > Max->x)
		{
			Max->x = vtx.x;
		}
		if (vtx.y > Max->y)
		{
			Max->y = vtx.y;
		}
		if (vtx.z > Max->z)
		{
			Max->z = vtx.z;
		}

		// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		pVtxBuff += sizeFVF;
	}

	// ���_�o�b�t�@�[�̃A�����b�N
	Mesh->UnlockVertexBuffer();
}

//=========================================
// �������m�̓����蔻��
//=========================================
bool SegmentColision(Segment seg1, Segment seg2)
{
	return false;
}
