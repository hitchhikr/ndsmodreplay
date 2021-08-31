// ------------------------------------------------
// Protracker replay routine v1.5e
// ------------------------------------------------
// Generic replayer
// ------------------------------------------------
// (C) 2007-2021 Franck "hitchhikr" Charlet.
// ------------------------------------------------

// ------------------------------------------------
// Includes
#include <stdio.h>
#include "../../ptreplay.h"
#include "driver_nds.h"

// ------------------------------------------------
// Constants
#define MAX_SAMPLES 31
#define MAX_NOTES 36

// ------------------------------------------------
// Variables
//extern uint8 RES_MODULE;
LPMT_MSG_CMD mt_infos;
u8 *mt_SongDataPtr;
int mt_speed;
volatile int mt_counter;
u8 mt_SongPos;
u8 mt_PattDelTime2;
u8 mt_play;
u8 mt_PBreakPos;
u8 mt_PosJumpFlag;
u8 mt_PBreakFlag;
u8 mt_LowMask;
u8 mt_PattDelTime;
u8 mt_PattDelTime2;
u16 mt_PatternPos;
u32 mt_channels;
u8 *mt_SampleStarts[MAX_SAMPLES];
CHANNEL mt_chantemp[MAX_CHANNELS];
u8 mt_FunkTable[] =
{
    0, 5, 6, 7, 8, 10, 11, 13, 16, 19, 22, 26, 32, 43, 64, 128
};
u8 mt_VibratoTable[]=
{
    0, 24, 49, 74, 97, 120, 141, 161,
    180, 197, 212, 224, 235, 244, 250, 253,
    255, 253, 250, 244, 235, 224, 212, 197,
    180, 161, 141, 120, 97, 74, 49, 24
};
short mt_PeriodTable[] =
{
    856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480, 453,
    428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240, 226,
    214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120, 113,

    850, 802, 757, 715, 674, 637, 601, 567, 535, 505, 477, 450,
    425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 239, 225,
    213, 201, 189, 179, 169, 159, 150, 142, 134, 126, 119, 113,

    844, 796, 752, 709, 670, 632, 597, 563, 532, 502, 474, 447,
    422, 398, 376, 355, 335, 316, 298, 282, 266, 251, 237, 224,
    211, 199, 188, 177, 167, 158, 149, 141, 133, 125, 118, 112,

    838, 791, 746, 704, 665, 628, 592, 559, 528, 498, 470, 444,
    419, 395, 373, 352, 332, 314, 296, 280, 264, 249, 235, 222,
    209, 198, 187, 176, 166, 157, 148, 140, 132, 125, 118, 111,

    832, 785, 741, 699, 660, 623, 588, 555, 524, 495, 467, 441,
    416, 392, 370, 350, 330, 312, 294, 278, 262, 247, 233, 220,
    208, 196, 185, 175, 165, 156, 147, 139, 131, 124, 117, 110,

    826, 779, 736, 694, 655, 619, 584, 551, 520, 491, 463, 437,
    413, 390, 368, 347, 328, 309, 292, 276, 260, 245, 232, 219,
    206, 195, 184, 174, 164, 155, 146, 138, 130, 123, 116, 109,

    820, 774, 730, 689, 651, 614, 580, 547, 516, 487, 460, 434,
    410, 387, 365, 345, 325, 307, 290, 274, 258, 244, 230, 217,
    205, 193, 183, 172, 163, 154, 145, 137, 129, 122, 115, 109,

    814, 768, 725, 684, 646, 610, 575, 543, 513, 484, 457, 431,
    407, 384, 363, 342, 323, 305, 288, 272, 256, 242, 228, 216,
    204, 192, 181, 171, 161, 152, 144, 136, 128, 121, 114, 108,

    907, 856, 808, 762, 720, 678, 640, 604, 570, 538, 508, 480,
    453, 428, 404, 381, 360, 339, 320, 302, 285, 269, 254, 240,
    226, 214, 202, 190, 180, 170, 160, 151, 143, 135, 127, 120,

    900, 850, 802, 757, 715, 675, 636, 601, 567, 535, 505, 477,
    450, 425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 238,
    225, 212, 200, 189, 179, 169, 159, 150, 142, 134, 126, 119,

    894, 844, 796, 752, 709, 670, 632, 597, 563, 532, 502, 474,
    447, 422, 398, 376, 355, 335, 316, 298, 282, 266, 251, 237,
    223, 211, 199, 188, 177, 167, 158, 149, 141, 133, 125, 118,

    887, 838, 791, 746, 704, 665, 628, 592, 559, 528, 498, 470,
    444, 419, 395, 373, 352, 332, 314, 296, 280, 264, 249, 235,
    222, 209, 198, 187, 176, 166, 157, 148, 140, 132, 125, 118,

    881, 832, 785, 741, 699, 660, 623, 588, 555, 524, 494, 467,
    441, 416, 392, 370, 350, 330, 312, 294, 278, 262, 247, 233,
    220, 208, 196, 185, 175, 165, 156, 147, 139, 131, 123, 117,

    875, 826, 779, 736, 694, 655, 619, 584, 551, 520, 491, 463,
    437, 413, 390, 368, 347, 328, 309, 292, 276, 260, 245, 232,
    219, 206, 195, 184, 174, 164, 155, 146, 138, 130, 123, 116,

    868, 820, 774, 730, 689, 651, 614, 580, 547, 516, 487, 460,
    434, 410, 387, 365, 345, 325, 307, 290, 274, 258, 244, 230,
    217, 205, 193, 183, 172, 163, 154, 145, 137, 129, 122, 115,

    862, 814, 768, 725, 684, 646, 610, 575, 543, 513, 484, 457,
    431, 407, 384, 363, 342, 323, 305, 288, 272, 256, 242, 228,
    216, 203, 192, 181, 171, 161, 152, 144, 136, 128, 121, 114
};
u32 mt_tags[] =
{
    '.K.M', 'NHC5', 'NHC6', 'NHC7',
    'NHC8', 'NHC9', 'HC01', 'HC11',
    'HC21', 'HC31', 'HC41', 'HC51',
    'HC61'
};

