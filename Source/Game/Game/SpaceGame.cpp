#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"

#include "Framework/Scene.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/Text.h"
#include "Renderer/ModelManager.h"

bool SpaceGame::Initialize() {
	//font and text initialize
	m_font = std::make_shared<kiko::Font>("Freshman.ttf", 24);
	m_scoreText = std::make_unique<kiko::Text>(m_font);
	m_scoreText->Create(kiko::g_renderer, "SCORE 0000", kiko::Color{ 1, 0, 1, 1 });
	m_titleText = std::make_unique<kiko::Text>(m_font);
	m_titleText->Create(kiko::g_renderer, "KIKO ASTROIDS", kiko::Color{ 1, 0, 1, 1 });
	m_livesText = std::make_unique<kiko::Text>(m_font);
	m_livesText->Create(kiko::g_renderer, "LIVES: 03", kiko::Color{ 1, 0, 1, 1 });
	m_healthText = std::make_unique<kiko::Text>(m_font);
	m_healthText->Create(kiko::g_renderer, "HEALTH: 2", kiko::Color{ 1, 0, 1, 1 });


	//initialize audio
	kiko::g_audioSystem.AddAudio("thrusters", "Thrusters.wav");
	kiko::g_audioSystem.AddAudio("explosion", "Explosion.wav");
	kiko::g_audioSystem.AddAudio("laser", "Laser_Shoot.wav");

	//creates window
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	//makes
	m_scene = std::make_unique<kiko::Scene>();


	return true;
}

void SpaceGame::Shutdown() {
}

void SpaceGame::Update(float dt) {
	switch (m_state) {
	case eState::Title:
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) {
			m_state = eState::StartGame;
		}
		break;
	case eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		break;
	case eState::StartLevel:
		m_scene->RemoveAll(); {
			std::unique_ptr<Player> player = std::make_unique<Player>(200.0f, kiko::Pi, kiko::Transform{ {400, 300}, 1.2f, 2 }, kiko::g_modelManager.Get("ship.txt"));
			player->m_tag = "Player";
			player->m_game = this;
			m_scene->Add(std::move(player));
			m_state = eState::Game;
		}
		break;
	case eState::Game:
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime) {
			m_spawnTimer = 0;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(150.0f, kiko::Pi, kiko::Transform{{ (float)kiko::random(800), (float)kiko::random(600) }, kiko::randomf(3), 1.5f }, kiko::g_modelManager.Get("ship.txt"));
			enemy->m_tag = "Enemy";
			enemy->m_game = this;
			m_scene->Add(std::move(enemy));
		}
		
		break;
	case eState::PlayerDead:
		if (m_lives == 0) m_state = eState::GameOver;
		else m_state = eState::StartLevel;
		break;
	case eState::GameOver:
		break;
	}
	m_scoreText->Create(kiko::g_renderer, "SCORE: " + std::to_string(m_score), {1, 1, 1, 1});
	m_livesText->Create(kiko::g_renderer, "LIVES: " + std::to_string(m_lives), {1, 1, 1, 1});
	//m_healthText->Create(kiko::g_renderer, "HEALTH: " + std::to_string(m_scene->GetActor<Player>()->GetHealth()), {1, 1, 1, 1});
	m_scene->Update(dt);
}

void SpaceGame::Draw(kiko::Renderer& renderer) {
	if (m_state == eState::Title) m_titleText->Draw(renderer, 400, 300);
	m_scoreText->Draw(renderer, 40, 20);
	m_livesText->Draw(renderer, 40, 50);
	m_healthText->Draw(renderer, 40, 80);
	m_scene->Draw(renderer);
}
