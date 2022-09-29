#pragma once
#include <SFML/Graphics.hpp>
#include "Utils.h"

using namespace sf;
using namespace std;

class SpriteGameObject
{
protected:
	Sprite sprite;

	SpriteGameObject(const SpriteGameObject& ref);
	SpriteGameObject& operator=(const SpriteGameObject& ref);
public:
	SpriteGameObject(Texture& tex, Vector2f pos = Vector2f());
	virtual ~SpriteGameObject();

	virtual void Init();
	virtual void Release();
	virtual void Update(float dt);
	virtual void Draw(RenderWindow& window);
	void SetOrigin(Origins origin);
	Vector2f GetSize() const;

	Vector2f GetPosition();
	virtual void SetPosition(Vector2f pos);
	virtual void SetFlipX(bool flip);

	void Translate(Vector2f delta);
};

