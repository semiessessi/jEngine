// .png texture loader by Semi Essessi
// only supports loading 3/4 channel pngs to a rgba/rgb0 unsigned int bitmap
// can be easily updated to support further types or perhaps ALL using the libpng functions

#include "pngtex.h"
#include <stdio.h>

bool png_tex::LoadFromFile(char* filename)
{
    char header[8];    // 8 character png header to be loaded and checked first
    
    FILE* f = fopen(filename,"rb");
    // if no file then return false
    if(!f) return false;
    fread(header,1,8,f);
    // use libpngs header check function
    if(png_sig_cmp((png_bytep)header,0,8)) return false;

    // png struct pointer
    png_structp    pngptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if(!pngptr) 
    {
        fclose(f);
        return false;
    }

    // png info header
    png_infop infptr = png_create_info_struct(pngptr);

    if(!infptr)
    {
        png_destroy_read_struct(&pngptr, &infptr,(png_infopp)NULL);
        fclose(f);
        return false;
    }

    // read the image using libpng functions
    png_init_io(pngptr, f);
    png_set_sig_bytes(pngptr, 8);

    // this function may be better to use since it does the transform for us from all pngs
    //png_read_png(pngptr, infptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING, NULL);
    png_read_info(pngptr, infptr);

    // read data we need from the info pointer
    width = infptr->width;
    height = infptr->height;
    int channels = png_get_channels(pngptr, infptr);
    int bpc    = png_get_bit_depth(pngptr,infptr);


    
    // allocate row pointers (annoying)
    png_bytep* rp = (png_bytep*)malloc(sizeof(png_bytep)*height);
    for(int i=0;i<height;i++) rp[i] = (png_byte*)malloc(infptr->rowbytes);
    

    //png_bytep* rp = png_get_rows(pngptr,infptr);

    if(!rp) 
    {
        png_destroy_read_struct(&pngptr, &infptr,(png_infopp)NULL);
        fclose(f);
        return false;
    }
    
    png_set_interlace_handling(pngptr);
    png_read_update_info(pngptr, infptr);

    png_read_image(pngptr, rp);

    // end png reading and clean up (awkward again...)

    png_read_end(pngptr,NULL);
    fclose(f);
    png_destroy_read_struct(&pngptr, &infptr,(png_infopp)NULL);
    
    // convert into a nice rgba imagedata array:
    if(imagedata) free(imagedata);
    imagedata = (unsigned int*)malloc(sizeof(unsigned int)*width*height);

    if(!imagedata)
    {
        png_destroy_read_struct(&pngptr, &infptr,(png_infopp)NULL);
        fclose(f);
        return false;
    }

    // transform to rgba image data from 4 channels
    if(channels==4)
    {
        for(int y=0;y<height;y++)
        {
            png_byte* row = rp[y];
            for(int x=0;x<width;x++)
            {
                png_byte* p = &(row[x*4]);
                imagedata[y*width+x]=p[0];        // red
                imagedata[y*width+x]|=p[1]<<8;        // green
                imagedata[y*width+x]|=p[2]<<16;        // blue
                imagedata[y*width+x]|=p[3]<<24;        // alpha
            }
        }
    } else if(channels==3) // transform to rgba image data from 3 channels
    {
        for(int y=0;y<height;y++)
        {
            png_byte* row = rp[y];
            for(int x=0;x<width;x++)
            {
                png_byte* p = &(row[x*3]);
                imagedata[y*width+x]=p[0];                // red
                imagedata[y*width+x]|=p[1]<<8;            // green
                imagedata[y*width+x]|=p[2]<<16;        // blue
                //imagedata[y*height+x]|=p[3]<<24;        // alpha
            }
        }
    } else if(channels==1) // transform to rgba image data from greyscale
    {
        for(int y=0;y<height;y++)
        {
            png_byte* row = rp[y];
            for(int x=0;x<width;x++)
            {
                png_byte* p = &(row[x]);
                imagedata[y*width+x]=p[0];        // red
                imagedata[y*width+x]|=p[0]<<8;        // green
                imagedata[y*width+x]|=p[0]<<16;        // blue
                imagedata[y*width+x]|=p[0]<<24;        // alpha
            }
        }
    } else
    {
        free(rp);    
        return false;
    }

    for(int j=0;j<height;j++) free(rp[j]);
    if(rp) free(rp);

    return true;
}

int png_tex::GetHeight()
{
    return height;
}

int png_tex::GetWidth()
{
    return width;
}