// ------------------------------------------------
// Functions
void mt_NoNewAllChannels(void);
void mt_PlayVoice(volatile LPCHANNEL Channel, volatile u32 patt_dat);
void mt_CheckEfx(LPCHANNEL Channel);
void mt_CheckMoreEfx(LPCHANNEL Channel);
void mt_SetTonePorta(LPCHANNEL Channel);
void mt_SetFineTune(LPCHANNEL Channel);
void mt_UpdateFunk(LPCHANNEL Channel);
void mt_E_Commands(LPCHANNEL Channel);
void mt_SampleOffset(LPCHANNEL Channel);
void mt_PositionJump(LPCHANNEL Channel);
void mt_PatternBreak(LPCHANNEL Channel);
void mt_SetSpeed(LPCHANNEL Channel);
void mt_VolumeChange(LPCHANNEL Channel);
void mt_CheckEfx(LPCHANNEL Channel);
void mt_Arpeggio(LPCHANNEL Channel);
void mt_JumpLoop(LPCHANNEL Channel);
void mt_SetVibratoControl(LPCHANNEL Channel);
void mt_SetTremoloControl(LPCHANNEL Channel);
void mt_SetGlissControl(LPCHANNEL Channel);
void mt_NoteCut(LPCHANNEL Channel);
void mt_PatternDelay(LPCHANNEL Channel);
void mt_RetrigNote(LPCHANNEL Channel);
void mt_NoteDelay(LPCHANNEL Channel);
void mt_FinePortaUp(LPCHANNEL Channel);
void mt_PortaUp(LPCHANNEL Channel);
void mt_FinePortaDown(LPCHANNEL Channel);
void mt_PortaDown(LPCHANNEL Channel);
void mt_VolumeSlide(LPCHANNEL Channel);
void mt_VolSlideUp(LPCHANNEL Channel, int Level);
void mt_VolSlideDown(LPCHANNEL Channel, int Level);
void mt_VolumeFineUp(LPCHANNEL Channel);
void mt_VolumeFineDown(LPCHANNEL Channel);
void mt_FunkIt(LPCHANNEL Channel);
void mt_TonePlusVolSlide(LPCHANNEL Channel);
void mt_TonePortamento(LPCHANNEL Channel);
void mt_TonePortNoChange(LPCHANNEL Channel);
void mt_TonePortaDown(LPCHANNEL Channel);
void mt_TonePortaUp(LPCHANNEL Channel);
void mt_TonePortaSetPer(LPCHANNEL Channel);
void mt_Vibrato2(LPCHANNEL Channel);
void mt_Vibrato(LPCHANNEL Channel);
void mt_VibratoPlusVolSlide(LPCHANNEL Channel);
void mt_Tremolo(LPCHANNEL Channel);
int mt_Init(void);
void mt_SetPosition(void);
void mt_End(void);

