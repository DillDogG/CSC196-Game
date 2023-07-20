#pragma once
#include "Framework/Actor.h"

class Player : public kiko::Actor {
public:
	Player(float speed, float turnRate, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate}
	{
		m_fireRate = 0.25f;
		m_fireTimer = m_fireRate;
		m_immuneTime = 2.5f;
		m_immuneTimer = m_immuneTime;
		m_health = 3;
	}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;
private:
	float m_speed = 0;
	float m_turnRate = 0;
	int m_health = 0;

	float m_fireRate = 0;
	float m_fireTimer = 0;
	float m_immuneTime = 0;
	float m_immuneTimer = 0;
};