//=========================================
// 
// �|���S���̍쐬(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitPolygon(void);		// �|���S���̏���������
void UninitPolygon(void);	// �|���S���̏I������
void UpdatePolygon(void);	// �|���S���̍X�V����
void DrawPolygon(void);		// �|���S���̕`�揈��
D3DXVECTOR3 GetPolygonPos(void);		// �|���S���̎擾����

#endif // !_POLYGON_H_