// ------------------------------------------------
// Init the replay
int mt_Init(void)
{
    int i;
    int max_patterns;
    u8 *mt_data8;
    u16 *mt_data16;
    u32 *mt_data32;
    u32 *Channel_Dat;
    u16 mt_smplen;

    mt_SongDataPtr = mt_infos->module;

    mt_channels = 0;
    mt_data32 = (u32 *) &mt_SongDataPtr[1080];
    for(i = 0; i < sizeof(mt_tags) / sizeof(u32); i++)
    {
        if(mt_tags[i] == *mt_data32)
        {
            mt_channels = i + 4;
            break;
        }
    }
    if(!mt_channels) return(FALSE);

    // Find the highest pattern number
    max_patterns = -1;
    for(i = 0; i < 128; i++)
    {
        if(mt_SongDataPtr[952 + i] > max_patterns) max_patterns = mt_SongDataPtr[952 + i];
    }
    max_patterns++;
    max_patterns *= (64 * mt_channels * 4);
    max_patterns += 1084;

    // Retrieve the samples addresses
    mt_data8 = &mt_SongDataPtr[max_patterns];
    for(i = 0; i < MAX_SAMPLES; i++)
    {
        mt_data16 = (u16 *) &mt_SongDataPtr[42 + (i * 30)];
        mt_smplen = mt_get_u16(*mt_data16);
        mt_data16 = (u16 *) mt_data8;
        if(mt_smplen) *mt_data16 = 0;
        mt_SampleStarts[i] = mt_data8;
        mt_data8 += ((u32) mt_smplen) << 1;
    }
    for(i = 0; i < mt_channels; i++)
    {
        Channel_Dat = (u32 *) &mt_chantemp[i];
        *Channel_Dat = 0;
        mt_chantemp[i].n_index = i;
        mt_RefreshChannel(&mt_chantemp[i]);
    }
    mt_LowMask = 0xff;
    mt_speed = 6;
    mt_SetPosition();
    mt_SetBpm(125);
    mt_SetMasterVol(mt_infos->master_volume);

    mt_play = TRUE;
    return(TRUE);
}

// ------------------------------------------------
// Set the current position to play
void mt_SetPosition(void)
{
    mt_SongPos = mt_infos->position.pos;
    mt_SongPos &= 0x7f;
    if(mt_SongPos >= mt_SongDataPtr[950]) mt_SongPos = 0;
    mt_PatternPos = mt_infos->position.row;
    if(mt_PatternPos >= 64) mt_PatternPos = 0;
    mt_PatternPos *= (mt_channels * 4);
    mt_counter = 0;
}

// ------------------------------------------------
// Stop the replay
void mt_End(void)
{
    mt_play = FALSE;
    mt_DriverStop();
}

// ------------------------------------------------
// Play the tune (spaghetti code here i come)
void mt_Music(void)
{
    int i;
    u8 *song_datas;
    u8 *patt_datas;
    short cur_fx;
    volatile u32 pattern_offset;

    if(!mt_play)
    {
        return;
    }
    mt_counter++;
    if(mt_counter < mt_speed)
    {
        goto mt_NoNewNote;
    }
    mt_counter = 0;
    if(!mt_PattDelTime2)
    {
        goto mt_GetNewNote;
    }
    mt_NoNewAllChannels();
    goto mt_dskip;

mt_NoNewNote:
    mt_NoNewAllChannels();
    goto mt_NoNewPosYet;

mt_GetNewNote:
    song_datas = mt_SongDataPtr + 952;
    patt_datas = mt_SongDataPtr + 1084;
    pattern_offset = (song_datas[mt_SongPos] * (64 * mt_channels * 4)) + mt_PatternPos;

    for(i = 0; i < mt_channels; i++)
    {
        mt_PlayVoice(&mt_chantemp[i], mt_get_u32(*((u32 *) &patt_datas[pattern_offset])));
        pattern_offset += 4;
    }

    // Now that we have all the infos update them bloody channels
    for(i = 0; i < mt_channels; i++)
    {
        if(mt_chantemp[i].n_note & 0xfff)
        {
            cur_fx = mt_chantemp[i].n_fx;
            // Note delay
            if((cur_fx & 0x0ff0) != 0x0ed0)
            {
                // Don't retrigger
                cur_fx = mt_chantemp[i].n_cmd.cmd & 0xf;
                mt_UpdateChannel(&mt_chantemp[i], (cur_fx != 3 && cur_fx != 5) ? TRUE : FALSE);
            }
        }
    }

mt_dskip:
    mt_PatternPos += (mt_channels * 4);
    if(mt_PattDelTime)
    {
        mt_PattDelTime2 = mt_PattDelTime;
        mt_PattDelTime = 0;
    }
    if(mt_PattDelTime2)
    {
        mt_PattDelTime2--;
        if(mt_PattDelTime2)
        {
            mt_PatternPos -= (mt_channels * 4);
        }
    }
    if(mt_PBreakFlag)
    {
        mt_PBreakFlag = FALSE;
        mt_PatternPos = mt_PBreakPos * (mt_channels * 4);
        mt_PBreakPos = 0;
    }
    if(mt_PatternPos < (64 * mt_channels * 4))
    {
        goto mt_NoNewPosYet;
    }
mt_NextPosition:
    mt_PatternPos = mt_PBreakPos * (mt_channels * 4);
    mt_PBreakPos = 0;
    mt_PosJumpFlag = FALSE;
    mt_SongPos++;
    mt_SongPos &= 0x7f;
    if(mt_SongPos >= mt_SongDataPtr[950])
    {
        mt_SongPos = 0;
    }
mt_NoNewPosYet:
    if(mt_PosJumpFlag)
    {
        goto mt_NextPosition;
    }
}

