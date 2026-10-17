/*
 * SA2 2P stage API (SA2_1P_in_2P mod), version 2.
 *
 * Lets a level mod add its level to the 2P battle stage select WITHOUT
 * replacing a stock level and without being bundled with the 2P mod. A mod
 * stage borrows a stock "host" level only while it is being played: the 2P
 * mod applies the stage right before the match loads and undoes it once the
 * game is back in the menus, so the host level is the stock level everywhere
 * else (story mode, other 2P picks).
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
 *       d.filesDir = "<your mod folder>\\stage14";
 *       d.startPosition = &myStart;  d.intro2P = &my2PIntro;  // optional
 *       d.activate = MyApply;        d.deactivate = MyRevert; // optional
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
 * version 1 mod (no fields after `user`) keeps working with this version.
 *
 * Full guide: docs/mod_stage_api.md in the SA2_1P_in_2P repository.
 */
#ifndef SA2_2P_API_H
#define SA2_2P_API_H

#define SA2TP_API_VERSION 2

#ifdef __cplusplus
extern "C" {
#endif

/* From the SA2 mod loader (SA2ModInfo.h / SA2Structs.h). */
struct HelperFunctions;
struct StartPosition;
struct LevelEndPosition;

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
    /* Positions for the host level, registered for every character while
       active. Copied during AddStage. */
    const struct StartPosition*    startPosition;
    const struct StartPosition*    endPosition;
    const struct LevelEndPosition* intro2P;
    const struct LevelEndPosition* end23;
} SA2TP_StageDesc;

typedef struct SA2TP_API {
    int version; /* SA2TP_API_VERSION of the 2P mod */
    /* Returns 1 on success. The descriptor and its strings are copied. */
    int (__cdecl* AddStage)(const SA2TP_StageDesc* desc);
} SA2TP_API;

/* Export this from your mod as "SA2TP_Init". */
typedef void (__cdecl* SA2TP_InitFunc)(const SA2TP_API* api,
                                       const struct HelperFunctions* helpers);

#ifdef __cplusplus
}
#endif

#endif /* SA2_2P_API_H */
