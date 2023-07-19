#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model.h"
#include "../../Input/InputSystem.h"
#include "Framework/Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "../../Audio/AudioSystem.h"
#include "Framework/Scene.h"
#include <iostream>
#include <thread>
//#include <memory>

using namespace std;

class Star {
public:
	Star(kiko::vec2 pos, kiko::vec2 vel) : m_pos{ pos }, m_vel{ vel } {}
	void Update() {
		m_pos += m_vel * kiko::g_time.GetDeltaTime();
	}
	void Wrap(int w, int h) {
		if (m_pos.x < 0) {
			m_pos.x = w;
		}
		if (m_pos.x > w) {
			m_pos.x = 0;
		}
		if (m_pos.y < 0) {
			m_pos.y = h;
		}
		if (m_pos.y > h) {
			m_pos.y = 0;
		}
	}
public:
	kiko::vec2 m_pos;
	kiko::vec2 m_vel;
};

int main(int argc, char* argv[]) {
	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);
	kiko::g_inputSystem.Initialize();
	kiko::g_audioSystem.Initialize();
	kiko::g_audioSystem.AddAudio("thrusters", "Thrusters.wav");
	kiko::g_audioSystem.AddAudio("explosion", "Explosion.wav");
	kiko::g_audioSystem.AddAudio("laser", "Laser_Shoot.wav");

	//std::vector<kiko::vec2> points{ { -10, 5 }, { 10, 5 }, { 0, -5 }, { -10, 5 } };
	kiko::Model model;
	model.Load("ship.txt");

	/* kiko::vec2 v{ 5, 5 };
	v.Normalize(); */

	vector<Star> stars;
	for (int i = 0; i < 1000; i++) {
		kiko::vec2 pos(kiko::vec2(kiko::random(kiko::g_renderer.GetWidth()), kiko::random(kiko::g_renderer.GetHeight())));
		kiko::vec2 vel(kiko::vec2(kiko::random(100, 200), kiko::random(2)));
		stars.push_back(Star(pos, vel));
	}


	kiko::Transform transform{ {400, 300}, 1.2f, 2 };

	kiko::vec2 position{ 400, 300 };
	float speed = 200;
	constexpr float turnRate = kiko::DegreesToRadians(180);

	kiko::Scene scene;
	std::unique_ptr<Player> player = std::make_unique<Player>(200.0f, kiko::Pi, transform, model);
	scene.Add(std::move(player));
	for (int i = 0; i < 5; i++) {
		unique_ptr<Enemy> enemy = make_unique<Enemy>(kiko::randomf(), kiko::Pi, kiko::Transform{{ (float)kiko::random(800), (float)kiko::random(600) }, kiko::randomf(3), 1 }, model);
		scene.Add(std::move(enemy));
	}

	bool quit = false;
	while (!quit) {
		kiko::g_time.Tick();
		kiko::g_inputSystem.Update();
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE)) {
			quit = true;
		}

		kiko::g_audioSystem.Update();
		
		float rotate = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
		transform.rotation += rotate * turnRate * kiko::g_time.GetDeltaTime();

		float thrust = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) {
			thrust = 1;
			//if (!kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_W)) {
			kiko::g_audioSystem.PlayOneShot("thrusters");
			//}
		}

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		transform.position += forward * speed * thrust * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());

		//position += direction * speed * kiko::g_time.GetDeltaTime();
		int mouseX = kiko::g_inputSystem.GetMousePosition().x;
		int mouseY = kiko::g_inputSystem.GetMousePosition().y;
		//cout << "x: " << mouseX << " y: " << mouseY << endl;
		//if (kiko::g_inputSystem.GetMouseButtonDown(0) == 1) cout << "Left mouse button is being pressed" << endl;
		//if (kiko::g_inputSystem.GetMouseButtonDown(1) == 1) cout << "Middle mouse button is being pressed" << endl;
		//if (kiko::g_inputSystem.GetMouseButtonDown(2) == 1) cout << "Right mouse button is being pressed" << endl;
		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();

		kiko::vec2 vel(2, 1);
		for (auto& star : stars) {
			star.Update();
			star.Wrap(kiko::g_renderer.GetWidth(), kiko::g_renderer.GetHeight());
			kiko::g_renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			kiko::g_renderer.DrawPoint(star.m_pos.x, star.m_pos.y);
		}

		kiko::g_renderer.SetColor(255, 255, 255, 255);
		scene.Update(kiko::g_time.GetDeltaTime());
		scene.Draw(kiko::g_renderer);
		
		kiko::g_renderer.EndFrame();

		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	scene.RemoveAll();
	return 0;
}