void mt_PlayVoice(volatile LPCHANNEL Channel, volatile u32 patt_dat)
{
    volatile u32 *Channel_Dat = (u32 *) Channel;
    volatile u16 instr = 0;
    u16 note;
    int i;
    LPINSTRUMENT instr_datas;

    if(!*Channel_Dat)
    {
        mt_UpdatePeriod(Channel, Channel->n_period);
    }
    *Channel_Dat = patt_dat;
    if((Channel->n_fx & 0xf00) == 0x800)
    {
        mt_infos->synchro_fx = Channel->n_fx & 0xfff;
    }
    else
    {
        mt_infos->synchro_fx = 0;
    }
    instr = (Channel->n_instrhi.hi & 0xf0) | ((Channel->n_instrlo.hi & 0xf0) >> 4);
    if(!instr)
    {
        goto mt_SetRegs;
    }
    Channel->n_start = mt_SampleStarts[instr - 1];
    instr *= 30;
    instr_datas = (LPINSTRUMENT) (mt_SongDataPtr + 12 + instr);
    Channel->n_length = mt_get_u16(instr_datas->i_length) << 1;
    Channel->n_finetune = instr_datas->i_finetune;
    Channel->n_volume = instr_datas->i_volume;
    if(instr_datas->i_repeat)
    {
        Channel->n_loopstart = mt_get_u16(instr_datas->i_repeat) << 1;
        Channel->n_wavestart = Channel->n_start + Channel->n_loopstart;
        Channel->n_replen = mt_get_u16(instr_datas->i_replen) << 1;
        Channel->n_length = Channel->n_replen;
    }
    else
    {
        Channel->n_loopstart = 0;
        Channel->n_wavestart = Channel->n_start;
        Channel->n_replen = mt_get_u16(instr_datas->i_replen) << 1;
    }
    if(Channel->n_replen <= 2)
    {
        Channel->n_replen = 0;
    }
    mt_UpdateLength(Channel, Channel->n_length);
    if((Channel->n_cmd.cmd & 0xf) != 0xc)
    {
        mt_UpdateVolume(Channel, Channel->n_volume);
    }
    // Set the waveform infos
    mt_UpdateWaveForm(Channel);

mt_SetRegs:
    if(!(Channel->n_note & 0xfff))
    {
        mt_CheckMoreEfx(Channel);
        return;
    }
    if((Channel->n_fx & 0x0ff0) == 0x0e50)
    {
        goto mt_DoSetFineTune;
    }
    switch(Channel->n_cmd.cmd & 0xf)
    {
        case 3:
        case 5:
            mt_SetTonePorta(Channel);
            mt_CheckMoreEfx(Channel);
            return;

        case 9:
            mt_CheckMoreEfx(Channel);

        default:
            goto mt_SetPeriod;
    }

mt_DoSetFineTune:
    mt_SetFineTune(Channel);

mt_SetPeriod:
    note = Channel->n_note & 0xfff;

    // Don't play the note if we can't find it in the table
    Channel->n_period = 0;
    for(i = 0; i < MAX_NOTES; i++)
    {
        if(note == mt_PeriodTable[i])
        {
            Channel->n_period = mt_PeriodTable[i + (Channel->n_finetune * MAX_NOTES)];
            break;
        }
    }
    if(!(Channel->n_wavecontrol & 4))
    {
        Channel->n_vibratopos = 0;
    }
    if(!(Channel->n_wavecontrol & 0x40))
    {
        Channel->n_tremolopos = 0;
    }
    mt_UpdatePeriod(Channel, Channel->n_period);
    mt_CheckMoreEfx(Channel);
}

void mt_CheckMoreEfx(LPCHANNEL Channel)
{
    mt_UpdateFunk(Channel);
    switch(Channel->n_cmd.cmd & 0xf)
    {
        case 9:
            mt_SampleOffset(Channel);
            break;

        case 0xb:
            mt_PositionJump(Channel);
            break;

        case 0xd:
            mt_PatternBreak(Channel);
            break;

        case 0xe:
            mt_E_Commands(Channel);
            break;

        case 0xf:
            mt_SetSpeed(Channel);
            break;

        case 0xc:
            mt_VolumeChange(Channel);
            break;

        default:
            mt_UpdatePeriod(Channel, Channel->n_period);
            break;
    }
}

