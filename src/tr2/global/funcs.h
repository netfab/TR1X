// This file is autogenerated. To update it, run tools/generate_funcs.

#pragma once

#include "global/types.h"

// clang-format off
#define Output_InsertInventoryBackground ((void __cdecl (*)(const int16_t *obj_ptr))0x00401D50)
#define Dog_Control ((void __cdecl (*)(int16_t item_num))0x00417160)
#define Tiger_Control ((void __cdecl (*)(int16_t item_num))0x00417510)
#define TRex_Control ((void __cdecl (*)(int16_t item_num))0x004186A0)
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
#define InitialiseHair ((void __cdecl (*)(void))0x00420EA0)
#define HairControl ((void __cdecl (*)(int32_t in_cutscene))0x00420F20)
#define DrawHair ((void __cdecl (*)(void))0x00421920)
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
#define Spider_Leap ((void __cdecl (*)(int16_t item_num, int16_t angle))0x0043FFC0)
#define Spider_Control ((void __cdecl (*)(int16_t item_num))0x00440070)
#define BigSpider_Control ((void __cdecl (*)(int16_t item_num))0x00440290)
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
#define GiantYeti_Control ((void __cdecl (*)(int16_t item_num))0x00443050)
#define Yeti_Control ((void __cdecl (*)(int16_t item_num))0x00443350)
#define ReadFileSync ((BOOL __cdecl (*)(HANDLE handle, LPVOID lpBuffer, DWORD nBytesToRead, LPDWORD lpnBytesRead, LPOVERLAPPED lpOverlapped))0x004498D0)
#define AdjustTextureUVs ((void __cdecl (*)(bool reset_uv_add))0x00449E50)
#define S_LoadLevelFile ((BOOL __cdecl (*)(LPCTSTR file_name, int32_t level_num, GAMEFLOW_LEVEL_TYPE level_type))0x0044B4B0)
#define S_UnloadLevelFile ((void __cdecl (*)(void))0x0044B4D0)
#define S_AdjustTexelCoordinates ((void __cdecl (*)(void))0x0044B500)
#define S_ReloadLevelGraphics ((BOOL __cdecl (*)(BOOL reload_palettes, BOOL reload_tex_pages))0x0044B520)
#define GetValidLevelsList ((void __cdecl (*)(REQUEST_INFO *req))0x0044C9D0)
#define CalculateWibbleTable ((void __cdecl (*)(void))0x0044D780)
#define S_GetObjectBounds ((int32_t __cdecl (*)(const BOUNDS_16 *bounds))0x00450CC0)
#define S_PrintShadow ((void __cdecl (*)(int16_t radius, const BOUNDS_16 *bounds, const ITEM *item))0x00450F80)
#define S_CalculateLight ((void __cdecl (*)(int32_t x, int32_t y, int32_t z, int16_t room_num))0x00451180)
#define S_CalculateStaticLight ((void __cdecl (*)(int16_t adder))0x00451480)
#define S_CalculateStaticMeshLight ((void __cdecl (*)(int32_t x, int32_t y, int32_t z, int32_t shade_1, int32_t shade_2, ROOM *room))0x004514C0)
#define S_LightRoom ((void __cdecl (*)(ROOM *room))0x004515F0)
#define S_DrawHealthBar ((void __cdecl (*)(int32_t percent))0x00451800)
#define S_DrawAirBar ((void __cdecl (*)(int32_t percent))0x004519D0)
#define AnimateTextures ((void __cdecl (*)(int32_t ticks))0x00451BD0)
#define S_SetupBelowWater ((void __cdecl (*)(BOOL underwater))0x00451C90)
#define S_SetupAboveWater ((void __cdecl (*)(BOOL underwater))0x00451CF0)
#define S_AnimateTextures ((void __cdecl (*)(int32_t ticks))0x00451D20)
#define S_SyncPictureBufferPalette ((void __cdecl (*)(void))0x00451EF0)
#define S_DontDisplayPicture ((void __cdecl (*)(void))0x00451F70)
#define ScreenDump ((void __cdecl (*)(void))0x00451F80)
#define ScreenPartialDump ((void __cdecl (*)(void))0x00451F90)
#define FadeToPal ((void __cdecl (*)(int32_t fade_value, RGB_888 *palette))0x00451FA0)
#define ScreenClear ((void __cdecl (*)(bool is_phd_win_size))0x00452170)
#define S_CopyScreenToBuffer ((void __cdecl (*)(void))0x004521A0)
#define DecompPCX ((BOOL __cdecl (*)(const uint8_t *pcx, size_t pcx_size, LPBYTE pic, RGB_888 *pal))0x004522A0)
#define OpenGameRegistryKey ((bool __cdecl (*)(LPCTSTR key))0x004523C0)
#define CloseGameRegistryKey ((LONG __cdecl (*)(void))0x00452410)
#define SE_WriteAppSettings ((bool __cdecl (*)(APP_SETTINGS *settings))0x00452420)
#define SE_ReadAppSettings ((int32_t __cdecl (*)(APP_SETTINGS *settings))0x00452690)
#define SE_GraphicsTestStart ((bool __cdecl (*)(void))0x004529E0)
#define SE_GraphicsTestFinish ((void __cdecl (*)(void))0x00452AB0)
#define SE_GraphicsTestExecute ((int32_t __cdecl (*)(void))0x00452AD0)
#define SE_GraphicsTest ((int32_t __cdecl (*)(void))0x00452AE0)
#define SE_SoundTestStart ((bool __cdecl (*)(void))0x00452B40)
#define SE_SoundTestFinish ((void __cdecl (*)(void))0x00452C00)
#define SE_SoundTestExecute ((int32_t __cdecl (*)(void))0x00452C10)
#define SE_SoundTest ((int32_t __cdecl (*)(void))0x00452C50)
#define SE_PropSheetCallback ((int32_t __stdcall (*)(HWND hwndDlg, UINT uMsg, LPARAM lParam))0x00452CB0)
#define SE_NewPropSheetWndProc ((LRESULT __stdcall (*)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam))0x00452CF0)
#define SE_ShowSetupDialog ((bool __cdecl (*)(HWND hParent, bool isDefault))0x00452D50)
#define SE_GraphicsDlgProc ((INT_PTR __stdcall (*)(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam))0x00453030)
#define SE_GraphicsDlgFullScreenModesUpdate ((void __cdecl (*)(HWND hwndDlg))0x004533F0)
#define SE_GraphicsAdapterSet ((void __cdecl (*)(HWND hwndDlg, DISPLAY_ADAPTER_NODE *adapter))0x004535E0)
#define SE_GraphicsDlgUpdate ((void __cdecl (*)(HWND hwndDlg))0x00453600)
#define SE_GraphicsDlgInit ((void __cdecl (*)(HWND hwndDlg))0x00453D40)
#define SE_SoundDlgProc ((INT_PTR __stdcall (*)(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam))0x00453EC0)
#define SE_SoundAdapterSet ((void __cdecl (*)(HWND hwndDlg, SOUND_ADAPTER_NODE *adapter))0x00454050)
#define SE_SoundDlgUpdate ((void __cdecl (*)(HWND hwndDlg))0x00454060)
#define SE_SoundDlgInit ((void __cdecl (*)(HWND hwndDlg))0x00454180)
#define SE_ControlsDlgProc ((INT_PTR __stdcall (*)(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam))0x00454240)
#define SE_ControlsJoystickSet ((void __cdecl (*)(HWND hwndDlg, JOYSTICK_NODE *joystick))0x00454350)
#define SE_ControlsDlgUpdate ((void __cdecl (*)(HWND hwndDlg))0x00454360)
#define SE_ControlsDlgInit ((void __cdecl (*)(HWND hwndDlg))0x004543D0)
#define SE_OptionsDlgProc ((INT_PTR __stdcall (*)(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam))0x00454490)
#define SE_OptionsDlgUpdate ((void __cdecl (*)(HWND hwndDlg))0x00454520)
#define SE_OptionsStrCat ((void __cdecl (*)(LPTSTR *dstString, bool isEnabled, bool *isNext, LPCTSTR srcString))0x00454760)
#define SE_AdvancedDlgProc ((INT_PTR __stdcall (*)(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam))0x004547B0)
#define SE_AdvancedDlgUpdate ((void __cdecl (*)(HWND hwndDlg))0x004548B0)
#define SE_AdvancedDlgInit ((void __cdecl (*)(HWND hwndDlg))0x00454950)
#define SE_FindSetupDialog ((HWND __cdecl (*)(void))0x00454960)
#define CheckCheatMode ((void __cdecl (*)(void))0x00454D60)
#define FreePalette ((void __cdecl (*)(int32_t palette_idx))0x00455B10)
#define SafeFreePalette ((void __cdecl (*)(int32_t palette_idx))0x00455B40)
#define CreateTexturePage ((int32_t __cdecl (*)(int32_t width, int32_t height, bool alpha))0x00455B90)
#define GetFreeTexturePageIndex ((int32_t __cdecl (*)(void))0x00455C00)
#define CreateTexturePageSurface ((bool __cdecl (*)(TEXPAGE_DESC *desc))0x00455C20)
#define TexturePageInit ((bool __cdecl (*)(TEXPAGE_DESC *page))0x00455CC0)
#define Create3DTexture ((LPDIRECT3DTEXTURE2 __cdecl (*)(LPDDS surface))0x00455E40)
#define SafeFreeTexturePage ((void __cdecl (*)(int32_t page_idx))0x00455E70)
#define FreeTexturePage ((void __cdecl (*)(int32_t page_idx))0x00455E90)
#define TexturePageReleaseVidMemSurface ((void __cdecl (*)(TEXPAGE_DESC *page))0x00455ED0)
#define FreeTexturePages ((void __cdecl (*)(void))0x00455F10)
#define LoadTexturePage ((bool __cdecl (*)(int32_t page_idx, bool reset))0x00455F40)
#define ReloadTextures ((bool __cdecl (*)(bool reset))0x00455FF0)
#define GetTexturePageHandle ((HWR_TEXTURE_HANDLE __cdecl (*)(int32_t page_idx))0x00456030)
#define AddTexturePage8 ((int32_t __cdecl (*)(int32_t width, int32_t height, const uint8_t *page_buf, int32_t pal_idx))0x00456070)
#define AddTexturePage16 ((int32_t __cdecl (*)(int32_t width, int32_t height, const uint8_t *page_buf))0x00456170)
#define EnumTextureFormatsCallback ((HRESULT __stdcall (*)(LPDDSDESC lpDdsd, LPVOID lpContext))0x00456310)
#define EnumerateTextureFormats ((HRESULT __cdecl (*)(void))0x00456430)
#define CleanupTextures ((void __cdecl (*)(void))0x00456460)
#define InitTextures ((bool __cdecl (*)(void))0x00456470)
#define UT_LoadResource ((LPVOID __cdecl (*)(LPCTSTR lpName, LPCTSTR lpType))0x00456590)
#define UT_CenterWindow ((BOOL __cdecl (*)(HWND hWnd))0x00456680)
#define UT_FindArg ((LPTSTR __cdecl (*)(LPCTSTR str))0x004566F0)
#define UT_MessageBox ((int32_t __cdecl (*)(LPCTSTR lpText, HWND hWnd))0x00456720)
#define UT_ErrorBox ((int32_t __cdecl (*)(UINT uID, HWND hWnd))0x00456740)
#define GuidBinaryToString ((LPCTSTR __cdecl (*)(GUID *guid))0x00456790)
#define GuidStringToBinary ((bool __cdecl (*)(LPCTSTR lpString, GUID *guid))0x004567F0)
#define OpenRegistryKey ((BOOL __cdecl (*)(LPCTSTR lpSubKey))0x004568A0)
#define IsNewRegistryKeyCreated ((bool __cdecl (*)(void))0x004568D0)
#define CloseRegistryKey ((LONG __cdecl (*)(void))0x004568E0)
#define SetRegistryDwordValue ((LONG __cdecl (*)(LPCTSTR lpValueName, DWORD value))0x004568F0)
#define SetRegistryBoolValue ((LONG __cdecl (*)(LPCTSTR lpValueName, bool value))0x00456910)
#define SetRegistryFloatValue ((LONG __cdecl (*)(LPCTSTR lpValueName, double value))0x00456940)
#define SetRegistryBinaryValue ((LONG __cdecl (*)(LPCTSTR lpValueName, LPBYTE value, DWORD valueSize))0x00456980)
#define SetRegistryStringValue ((LONG __cdecl (*)(LPCTSTR lpValueName, LPCTSTR value, int32_t length))0x004569C0)
#define DeleteRegistryValue ((LONG __cdecl (*)(LPCTSTR lpValueName))0x00456A10)
#define GetRegistryDwordValue ((bool __cdecl (*)(LPCTSTR lpValueName, DWORD *pValue, DWORD defaultValue))0x00456A30)
#define GetRegistryBoolValue ((bool __cdecl (*)(LPCTSTR lpValueName, bool *pValue, bool defaultValue))0x00456A90)
#define GetRegistryFloatValue ((bool __cdecl (*)(LPCTSTR lpValueName, double *value, double defaultValue))0x00456B10)
#define GetRegistryBinaryValue ((bool __cdecl (*)(LPCTSTR lpValueName, LPBYTE value, DWORD valueSize, LPBYTE defaultValue))0x00456B70)
#define GetRegistryStringValue ((bool __cdecl (*)(LPCTSTR lpValueName, LPTSTR value, DWORD maxSize, LPCTSTR defaultValue))0x00456BF0)
#define GetRegistryGuidValue ((bool __cdecl (*)(LPCTSTR lpValueName, GUID *value, GUID *defaultValue))0x00456C90)
#define SE_ReleaseBitmapResource ((void __thiscall (*)(BITMAP_RESOURCE *bmpRsrc))0x00456D30)
#define SE_LoadBitmapResource ((void __thiscall (*)(BITMAP_RESOURCE *bmpRsrc, LPCTSTR lpName))0x00456D70)
#define SE_DrawBitmap ((void __thiscall (*)(BITMAP_RESOURCE *bmpRsrc, HDC hdc, int32_t x, int32_t y))0x00456E40)
#define SE_UpdateBitmapPalette ((void __thiscall (*)(BITMAP_RESOURCE *bmpRsrc, HWND hWnd, HWND hSender))0x00456EB0)
#define SE_ChangeBitmapPalette ((void __thiscall (*)(BITMAP_RESOURCE *bmpRsrc, HWND hWnd))0x00456ED0)
#define SE_RegisterSetupWindowClass ((bool __cdecl (*)(void))0x00456F30)
#define SE_SetupWindowProc ((LRESULT __stdcall (*)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam))0x00456FA0)
#define SE_PassMessageToImage ((void __cdecl (*)(HWND hWnd, UINT uMsg, WPARAM wParam))0x004571E0)
#define UT_MemBlt ((void __cdecl (*)(BYTE *dstBuf, DWORD dstX, DWORD dstY, DWORD width, DWORD height, DWORD dstPitch, BYTE *srcBuf, DWORD srcX, DWORD srcY, DWORD srcPitch))0x00457210)
// clang-format on
