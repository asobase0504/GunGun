//==================================================
//
// ���̉~
// Author : Yuda Kaito
//
//==================================================
#ifndef _FAN_H_		// ���̃}�N����`������ĂȂ�������
#define _FAN_H_		// �Q�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"
//--------------------------------------------------
// �\���̂��`
//--------------------------------------------------
/*�� ���b�V���� ��*/
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		rot;			// ����
	D3DXMATRIX		mtxWorld;		// ���[���h�}�g���b�N�X
	float fLength;					// ����
}Fan;

//--------------------------------------------------
// �v���g�^�C�v�錾
//--------------------------------------------------
void InitFan(void);					// ������
void UninitFan(void);				// �I��
void UpdateFan(void);				// �X�V
void DrawFan(void);					// �`��
void SetFan(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLength);					// �ݒ�
void OpenFan(int nCnt);						// ���ɊJ��
void CloseFan(int nCnt);					// ���ɕ���
Fan *GetFan(void);				// �ʒu���̏��擾

#endif // !_FAN_H_
