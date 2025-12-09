#include <iostream>

#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "game.h"

int main(int argc, char* argv[])
{
	Game game;
	spn::SpinachCore sc(640, 480, "../res/");
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	sc.SetUpdateAndRenderHandler(
		std::bind(&Game::UpdateAndRender, 
			&game, 
			std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&Game::HandleInput, 
			&game, 
			std::placeholders::_1));
	sc.SetWindowTitle("Match");
	sc.GetCanvas()->SetPrimaryColor(0, 0, 255);
	sc.SetTargetFramesPerSecond(30);
	sc.MainLoop();
	return 0;
}


//please play my game made for 5 - 10 year old kids : https://mmjthefighter.itch.io/match5x5