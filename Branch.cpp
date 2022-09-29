#include "Branch.h"

Branch::Branch(Texture& tex, SpriteGameObject* tree)
	:SpriteGameObject(tex),tree(tree)
{
}

void Branch::Init()
{
	SetSide((Sides)Utils::Range(0, 2));
	sprite.setPosition(tree->GetPosition().x, sprite.getPosition().y);
	sprite.setOrigin(tree->GetSize().x * 0.5f * -1, 0);
}

void Branch::SetSide(Sides side)
{
	switch (side)
	{
	case Sides::Left:
		sprite.setScale(-1,1 );
		break;
	case Sides::Right:
		sprite.setScale(1,1 );
		break;
	}
	this->side = side;	
}

Sides Branch::GetSide()
{
	return side;
}

