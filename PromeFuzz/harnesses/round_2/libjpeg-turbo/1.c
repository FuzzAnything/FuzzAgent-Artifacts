// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;
    
    int ret = tj3DecompressHeader(handle, Data, Size);
    if (ret == 0) {
        int width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        int height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        int subsamp = tj3Get(handle, TJPARAM_SUBSAMP);
        
        tj3Set(handle, TJPARAM_BOTTOMUP, Data[0] & 1);
        tj3Set(handle, TJPARAM_FASTUPSAMPLE, Data[1] & 1);
        tj3Set(handle, TJPARAM_FASTDCT, (Data[0] >> 1) & 1);
        
        int scanlimit = tj3Get(handle, TJPARAM_SCANLIMIT);
        
        tj3Set(handle, TJPARAM_ARITHMETIC, (Data[1] >> 1) & 1);
        tj3Set(handle, TJPARAM_OPTIMIZE, (Data[0] >> 2) & 1);
        
        tjscalingfactor sf1 = {1, 1};
        tj3SetScalingFactor(handle, sf1);
        
        tjscalingfactor sf2 = {1, 2};
        tj3SetScalingFactor(handle, sf2);
        
        tjregion region1 = {0, 0, 0, 0};
        tj3SetCroppingRegion(handle, region1);
        
        if (width > 0 && height > 0) {
            int scaled_width = TJSCALED(width, sf2);
            int scaled_height = TJSCALED(height, sf2);
            int mcu_width = tjMCUWidth[subsamp];
            int scaled_mcu_width = TJSCALED(mcu_width, sf2);
            
            if (scaled_mcu_width > 0) {
                int crop_x = (Data[0] % scaled_width) & ~(scaled_mcu_width - 1);
                int crop_y = (Data[1] % scaled_height) & ~(scaled_mcu_width - 1);
                int crop_w = (Data[0] % (scaled_width - crop_x)) & ~(scaled_mcu_width - 1);
                int crop_h = (Data[1] % (scaled_height - crop_y)) & ~(scaled_mcu_width - 1);
                
                if (crop_w > 0 && crop_h > 0) {
                    tjregion region2 = {crop_x, crop_y, crop_w, crop_h};
                    tj3SetCroppingRegion(handle, region2);
                }
            }
        }
        
        int pixel_format = TJPF_RGB;
        int pitch = width * tjPixelSize[pixel_format];
        size_t alloc_size = pitch * height;
        
        if (alloc_size > 0 && alloc_size < 100 * 1024 * 1024) {
            unsigned short *buffer = (unsigned short *)tj3Alloc(alloc_size);
            if (buffer) {
                tj3Decompress16(handle, Data, Size, buffer, pitch, pixel_format);
                tj3Free(buffer);
            }
        }
    }
    
    tj3Destroy(handle);
    return 0;
}