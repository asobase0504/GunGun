//=========================================
// 
// �Q�[��������
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "debug.h"
#include <stdio.h>
#include "polygon.h"
#include "input.h"
#include "player.h"
#include "camera.h"
#include "light.h"
#include "model.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "mesh_field.h"
#include "mesh_cylinder.h"
#include "mesh_sphere.h"
#include "mesh_sky.h"
#include <stdio.h>

//------------------------------------
// �}�N���錾
//------------------------------------
#define DEBUG_NUM	(15)

//------------------------------------
// �ÓI�ϐ��錾
//------------------------------------
static LPD3DXFONT s_pFont = NULL;	// �t�H���g�ւ̃|�C���^

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------

//=========================================
// FPS�\���̏�����
//=========================================
void InitFPS(void)
{
	// �f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(GetDevice(), 32, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &s_pFont);
}

//=========================================
// FPS�\���̏I������
//=========================================
void UninitFPS(void)
{
	// �f�o�b�O�\���p�t�H���g�̔j��
	if (s_pFont != NULL)
	{
		s_pFont->Release();
		s_pFont = NULL;
	}
}

//=========================================
// FPS�̕\��
//=========================================
void DrawFPS(void)
{
	char aStr[DEBUG_NUM][256];	// �\������

	// ���̎擾
	D3DXVECTOR3 camerarot = GetRotCamera();
	Player* player = GetPlayer();
	Model* model = GetModel();
	Camera* camera = GetCamera();
	Shadow* shadow = GetShadow();
	MODE mode = GetMode();
	D3DXVECTOR3 stickL = GetJoypadStick(JOYKEY_LEFT_STICK, 0);
	D3DXVECTOR3 stickR = GetJoypadStick(JOYKEY_RIGHT_STICK, 0);

	// ������ɑ��
	wsprintf(&aStr[0][0], "FPS: %d\n", GetFPS());

	// ������ɑ��
	sprintf(&aStr[1][0],	"rot: %f\n", camera->rot.y);
	sprintf(&aStr[2][0],	"Player.pos     : %.3f|%.3f|%.3f\n", player->pos.x, player->pos.y, player->pos.z);
	sprintf(&aStr[3][0],	"Player.fLength : %.3f\n", player->fLength);
	sprintf(&aStr[4][0],	"Player.MinVtx  : %.3f|%.3f|%.3f\n", player->MinVtx.x, player->MinVtx.y, player->MinVtx.z);
	sprintf(&aStr[5][0],	"Player.MaxVtx  : %.3f|%.3f|%.3f\n", player->MaxVtx.x, player->MaxVtx.y, player->MaxVtx.z);
	sprintf(&aStr[6][0],	"quaternion     : %.3f|%.3f|%.3f|%.3f\n", player->aModel[0].quaternion.x, player->aModel[0].quaternion.y, player->aModel[0].quaternion.z, player->aModel[0].quaternion.w);
	sprintf(&aStr[7][0],	"posV : %.3f|%.3f|%.3f\n", camera->posV.x, camera->posV.y, camera->posV.z);
	sprintf(&aStr[8][0],	"posR : %.3f|%.3f|%.3f\n", camera->posR.x, camera->posR.y, camera->posR.z);
	sprintf(&aStr[9][0],	"posVDest : %.3f|%.3f|%.3f\n", camera->posVDest.x, camera->posVDest.y, camera->posVDest.z);
	sprintf(&aStr[10][0],	"posRDest : %.3f|%.3f|%.3f\n", camera->posRDest.x, camera->posRDest.y, camera->posRDest.z);
	sprintf(&aStr[11][0],	"MODE : %d\n", mode);
	sprintf(&aStr[12][0],	"stickL : %.3f|%.3f|%.3f\n", stickL.x, stickL.y, stickL.z);
	sprintf(&aStr[13][0],	"stickR : %.3f|%.3f|%.3f\n", stickR.x, stickR.y, stickR.z);
	sprintf(&aStr[14][0],	"move : %.3f|%.3f|%.3f\n", player->movevec.x, player->movevec.y, player->movevec.z);

	// �\���̈�̍쐬
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	for (int i = 0; i < DEBUG_NUM; i++)
	{
		// �e�L�X�g�̕`��
		rect = { 0,i * 30,SCREEN_WIDTH,SCREEN_HEIGHT };

		s_pFont->DrawText(NULL, &aStr[i][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 0, 255, 255));

	}
}