#include <raylib.h>
#include "./input.h"
#include "./text.h"

void textInput(Text* pText, KB* pKB)
{
    // Delete a character from string
    if(pKB->backspace && pText->len)
    {
        *(pText->str + pText->len - 1) = '\0';
        --pText->len;
    }

    // Get character pressed
    char c = GetCharPressed();
    
    // Write the character to string
    if(c && pText->len < pText->maxLen)
    {
        *(pText->str + pText->len) = c;
        ++pText->len;
        //putc(c, stdout);
        //putc('\n', stdout);
    }
}

void clearText(Text* pText)
{
    for(int i = 0; i < pText->len; ++i)
    {
        *(pText->str + i) = '\0';
    }
    pText->len = 0;
}

void textBox(char* str, const int W, const int H)
{
    DrawRectangle(128, H - 160, 512, 128, DARKBLUE);
    DrawText(str, 160, H - 128, 24, YELLOW);
}