#pragma once

// Game code this mod needs that the mod loader includes don't name the same
// way. Built against TurboTGS/sa2-mod-loader (branch turbo), which declares
// the rest: DrawChunkModel, njDrawSprite3D, DrawObject, csts, DynColInfo,
// LandColList(_Count), CL_ColPolListUpNear, CL_PolyInfo, CL_ObjInfo,
// CL_ColPolCheckTouchRe, CalcVector_PlayerRot(Inverse) (was PConvertVector_
// P2G / G2P), GamePlayerMissed_Load (BGCheckAndKillPlayer),
// Player_SetRunAnimBySpeed (PhysicsAndAnimCheck), Checkpoint_Clear
// (CP_ResetPosition), Sonic_CheckJump, Player_TurnToAngleBySpeed
// (PAdjustAngleYS), ChaosDriveFuncs (dword_1DE4680..68C), GetPathStatus /
// PathInfo (GetStatusOnPath / pathinfo), SetMaterial (SetMaterialColor),
// nj_constant_material_ (GlobalMatColorA/R/G/B), Sonic_CheckNextAction
// (Sonic_CheckNextActions), CheckpointRecords (byte_1A558A0).

// SADX's motionwk field names (force, accel, frict) over SA2's EntityData2:
// the SADX-ported physics reads them by those names.
#pragma pack(push, 1)
struct EntityData2_R
{
	NJS_POINT3 spd;
	NJS_POINT3 acc;
	Rotation ang_aim;
	Rotation ang_spd;
	float force;
	float accel;
	float frict;
};
#pragma pack(pop)

int VectorAngle(NJS_VECTOR* a1, NJS_VECTOR* a2, NJS_VECTOR* a3);

void ResetMaterialColorOffset();

void SetCameraPos(float x, float y, float z);
bool isSonicBouncing();
