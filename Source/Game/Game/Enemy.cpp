#include "Enemy.h"
#include "Framework/Scene.h"
#include "Player.h"
#include "SpaceGame.h"
#include "Weapon.h"
#include "Renderer/Renderer.h"

void Enemy::Update(float dt) {
	Actor::Update(dt);

	Player* player = m_scene->GetActor<Player>();
	if (player) {
		kiko::Vector2 direction = player->m_transform.position - m_transform.position;
		m_transform.rotation = direction.Angle() + kiko::HalfPi;
	}

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	m_transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());

	if (m_fireTimer < 0) {
		kiko::Transform transform { m_transform.position, m_transform.rotation, 1 };
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, m_transform, m_model);
		weapon->m_transform.scale /= 2;
		weapon->m_tag = "eWeapon";
		m_scene->Add(std::move(weapon));
		m_fireTimer = m_fireRate;
	}
	else {
		m_fireTimer -= dt;
	}
}

void Enemy::OnCollision(Actor* other) {
	if (other->m_tag == "pWeapon") {
		m_game->AddPoints(100);
		m_destroyed = true;
	}
}
