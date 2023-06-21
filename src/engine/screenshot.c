#include <stdio.h>
#include <raylib.h>
#include "./screenshot.h"

bool screenshot(void)
{
    if(DirectoryExists("../screenshots"))
    {
        char filepath[32];
        // Up to 10 screenshots (1-9)
        for(int i = 1; i < 10; ++i)
        {
            sprintf_s(filepath, 28, "../screenshots/screen%d.png", i);
            if(!FileExists(filepath))
            {
                TakeScreenshot(filepath);
                return 1;
                break;
            }
        }
    }
    return 0;
}
