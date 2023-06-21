#ifndef INPUT_H
#define INPUT_H
#include <raylib.h>

typedef struct {
    bool left;
    bool right;
    bool up;
    bool down;
    bool esc;
    bool space;
    bool enter;
    bool backspace;
    bool ctrl;
    bool tab;
    bool s;
    bool l;
    bool n;
    bool p;
} KB;

typedef struct {
    Vector2 screenPos;
    Vector2 worldPos;
 } Mouse;

KB initKB(void);
void checkKbInput(KB* pKB);
void checkMouseInput(Mouse* pMouse, Camera2D* pCam);

#endif