//=========================================
// 
// �|���S���̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _SETUP_H_
#define _SETUP_H_

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
float NormalizeRot(float rot);
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// �ʏ�̕`��
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// ���Z�����̕`��

#endif // !_SETUP_H_
