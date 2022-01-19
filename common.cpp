//=========================================
// 
// ���ʏ���
// Author YudaKaito
// 
//=========================================
#include "common.h"

//=========================================
// �����`�̏�����
//=========================================
void InitRect(VERTEX_2D * vtx)
{
	// ���_���W�̐ݒ�
	InitRectPos(vtx);

	// ���_�J���[�̐ݒ�
	InitRectColor(vtx);

	// �e�N�X�`�����W�̐ݒ�
	InitRectTex(vtx);

	// rhw �̐ݒ�
	InitRectRhw(vtx);
}

//=========================================
// ���_�o�b�t�@��POS��������
//=========================================
void InitRectPos(VERTEX_2D * vtx)
{
	vtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=========================================
// ���_���W��POS��ݒ�(���S����)
//=========================================
void SetRectCenterPos(VERTEX_2D *vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth)
{
	vtx[0].pos.x = pos.x - fWidth;
	vtx[0].pos.y = pos.y - fHeigth;
	vtx[0].pos.z = pos.z + 0.0f;

	vtx[1].pos.x = pos.x + fWidth;
	vtx[1].pos.y = pos.y - fHeigth;
	vtx[1].pos.z = pos.z + 0.0f;

	vtx[2].pos.x = pos.x - fWidth;
	vtx[2].pos.y = pos.y + fHeigth;
	vtx[2].pos.z = pos.z + 0.0f;

	vtx[3].pos.x = pos.x + fWidth;
	vtx[3].pos.y = pos.y + fHeigth;
	vtx[3].pos.z = pos.z + 0.0f;
}

//=========================================
// ���_���W��ݒ�(���S)(�p�x�Ή�)
//=========================================
void SetRectCenterRotPos(VERTEX_2D * vtx, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fAngle, float fLength)
{
	//���_���W�̐ݒ�
	vtx[0].pos.x = pos.x + sinf(rot.z + (-D3DX_PI + fAngle)) * fLength;
	vtx[0].pos.y = pos.y + cosf(rot.z + (-D3DX_PI + fAngle)) * fLength;
	vtx[0].pos.z = 0.0f;

	vtx[1].pos.x = pos.x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;
	vtx[1].pos.y = pos.y + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;
	vtx[1].pos.z = 0.0f;

	vtx[2].pos.x = pos.x + sinf(rot.z + (fAngle * -1.0f)) * fLength;
	vtx[2].pos.y = pos.y + cosf(rot.z + (fAngle * -1.0f)) * fLength;
	vtx[2].pos.z = 0.0f;

	vtx[3].pos.x = pos.x + sinf(rot.z + fAngle) * fLength;
	vtx[3].pos.y = pos.y + cosf(rot.z + fAngle) * fLength;
	vtx[3].pos.z = 0.0f;

}

//=========================================
// ���_���W��ݒ�(���ォ��)
//=========================================
void SetRectUpLeftPos(VERTEX_2D * vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth)
{
	vtx[0].pos.x = pos.x;
	vtx[0].pos.y = pos.y;
	vtx[0].pos.z = pos.z;

	vtx[1].pos.x = pos.x + fWidth;
	vtx[1].pos.y = pos.y;
	vtx[1].pos.z = pos.z + 0.0f;

	vtx[2].pos.x = pos.x;
	vtx[2].pos.y = pos.y + fHeigth;
	vtx[2].pos.z = pos.z + 0.0f;

	vtx[3].pos.x = pos.x + fWidth;
	vtx[3].pos.y = pos.y + fHeigth;
	vtx[3].pos.z = pos.z + 0.0f;
}

//=========================================
// ���_���W��ݒ�(�E�ォ��)
//=========================================
void SetRectUpRightPos(VERTEX_2D * vtx, D3DXVECTOR3 pos, float fWidth, float fHeigth)
{
	vtx[0].pos.x = pos.x - fWidth;
	vtx[0].pos.y = pos.y;
	vtx[0].pos.z = pos.z;

	vtx[1].pos.x = pos.x;
	vtx[1].pos.y = pos.y;
	vtx[1].pos.z = pos.z;

	vtx[2].pos.x = pos.x - fWidth;
	vtx[2].pos.y = pos.y + fHeigth;
	vtx[2].pos.z = pos.z;

	vtx[3].pos.x = pos.x;
	vtx[3].pos.y = pos.y + fHeigth;
	vtx[3].pos.z = pos.z;
}

//=========================================
// ���_�o�b�t�@�̃J���[��������
//=========================================
void InitRectColor(VERTEX_2D * vtx)
{
	for (int i = 0; i < 4; ++i)
	{
		vtx->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		vtx++;
	}
}

//=========================================
// ���_�o�b�t�@�̃J���[��ݒ�
//=========================================
void SetRectColor(VERTEX_2D *vtx, D3DXCOLOR *inColor)
{
	for (int i = 0; i < 4; ++i)
	{
		vtx->col = *inColor;
		vtx++;
	}
}

//=========================================
// ���_�o�b�t�@�̃e�N�X�`�����W�̏�����
//=========================================
void InitRectTex(VERTEX_2D * vtx)
{
	vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

//=========================================
// ���_�o�b�t�@�̃e�N�X�`�����W�̐ݒ�
//=========================================
void SetRectTex(VERTEX_2D * vtx, float top, float bottom, float left, float right)
{
	vtx[0].tex = D3DXVECTOR2(left, top);
	vtx[1].tex = D3DXVECTOR2(right, top);
	vtx[2].tex = D3DXVECTOR2(left, bottom);
	vtx[3].tex = D3DXVECTOR2(right, bottom);

}

//=========================================
// rhw �̏�����
//=========================================
void InitRectRhw(VERTEX_2D * vtx)
{
	// rhw �̐ݒ�
	vtx[0].rhw = 1.0f;
	vtx[1].rhw = 1.0f;
	vtx[2].rhw = 1.0f;
	vtx[3].rhw = 1.0f;
}


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
// �`�ʏ����ɑO��Ƃ��ĕK�v�ȕ���
//=========================================
void InitDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DVERTEXBUFFER9 VtxBuff)
{
	// ���_�o�b�t�@���f�[�^�X�g���[���ݒ�
	pDevice->SetStreamSource(0, VtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
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

////=========================================
//// �������m�̓����蔻��
////=========================================
//bool SegmentColision(Segment seg1, Segment seg2)
//{
//	// �x�N�g���̎n�_���m�̋����B
//	D3DXVECTOR3 v = seg2.s - seg1.s;
//
//	// �u���b�N�̃x�N�g���Ɣ�Ώۂ̃x�N�g�������s�����ׂ�
//	float Bv_Tv = D3DXVec2Cross(&(seg1.v), &(seg2.v));
//	if (Bv_Tv == 0.0f)
//	{
//		// ���s�ł���B
//		return false;
//	}
//
//	D3DXVec3Cross()
//
//	float v_Bv = D3DXVec2Cross(&(v), &(seg1.v));
//	float v_Tv = D3DXVec2Cross(&(v), &(seg2.v));
//
//	float hit1 = v_Tv / Bv_Tv;
//	float hit2 = v_Bv / Bv_Tv;
//
//	if ((hit1 < 0.0f) || (hit1 > 1.0f) || (hit2 < 0.0f) || (hit2 > 1.0f))
//	{
//		return false;
//	}
//
//	if (Outpos != NULL)
//	{
//		*Outpos = seg1.s + seg1.v * hit1;
//	}
//	return true;
//}

//=========================================
// ���f���̃T�C�Y���Z�o(���Ƌ��̓����蔻��)
//=========================================
bool SphereColision(D3DXVECTOR3 pos1, float fLength1, D3DXVECTOR3 pos2, float fLength2)
{
#if 1
	D3DXVECTOR3 v = pos1 - pos2;
	return D3DXVec3LengthSq(&v) <= (fLength1 + fLength2)*(fLength1 + fLength2);
#else
	return (pow((pos2.x - pos1.x), 2.0f) + pow((pos2.y - pos1.y), 2.0f) + pow((pos2.z - pos1.z), 2.0f)) <= (pow((fLength1 + fLength2), 2.0f));
#endif
}
