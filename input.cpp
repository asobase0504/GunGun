//=========================================
// 
// �L�[�{�[�h�̏��u
// Author YudaKaito
// 
//=========================================
#include "main.h"
#include "input.h"

// �}�N����`
#define NUM_KEY_MAX	(256)	// �L�[�̍ő吔

// �O���[�o���ϐ�
static LPDIRECTINPUT8 s_pInput = NULL;					// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECTINPUTDEVICE8 s_pDevKeyboard = NULL;		// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
static BYTE s_aKeyState[NUM_KEY_MAX];					// �L�[�{�[�h�̃v���X���
static BYTE s_akeyStateTrigger[NUM_KEY_MAX];			// �L�[�{�[�h�̃g���K�[���
static BYTE s_akeyStateRelese[NUM_KEY_MAX];				// �L�[�{�[�h�̃����[�X���

static XINPUT_STATE s_joyKeyState;						// �W���C�p�b�h�̃v���X���
static XINPUT_STATE s_joykeyStateTrigger;				// �W���C�p�b�h�̃g���K�[���
static XINPUT_STATE s_joykeyStateRelese;				// �W���C�p�b�h�̃����[�X���
static JOYSTICK s_JoyStick;								// �W���C�X�e�B�b�N���
static float s_JoypadLStickAngle;						// L�X�e�B�b�N�̊p�x
static bool s_bUseJoyPad;								// �W���C�p�b�h���g�p���Ă邩

//====================================
// �L�[�{�[�h�̏���������
//====================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// Direct�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&s_pInput, NULL)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(s_pInput->CreateDevice(GUID_SysKeyboard, &s_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(s_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(s_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
	s_pDevKeyboard->Acquire();

	return S_OK;
}

//====================================
// �L�[�{�[�h�̏I������
//====================================
void UninitKeyboard(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (s_pDevKeyboard != NULL)
	{
		s_pDevKeyboard->Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�������
		s_pDevKeyboard->Release();
		s_pDevKeyboard = NULL;
	}

	// DirectInupt�I�u�W�F�N�g�̔j��
	if (s_pInput != NULL)
	{
		s_pInput->Release();
	}
}

//====================================
// �L�[�{�[�h�̍X�V����
//====================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// �L�[�{�[�h�̓��͏��
	int nCntKey;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(s_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			s_akeyStateTrigger[nCntKey] = ~s_aKeyState[nCntKey] & aKeyState[nCntKey];
			s_akeyStateRelese[nCntKey] = s_aKeyState[nCntKey] & ~aKeyState[nCntKey];
			s_aKeyState[nCntKey] = aKeyState[nCntKey];	// �L�[�{�[�h�̃v���X����ۑ�
		}
	}
	else
	{
		s_pDevKeyboard->Acquire();	// �L�[�{�[�h�̃A�N�Z�X�����l��
	}
}

//====================================
// �L�[�{�[�h�̃v���X�����擾
//====================================
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) ? true : false;
}

//====================================
// �L�[�{�[�h�̃g���K�[�����擾
//====================================
bool GetKeyboardTrigger(int nKey)
{
	return (s_akeyStateTrigger[nKey] & 0x80) ? true : false;
}

//====================================
// �L�[�{�[�h�̃����[�X�����擾
//====================================
bool GetKeyboardRelese(int nKey)
{
	return (s_akeyStateRelese[nKey] & 0x80) ? true : false;
}

//====================================
// �W���C�p�b�g�̏���������
//====================================
HRESULT InitJoypad(void)
{
	// �������̃N���A
	memset(&s_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInput�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);

	// �W���C�X�e�B�b�N���̏�����
	s_JoyStick.nTypeJoyStick = 0;
	s_JoyStick.nStickDeadLine = 0;
	s_JoyStick.fStickAngle = 0.0f;
	s_JoyStick.bUse = false;

	// �W���C�X�e�B�b�N�̊p�x��������
	s_JoypadLStickAngle = 0;

	// �g�p�󋵂̏�����
	s_bUseJoyPad = false;
	return S_OK;
}

//====================================
// �W���C�p�b�g�̏I������
//====================================
void UninitJoypad(void)
{
	// XInput�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

//====================================
// �W���C�p�b�g�̍X�V����
//====================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;	// �W���C�p�b�h�̓��͏���

	// �W���C�p�b�h�̏�Ԃ̎擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		s_joykeyStateTrigger.Gamepad.wButtons = ~s_joyKeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons;
		s_joykeyStateRelese.Gamepad.wButtons = s_joyKeyState.Gamepad.wButtons & ~joykeyState.Gamepad.wButtons;
		s_joyKeyState = joykeyState;	// �W���C�p�b�h�̃v���X����ۑ�
		// �g�p�󋵂̍X�V
		s_bUseJoyPad = true;
	}
	else
	{
		// �g�p�󋵂̍X�V
		s_bUseJoyPad = false;
	}

}

//====================================
// �W���C�p�b�g�̃v���X�����擾
//====================================
bool GetJoypadPress(JOYKEY key)
{
	return(s_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//====================================
// �W���C�p�b�g�̃g���K�[�����擾
//====================================
bool GetJoypadTrigger(JOYKEY key)
{
	return(s_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//====================================
// �W���C�p�b�g�̃X�e�B�b�N����
//====================================
D3DXVECTOR3 GetJoypadStick(JOYKEY nKey)
{
	// �ϐ��錾
	D3DXVECTOR3 stick;

	switch (nKey)
	{
	case JOYKEY_L_STICK:
		stick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbLX / 30000.0f, -s_joyKeyState.Gamepad.sThumbLY / 30000.0f, 0.0f);

		s_JoypadLStickAngle = atan2f(s_joyKeyState.Gamepad.sThumbLX / 30000.0f, -s_joyKeyState.Gamepad.sThumbLY / 30000.0f);
		break;

	case JOYKEY_R_STICK:
		stick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbRX / 30000.0f, -s_joyKeyState.Gamepad.sThumbRY / 30000.0f, 0.0f);
		break;
	}

	return stick;
}


//====================================
// �W���C�p�b�g�̊p�x�̎擾
//====================================
float GetJoyStickAngle(void)
{
	return s_JoypadLStickAngle;
}

//====================================
// �W���C�p�b�g�̎g�p���̎擾
//====================================
bool GetUseJoyPad(void)
{
	return s_bUseJoyPad;
}