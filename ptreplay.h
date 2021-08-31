// ------------------------------------------------
// Protracker replay routine v1.5e
// ------------------------------------------------
// (C) 2007-2021 Franck "hitchhikr" Charlet.
// ------------------------------------------------

// ------------------------------------------------
// Includes
#include <nds/ipc.h>

// ------------------------------------------------
// Constants
#define MT_MSG_INIT 0x4444
#define MT_MSG_END 0x5555
#define MT_MSG_SETPOS 0x6666
#define MT_MSG_GETPOS 0x7777
#define MT_MSG_MASTERVOL 0x8888

// ------------------------------------------------
// IPC array
static inline int volatile * getIPC(void)
{
	return (int volatile *)(0x027FF000);
}

// ------------------------------------------------
// Structures
typedef struct
{
	volatile u16 row;
	volatile u16 pos;
} MT_POSITION, *LPMT_POSITION;

typedef struct
{
	u8 *module;
	volatile u16 command;
	volatile u16 master_volume;
    volatile u16 synchro_fx;
	MT_POSITION position;
} MT_MSG_CMD, *LPMT_MSG_CMD;
