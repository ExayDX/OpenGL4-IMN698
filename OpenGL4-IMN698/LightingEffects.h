#ifndef _LIGHTING_EFFECTS_H_
#define _LIGHTING_EFFECTS_H_

#include "Flag.h"

enum class LightingEffects
{
	eNone = 0x00,
	eHDR = 1 << 0,
	eBloom = 1 << 1,
	eSSS = 1 << 2,
	eMaxValue = eHDR + eBloom + eSSS
};

template<>
struct enable_bitmask_operators<LightingEffects>{
	static const bool enable = true;
};

#endif // !_LIGHTING_EFFECTS_H_
