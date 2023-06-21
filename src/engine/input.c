#include <raylib.h>
#include "./input.h"

KB initKB(void)
{
    return (KB){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

void checkKbInput(KB* pKB)
{
    // Set to 0
    pKB->space = 0;
    pKB->enter = 0;
    pKB->backspace = 0;
    pKB->tab = 0;
    pKB->s = 0;
    pKB->l = 0;
    pKB->n = 0;
    pKB->p = 0;

    // Check keys pressed
    switch(GetKeyPressed())
    {
    case KEY_LEFT:
        pKB->left = 1;
        break;
    case KEY_RIGHT:
        pKB->right = 1;
        break;
	case KEY_UP:
        pKB->up = 1;
        break;
    case KEY_DOWN:
        pKB->down = 1;
        break;
    case KEY_ESCAPE:
        pKB->esc = 1;
        break;
    case KEY_SPACE:
        pKB->space = 1;
        break;
    case KEY_ENTER:
        pKB->enter = 1;
        break;
    case KEY_BACKSPACE:
        pKB->backspace = 1;
        break;
    case KEY_LEFT_CONTROL:
        pKB->ctrl = 1;
        break;
    case KEY_TAB:
        pKB->tab = 1;
        break;
    case KEY_S:
        pKB->s = 1;
        break;
    case KEY_L:
        pKB->l = 1;
        break;
    case KEY_N:
        pKB->n = 1;
        break;
    case KEY_P:
        pKB->p = 1;
        break;
    }

    // Check keys released
    if(IsKeyReleased(KEY_LEFT)){ pKB->left = 0; }
    else if(IsKeyReleased(KEY_RIGHT)){ pKB->right = 0; }
	else if(IsKeyReleased(KEY_UP)){ pKB->up = 0; }
    else if(IsKeyReleased(KEY_DOWN)){ pKB->down = 0; }
    else if(IsKeyReleased(KEY_ESCAPE)){ pKB->esc = 0; }
    else if(IsKeyReleased(KEY_SPACE)){ pKB->space = 0; }
    else if(IsKeyReleased(KEY_ENTER)){ pKB->enter = 0; }
    else if(IsKeyReleased(KEY_BACKSPACE)){ pKB->backspace = 0; }
    else if(IsKeyReleased(KEY_LEFT_CONTROL)){ pKB->ctrl = 0; }
    else if(IsKeyReleased(KEY_TAB)){ pKB->tab = 0; }
    else if(IsKeyReleased(KEY_S)){ pKB->s = 0; }
    else if(IsKeyReleased(KEY_L)){ pKB->l = 0; }
    else if(IsKeyReleased(KEY_N)){ pKB->n = 0; }
    else if(IsKeyReleased(KEY_P)){ pKB->p = 0; }
}

void checkMouseInput(Mouse* pMouse, Camera2D* pCam)
{
    // Get mouse sceen position
    pMouse->screenPos = GetMousePosition();

    // If camera is not NULL
    if(pCam)
    {
        // Calculate mouse world position
        pMouse->worldPos = GetScreenToWorld2D(pMouse->screenPos, *pCam);
    }
}
