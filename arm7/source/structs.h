// ------------------------------------------------
// Protracker/Fasttracker replay routine v1.5f
// ------------------------------------------------
// Structures
// ------------------------------------------------
// (C) 2007-2021 Franck "hitchhikr" Charlet.
// ------------------------------------------------

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

// ------------------------------------------------
// Structures
#ifdef REPLAY_LITTLE_ENDIAN
typedef struct
{
	u8 lo;
	u8 hi;
} SPLITW, *LPSPLITW;
#else
typedef struct
{
	u8 hi;
	u8 lo;
} SPLITW, *LPSPLITW;
#endif

typedef struct
{
	u8 datas;
	u8 cmd;
} CMD, *LPCMD;

typedef struct
{
	union
    {
		u16 n_note;
		SPLITW n_instrhi;
	};
	// instr/cmd
	union
    {
		u16 n_fx;
		SPLITW n_instrlo;
		CMD n_cmd;
	};
	u8 *n_start;
	u32 n_length;
	u32 n_loopstart;
	u32 n_replen;
	short n_period;
	u8 n_finetune;
	short n_volume;
	u8 n_toneportdirec;
	u8 n_toneportspeed;
	short n_wantedperiod;
	u8 n_vibratocmd;
	s8 n_vibratopos;
	u8 n_tremolocmd;
	s8 n_tremolopos;
	u8 n_wavecontrol;
	u8 n_glissfunk;
	u8 n_sampleoffset;
	u8 n_pattpos;
	u8 n_loopcount;
	u8 n_funkoffset;
	u8 *n_wavestart;
	u8 n_index;
} CHANNEL, *LPCHANNEL;

typedef struct
{
	u16 i_length;
	u8 i_finetune;
	u8 i_volume;
	u16 i_repeat;
	u16 i_replen;
} INSTRUMENT, *LPINSTRUMENT;

#endif
