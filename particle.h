//=========================================
// 
// �p�[�e�B�N���̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//=========================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//-----------------------------------------
// include
//-----------------------------------------

//------------------------------------
// ����ʒu�̗񋓌^�̒�`
//------------------------------------
typedef enum
{
	PARTICLE_PLAYER_GETMODEL = 0,		// �v���C���[�����f�����Q�b�g����Ƃ��ɏo��G�t�F�N�g
	MAX_PARTICLE
}PARTICLE_TYPE;

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, PARTICLE_TYPE type);

#endif // _PARTICLE_H_
