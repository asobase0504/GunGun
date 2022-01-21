//=========================================
// 
// �Q�[��������
// Author YudaKaito
// 
//=========================================
#include "game.h"
#include "main.h"
#include "input.h"
#include "polygon.h"
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
#include "line.h"
#include "pause.h"

//------------------------------------
// �X�^�e�B�b�N�ϐ�
//------------------------------------
static bool s_bPause;			//�|�[�Y�����ǂ���


//=========================================
// ������
//=========================================
void InitGame(void)
{
	s_bPause = false;
#ifdef _DEBUG
	// ���C���̏���������
	InitLine();
#endif // !_DEBUG

	// �|�[�Y�̏���������
	InitPause();

	// �|���S���̏���������
	InitPolygon();

	// �v���C���[�̏���������
	InitPlayer();

	// �e�̏���������
	InitShadow();

	// ���f���̏���������
	InitModel();

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �r���{�[�h�̏���������
	InitBillboard();

	SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// ���b�V���̏���������
	InitMeshBuild();

	// ���b�V��(�~��)�̏���������
	InitMeshCylinder();

	// ���b�V��(��)�̏���������
	InitMeshSphere();

	// ���b�V��(��)�̏���������
	InitMeshSky();

	// �ǂ̏���������
	InitWall();

	//// �ǂ̐ݒ菈��
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//SetWall(D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	//SetWall(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	//SetWall(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));
}

//=========================================
// �I��
//=========================================
void UninitGame(void)
{
	// �|�[�Y�̏I������
	UninitPause();

	// �|���S���̏I������
	UninitPolygon();

	// �v���C���[�̏I������
	UninitPlayer();

	// ���f���̏I������
	UninitModel();

	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// �e�̏I������
	UninitShadow();

	// �r���{�[�h�̏I������
	UninitBillboard();

	// ���b�V���̏I������
	UninitMeshBuild();

	// ���b�V��(�~��)�̏I������
	UninitMeshCylinder();

	// ���b�V��(��)�̏I������
	UninitMeshSphere();

	// ���b�V��(��)�̏I������
	UninitMeshSky();

	// �ǂ̏I������
	UninitWall();

#ifdef _DEBUG
	// ���C���̍X�V����
	UninitLine();
#endif // !_DEBUG
}

//=========================================
// �X�V
//=========================================
void UpdateGame(void)
{
	if (GetJoypadTrigger(JOYKEY_START))
	{
		s_bPause = !s_bPause;
	}

	if (s_bPause)
	{
		// �|�[�Y
		UpdatePause();
	}
	else
	{
		// ���f���̍X�V
		UpdateModel();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �J�����̍X�V
		UpdateCamera();

		// ���C�g�̍X�V
		UpdateLight();

		// �|���S���̍X�V
		UpdatePolygon();

		// �e�̍X�V
		UpdateShadow();

		// �r���{�[�h�̍X�V
		UpdateBillboard();

		// ���b�V���̍X�V����
		UpdateMeshBuild();

		// ���b�V��(�~��)�̍X�V����
		UpdateMeshCylinder();

		// ���b�V��(��)�̍X�V����
		UpdateMeshSphere();

		// ���b�V��(��)�̍X�V����
		UpdateMeshSky();

		// �ǂ̍X�V
		UpdateWall();
	}

#ifdef _DEBUG
	// ���C���̍X�V����
	UpdateLine();
#endif // !_DEBUG
}

//=========================================
// �`��
//=========================================
void DrawGame(void)
{
	// �J�����̐ݒ菈��
	SetCamera();

	// �|���S���̕`�揈��
	//DrawPolygon();

	// �ǂ̕`�揈��
	DrawWall();

	// ���f���̕`�揈��
	DrawModel();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	// ���b�V���̕`�揈��
	DrawMeshBuild();

	//// ���b�V��(�~��)�̕`�揈��
	//DrawMeshCylinder();

	//// ���b�V��(��)�̕`�揈��
	//DrawMeshSphere();

	//// ���b�V��(��)�̕`�揈��
	//DrawMeshSky();

	// �e�̕`�揈��
	DrawShadow();

	// ���f���̕`�揈��
	//		DrawModel();

	//// �r���{�[�h�̕`�揈��
	//DrawBillboard();

#ifdef _DEBUG
	// ���C���̕`�揈��
	DrawLine();
#endif // !_DEBUG

	if (s_bPause)
	{
		DrawPause();
	}
}
