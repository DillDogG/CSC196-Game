#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Renderer/Model.h"
#include "../../Input/InputSystem.h"
#include "Actor.h"
#include "Player.h"
#include <iostream>
#include <thread>

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
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	kiko::Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("CSC196", 800, 600);
	kiko::InputSystem inputSystem;
	inputSystem.Initialize();

	//std::vector<kiko::vec2> points{ { -10, 5 }, { 10, 5 }, { 0, -5 }, { -10, 5 } };
	kiko::Model model;
	model.Load("ship.txt");

	/* kiko::vec2 v{ 5, 5 };
	v.Normalize(); */

	vector<Star> stars;
	for (int i = 0; i < 1000; i++) {
		kiko::vec2 pos(kiko::vec2(kiko::random(renderer.GetWidth()), kiko::random(renderer.GetHeight())));
		kiko::vec2 vel(kiko::vec2(kiko::random(100, 200), kiko::random(2)));
		stars.push_back(Star(pos, vel));
	}

	kiko::Transform transform{ {400, 300}, 1.2f, 2 };

	kiko::vec2 position{ 400, 300 };
	float speed = 200;
	float turnRate = kiko::DegreesToRadians(180);

	Player player{ 200, kiko::DegreesToRadians(180), transform, model};

	bool quit = false;
	while (!quit) {
		kiko::g_time.Tick();
		inputSystem.Update();
		if (inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE)) {
			quit = true;
		}
		
		float rotate = 0;
		if (inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
		if (inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
		transform.rotation += rotate * turnRate * kiko::g_time.GetDeltaTime();

		float thrust = 0;
		if (inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		transform.position += forward * speed * thrust * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, (float)renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)renderer.GetHeight());

		//kiko::vec2 direction;
		//if (inputSystem.GetKeyDown(SDL_SCANCODE_W)) direction.y = -1;
		//if (inputSystem.GetKeyDown(SDL_SCANCODE_S)) direction.y =  1;
		//if (inputSystem.GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
		//if (inputSystem.GetKeyDown(SDL_SCANCODE_D)) direction.x =  1;

		//position += direction * speed * kiko::g_time.GetDeltaTime();
		int mouseX = inputSystem.GetMousePosition().x;
		int mouseY = inputSystem.GetMousePosition().y;
		cout << "x: " << mouseX << " y: " << mouseY << endl;
		if (inputSystem.GetMouseButtonDown(0) == 1) cout << "Left mouse button is being pressed" << endl;
		if (inputSystem.GetMouseButtonDown(1) == 1) cout << "Middle mouse button is being pressed" << endl;
		if (inputSystem.GetMouseButtonDown(2) == 1) cout << "Right mouse button is being pressed" << endl;
		renderer.SetColor(0, 0, 0, 0);
		renderer.BeginFrame();

		kiko::vec2 vel(2, 1);
		for (auto& star : stars) {
			star.Update();
			star.Wrap(renderer.GetWidth(), renderer.GetHeight());
			renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			renderer.DrawPoint(star.m_pos.x, star.m_pos.y);
		}
		player.Draw(renderer);
		
		renderer.EndFrame();

		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
