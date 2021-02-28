#ifndef T1M_SPECIFIC_SHED_H
#define T1M_SPECIFIC_SHED_H

// a place for odd functions that have no place to go yet

// clang-format off
#define WinVidSpinMessageLoop   ((void          (*)())0x00437AD0)
#define ShowFatalError          ((void          (*)(const char *message))0x0043D770)
#define S_ExitSystem            ((void          (*)(const char *message))0x0041E260)
#define S_PlayFMV               ((void          (*)(int32_t sequence, int32_t mode))0x0041D040)
#define WriteTombAtiSettings    ((void          (*)())0x00438B60)
#define sub_4380E0              ((void          (*)(int16_t *unk))0x004380E0)
#define InitialiseHardware      ((void          (*)())0x00408005)
#define mn_stop_ambient_samples ((void          (*)())0x0042B000)
#define mn_reset_sound_effects  ((void          (*)())0x0042A940)
#define mn_reset_ambient_loudness ((void        (*)())0x0042AFD0)
#define mn_update_sound_effects ((void          (*)())0x0042B080)
#define S_FinishInventory       ((void          (*)())0x0041CCC0)
#define CheckCheatMode          ((void          (*)())0x00438920)
#define DownloadTexturesToHardware  ((void      (*)(int16_t level_num))0x004084DE)
#define PaletteSetHardware      ((void          (*)())0x004087EA)
#define S_Wait                  ((int32_t       (*)(int32_t nframes))0x0041CD50)
#define sub_408E41              ((void          (*)())0x00408E41)
// clang-format on

#endif
