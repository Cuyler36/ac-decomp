#include "gckeyboard/gc_keyboard.h"

#define SI_ANALOG_MODE 0x00000300
#define SI_ENABLE 0x00400000

static s32 keyboardChan = -1;

extern s32 GCKB_Detect(void) {
    u32 buf[2];
    s32 i;

    SISync();
    for (i = 0; i < SI_MAX_CHAN; i++) {
        SIGetResponse(i, buf);
        SISetCommand(i, SI_ENABLE | SI_ANALOG_MODE);
        SIEnablePolling(SI_POLL_MASK(i));
    }

    SISync();
    for (i = 0; i < SI_MAX_CHAN; i++) {
        u32 type = SIGetType(i);

        if (type == SI_GC_KEYBOARD) {
            keyboardChan = i;
            return i;
        }
    }

    keyboardChan = -1;
    return -1;
}

extern BOOL GCKB_Init(s32 chan) {
    u32 buf[2];

    if (!(chan >= 0 && chan < SI_MAX_CHAN)) {
        return FALSE;
    }

    SIGetResponse(chan, buf);
    SISetCommand(chan, 0x00540000);
    SIEnablePolling(SI_POLL_MASK(chan));
    SITransferCommands();
    SISync();
    return TRUE;
}

extern BOOL GCKB_ReadKeys(s32 chan, u8* pressed_keys) {
    u32 buf[2];

    if (!(chan >= 0 && chan < SI_MAX_CHAN)) {
        return FALSE;
    }

    if (pressed_keys == NULL) {
        return FALSE;
    }

    /* Read keys */
    if (SIGetResponse(chan, buf)) {
        pressed_keys[0] = buf[1] >> 24;
        pressed_keys[1] = buf[1] >> 16;
        pressed_keys[2] = buf[1] >> 8;
        return TRUE;
    }

    /* Failed to read */
    pressed_keys[0] = 0;
    pressed_keys[1] = 0;
    pressed_keys[2] = 0;
    return FALSE;
}
