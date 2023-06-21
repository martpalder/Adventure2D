#ifndef TEXT_H
#define TEXT_H

typedef struct
{
    int len;
    int maxLen;
    char* str;
} Text;

void textInput(Text* pText, KB* pKB);
void clearText(Text* pText);
void textBox(char* str, const int W, const int H);

#endif