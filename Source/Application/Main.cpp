#include "World01.h"
#include "World02.h"
#include "World03.h"
#include "World04.h"
#include "World05.h"
#include "World055.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

using namespace std;

int main(int argc, char* argv[])
{
	
	INFO_LOG("Initialize Engine...")

	//nc::MemoryTracker::Initialize();
	nc::seedRandom((unsigned int)time(nullptr));
	nc::setFilePath("assets");

	ENGINE.Initialize();

	auto world = make_unique<nc::World055>();
	world->Initialize();

	// main loop
	bool quit = false;
	while (!quit)
	{
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<nc::Renderer>());
	}

	world->Shutdown();
	ENGINE.Shutdown();
	

	/*std::string toLower = "THIS WILL BE LOWERCASE";
	std::string toUpper = "this will be uppercase";
	std::string equalsCheck = "tHiS WiLl Be uPPErcAsE";

	std::cout << nc::ToLower(toLower) << endl;
	std::cout << nc::ToUpper(toUpper) << endl;
	std::cout << nc::IsEqualIgnoreCase(toUpper, equalsCheck) << endl;
	std::cout << nc::createUnique(toLower) << endl;
	std::cout << nc::createUnique(toUpper) << endl;*/
	return 0;
}
