#include "Menu.h"
#include "NewGame.h"
#include "Setting.h"
#include "TopPlayer.h"
#include "Guide.h"
#include "Resume.h"
#include "MiniGame.h"

using namespace std;

mutex m;

int main()
{
	SettingScreen();	
	Scene scene = menu;
	while (start)
	{
		if (scene == menu)
			MenuScene(scene);
		if (scene == newgame)
			NewGameScene(scene, -1);
		if (scene == setting)
			SettingScene(scene);
		if (scene == topplayer)
			TopPlayerScene(scene);
		if (scene == guide)
			GuideScene(scene);
		if (scene == minigame)
			miniGameScene(scene);
		if (scene == resume)
			ResumeScene(scene);
	}
	return 0;
}



