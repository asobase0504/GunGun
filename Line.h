//=========================================
// 
// �f�o�b�O�p���C���̍쐬
// Author YudaKaito
// 
//=========================================
#ifndef _LINE_H_
#define _LINE_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitLine(void);
void UninitLine(void);
void UpdateLine(void);
void DrawLine(void);
void SetLine(D3DXVECTOR3* pos, D3DXVECTOR3* rot, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col);
void SetLine(D3DXVECTOR3* pos, D3DXQUATERNION* quaternion, D3DXVECTOR3 start, D3DXVECTOR3 end, D3DXCOLOR col);
void SetModelLine(D3DXVECTOR3* pos, D3DXQUATERNION* quaternion, D3DXVECTOR3 Max, D3DXVECTOR3 Min, D3DXCOLOR col);

#endif // !_LINE_H_