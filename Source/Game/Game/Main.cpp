#include "Core/Random.h"
#include "Core/FileIO.h"
#include "Core/Memory.h"
#include "Core/Time.h"
#include "Renderer/Renderer.h"
#include <iostream>

using namespace std;

int main() {
	kiko::g_memoryTracker.DisplayInfo();
	int* p = new int;
	kiko::g_memoryTracker.DisplayInfo();
	delete p;
	kiko::g_memoryTracker.DisplayInfo();

	/*auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++) {
		cout << "kevin arrives in " << 100000 - i << " ticks." << endl;
	}
	auto end = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<chrono::seconds>(end - start).count() << endl; */

	kiko::Time timer;
	for (int i = 0; i < 10000000; i++) {

	}
	cout << timer.GetElapsedSeconds() << endl;




	/*cout << kiko::getFilePath() << endl;
	kiko::setFilePath("Assets");
	cout << kiko::getFilePath() << endl;
	size_t size;
	kiko::getFileSize("game.txt", size);
	cout << size << endl;
	string s;
	kiko::readFile("game.txt", s);
	cout << s << endl;


	kiko::seedRandom((unsigned int)time(nullptr));
	for (int i = 0; i < 3; i++) { cout << kiko::random(5, 10) << endl; }*/
}
