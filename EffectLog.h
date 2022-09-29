#pragma once
#include "SpriteGameObject.h"

class EffectLog : public SpriteGameObject
{
protected:
	// 중력
	// 속도 (방향)
	// 각속도 (회전)
	// 타이머
	Vector2f velocity;
	float angularVelocity;
	float duration;
	float timer;
	bool active;
public:
	static const Vector2f Gravity;

	EffectLog(Texture& tex, float duration);
	virtual ~EffectLog();

	void Fire(const Vector2f v, float av);
	bool GetActive() const;
	virtual void Init() override;
	virtual void Release() override;
	virtual void Update(float dt) override;
	virtual void Draw(RenderWindow& window) override;
	


	// 속도 (방향), 각속도 (회전)
};

