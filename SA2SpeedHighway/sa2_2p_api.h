/*
 * SA2 2P stage API (SA2_1P_in_2P mod), version 4.
 *
 * Lets a level mod add its level to the 2P battle stage select WITHOUT
 * replacing a stock level and without being bundled with the 2P mod. A mod
 * stage either
 *   - plays on its own new level ID (version 3, levelId): the game looks for
 *     the stage's own set / camera files and positions under that ID, and runs
 *     the host level's module (or yours) for it; or
 *   - borrows a stock "host" level only while it is being played (levelId 0).
 * Either way the 2P mod applies the stage right before the match loads and
 * undoes it once the game is back in the menus, so the host level is the stock
 * level everywhere else (story mode, other 2P picks).
 *
 * What the 2P mod does for you, if you give it the data:
 *   - filesDir:  files that replace stock files of the host level. Put them in
 *                a folder (not in your gd_PC, which the mod loader applies
 *                globally) that mirrors gd_PC, e.g. <mod>\stage13\set0013_s.bin.
 *                They are swapped in on activate and out on deactivate.
 *   - positions: start, end, 2P intro and mission 2/3 end positions of the
 *                host level, registered for all characters on activate; the
 *                stock ones are restored on deactivate.
 * You still do everything code-shaped in activate / deactivate (patch the
 * level's header, object lists, hooks, ...) and must undo it exactly.
 *
 * How to use (copy this header into your mod):
 *
 *   extern "C" __declspec(dllexport)
 *   void __cdecl SA2TP_Init(const SA2TP_API* api, const HelperFunctions* helpers)
 *   {
 *       SA2TP_StageDesc d = {};
 *       d.size = sizeof(d);
 *       d.id = "Author.MyLevel";
 *       d.name = "My Level";
 *       d.modes = SA2TP_MODE_RACE;
 *       d.hostLevel = LevelIDs_RadicalHighway;
 *       d.levelId = 97;              // own level ID (v3), or 0 to borrow
 *       d.filesDir = "<your mod folder>\\stage14";
 *       d.startPosition = &myStart;  d.intro2P = &my2PIntro;  // optional
 *       d.activate = MyApply;        d.deactivate = MyRevert; // optional
 *       d.character = 0;             // v4: 1P Stage Select character (Sonic)
 *       d.missions = 1;              // v4: 1P missions (0 = none)
 *       api->AddStage(&d);
 *   }
 *
 * The 2P mod calls SA2TP_Init in every loaded mod that exports it, from its
 * own Init. All mod DLLs are loaded before any Init runs, so mod order does
 * not matter. SA2TP_Init may run before or after your own Init. If it is never
 * called (2P mod not installed), apply your changes permanently yourself, e.g.
 * on your first OnFrame.
 *
 * Versions: the descriptor's `size` tells the 2P mod which fields exist, so a
 * version 1 or 2 mod keeps working with this version. A mod that sets
 * levelId should check api->version >= 3, and fall back to borrowing (levelId
 * 0) if AddStage returns 0.
 *
 * Full guide: docs/mod_stage_api.md in the SA2_1P_in_2P repository.
 */
#ifndef SA2_2P_API_H
#define SA2_2P_API_H

#define SA2TP_API_VERSION 4

/* Level IDs a stage can claim with levelId (version 3). Pick one and keep it:
   your file names depend on it. Taken IDs are rejected. */
#define SA2TP_FIRST_LEVEL_ID 96
#define SA2TP_LAST_LEVEL_ID  127

