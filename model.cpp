//=========================================
// 
// ���f�������֐�
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "model.h"
#include "main.h"
#include "input.h"
#include "camera.h"

//------------------------------------
// �}�N����`
//------------------------------------
#define MODEL_MOVE	(1.0f)

//------------------------------------
// ���f���X�e�[�^�X�񋓌^
//------------------------------------
typedef enum
{
	MODELSTATEL_NONE = 0,
	MODELSTATEL_MOVE,
	MODELSTATEL_MAX
}MODEL_STATE;


//------------------------------------
// ���f���̍\���̒�`
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 rot;				// ����
}Model;

//------------------------------------
// �ÓI�ϐ�
//------------------------------------
static LPD3DXMESH s_pMesh = NULL;		// ���b�V�����ւ̃|�C���^		// ���_�̏W�܂�̂���
static LPD3DXBUFFER s_pBuffMat = NULL;	// �}�e���A�����ւ̃|�C���^	// 1��X�t�@�C���ɕ����̃}�e���A���������Ă���
static DWORD s_nNumMat = 0;				// �}�e���A�����̐�
static D3DXMATRIX s_mtxWorld;			// ���[���h�}�g���b�N�X
static Model s_model;					// ���f���̍\����
static MODEL_STATE s_state;				// ���f���̃X�e�[�^�X


//=========================================
// ������
//=========================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/bee_butt.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&s_pBuffMat,
		NULL,
		&s_nNumMat,
		&s_pMesh);

	s_model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_model.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

}

//=========================================
// �I��
//=========================================
void UninitModel(void)
{
	// ���b�V���̉��
	if(s_pMesh != NULL)
	{
		s_pMesh->Release();
		s_pMesh = NULL;
	}
	// �}�e���A���̉��
	if (s_pBuffMat != NULL)
	{
		s_pBuffMat->Release();
		s_pBuffMat = NULL;
	}
}

//=========================================
// �X�V
//=========================================
void UpdateModel(void)
{
	MoveModel();
}

//=========================================
// �`��
//=========================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;	// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;		// �}�e���A���f�[�^�ւ̃|�C���^

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&s_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, s_model.rot.y, s_model.rot.x, s_model.rot.z);		// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxRot);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, s_model.pos.x, s_model.pos.y, s_model.pos.z);			// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixMultiply(&s_mtxWorld, &s_mtxWorld, &mtxTrans);				// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &s_mtxWorld);;

	// ���݂̃}�e���A����ێ�
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)s_pBuffMat->GetBufferPointer();
	for (int i = 0; i < (int)s_nNumMat; i++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		// ���f���p�[�c�̕`��
		s_pMesh->DrawSubset(i);
	}
	// �ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=========================================
// �ړ�
//=========================================
void MoveModel()
{
	D3DXVECTOR3 CameraRot = GetRotCamera();		//�J�����̊p�x���擾
	float fAngle = CameraRot.y;	// �ŏI�p�x
	Model* pModel = &(s_model);
	float fAngleK = 0.0f;
	int KeyCnt = 0;

	// ���f���̈ړ�
	if (GetKeyboardPress(DIK_UP))
	{
		KeyCnt++;
	}
	if (GetKeyboardPress(DIK_LEFT))
	{
		KeyCnt++;
		fAngleK += -0.5f;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		KeyCnt++;
		if (KeyCnt > 1)
		{
			fAngleK -= 1.0f;
		}
		else
		{
			fAngleK += 1.0f;
		}
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		KeyCnt++;
		fAngleK += 0.5f;
	}

	if (KeyCnt > 0)
	{
		s_state = MODELSTATEL_MOVE;
		fAngle = D3DX_PI * fAngleK / KeyCnt + CameraRot.y;
		pModel->pos.x += sinf(fAngle) * MODEL_MOVE;
		pModel->pos.z += cosf(fAngle) * MODEL_MOVE;
		pModel->rot.y = fAngle;
	}
	else
	{
		s_state = MODELSTATEL_NONE;
	}

	//// ���f���̈ړ�
	//if (GetKeyboardPress(DIK_UP) && !(GetKeyboardPress(DIK_DOWN)))
	//{
	//	s_state = MODELSTATEL_MOVE;
	//	if (GetKeyboardPress(DIK_LEFT))
	//	{
	//		fAngle = D3DX_PI * -0.25f + CameraRot.y;
	//	}
	//	else if (GetKeyboardPress(DIK_RIGHT))
	//	{
	//		fAngle = D3DX_PI * 0.25f + CameraRot.y;
	//	}
	//	else
	//	{
	//		fAngle = CameraRot.y;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_LEFT) && !(GetKeyboardPress(DIK_RIGHT)))
	//{
	//	s_state = MODELSTATEL_MOVE;
	//	if (GetKeyboardPress(DIK_DOWN))
	//	{
	//		fAngle = D3DX_PI * -0.75f + CameraRot.y;
	//	}
	//	else
	//	{
	//		fAngle = D3DX_PI * -0.5f + CameraRot.y;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_DOWN) && !(GetKeyboardPress(DIK_UP)))
	//{
	//	s_state = MODELSTATEL_MOVE;
	//	if (GetKeyboardPress(DIK_RIGHT))
	//	{
	//		fAngle = D3DX_PI * 0.75f + CameraRot.y;
	//	}
	//	else
	//	{
	//		fAngle = D3DX_PI + CameraRot.y;
	//	}
	//}
	//else if (GetKeyboardPress(DIK_RIGHT) && !(GetKeyboardPress(DIK_LEFT)))
	//{
	//	s_state = MODELSTATEL_MOVE;
	//	fAngle = D3DX_PI * 0.5f + CameraRot.y;
	//}
	//else
	//{
	//	s_state = MODELSTATEL_NONE;
	//}
}
