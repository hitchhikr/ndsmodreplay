// ------------------------------------------------
// Protracker/Fasttracker replay routine v1.5f
// ------------------------------------------------
// (C) 2007-2021 Franck "hitchhikr" Charlet.
// ------------------------------------------------

// ------------------------------------------------
// Includes
#include <nds.h>
#include "../../ptreplay.h"

// ------------------------------------------------
// External datas
extern uint8 RES_MODULE;

// ------------------------------------------------
// Variables
LPMT_MSG_CMD Replay;

// ------------------------------------------------
// Entry point
int main()
{
    Replay = (LPMT_MSG_CMD) getIPC();
    Replay->module = &RES_MODULE;
    Replay->position.pos = 0;               // Starting position
    Replay->position.row = 0;               // "
    Replay->master_volume = 127;            // From 0 to 127
    Replay->command = MT_MSG_INIT;
    // Wait for this command completion before triggering another one
    while(Replay->command == MT_MSG_INIT)
    {
    }

    // Also available:

    // Jump to a specified song position
    // (can be used for seeking or for using multi-song inside one single file).
    //
    // Replay->position.row = 0;
    // Replay->position.pos = 3;
    // Replay->command = MT_MSG_SETPOS;

    // Retrieve the currently replayed song position
    // (can be used for synchronization).
    //
    // Replay->command = MT_MSG_GETPOS;
    // Returns the position located in: Replay->position.row / Replay->position.pos

    // Set the master volume
    // (can be used for fade out, etc.).
    //
    // Replay->command = MT_MSG_MASTERVOL;
    // Replay->master_volume = xxxx;

    // Stop the replay routine
    //
    // Replay->command = MT_MSG_END;

    // -----------------------------------------
    // ARM9 loop
    while(1)
    {
    }
   
    return 0;
}