#ifdef __cplusplus
extern "C" {
#endif

/* From the SA2 mod loader (SA2ModInfo.h / SA2Structs.h). */
struct HelperFunctions;
struct StartPosition;
struct LevelEndPosition;
struct LevelHeader;

enum {
    SA2TP_MODE_RACE  = 1, /* Action Race     (BattleSelection 1) */
    SA2TP_MODE_HUNT  = 2, /* Treasure Hunt   (BattleSelection 2) */
    SA2TP_MODE_SHOOT = 4  /* Shooting Battle (BattleSelection 3) */
};

typedef struct SA2TP_StageDesc {
    /* ---- version 1 ---- */
    int         size;      /* sizeof(SA2TP_StageDesc), for future versions */
    const char* id;        /* unique, e.g. "MainMemory.EmeraldCoast"      */
    const char* name;      /* shown in the menus, e.g. "Emerald Coast"    */
    int         modes;     /* SA2TP_MODE_* bitmask                         */
    int         hostLevel; /* level ID borrowed while playing             */
    int         group;     /* Current2PLevelGroup ruleset, 0 = default:
                              1 special (no health bar...), 2 = 4 min,
                              3 = 5 min, 4 = 7 min, 5+ = 10 min           */
    /* Called before the level loads / once back in the menus. deactivate is
       only called after activate. Either may be NULL (version 2). */
    void (__cdecl* activate)(void* user);
    void (__cdecl* deactivate)(void* user);
    void*       user;      /* passed to the callbacks                      */

    /* ---- version 2 (all optional, NULL = none) ---- */
    /* Absolute path of a folder mirroring gd_PC; every file in it (including
       subfolders) replaces the same-named stock file while active. Needs
       mod loader API 12+ to undo the replacement. */
    const char* filesDir;
    /* Positions for the stage's level (levelId, else the host level),
       registered for every character while active; their Level field is
       ignored. Copied during AddStage. */
    const struct StartPosition*    startPosition;
    const struct StartPosition*    endPosition;
    const struct LevelEndPosition* intro2P;
    const struct LevelEndPosition* end23;

    /* ---- version 3 ---- */
    /* Play the stage on this new level ID (SA2TP_FIRST_LEVEL_ID ..
       SA2TP_LAST_LEVEL_ID) instead of borrowing hostLevel; 0 = borrow.
       hostLevel is then the template: its module runs unless levelHeader is
       set, and engine checks that only ask "is this a normal stage?" (camera
       modes, 2P intro pose, voices, animals, Treasure Hunt) treat the stage
       like hostLevel. Code that checks for one specific level (e.g. City
       Escape's board) does not. Files the game names after the level ID use
       the new ID: set%04d (set0096_2p_s.bin, ...), stg%02dcam.prs
       (stg96cam.prs). They don't exist in the stock game, so they can ship
       in your gd_PC. The save is never written for these IDs. */
    int levelId;
    /* Your own level module for levelId (Name, Init, Free, Main), or NULL to
       run hostLevel's stock module. */
    const struct LevelHeader* levelHeader;

    /* ---- version 4: the 1P Stage Select's Custom page ---- */
    /* The character it's played as from the 1P Stage Select (Characters_*:
       0 Sonic, 1 Shadow, 2 Tails, 3 Eggman, 4 Knuckles, 5 Rouge), or -1 for
       the host level's Stage Select character. Its icon is shown on the
       stage's row. Descriptors older than version 4: -1. */
    int character;
    /* How many 1P missions the stage has (0..5). 0: no mission boxes on its
       row, picking it starts mission 1. 1..5: missions 1..n can be picked
       (MissionNum 0..n-1); make sure your level handles them (mission 2:
       100 rings, 3: the lost Chao, 4: time limit, 5: hard mode). Descriptors
       older than version 4: 0. */
    int missions;
} SA2TP_StageDesc;

typedef struct SA2TP_API {
    int version; /* SA2TP_API_VERSION of the 2P mod */
    /* Returns 1 on success. The descriptor and its strings are copied.
       Returns 0 for a bad descriptor, a duplicate id, or (version 3) a
       levelId that is taken, out of range, or not available because the 2P
       mod's CustomLevelIDs patch is off. */
    int (__cdecl* AddStage)(const SA2TP_StageDesc* desc);
} SA2TP_API;

/* Export this from your mod as "SA2TP_Init". */
typedef void (__cdecl* SA2TP_InitFunc)(const SA2TP_API* api,
                                       const struct HelperFunctions* helpers);

#ifdef __cplusplus
}
#endif

#endif /* SA2_2P_API_H */
