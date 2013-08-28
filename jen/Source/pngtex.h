// .png texture loader by Semi Essessi
// only supports loading 3/4 channel pngs to a rgba/rgb0 unsigned int bitmap
// can be easily updated to support further types or perhaps ALL using the libpng functions

#ifndef PNGTEX_H
#define PNGTEX_H

#pragma comment(lib, "libpng.lib")
#pragma comment(lib, "zlib.lib")

#include "libpng\png.h"
#include <malloc.h>

class png_tex
{
private:
    int width;
    int height;
public:
    unsigned int* imagedata;
    
    png_tex()
    {
        imagedata = 0;
    }

    ~png_tex()
    {
        if(imagedata) free(imagedata);
    }

    bool LoadFromFile(char* filename);
    int GetWidth();
    int GetHeight();
};

#endif