void mt_E_Commands(LPCHANNEL Channel)
{
    switch((Channel->n_cmd.datas & 0xf0) >> 4)
    {
        case 0:
            break;        /* we don't handle this one */

        case 1:
            mt_FinePortaUp(Channel);
            break;

        case 2:
            mt_FinePortaDown(Channel);
            break;

        case 3:
            mt_SetGlissControl(Channel);
            break;

        case 4:
            mt_SetVibratoControl(Channel);
            break;

        case 5:
            mt_SetFineTune(Channel);
            break;

        case 6:
            mt_JumpLoop(Channel);
            break;

        case 7:
            mt_SetTremoloControl(Channel);
            break;

        case 9:
            mt_RetrigNote(Channel);
            break;

        case 0xa:
            mt_VolumeFineUp(Channel);
            break;

        case 0xb:
            mt_VolumeFineDown(Channel);
            break;

        case 0xc:
            mt_NoteCut(Channel);
            break;

        case 0xd:
            mt_NoteDelay(Channel);
            break;

        case 0xe:
            mt_PatternDelay(Channel);
            break;

        case 0xf:
            mt_FunkIt(Channel);
            break;
    }
}

void mt_JumpLoop(LPCHANNEL Channel)
{
    u8 dat = Channel->n_cmd.datas & 0xf;
    if(!mt_counter)
    {
        if(!dat)
        {
            Channel->n_pattpos = mt_PatternPos / (mt_channels * 4);
            return;
        }
        if(!Channel->n_loopcount)
        {
            Channel->n_loopcount = dat;
        }
        else
        {
            Channel->n_loopcount--;
            if(!Channel->n_loopcount)
            {
                return;
            }
        }
        mt_PBreakPos = Channel->n_pattpos;
        mt_PBreakFlag = TRUE;
    }
}

void mt_SetGlissControl(LPCHANNEL Channel)
{
    Channel->n_glissfunk &= 0xf0;
    Channel->n_glissfunk |= (Channel->n_cmd.datas & 0xf);
}

void mt_SetVibratoControl(LPCHANNEL Channel)
{
    Channel->n_wavecontrol &= 0xf0;
    Channel->n_wavecontrol |= (Channel->n_cmd.datas & 0xf);
}

void mt_SetTremoloControl(LPCHANNEL Channel)
{
    Channel->n_wavecontrol &= 0xf;
    Channel->n_wavecontrol |= (Channel->n_cmd.datas & 0xf) << 4;
}

void mt_NoteCut(LPCHANNEL Channel)
{
    if(mt_counter == (Channel->n_cmd.datas & 0xf))
    {
        Channel->n_volume = 0;
        mt_UpdateVolume(Channel, Channel->n_volume);
    }
}

void mt_RetrigNote(LPCHANNEL Channel)
{
    if(Channel->n_cmd.datas & 0xf)
    {
        if(!mt_counter)
        {
            if(Channel->n_note & 0xfff)
            {
                return;
            }
        }
        if(mt_counter % (Channel->n_cmd.datas & 0xf))
        {
            return;
        }
        mt_UpdateChannel(Channel, TRUE);
    }   
}

void mt_NoteDelay(LPCHANNEL Channel)
{
    if(mt_counter == (Channel->n_cmd.datas & 0xf))
    {
        if(Channel->n_note & 0xfff)
        {
            mt_UpdateChannel(Channel, TRUE);
        }
    }
}

void mt_PatternDelay(LPCHANNEL Channel)
{
    if(!mt_counter)
    {
        if(!mt_PattDelTime2)
        {
            mt_PattDelTime = (Channel->n_cmd.datas & 0xf) + 1;
        }
    }
}

void mt_SampleOffset(LPCHANNEL Channel)
{
    u32 new_offset;

    if(Channel->n_cmd.datas)
    {
        Channel->n_sampleoffset = Channel->n_cmd.datas;
    }
    new_offset = Channel->n_sampleoffset << 7;
    if(new_offset < Channel->n_length)
    {
        Channel->n_length -= new_offset;
        Channel->n_start += new_offset;
    }
    else
    {
        Channel->n_length = 0;
    }
    mt_UpdateWaveForm(Channel);
    mt_UpdateLength(Channel, Channel->n_length);
}

void mt_PositionJump(LPCHANNEL Channel)
{
    mt_SongPos = Channel->n_cmd.datas - 1;
    mt_PBreakPos = 0;
    mt_PosJumpFlag = TRUE;
}

void mt_PatternBreak(LPCHANNEL Channel)
{
    u16 new_patt_pos = ((Channel->n_cmd.datas >> 4) * 10) + (Channel->n_cmd.datas & 0xf);
    if(new_patt_pos > 63) mt_PBreakPos = 0;
    else mt_PBreakPos = new_patt_pos;
    mt_PosJumpFlag = TRUE;
}

