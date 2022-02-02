//=========================================
// 
// UI����(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _UI_H_
#define _UI_H_

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitUI(void);		// ����������
void UninitUI(void);	// �I������
void UpdateUI(void);	// �X�V����
void DrawUI(void);		// �`�揈��
void SetUI(char* pFile, D3DXVECTOR3* pos, D3DXCOLOR* col, D3DXVECTOR3* size);	// �ݒ菈��
void LoadTitleUI(void);		// �ǂݍ��ݏ���(�^�C�g��)
void LoadGameUI(void);		// �ǂݍ��ݏ���(�Q�[��)
void LoadResultUI(void);	// �ǂݍ��ݏ���(���U���g)

#endif // !_UI_H_