#pragma once
#include "../MapObject.h"
class AppleWeaponEffectBoss : public MapObject
{
public:
	~AppleWeaponEffectBoss();

	AppleWeaponEffectBoss(D3DXVECTOR3 position);

	virtual const char* FileName();
	virtual int TotalFrame();
	virtual int Row();
	virtual int Column();
	virtual float SecondPerFrame();
};