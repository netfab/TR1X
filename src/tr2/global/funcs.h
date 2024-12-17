// This file is autogenerated. To update it, run tools/generate_funcs.

#pragma once

#include "global/types.h"

// clang-format off
#define Output_InsertInventoryBackground ((void __cdecl (*)(const int16_t *obj_ptr))0x00401D50)
#define Gun_DrawFlash ((void __cdecl (*)(LARA_GUN_TYPE weapon_type, int32_t clip))0x0041BD30)
#define Output_CalculateObjectLighting ((void __cdecl (*)(const ITEM *item, const BOUNDS_16 *bounds))0x0041BEA0)
#define AddDynamicLight ((void __cdecl (*)(int32_t x, int32_t y, int32_t z, int32_t intensity, int32_t falloff))0x0041C0F0)
#define BigEel_Control ((void __cdecl (*)(int16_t item_num))0x0041C140)
#define Eel_Control ((void __cdecl (*)(int16_t item_num))0x0041C2E0)
#define DoBloodSplat ((int16_t __cdecl (*)(int32_t x, int32_t y, int32_t z, int16_t speed, PHD_ANGLE direction, int16_t room_num))0x0041C5D0)
#define DoLotsOfBlood ((void __cdecl (*)(int32_t x, int32_t y, int32_t z, int16_t speed, PHD_ANGLE direction, int16_t room_num, int32_t num))0x0041C630)
#define Blood_Control ((void __cdecl (*)(int16_t fx_num))0x0041C6E0)
#define Explosion_Control ((void __cdecl (*)(int16_t fx_num))0x0041C770)
#define Richochet ((void __cdecl (*)(GAME_VECTOR *pos))0x0041C7F0)
#define Richochet_Control ((void __cdecl (*)(int16_t fx_num))0x0041C870)
#define WaterSprite_Control ((void __cdecl (*)(int16_t fx_num))0x0041CBE0)
#define SnowSprite_Control ((void __cdecl (*)(int16_t fx_num))0x0041CC90)
#define HotLiquid_Control ((void __cdecl (*)(int16_t fx_num))0x0041CD20)
#define Waterfall_Control ((void __cdecl (*)(int16_t fx_num))0x0041CE00)
#define FX_FinishLevel ((void __cdecl (*)(ITEM *item))0x0041CF40)
#define FX_Turn180 ((void __cdecl (*)(ITEM *item))0x0041CF50)
#define FX_FloorShake ((void __cdecl (*)(ITEM *item))0x0041CF70)
#define FX_LaraNormal ((void __cdecl (*)(ITEM *item))0x0041D010)
#define FX_Boiler ((void __cdecl (*)(ITEM *item))0x0041D050)
#define FX_Flood ((void __cdecl (*)(ITEM *item))0x0041D070)
#define FX_Rubble ((void __cdecl (*)(ITEM *item))0x0041D100)
#define FX_Chandelier ((void __cdecl (*)(ITEM *item))0x0041D130)
#define FX_Explosion ((void __cdecl (*)(ITEM *item))0x0041D160)
#define FX_Piston ((void __cdecl (*)(ITEM *item))0x0041D190)
#define FX_Curtain ((void __cdecl (*)(ITEM *item))0x0041D1B0)
#define FX_Statue ((void __cdecl (*)(ITEM *item))0x0041D1D0)
#define FX_SetChange ((void __cdecl (*)(ITEM *item))0x0041D1F0)
#define DingDong_Control ((void __cdecl (*)(int16_t item_num))0x0041D210)
#define LaraAlarm_Control ((void __cdecl (*)(int16_t item_num))0x0041D250)
#define AlarmSound_Control ((void __cdecl (*)(int16_t item_num))0x0041D290)
#define BirdTweeter_Control ((void __cdecl (*)(int16_t item_num))0x0041D300)
#define DoChimeSound ((void __cdecl (*)(ITEM *item))0x0041D360)
#define ClockChimes_Control ((void __cdecl (*)(int16_t item_num))0x0041D3C0)
#define SphereOfDoom_Collision ((void __cdecl (*)(int16_t item_num, ITEM *lara_item, COLL_INFO *coll))0x0041D430)
#define SphereOfDoom_Control ((void __cdecl (*)(int16_t item_num))0x0041D560)
#define SphereOfDoom_Draw ((void __cdecl (*)(const ITEM *item))0x0041D650)
#define FX_FlipMap ((void __cdecl (*)(ITEM *item))0x0041D790)
#define FX_LaraDrawRightGun ((void __cdecl (*)(ITEM *item))0x0041D7A0)
#define FX_LaraDrawLeftGun ((void __cdecl (*)(ITEM *item))0x0041D7F0)
#define FX_SwapMeshesWithMeshSwap1 ((void __cdecl (*)(ITEM *item))0x0041D840)
#define FX_SwapMeshesWithMeshSwap2 ((void __cdecl (*)(ITEM *item))0x0041D8B0)
#define FX_SwapMeshesWithMeshSwap3 ((void __cdecl (*)(ITEM *item))0x0041D920)
#define FX_InvisibilityOn ((void __cdecl (*)(ITEM *item))0x0041D9C0)
#define FX_InvisibilityOff ((void __cdecl (*)(ITEM *item))0x0041D9D0)
#define FX_DynamicLightOn ((void __cdecl (*)(ITEM *item))0x0041D9F0)
#define FX_DynamicLightOff ((void __cdecl (*)(ITEM *item))0x0041DA00)
#define FX_ResetHair ((void __cdecl (*)(ITEM *item))0x0041DA10)
#define FX_AssaultStop ((void __cdecl (*)(ITEM *item))0x0041DA50)
#define FX_AssaultReset ((void __cdecl (*)(ITEM *item))0x0041DA70)
#define FX_AssaultFinished ((void __cdecl (*)(ITEM *item))0x0041DA90)
#define Knife ((int16_t __cdecl (*)(int32_t x, int32_t y, int32_t z, int16_t speed, PHD_ANGLE yrot, int16_t room_num))0x0041DB50)
#define Cultist2_Control ((void __cdecl (*)(int16_t item_num))0x0041DBD0)
#define Monk_Control ((void __cdecl (*)(int16_t item_num))0x0041E000)
#define Worker3_Control ((void __cdecl (*)(int16_t item_num))0x0041E4D0)
#define XianKnight_Draw ((void __cdecl (*)(const ITEM *item))0x0041EAE0)
#define XianDamage ((void __cdecl (*)(ITEM *item, CREATURE *xian, int32_t damage))0x0041EEE0)
#define XianKnight_Initialise ((void __cdecl (*)(int16_t item_num))0x0041EF90)
#define XianSpearman_Control ((void __cdecl (*)(int16_t item_num))0x0041EFF0)
#define WarriorSparkleTrail ((void __cdecl (*)(ITEM *item))0x0041F5D0)
#define XianKnight_Control ((void __cdecl (*)(int16_t item_num))0x0041F670)
#define Lara_Hair_Control ((void __cdecl (*)(bool in_cutscene))0x00420F20)
#define Lara_Hair_Draw ((void __cdecl (*)(void))0x00421920)
#define Earthquake_Control ((void __cdecl (*)(int16_t item_num))0x00434210)
#define FinalCutscene_Control ((void __cdecl (*)(int16_t item_num))0x004342F0)
#define InitialiseFinalLevel ((void __cdecl (*)(void))0x00434330)
#define MiniCopterControl ((void __cdecl (*)(int16_t item_num))0x00434610)
#define InitialiseDyingMonk ((void __cdecl (*)(int16_t item_num))0x004346F0)
#define DyingMonk ((void __cdecl (*)(int16_t item_num))0x00434770)
#define BigBowl_Control ((void __cdecl (*)(int16_t item_num))0x00434C10)
#define CopterControl ((void __cdecl (*)(int16_t item_num))0x00435B80)
#define GeneralControl ((void __cdecl (*)(int16_t item_num))0x00435C90)
#define Glow_Control ((void __cdecl (*)(int16_t fx_num))0x00435E90)
#define GunFlash_Control ((void __cdecl (*)(int16_t fx_num))0x00435ED0)
#define GunShot ((int16_t __cdecl (*)(int32_t x, int32_t y, int32_t z, int16_t speed, PHD_ANGLE yrot, int16_t room_num))0x00435F20)
#define GunHit ((int16_t __cdecl (*)(int32_t x, int32_t y, int32_t z, int16_t speed, PHD_ANGLE yrot, int16_t room_num))0x00435F90)
#define GunMiss ((int16_t __cdecl (*)(int32_t x, int32_t y, int32_t z, int16_t speed, PHD_ANGLE yrot, int16_t room_num))0x00436050)
#define ShotLara ((int32_t __cdecl (*)(ITEM *item, AI_INFO *info, BITE *gun, int16_t extra_rotation, int32_t damage))0x00436100)
#define Cultist1_Initialise ((void __cdecl (*)(int16_t item_num))0x004362D0)
#define Cultist1_Control ((void __cdecl (*)(int16_t item_num))0x00436320)
#define Cultist3_Initialise ((void __cdecl (*)(int16_t item_num))0x00436750)
#define Cultist3_Control ((void __cdecl (*)(int16_t item_num))0x004367A0)
#define Worker1_Control ((void __cdecl (*)(int16_t item_num))0x00436D10)
#define Worker2_Control ((void __cdecl (*)(int16_t item_num))0x00437110)
#define Bandit1_Control ((void __cdecl (*)(int16_t item_num))0x00437570)
#define Bandit2_Control ((void __cdecl (*)(int16_t item_num))0x004378B0)
#define Winston_Control ((void __cdecl (*)(int16_t item_num))0x00437CF0)
#define Secret_Control ((void __cdecl (*)(int16_t item_num))0x00438EC0)
#define Mouse_Control ((void __cdecl (*)(int16_t item_num))0x00438EF0)
#define InitialiseGameFlags ((void __cdecl (*)(void))0x0043A3E0)
#define InitialiseLevelFlags ((void __cdecl (*)(void))0x0043A450)
#define Object_SetupTrapObjects ((void __cdecl (*)(void))0x0043B4C0)
#define GetCarriedItems ((void __cdecl (*)(void))0x0043C780)
#define Jelly_Control ((void __cdecl (*)(int16_t item_num))0x0043C850)
#define Baracudda_Control ((void __cdecl (*)(int16_t item_num))0x0043C970)
#define Shark_Control ((void __cdecl (*)(int16_t item_num))0x0043CBA0)
#define DoShift ((int32_t __cdecl (*)(ITEM *skidoo, XYZ_32 *pos, XYZ_32 *old))0x0043D320)
#define DoDynamics ((int32_t __cdecl (*)(int32_t height, int32_t fall_speed, int32_t *y))0x0043D5A0)
#define GetCollisionAnim ((int32_t __cdecl (*)(ITEM *skidoo, XYZ_32 *moved))0x0043D600)
#define Collide_TestCollision ((int32_t __cdecl (*)(ITEM *item, const ITEM *lara_item))0x0043F9B0)
#define Collide_GetSpheres ((int32_t __cdecl (*)(const ITEM *item, SPHERE *spheres, bool world_space))0x0043FAE0)
#define Collide_GetJointAbsPosition ((void __cdecl (*)(const ITEM *item, const XYZ_32 *vec, int32_t joint))0x0043FDC0)
#define BaddieBiteEffect ((void __cdecl (*)(ITEM *item, BITE *bite))0x0043FF60)
#define Hook_Control ((void __cdecl (*)(int16_t item_num))0x00441370)
#define Propeller_Control ((void __cdecl (*)(int16_t item_num))0x00441400)
#define SpinningBlade_Control ((void __cdecl (*)(int16_t item_num))0x00441590)
#define Icicle_Control ((void __cdecl (*)(int16_t item_num))0x00441710)
#define Blade_Initialise ((void __cdecl (*)(int16_t item_num))0x00441810)
#define Blade_Control ((void __cdecl (*)(int16_t item_num))0x00441850)
#define KillerStatue_Initialise ((void __cdecl (*)(int16_t item_num))0x004418F0)
#define KillerStatue_Control ((void __cdecl (*)(int16_t item_num))0x00441940)
#define SpringBoard_Control ((void __cdecl (*)(int16_t item_num))0x00441A50)
#define Spike_Collision ((void __cdecl (*)(int16_t item_num, ITEM *litem, COLL_INFO *coll))0x00442110)
#define Pendulum_Control ((void __cdecl (*)(int16_t item_num))0x004423F0)
#define TeethTrap_Control ((void __cdecl (*)(int16_t item_num))0x004426A0)
#define FallingCeiling_Control ((void __cdecl (*)(int16_t item_num))0x00442760)
#define DartEmitter_Control ((void __cdecl (*)(int16_t item_num))0x00442840)
#define Dart_Control ((void __cdecl (*)(int16_t item_num))0x00442980)
#define DartEffect_Control ((void __cdecl (*)(int16_t fx_num))0x00442AE0)
// clang-format on
