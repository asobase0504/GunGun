//=========================================
//
// ���U���g����
// Author:Yuda Kaito
//
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "input.h"
#include "key_config.h"

//=========================================
// �I���̃L�[
// GetJoypadTrigger(JOYKEY_A)
// GetJoypadTrigger(JOYKEY_B)
// GetKeyboardTrigger(DIK_RETURN)
//=========================================
bool DecisionKey(void)
{
	return GetJoypadTrigger(JOYKEY_A) || GetJoypadTrigger(JOYKEY_B) || GetKeyboardTrigger(DIK_RETURN);
}