void mt_SetSpeed(LPCHANNEL Channel)
{
    if(Channel->n_cmd.datas)
    {
        if(Channel->n_cmd.datas < 32)
        {
            mt_counter = 0;
            mt_speed = Channel->n_cmd.datas;
        }
        else
        {
            mt_SetBpm(Channel->n_cmd.datas);
        }
    }
}

void mt_VolumeChange(LPCHANNEL Channel)
{
    int new_volume = Channel->n_cmd.datas;

    if(new_volume > 64)
    {
        Channel->n_volume = 64;
    }
    else
    {
        Channel->n_volume = new_volume;
    }
    mt_UpdateVolume(Channel, Channel->n_volume);
}

void mt_FunkIt(LPCHANNEL Channel)
{
    int new_funk;
    
    if(!mt_counter)
    {
        new_funk = (Channel->n_cmd.datas & 0x0f) << 4;
        Channel->n_glissfunk &= 0xf;
        Channel->n_glissfunk |= new_funk;
        if(new_funk)
        {
            mt_UpdateFunk(Channel);
        }
    }
}

void mt_UpdateFunk(LPCHANNEL Channel)
{
    u8 *funk_start;
    u8 new_funk = Channel->n_glissfunk >> 4;

    if(new_funk)
    {
        new_funk = mt_FunkTable[new_funk];
        Channel->n_funkoffset += new_funk;
        if(Channel->n_funkoffset & 0x80)
        {
            Channel->n_funkoffset = 0;
            funk_start = Channel->n_start + Channel->n_loopstart;
            funk_start += Channel->n_replen;
            Channel->n_wavestart++;
            if(Channel->n_wavestart >= funk_start)
            {
                Channel->n_wavestart = Channel->n_start + Channel->n_loopstart;
            }
            new_funk = 0xff;
            new_funk -= *Channel->n_wavestart;
            *Channel->n_wavestart = new_funk;
        }
    }
}

void mt_SetTonePorta(LPCHANNEL Channel)
{
    int i;
    short *fine_notetable;
    short note = Channel->n_note & 0xfff;
    
    fine_notetable = &mt_PeriodTable[Channel->n_finetune * MAX_NOTES];
    for(i = 0; i < MAX_NOTES; i++)
    {
        if(fine_notetable[i] <= note)
        {
            break;
        }
    }
    if(Channel->n_finetune & 8)
    {
        if(i)
        {
            i--;
        }
    }
    Channel->n_wantedperiod = fine_notetable[i];
    Channel->n_toneportdirec = FALSE;
    if(Channel->n_period == Channel->n_wantedperiod)
    {
        Channel->n_wantedperiod = 0;
    }
    if(Channel->n_period > Channel->n_wantedperiod)
    {
        Channel->n_toneportdirec = TRUE;
    }
}

void mt_NoNewAllChannels(void)
{
    int i;

    for(i = 0; i < mt_channels; i++)
    {
        mt_CheckEfx(&mt_chantemp[i]);
    }
}

void mt_SetFineTune(LPCHANNEL Channel)
{
    Channel->n_finetune = Channel->n_cmd.datas & 0x0f;
}

void mt_CheckEfx(LPCHANNEL Channel)
{
    mt_UpdateFunk(Channel);
    if(!(Channel->n_fx & 0xfff))
    {
        mt_UpdatePeriod(Channel, Channel->n_period);
    }
    else
    {
        switch(Channel->n_cmd.cmd & 0xf)
        {
            case 0:
                mt_Arpeggio(Channel);
                break;

            case 1:
                mt_PortaUp(Channel);
                break;

            case 2:
                mt_PortaDown(Channel);
                break;

            case 3:
                mt_TonePortamento(Channel);
                break;

            case 4:
                mt_Vibrato(Channel);
                break;

            case 5:
                mt_TonePlusVolSlide(Channel);
                break;

            case 6:
                mt_VibratoPlusVolSlide(Channel);
                break;

            case 0xe:
                mt_E_Commands(Channel);
                break;

            case 7:
                mt_UpdatePeriod(Channel, Channel->n_period);
                mt_Tremolo(Channel);
                break;

            case 0xa:
                mt_UpdatePeriod(Channel, Channel->n_period);
                mt_VolumeSlide(Channel);
                break;

            default:
                mt_UpdatePeriod(Channel, Channel->n_period);
                break;
        }
    }
}

void mt_Arpeggio(LPCHANNEL Channel)
{
    short *fine_notetable;
    int i;
    u32 cur_arp = mt_counter % 3;

    switch(cur_arp)
    {
        case 0:
            mt_UpdatePeriod(Channel, Channel->n_period);
            return;

        case 2:
            cur_arp = Channel->n_cmd.datas & 0xf;
            break;

        default:
            cur_arp = Channel->n_cmd.datas >> 4;
            break;
    }
    fine_notetable = &mt_PeriodTable[(Channel->n_finetune * MAX_NOTES)];
    for(i = 0; i < MAX_NOTES; i++)
    {
        if(*fine_notetable <= Channel->n_period)
        {
            mt_UpdatePeriod(Channel, fine_notetable[cur_arp]);
            break;
        }
        fine_notetable++;
    }
}

