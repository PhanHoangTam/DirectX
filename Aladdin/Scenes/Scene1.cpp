#include "Scene1.h"
#include "../GameDefines/GameDefine.h"

Scene1::Scene1()
{
    LoadContent();
}

void Scene1::LoadContent()
{
    //set mau backcolor cho scene o day la mau xanh
    mBackColor = 0x54acd2;

    mMap = new GameMap("Resources/Scene_1.tmx");

    mCamera = new Camera(GameGlobal::GetWidth(), GameGlobal::GetHeight());
    mCamera->SetPosition(GameGlobal::GetWidth() / 2, 
                         mMap->GetHeight() - mCamera->GetHeight());

    mMap->SetCamera(mCamera);

    mPlayer = new Player();
    mPlayer->SetPosition(GameGlobal::GetWidth() / 4, GameGlobal::GetHeight() / 2);
    mPlayer->SetCamera(mCamera);

	mMap->SetPlayer(mPlayer);
}

void Scene1::Update(float dt)
{
    checkCollision();

    mMap->Update(dt);

    mPlayer->HandleKeyboard(keys);

    mPlayer->Update(dt);

    CheckCameraAndWorldMap();
}

void Scene1::Draw()
{
    mMap->Draw();

    mPlayer->Draw();
}

void Scene1::OnKeyDown(int keyCode)
{
    keys[keyCode] = true;

    mPlayer->OnKeyPressed(keyCode);
}

void Scene1::OnKeyUp(int keyCode)
{
    keys[keyCode] = false;

    mPlayer->OnKeyUp(keyCode);
}

void Scene1::OnMouseDown(float x, float y)
{

}

void Scene1::CheckCameraAndWorldMap()
{
    mCamera->SetPosition(mPlayer->GetPosition());

    if (mCamera->GetBound().left < 0)
    {
        //vi position cua camera ma chinh giua camera
        //luc nay o vi tri goc ben trai cua the gioi thuc
        mCamera->SetPosition(mCamera->GetWidth() / 2, mCamera->GetPosition().y);
    }

    if (mCamera->GetBound().right > mMap->GetWidth())
    {
        //luc nay cham goc ben phai cua the gioi thuc
        mCamera->SetPosition(mMap->GetWidth() - mCamera->GetWidth() / 2, 
                                mCamera->GetPosition().y);
    }

    if (mCamera->GetBound().top < 0)
    {
        //luc nay cham goc tren the gioi thuc
        mCamera->SetPosition(mCamera->GetPosition().x, mCamera->GetHeight() / 2);
    }

    if (mCamera->GetBound().bottom > mMap->GetHeight())
    {
        //luc nay cham day cua the gioi thuc
        mCamera->SetPosition(mCamera->GetPosition().x, 
                                mMap->GetHeight() - mCamera->GetHeight() / 2);
    }
}

void Scene1::checkCollision()
{
    /*su dung de kiem tra xem khi nao mario khong dung tren 1 object hoac
    dung qua sat mep trai hoac phai cua object do thi se chuyen state la falling*/ 
    int widthBottomPlayer = 0;
	int WidthBottomStrongGuard = 0;

    vector<Entity*> listCollisionPlayer;
	vector<Entity*> listCollisionGuards;
	vector<Entity*> listCollisionApple;
#pragma region --PLAYER--
    mMap->GetQuadTree()->getEntitiesCollideAble(listCollisionPlayer, mPlayer);

    for (size_t i = 0; i < listCollisionPlayer.size(); i++)
    {
        Entity::CollisionReturn r = GameCollision::RecteAndRect(mPlayer->GetBound(), 
																listCollisionPlayer.at(i)->GetBound());

        if (r.IsCollided)
        {
            //lay phia va cham cua Entity so voi Player
            Entity::SideCollisions sidePlayer = GameCollision::getSideCollision(mPlayer, r);

            //lay phia va cham cua Player so voi Entity
            Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollisionPlayer.at(i), r);

            //goi den ham xu ly collision cua Player va Entity
            mPlayer->OnCollision(listCollisionPlayer.at(i), r, sidePlayer);
            listCollisionPlayer.at(i)->OnCollision(mPlayer, r, sideImpactor);

            //kiem tra neu va cham voi phia duoi cua Player 
            if (sidePlayer == Entity::Bottom || sidePlayer == Entity::BottomLeft 
                || sidePlayer == Entity::BottomRight)
            {
                //kiem cha do dai ma mario tiep xuc phia duoi day
                int bot = r.RegionCollision.right - r.RegionCollision.left;

                if (bot > widthBottomPlayer)
                    widthBottomPlayer = bot;
            }

			if (mPlayer->collisionApple)
			{
				Item* item = mMap->GetItem(mMap->GetListItem(), (Item*)listCollisionPlayer.at(i));
				mMap->SetListItem(mMap->RemoveItem(mMap->GetListItem(), item)); //setlistItem lai sau khi xoa Item khoi list
				mMap->GetQuadTree()->removeEntity(listCollisionPlayer.at(i)); //clear Item khoi QuadTree
				mPlayer->AddListApple(item); //them apple sau khi an vao listapple cua player de nem apple
				mPlayer->collisionApple = false;
				break;
			}
        }
    }

    //Neu mario dung ngoai mep thi luc nay cho mario rot xuong duoi dat    
    if (widthBottomPlayer < Define::PLAYER_BOTTOM_RANGE_FALLING)
    {
        mPlayer->OnNoCollisionWithBottom();
    }
