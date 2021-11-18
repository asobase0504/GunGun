//=========================================
// 
// �L�[�{�[�h�̃w�b�_�[�t�@�C��
// Author YudaKaito
// 
//=========================================
#ifndef  _INPUT_H_
#define	_INPUT_H_

//------------------------------------
// �L�[�̎�ނ̗񋓌^
//------------------------------------
typedef enum
{
	JOYKEY_UP = 0,		// �\���L�[�i��j
	JOYKEY_DOWN,		// �\���L�[�i���j
	JOYKEY_LEFT,		// �\���L�[�i���j
	JOYKEY_RIGHT,		// �\���L�[�i�E�j
	JOYKEY_START,		// START�{�^��
	JOYKEY_BACK,		// BACK�{�^��
	JOYKEY_PUSHLSTICK,	// ���X�e�B�b�N��������
	JOYKEY_PUSHRSTICK,	// �E�X�e�B�b�N��������
	JOYKEY_L_B,			// LB�L�[
	JOYKEY_R_B,			// RB�L�[
	JOYKEY_NONE0,		// ���蓖�ăL�[�Ȃ�
	JOYKEY_NONE1,		// ���蓖�ăL�[�Ȃ�
	JOYKEY_A,			// A�{�^��
	JOYKEY_B,			// B�{�^��
	JOYKEY_X,			// X�{�^��
	JOYKEY_Y,			// Y�{�^��
	JOYKEY_L_STICK,		// L�X�e�B�b�N
	JOYKEY_R_STICK,		// R�X�e�B�b�N
	MAX_JOYKEY,			// �ő吔
}JOYKEY;

//------------------------------------
// �W���C�X�e�B�b�N�̍\����
//------------------------------------
typedef struct
{
	int nTypeJoyStick;	// �X�e�B�b�N�̃^�C�v
	int nStickDeadLine;	// �X�e�B�b�N�̔���̈�
	float fStickAngle;	// �X�e�B�b�N�̓|���ꂽ�p�x
	bool bUse;			// �X�e�B�b�N���g���Ă��邩�̔���
}JOYSTICK;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
// �L�[�{�[�h�֐�
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	// �L�[�{�[�h�̏���������
void UninitKeyboard(void);								// �L�[�{�[�h�̏I������
void UpdateKeyboard(void);								// �L�[�{�[�h�̍X�V����
bool GetKeyboardPress(int nKey);						// �L�[�{�[�h��Press���擾
bool GetKeyboardTrigger(int nKey);						// �L�[�{�[�h��Trigger���擾
bool GetKeyboardRelease(int nKey);						// �L�[�{�[�h��Release���擾

// �p�b�h�֐�
HRESULT InitJoypad(void);								// �W���C�p�b�h�̏���������
void UninitJoypad(void);								// �W���C�p�b�h�̏I������
void UpdateJoypad(void);								// �W���C�p�b�h�̍X�V����
bool GetJoypadPress(JOYKEY key);						// �W���C�p�b�h��Press���擾
bool GetJoypadTrigger(JOYKEY key);						// �W���C�p�b�h��Trigger���擾
bool GetJoypadRelease(JOYKEY key);						// �W���C�p�b�h��Release���擾
D3DXVECTOR3 GetJoypadStick(JOYKEY nKey);				// �W���C�p�b�h�̃X�e�B�b�N���擾
float GetJoyStickAngle(void);							// �W���C�p�b�h�̊p�x���擾
bool IsUseJoyPad(void);									// �W���C�p�b�h���g�p���Ă��邩�ǂ���

#endif //  _INPUT_H_
