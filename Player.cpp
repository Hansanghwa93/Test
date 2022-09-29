#include "Player.h"
#include "InputMgr.h"

Player::Player(Texture& player, Texture& rip, Texture& axe, int& score)
	:SpriteGameObject(player),texPlayer(player),texRip(rip), originalPos(2), isAlive(true), isChopping(true), score(score)
{	
	this->axe.setTexture(axe);
	Utils::SetOrigin(this->axe, Origins::MR);
	Vector2f size = GetSize();
	axePos.x = -size.x * 0.5f + 50;
	axePos.y = -size.y * 0.5f + 30;

	pos = Sides::Right;

	SetOrigin(Origins::BC);
	texLog.loadFromFile("graphics/log.png");

	chopsoundbuffer.loadFromFile("sound/chop.wav");
	chopsound.setBuffer(chopsoundbuffer);
	deathsoundbuffer.loadFromFile("sound/death.wav");
	deathsound.setBuffer(deathsoundbuffer);

	for (int i = 0; i < 21; ++i)
	{
		auto log = new EffectLog(texLog, 1.f);
		unuseLogs.push_back(log);		
	}
}

void Player::Set(Vector2f center)
{
	centerPos = center;
}

void Player::Init()
{
	
	sprite.setTexture(texPlayer, true);
	Utils::SetOrigin(sprite, Origins::BC);
	SetFlipX(pos == Sides::Left);

	Vector2f size = GetSize();

	originalPos[(int)Sides::Left].x = centerPos.x - size.x * 2.f;
	originalPos[(int)Sides::Left].y = centerPos.y;
	originalPos[(int)Sides::Right].x = centerPos.x + size.x * 2.f;
	originalPos[(int)Sides::Right].y = centerPos.y;		
	SetPosition(originalPos[(int)pos]);


	isAlive = true;
	isChopping = false;
}

void Player::Release()
{
	for (auto log : unuseLogs)
	{
		delete log;
	}
	unuseLogs.clear();

	for (auto log : useLogs)
	{
		delete log;
	}
	useLogs.clear();
}

void Player::Update(float dt)
{
	SpriteGameObject::Update(dt);
	auto it = useLogs.begin();
	while (it != useLogs.end())
	{
		(*it)->Update(dt);
		if (!(*it)->GetActive())
		{
			unuseLogs.push_back(*it);
			it = useLogs.erase(it);
		}
		else
		{
			++it;
		}
	}


	if (InputMgr::GetKeyDown(Keyboard::Key::Space))
	{
		if (isAlive)
			Die();		
		return;
	}

	if (!isAlive)
		return;

	//입력 처리	
	if (!InputMgr::GetKey(Keyboard::Key::Right) && InputMgr::GetKeyDown(Keyboard::Key::Left))
	{
		Chop(Sides::Left);
	}

	if (!InputMgr::GetKey(Keyboard::Key::Left) && InputMgr::GetKeyDown(Keyboard::Key::Right))
	{
		Chop(Sides::Right);
	}	
	isChopping = (InputMgr::GetKey(Keyboard::Left) || InputMgr::GetKey(Keyboard::Right));
}

void Player::Draw(RenderWindow& window)
{	
	SpriteGameObject::Draw(window);
	if(isChopping)
		window.draw(axe);

	for (auto log : useLogs)
	{

		log->Draw(window);
	}
	
}

void Player::SetPosition(Vector2f pos)
{
	SpriteGameObject::SetPosition(pos);
	Vector2f axePos2 = axePos;
	if (axe.getScale().x < 0)
	{
		axePos2.x *= -1;
	}
	axe.setPosition(pos + axePos2);
}

void Player::SetFlipX(bool flip)
{
	SpriteGameObject::SetFlipX(flip);
	Vector2f scale = axe.getScale();
	scale.x = flip ? -abs(scale.x) : abs(scale.x);
	axe.setScale(scale);
}

void Player::ShowLogEffect()
{
	if (unuseLogs.empty())
		return;

	auto log = unuseLogs.front();
	unuseLogs.pop_front();
	useLogs.push_back(log);

	Vector2f force;
	force.x = pos == Sides::Left ? 1500 : -1500;
	force.y = -1500;
	float aForce = pos == Sides::Left ? 360 * 5 : -360 * 5;

	Vector2f pos = centerPos;
	pos.y = axe.getPosition().y;

	log->SetPosition(pos);
	log->Fire(force, aForce);
}

bool Player::GetAlive() const
{
	return isAlive;
}

Sides Player::GetPos() const
{
	return pos;
}

void Player::Die()
{
	
	deathsound.setVolume(50);
	deathsound.play();

	isAlive = false;
	isChopping = false;

	sprite.setTexture(texRip, true);
	SetFlipX(false);
	Utils::SetOrigin(sprite, Origins::BC);
}

void Player::Chop(Sides side)
{	
	chopsound.play();

	pos = side;
	SetFlipX(pos == Sides::Left);
	SetPosition(originalPos[(int)pos]);

	isChopping = true;
	ShowLogEffect();
	score += 10;
}
