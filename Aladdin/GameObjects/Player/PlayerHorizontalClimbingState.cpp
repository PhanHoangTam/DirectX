#include "PlayerHorizontalClimbingState.h"
#include "PlayerHorizontalClimbingDefaultState.h"
#include "PlayerFallingState.h"
#include "../MapObjects/Weapons/AppleWeapon.h"
#include "../../GameComponents/GameCollision.h"
#include "../../GameDefines/GameDefine.h"

PlayerHorizontalClimbingState::PlayerHorizontalClimbingState(PlayerData *playerData)
{
	this->mPlayerData = playerData;
	this->mPlayerData->player->SetVx(0);
	this->mPlayerData->player->SetVy(0);
}

PlayerHorizontalClimbingState::~PlayerHorizontalClimbingState()
{

}

void PlayerHorizontalClimbingState::HandleKeyboard(std::map<int, bool> keys)
{
	if (keys[VK_LEFT])
	{
		if (mPlayerData->player->allowMoveLeft)
		{
			mPlayerData->player->SetReverse(false);
			mPlayerData->player->allowMoveRight = true;

			if (this->mPlayerData->player->GetVx() > -Define::PLAYER_MAX_HORIZONTAL_CLIMBING_SPEED)
			{
				this->mPlayerData->player->AddVx(-Define::PLAYER_CLIM_SPPED);

				if (this->mPlayerData->player->GetVx() <= -Define::PLAYER_MAX_HORIZONTAL_CLIMBING_SPEED)
				{
					this->mPlayerData->player->SetVx(-Define::PLAYER_MAX_HORIZONTAL_CLIMBING_SPEED);
				}
			}
		}
	}
	else if (keys[VK_RIGHT])
	{
		if (mPlayerData->player->allowMoveRight)
		{
			mPlayerData->player->SetReverse(true);
			mPlayerData->player->allowMoveLeft = true;

			if (this->mPlayerData->player->GetVx() < Define::PLAYER_MAX_HORIZONTAL_CLIMBING_SPEED)
			{
				this->mPlayerData->player->AddVx(Define::PLAYER_CLIM_SPPED);

				if (this->mPlayerData->player->GetVx() > Define::PLAYER_MAX_HORIZONTAL_CLIMBING_SPEED)
				{
					this->mPlayerData->player->SetVx(Define::PLAYER_MAX_HORIZONTAL_CLIMBING_SPEED);
				}
			}
		}
	}
	else
	{
		this->mPlayerData->player->SetState(new PlayerHorizontalClimbingDefaultState(this->mPlayerData));
	}
}

void PlayerHorizontalClimbingState::OnCollision(Entity *impactor, Entity::SideCollisions side, Entity::CollisionReturn data)
{
	if (impactor->Tag == Entity::EntityTypes::Item)
	{
		if (impactor->Id == Entity::EntityId::Revitalization_Default)
			return;
		this->mPlayerData->player->allowItemEffect = true;
		this->mPlayerData->player->collisionItem = true;
		this->mPlayerData->player->mOriginPositionItem = impactor->GetPosition();
		if (impactor->Id == Entity::EntityId::AppleItem)
		{
			this->mPlayerData->player->apple = new AppleWeapon();
			this->mPlayerData->player->mListApplePlayer.push_back(this->mPlayerData->player->apple);
		}
	}
	else if ((impactor->Tag == Entity::EntityTypes::Sword || impactor->Tag == Entity::EntityTypes::Pot) &&
		!this->mPlayerData->player->allowImunity)
	{
		this->mPlayerData->player->bloodOfEntity--;
	}
	else if (impactor->Tag == Entity::EntityTypes::VerticalRopeControl)
	{

	}
	else
	{
		switch (side)
		{
		case Entity::Left: case Entity::TopLeft:
			//va cham phia ben trai player
			if (this->mPlayerData->player->getMoveDirection() == Player::MoveToLeft)
			{
				this->mPlayerData->player->allowMoveLeft = false;
				//day Player ra phia ben phai de cho player khong bi xuyen qua object
				this->mPlayerData->player->AddPosition(data.RegionCollision.right - data.RegionCollision.left, 0);
			}
			break;

		case Entity::Right: case Entity::TopRight:
			//va cham phia ben phai player
			if (this->mPlayerData->player->getMoveDirection() == Player::MoveToRight)
			{
				this->mPlayerData->player->allowMoveRight = false;
				this->mPlayerData->player->AddPosition(-(data.RegionCollision.right - data.RegionCollision.left), 0);
			}
			break;

		default:
			break;
		}
	}
}

PlayerState::StateName PlayerHorizontalClimbingState::GetState()
{
	return PlayerState::HorizontalClimbing;
}