// ------------------------------------------------
// Protracker replay routine v1.5e
// ------------------------------------------------
// NDS driver
// ------------------------------------------------
// (C) 2007-2021 Franck "hitchhikr" Charlet.
// ------------------------------------------------

// ------------------------------------------------
// Includes
#include <nds.h>
#include <nds/system.h>
#include "../../ptreplay.h"
#include "driver_nds.h"
#include "structs.h"

// ------------------------------------------------
// Constants
// The defines of the libnds aren't accurate enough for us,
// we need custom ones to have control of the channels volumes independently of the channels status
#define SOUND_CR1 (*(vuint8 *) 0x04000501)
#define SCHANNEL_CONTROL(n) (*(vuint8 *) (0x04000403 + ((n) << 4)))
#define SOUND_CTRL_REPEAT BIT(3)
#define SOUND_CTRL_ONE_SHOT BIT(4)
#define SOUND_CTRL_FORMAT_8BIT (0 << 5)
#define SOUND_CTRL_FORMAT_16BIT (1 << 5)
#define SOUND_CTRL_FORMAT_ADPCM (2 << 5)
#define SOUND_CTRL_FORMAT_PSG (3 << 5)
#define SCHANNEL_CTRL_ENABLE BIT(7)

#define AMIGA_CLOCK_PAL 614361

// ------------------------------------------------
// Variables
u8 mt_def_panning[MAX_CHANNELS] =
{
    48, 80, 80, 48, 48, 80, 80, 48,
    48, 80, 80, 48, 48, 80, 80, 48
};

// ------------------------------------------------
// Set a channel waveform address and repeat point
void mt_UpdateWaveForm(LPCHANNEL Channel)
{
    SCHANNEL_SOURCE(Channel->n_index) = (u32) Channel->n_start;
    SCHANNEL_REPEAT_POINT(Channel->n_index) = Channel->n_loopstart >> 2;
}

// ------------------------------------------------
// Set a channel period
void mt_UpdatePeriod(LPCHANNEL Channel, int Frequency)
{
    if(Frequency)
    {
        SCHANNEL_TIMER(Channel->n_index) = 65536 - (AMIGA_CLOCK_PAL * Frequency >> 17);
    }
}

// ------------------------------------------------
// Set a channel volume
void mt_UpdateVolume(LPCHANNEL Channel, short Volume)
{
    Volume <<= 1;
    if(Volume > 127)
    {
        Volume = 127;
    }
    SCHANNEL_VOL(Channel->n_index) = Volume;
}

// ------------------------------------------------
// Set a channel waveform length
void mt_UpdateLength(LPCHANNEL Channel, int Length)
{
    SCHANNEL_LENGTH(Channel->n_index) = Length >> 2;
}

// ------------------------------------------------
// Trigger a channel
void mt_UpdateChannel(LPCHANNEL Channel, int Retrigger)
{
    if(Retrigger)
    {
        SCHANNEL_CONTROL(Channel->n_index) = 0;
    }
    SCHANNEL_PAN(Channel->n_index) = mt_def_panning[Channel->n_index];
    if(Channel->n_period)
    {
        SCHANNEL_CONTROL(Channel->n_index) = SCHANNEL_CTRL_ENABLE | (Channel->n_replen == 0 ? SOUND_CTRL_ONE_SHOT : SOUND_CTRL_REPEAT) | SOUND_CTRL_FORMAT_8BIT;
    }
}

// ------------------------------------------------
// Refresh a channel
void mt_RefreshChannel(LPCHANNEL Channel)
{
    SCHANNEL_CR(Channel->n_index) = 0;
}

// ------------------------------------------------
// Set the number of beats per minute
void mt_SetBpm(int Amount)
{
    TIMER_DATA(0) = 0x10000 - (0x140000 / Amount);
}

// ------------------------------------------------
// Set the master volume
void mt_SetMasterVol(int Volume)
{
    REG_MASTER_VOLUME = SOUND_VOL(Volume & 0x7f);
}

// ------------------------------------------------
// Init the driver
void mt_DriverInit(void)
{
    REG_POWERCNT |= POWER_SOUND;
    irqInit();
	fifoInit();
    TIMER_CR(0) = 0;
    irqSet(IRQ_TIMER0, mt_Music);
    irqEnable(IRQ_TIMER0);
    TIMER_CR(0) = TIMER_DIV_64 | TIMER_IRQ_REQ | TIMER_ENABLE;
    SOUND_CR1 = BIT(7);
}

// ------------------------------------------------
// Stop the driver
void mt_DriverStop(void)
{
    SOUND_CR1 = 0;
}

// ------------------------------------------------
// Retrieve a 16 bits word
u16 mt_get_u16(u16 dat)
{
    return(((dat & 0xff00) >> 8) | ((dat & 0xff) << 8));
}

// ------------------------------------------------
// Retrieve a 32 bits word
u32 mt_get_u32(u32 dat)
{
    return(((dat & 0xff00) >> 8) | ((dat & 0xff) << 8) |
           ((dat & 0xff000000) >> 8) | ((dat & 0xff0000) << 8));
}