void mt_TonePlusVolSlide(LPCHANNEL Channel)
{
    mt_TonePortNoChange(Channel);
    mt_VolumeSlide(Channel);
}

void mt_FinePortaUp(LPCHANNEL Channel)
{
    if(!mt_counter)
    {
        mt_LowMask = 0xf;
        mt_PortaUp(Channel);
    }
}

void mt_PortaUp(LPCHANNEL Channel)
{
    u32 new_per = Channel->n_cmd.datas & mt_LowMask;

    mt_LowMask = 0xff;
    Channel->n_period -= new_per;
    if(Channel->n_period < 113)
    {
        Channel->n_period = 113;
    }
    mt_UpdatePeriod(Channel, Channel->n_period);
}

void mt_FinePortaDown(LPCHANNEL Channel)
{
    if(!mt_counter)
    {
        mt_LowMask = 0xf;
        mt_PortaDown(Channel);
    }
}

void mt_PortaDown(LPCHANNEL Channel)
{
    u32 new_per = Channel->n_cmd.datas & mt_LowMask;

    mt_LowMask = 0xff;
    Channel->n_period += new_per;
    if(Channel->n_period > 856)
    {
        Channel->n_period = 856;
    }
    mt_UpdatePeriod(Channel, Channel->n_period);
}

void mt_TonePortamento(LPCHANNEL Channel)
{
    if(Channel->n_cmd.datas)
    {
        Channel->n_toneportspeed = Channel->n_cmd.datas;
        Channel->n_cmd.datas = 0;
    }
    mt_TonePortNoChange(Channel);
}

void mt_TonePortNoChange(LPCHANNEL Channel)
{
    if(Channel->n_wantedperiod)
    {
        if(Channel->n_toneportdirec)
        {
            mt_TonePortaUp(Channel);
        }
        else
        {
            mt_TonePortaDown(Channel);
        }
    }
}

void mt_TonePortaDown(LPCHANNEL Channel)
{
    Channel->n_period += Channel->n_toneportspeed;
    if(Channel->n_period >= Channel->n_wantedperiod)
    {
        Channel->n_period = Channel->n_wantedperiod;
        Channel->n_wantedperiod = 0;
    }
    mt_TonePortaSetPer(Channel);
}

void mt_TonePortaUp(LPCHANNEL Channel)
{
    Channel->n_period -= Channel->n_toneportspeed;
    if(Channel->n_period <= Channel->n_wantedperiod)
    {
        Channel->n_period = Channel->n_wantedperiod;
        Channel->n_wantedperiod = 0;
    }
    mt_TonePortaSetPer(Channel);
}

void mt_TonePortaSetPer(LPCHANNEL Channel)
{
    short *fine_notetable;
    int i;

    if(Channel->n_glissfunk & 0xf)
    {
        fine_notetable = &mt_PeriodTable[Channel->n_finetune * MAX_NOTES];
        for(i = 0; i < MAX_NOTES; i++)
        {
            if(fine_notetable[i] <= Channel->n_period)
            {
                mt_UpdatePeriod(Channel, fine_notetable[i]);
                break;
            }
        }
    }
    else
    {
        mt_UpdatePeriod(Channel, Channel->n_period);
    }
}

void mt_VolumeSlide(LPCHANNEL Channel)
{
    if(Channel->n_cmd.datas >> 4)
    {
        mt_VolSlideUp(Channel, Channel->n_cmd.datas >> 4);
    }
    else
    {
        mt_VolSlideDown(Channel, Channel->n_cmd.datas & 0xf);
    }
}

void mt_VolumeFineUp(LPCHANNEL Channel)
{
    if(!mt_counter)
    {
        mt_VolSlideUp(Channel, Channel->n_cmd.datas & 0xf);
    }
}

void mt_VolumeFineDown(LPCHANNEL Channel)
{
    if(!mt_counter)
    {
        mt_VolSlideDown(Channel, Channel->n_cmd.datas & 0xf);
    }
}

void mt_VolSlideUp(LPCHANNEL Channel, int Level)
{
    Channel->n_volume += Level;
    if(Channel->n_volume > 64)
    {
        Channel->n_volume = 64;
    }
    mt_UpdateVolume(Channel, Channel->n_volume);
}

void mt_VolSlideDown(LPCHANNEL Channel, int Level)
{
    Channel->n_volume -= Level;
    if(Channel->n_volume < 0)
    {
        Channel->n_volume = 0;
    }
    mt_UpdateVolume(Channel, Channel->n_volume);
}

