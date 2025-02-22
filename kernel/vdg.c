// See page 248 in
// "Assembly Language Programming for the Color Computer (1985)(Laurence A Tepolt)(pdf)".
// Our TextMode is Display Mode "AI".
// Our PMode1 is Display Mode "G3C".
//
// The vdg_op_mode are written to $FF22 (Pia1PortB).
// The low three bits can be set high.
//
// See page 3 in "CoCo Hardware Reference.pdf".
// The sam_control_bits are called "FFC0-FFC5 Video Display Mode".
static void SetDisplayMode(vptr fb, byte vdg_op_mode, byte sam_control_bits) {
    {
        word bit = 0x0200;  // Start with bit F0 (CoCo Hardware Reference.pdf)
        for (byte i=0; i<14; i+=2) {  // 7 iterations.
            bool b = (((word)fb & bit) != 0); 
            Poke1(0xFFC6 + i + b, 0); // 0xFFC6 is F0.
            bit <<= 1;
        }
    }
    Poke1(0xFF22, vdg_op_mode);
    {
        byte bit = 0x01;
        for (byte i=0; i<6; i+=2) {  // 3 iterations.
            bool b = ((sam_control_bits & bit) != 0); 
            Poke1(0xFFC0 + i + b, 0); // 0xFFC0 is V0.
            bit <<= 1;
        }
    }
}
// Effective immediately.
static void SetDisplayText(vptr fb, byte colorset) {
    SetDisplayMode(fb, 0x07 + (colorset?8:0), 0);
}
// Effective immediately.
static void SetDisplayPMode1(vptr fb, byte colorset) {
    SetDisplayMode(fb, 0xC7 + (colorset?8:0), 4);
}
// Effective immediately.
void Vdg_RestoreGameMode() {
    switch (Vdg.game_mode) {
    case 0:
        SetDisplayText(Vdg.game_framebuffer, Vdg.game_colorset);
        break;
    case 1:
        SetDisplayPMode1(Vdg.game_framebuffer, Vdg.game_colorset);
        break;
    }
}

// These are for Game Mode.
// They remember what to change back to, after Chat mode.
void Vdg_GameText(vptr fb, byte colorset) {
    Vdg.game_mode = GM_Text;
    Vdg.game_framebuffer = fb;
    Vdg.game_colorset = colorset;
    Vdg_RestoreGameMode();
}
void Vdg_GamePMode1(vptr fb, byte colorset) {
    Vdg.game_mode = GM_PMode1;
    Vdg.game_framebuffer = fb;
    Vdg.game_colorset = colorset;
    Vdg_RestoreGameMode();
}

void Vdg_SetConsoleMode() {
    SetDisplayText(Cons, COLORSET_ORANGE);
}

void Vdg_Init() {
    Vdg_SetConsoleMode();
}
