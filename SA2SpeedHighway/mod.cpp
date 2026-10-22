#include "pch.h"
#include "level.h"
#include "Sonic.h"
#include "sa2_2p_api.h"

HelperFunctions HelperFunctionsGlobal;

// Speed Highway replaces Radical Highway (level 14). With the SA2 2P mod
// (SA2_1P_in_2P) installed it registers as its own 2P Race stage instead and
// only takes over level 14 while a Speed Highway match is played, so Radical
// Highway stays Radical Highway everywhere else. Without it, it replaces
// Radical Highway for good (applied on the first frame).
static std::string modFolder;
static std::string stage14Folder;
static bool registeredWith2P = false;
static bool checkedStandalone = false;

static void __cdecl Activate2P(void*) { SpeedHighway_Apply(); }
static void __cdecl Deactivate2P(void*) { SpeedHighway_Revert(); }

extern "C"
{
	// Called by the SA2 2P mod from its Init (before or after ours).
	__declspec(dllexport) void __cdecl SA2TP_Init(const SA2TP_API* api, const HelperFunctions*)
	{
		if (!api || api->version < 2)
		{
			PrintDebug("[SH] The SA2 2P mod is too old for the stage API (needs version 2).\n");
			return;
		}

		SA2TP_StageDesc d = {};
		d.size = sizeof(d);
		d.id = "Sora.SpeedHighway";
		d.name = "Speed Highway";
		d.modes = SA2TP_MODE_RACE;
		d.hostLevel = LevelIDs_RadicalHighway;
		d.activate = Activate2P;
		d.deactivate = Deactivate2P;
		d.filesDir = stage14Folder.c_str();
		d.startPosition = SpeedHighway_StartPos();
		d.endPosition = SpeedHighway_EndPos();
		d.intro2P = SpeedHighway_2PIntro();
		d.end23 = SpeedHighway_EndPos23();
		// API v4: the 1P Stage Select's Custom page. Played as Radical
		// Highway's character (Shadow); missions 1 (goal) and 2 (100 rings).
		d.character = -1;
		d.missions = 2;
		// API v6: rank requirements for D, C, B, A (example values).
		// Older 2P mods ignore them (they go by the descriptor's size).
		// Mission 1: least points (score + time bonus).
		// Mission 2 (100 rings): slowest time in seconds.
		const int m1[4] = { 7000, 10000, 13000, 16000 };
		const int m2[4] = { 330, 270, 210, 170 }; // 5:30, 4:30, 3:30, 2:50
		for (int k = 0; k < 4; ++k)
		{
			d.rankRequirements[0][k] = m1[k];
			d.rankRequirements[1][k] = m2[k];
		}
		registeredWith2P = api->AddStage(&d) != 0;
	}

	__declspec(dllexport) void Init(const char* path, const HelperFunctions& helperFunctions)
	{
		if (helperFunctions.Version < 7) {
			MessageBoxA(MainWindowHandle,
				"Your copy of the Mod Loader is outdated, please update it to the latest version.",
				"SA2 Speed Highway",
				MB_ICONWARNING);
		}

		HelperFunctionsGlobal = helperFunctions;
		modFolder = path;
		stage14Folder = modFolder + "\\stage14";

		SpeedHighway_SaveStock();
		Init_Sonic();

		CommonObjects_Init();

		//WriteData<1>((void*)0x5239A0, 0xC3);
	}

	__declspec(dllexport) void __cdecl OnFrame() {

		// Every mod's Init has run by the first frame, so by now we know whether
		// the 2P mod registered us. If not, replace Radical Highway for good.
		if (!checkedStandalone)
		{
			checkedStandalone = true;
			if (!registeredWith2P)
				SpeedHighway_ApplyStandalone(modFolder.c_str());
		}

		if (GameState != GameStates_Ingame)
			return;

		if (Controllers[0].press & Buttons_B && CurrentLevel == LevelIDs_RadicalHighway) {
		//	MainCharObj1[0]->Position = { 2484.12, -2001.165, 3773.63 };
		}

		FreeCam();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}