void mt_VibratoPlusVolSlide(LPCHANNEL Channel)
{
    mt_Vibrato2(Channel);
    mt_VolumeSlide(Channel);
}

void mt_Vibrato(LPCHANNEL Channel)
{
    int new_vib;

    if(Channel->n_cmd.datas)
    {
        new_vib = Channel->n_vibratocmd;
        if(Channel->n_cmd.datas & 0xf)
        {
            new_vib &= 0xf0;
            new_vib |= (Channel->n_cmd.datas & 0xf);
        }
        if(Channel->n_cmd.datas & 0xf0)
        {
            new_vib &= 0xf;
            new_vib |= (Channel->n_cmd.datas & 0xf0);
        }
        Channel->n_vibratocmd = new_vib;
    }
    mt_Vibrato2(Channel);
}

void mt_Vibrato2(LPCHANNEL Channel)
{
    int new_vib;
    int vib_cr;

    new_vib = (Channel->n_vibratopos >> 2) & 0x1f;
    vib_cr = Channel->n_wavecontrol & 0x3;
    if(vib_cr)
    {
        new_vib <<= 3;
        if(vib_cr != 1)
        {
            vib_cr = 255;
            goto mt_vib_set;
        }
        if(Channel->n_vibratopos < 0)
        {
            vib_cr = 255 - new_vib;
            goto mt_vib_set;
        }
        vib_cr = new_vib;
        goto mt_vib_set;
    }
    vib_cr = mt_VibratoTable[new_vib];
mt_vib_set:
    vib_cr *= (Channel->n_vibratocmd & 0xf);
    vib_cr >>= 7;
    if(Channel->n_vibratopos >= 0)
    {
        vib_cr = Channel->n_period + vib_cr;
    }
    else
    {
        vib_cr = Channel->n_period - vib_cr;
    }
    mt_UpdatePeriod(Channel, vib_cr);
    Channel->n_vibratopos += (Channel->n_vibratocmd >> 2) & 0x3c;
}

void mt_Tremolo(LPCHANNEL Channel)
{
    int new_trem;
    int trem_cr;

    if(Channel->n_cmd.datas)
    {
        new_trem = Channel->n_tremolocmd;
        if(Channel->n_cmd.datas & 0xf)
        {
            new_trem &= 0xf0;
            new_trem |= Channel->n_cmd.datas & 0xf;
        }
        if(Channel->n_cmd.datas & 0xf0)
        {
            new_trem &= 0xf;
            new_trem |= Channel->n_cmd.datas & 0xf0;
        }
        Channel->n_tremolocmd = new_trem;
    }
    new_trem = (Channel->n_tremolopos >> 2) & 0x1f;
    trem_cr = (Channel->n_wavecontrol >> 4) & 3;
    if(trem_cr)
    {
        new_trem <<= 3;
        if(trem_cr != 1)
        {
            trem_cr = 255;
            goto mt_tre_set;
        }
        if(Channel->n_vibratopos < 0)
        {
            trem_cr = 255 - new_trem;
            goto mt_tre_set;
        }
        trem_cr = new_trem;
        goto mt_tre_set;
    }
    trem_cr = mt_VibratoTable[new_trem];
mt_tre_set:
    trem_cr *= (Channel->n_tremolocmd & 0xf);
    trem_cr >>= 6;
    if(Channel->n_tremolopos >= 0)
    {
        trem_cr = Channel->n_volume + trem_cr;
    }
    else
    {
        trem_cr = Channel->n_volume - trem_cr;
    }
    if(trem_cr < 0)
    {
        trem_cr = 0;
    }
    if(trem_cr > 64)
    {
        trem_cr = 64;
    }
    mt_UpdateVolume(Channel, trem_cr);
    Channel->n_tremolopos += (Channel->n_tremolocmd >> 2) & 0x3c;
}

// ------------------------------------------------
// Init the replay and wait for commands
int main(int argc, char ** argv)
{
    mt_DriverInit();

    // Sit there and wait for the ARM9 commands
    while(1)
    {
        mt_infos = (LPMT_MSG_CMD) getIPC();
        switch(mt_infos->command)
        {
            case MT_MSG_INIT:
                mt_infos->command = mt_Init();
                break;

            case MT_MSG_END:
                mt_End();
                mt_infos->command = 0;
                break;

            case MT_MSG_SETPOS:
                mt_SetPosition();
                mt_infos->command = 0;
                break;

            case MT_MSG_GETPOS:
                mt_infos->position.pos = mt_SongPos;
                mt_infos->position.row = mt_PatternPos;
                mt_infos->command = 0;
                break;

            case MT_MSG_MASTERVOL:
                mt_SetMasterVol(mt_infos->master_volume);
                mt_infos->command = 0;
                break;
        }
    }
    return 0;
}
