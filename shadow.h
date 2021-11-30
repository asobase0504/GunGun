//=========================================
// 
// �|���S���̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//------------------------------------
// �e�̍\���̂��`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 rot;	// ��]���W
	D3DXMATRIX mtxWorld;// ���[���h�}�g���b�N�X
	bool bUse;			// �g�p��
} Shadow;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitShadow(void);			// �e�̏���������
void UninitShadow(void);		// �e�̏I������
void UpdateShadow(void);		// �e�̍X�V����
void DrawShadow(void);			// �e�̕`�揈��
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot);			// �e�̐ݒ菈��
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);	// �e�ʒu�̐ݒ菈��
Shadow* GetShadow(void);			// �e�̎擾����

#endif // !_SHADOW_H_
