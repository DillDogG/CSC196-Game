#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"

void Player::Update(float dt) {
	Actor::Update(dt);
	float rotate = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnRate * kiko::g_time.GetDeltaTime();

	float thrust = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	m_transform.position += forward * m_speed * thrust * kiko::g_time.GetDeltaTime();
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());
	if (m_fireTimer < 0) {
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) /*&& !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)*/) {
			kiko::Transform transform { m_transform.position, m_transform.rotation, 1 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, m_transform, m_model);
			weapon->m_transform.scale /= 4;
			weapon->m_tag = "pWeapon";
			m_scene->Add(std::move(weapon));
			m_fireTimer = m_fireRate;
		}
	}
	else {
		m_fireTimer -= dt;
	}
	m_immuneTimer -= dt;
	/*if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
		kiko::Transform transform { m_transform.position, m_transform.rotation, 1 };
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>( 400.0f, m_transform, m_model );
		weapon->m_tag = "pWeapon";
		m_scene->Add(std::move(weapon));
	} */
	if (m_health < 1) {
		m_game->SetLives(m_game->GetLives() - 1);
		dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDead);
		m_destroyed = true;

	}
}

void Player::OnCollision(Actor* other) {
	if (m_immuneTimer <= 0) {
		if (other->m_tag == "eWeapon" || other->m_tag == "Enemy") {
			m_health--;
			m_immuneTimer = m_immuneTime;
		}
	}
}
