// ------------------------------------------------
// Protracker/Fasttracker replay routine v1.5f
// ------------------------------------------------
// NDS driver
// ------------------------------------------------
// (C) 2007-2021 Franck "hitchhikr" Charlet.
// ------------------------------------------------

#ifndef _NDS_H_
#define _NDS_H_

// ------------------------------------------------
// Constants
#define MAX_CHANNELS 16

#define REPLAY_LITTLE_ENDIAN

// ------------------------------------------------
// Includes
#include "structs.h"

// ------------------------------------------------
// Functions
void mt_UpdateWaveForm(LPCHANNEL Channel);
void mt_UpdatePeriod(LPCHANNEL Channel, int frequency);
void mt_UpdateVolume(LPCHANNEL Channel, short volume);
void mt_UpdateLength(LPCHANNEL Channel, int length);
void mt_UpdateChannel(LPCHANNEL Channel, int retrigger);
void mt_RefreshChannel(LPCHANNEL Channel);
void mt_SetBpm(int Amount);
void mt_SetMasterVol(int volume);
void mt_DriverInit(void);
void mt_DriverStop(void);
u16 mt_get_u16(u16 dat);
u32 mt_get_u32(u32 dat);

void mt_Music(void);

#endif
