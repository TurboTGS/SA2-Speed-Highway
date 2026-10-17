#pragma once


enum SADXLevelIDs {
	LevelIDs_HedgehogHammer,
	LevelIDs_EmeraldCoast,
	LevelIDs_WindyValley,
	LevelIDs_TwinklePark,
	LevelIDs_SpeedHighway,
	LevelIDs_RedMountain,
	LevelIDs_SkyDeck,
	LevelIDs_LostWorld,
	LevelIDs_IceCap,
	LevelIDs_Casinopolis,
	LevelIDs_FinalEgg,
	LevelIDs_B,
	LevelIDs_HotShelter,
	LevelIDs_D,
	LevelIDs_E,
	LevelIDs_Chaos0,
	LevelIDs_Chaos2,
	LevelIDs_Chaos4,
	LevelIDs_Chaos6,
	LevelIDs_PerfectChaos,
	LevelIDs_EggHornet,
	LevelIDs_EggWalker,
	LevelIDs_EggViper,
	LevelIDs_Zero,
	LevelIDs_E101,
	LevelIDs_E101R,
	LevelIDs_StationSquare,
	LevelIDs_EggCarrierOutside = 29,
	LevelIDs_EggCarrierInside = 32,
	LevelIDs_MysticRuins,
	LevelIDs_Past,
	LevelIDs_TwinkleCircuit,
	LevelIDs_SkyChase1,
	LevelIDs_SkyChase2,
	LevelIDs_SandHill,
	LevelIDs_SSGarden,
	LevelIDs_ECGarden,
	LevelIDs_MRGarden,
	LevelIDs_ChaoRace,
};

// Speed Highway borrows Radical Highway's slot while it is played.
// SpeedHighway_Init_Stock saves the stock level header (call once at Init);
// Apply swaps Speed Highway in, Revert puts Radical Highway back.
void SpeedHighway_SaveStock();
void SpeedHighway_Apply();
void SpeedHighway_Revert();
// Without the SA2 2P mod: apply for good (replaces Radical Highway), including
// the positions and files the 2P mod would otherwise handle.
void SpeedHighway_ApplyStandalone(const char* modPath);
// Data handed to the 2P mod through the stage API.
const StartPosition* SpeedHighway_StartPos();
const StartPosition* SpeedHighway_EndPos();
const LevelEndPosition* SpeedHighway_2PIntro();
const LevelEndPosition* SpeedHighway_EndPos23();
void LoadSH_DeathZonesModel();
void FreeDZModels();
void LoadSH_DeathZones(int act);

extern int CurrentSADXLevel;

void SetLevelPosAndRot(int playerID);