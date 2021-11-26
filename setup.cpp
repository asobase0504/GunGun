//=========================================
// 
// ˆ—‚ÌÝ’è
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "setup.h"

float NormalizeRot(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2;
	}
	if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2;
	}

	return rot;
}