#pragma endregion

#pragma region --APPLE--
	for (auto child : mPlayer->GetListAppleFly())
	{
		if (child != nullptr)
		{
			listCollisionApple.clear();
			mMap->GetQuadTree()->getEntitiesCollideAble(listCollisionApple, child);

			for (size_t i = 0; i < listCollisionApple.size(); i++)
			{
				Entity::CollisionReturn r = GameCollision::RecteAndRect(child->GetBound(),
					listCollisionApple.at(i)->GetBound());

				if (r.IsCollided)
				{
					//lay phia va cham cua Entity so voi StrongGuard
					Entity::SideCollisions sideApple = GameCollision::getSideCollision(child, r);

					//lay phia va cham cua StrongGuard so voi Entity
					Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollisionApple.at(i), r);

					//goi den ham xu ly collision cua StrongGuard va Entity
					child->OnCollision(listCollisionApple.at(i), r, sideApple);
					listCollisionApple.at(i)->OnCollision(child, r, sideImpactor);
				}

				if (child->collisionWithOroku)
				{
					Oroku *oroku = mMap->GetOroku(mMap->GetListOroku(), (Oroku*)listCollisionApple.at(i));
					mMap->SetListOroku(mMap->RemoveOroku(mMap->GetListOroku(), oroku));//xoa oroku khoi listoroku trong map
					mMap->GetQuadTree()->removeEntity(listCollisionApple.at(i)); //xoa oroku ra khoi quadtree
					if (oroku->sword != nullptr)
					{
						mMap->GetQuadTree()->removeEntity(oroku->sword); //xoa sword khoi quadtree
						delete oroku->sword;
						oroku->sword = nullptr;
					}
					delete oroku;
					oroku = nullptr;
					break;
				}
			}
		}	
	}
#pragma endregion

#pragma region --OROKU--
	for (auto child : mMap->GetListOroku())
	{
		listCollisionGuards.clear();
		mMap->GetQuadTree()->getEntitiesCollideAble(listCollisionGuards, child);

		for (size_t i = 0; i < listCollisionGuards.size(); i++)
		{
			Entity::CollisionReturn r = GameCollision::RecteAndRect(child->GetBound(),
																	listCollisionGuards.at(i)->GetBound());

			if (r.IsCollided)
			{
				//lay phia va cham cua Entity so voi StrongGuard
				Entity::SideCollisions sideGuard = GameCollision::getSideCollision(child, r);

				//lay phia va cham cua StrongGuard so voi Entity
				Entity::SideCollisions sideImpactor = GameCollision::getSideCollision(listCollisionGuards.at(i), r);

				//goi den ham xu ly collision cua StrongGuard va Entity
				child->OnCollision(listCollisionGuards.at(i), r, sideGuard);
				listCollisionGuards.at(i)->OnCollision(child, r, sideImpactor);
			}
		}
	}
#pragma endregion



}