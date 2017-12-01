#include "SwordWeapon.h"

SwordWeapon::SwordWeapon(D3DXVECTOR3 position)
{
    init(position);
}

SwordWeapon::~SwordWeapon()
{
}


const char * SwordWeapon::FileName()
{
    return "Resources/Orokus/Guards/SwordWeapon.png";
}

int SwordWeapon::TotalFrame()
{
    return 7;
}

int SwordWeapon::Row()
{
    return 1;
}

int SwordWeapon::Column()
{
    return 7;
}

float SwordWeapon::SecondPerFrame()
{
    return 0.05f;
}

void SwordWeapon::OnCollision(Entity *impactor, Entity::CollisionReturn data, Entity::SideCollisions side)
{
	if (impactor->Tag == Entity::EntityTypes::Aladdin)
	{
		this->collisionWithPlayer = true;
	}
	else if (impactor->Tag != Entity::EntityTypes::Guard && impactor->Tag != Entity::EntityTypes::AppleItem)
	{
		this->weaponCollided = true;
	}
}