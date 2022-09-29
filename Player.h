#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include "SpriteGameObject.h"
#include "EffectLog.h"
#include "Branch.h"
class Player : public SpriteGameObject
{
protected:
	Texture texLog;
	list<EffectLog*> unuseLogs;
	list<EffectLog*> useLogs;
	Texture& texPlayer;
	Texture& texRip;
	SoundBuffer chopsoundbuffer;
	Sound chopsound;
	SoundBuffer deathsoundbuffer;
	Sound deathsound;
	Sprite axe;
	Vector2f axePos;

	int& score;

	bool isAlive;
	bool isChopping;

	Sides pos;
	Vector2f centerPos;
	vector<Vector2f> originalPos;
	
public:
	Player(Texture& player, Texture& rip, Texture& axe, int& score);

	void Set(Vector2f center);
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	virtual void SetPosition(Vector2f pos) override;
	virtual void SetFlipX(bool flip) override;

	void ShowLogEffect();

	bool GetAlive() const;
	Sides GetPos() const;

	void Die();
	void Chop(Sides side);
};

