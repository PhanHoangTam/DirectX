#include "FatGuardDefaultState.h"
#include "FatGuardAttackState.h"

FatGuardDefaultState::FatGuardDefaultState(OrokuData *orokuData)
{
	this->mOrokuData = orokuData;
	timeDelay = 0;
	this->mOrokuData->fatGuard->allowDefault = true;
	//set lai huong tan cong cua fatguard thanh false
	this->mOrokuData->fatGuard->mSettingLeftAttack = false;
	this->mOrokuData->fatGuard->mSettingRightAttack = false;
	//set lai huong bay cua cay kiem thanh false
	this->mOrokuData->fatGuard->weapon->mSettingLeftItem = false;
	this->mOrokuData->fatGuard->weapon->mSettingRightItem = false;
	this->mOrokuData->fatGuard->SetVx(0);
	this->mOrokuData->fatGuard->SetVy(0);
}

FatGuardDefaultState::~FatGuardDefaultState()
{
}

void FatGuardDefaultState::Update(float dt)
{
	timeDelay += dt;

	if (timeDelay > 0.5f)
	{
		this->mOrokuData->fatGuard->allowDefault = false;
	}
}

void FatGuardDefaultState::OnCollision(Entity *impactor, Entity::SideCollisions side, Entity::CollisionReturn data)
{

}

OrokuState::StateName FatGuardDefaultState::GetState()
{
	return OrokuState::FatGuardDefault;
}