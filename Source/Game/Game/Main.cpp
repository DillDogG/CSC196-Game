#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include <iostream>

using namespace std;

class Star {
public:
	Star(kiko::Vector2 pos, kiko::Vector2 vel) : m_pos{ pos }, m_vel{ vel } {}
	void Update() {
		m_pos += m_vel;
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
	kiko::Vector2 m_pos;
	kiko::Vector2 m_vel;
};

int main(int argc, char* argv[]) {
	kiko::seedRandom((unsigned int)time(nullptr));

	kiko::Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("CSC196", 800, 600);

	vector<Star> stars;
	for (int i = 0; i < 1000; i++) {
		kiko::Vector2 pos(kiko::Vector2(kiko::random(renderer.GetWidth()), kiko::random(renderer.GetHeight())));
		kiko::Vector2 vel(kiko::Vector2(kiko::random(4), kiko::random(2)));
		stars.push_back(Star(pos, vel));
	}

	for (int i = 0; i < 2500; i++) {
		renderer.SetColor(0, 0, 0, 0);
		renderer.BeginFrame();
		kiko::Vector2 vel(2, 1);
		for (auto& star : stars) {
			star.Update();
			star.Wrap(renderer.GetWidth(), renderer.GetHeight());
			renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			renderer.DrawPoint(star.m_pos.x, star.m_pos.y);
		}
		renderer.EndFrame();
	}
	return 0;
}
