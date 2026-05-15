// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    write_dummy_file(Data, Size);
    
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        remove_dummy_file();
        return 0;
    }
    
    FILE *f = fopen("./dummy_file", "rb");
    if (!f) {
        tj3Destroy(handle);
        remove_dummy_file();
        return 0;
    }
    
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    unsigned char *jpeg_buf = NULL;
    if (file_size > 0) {
        jpeg_buf = (unsigned char *)malloc(file_size);
        if (jpeg_buf) {
            fread(jpeg_buf, 1, file_size, f);
        }
    }
    fclose(f);
    
    if (jpeg_buf && file_size > 0) {
        tj3DecompressHeader(handle, jpeg_buf, file_size);
        
        tj3Get(handle, TJPARAM_JPEGWIDTH);
        tj3Get(handle, TJPARAM_JPEGHEIGHT);
        tj3Get(handle, TJPARAM_SUBSAMP);
        
        tj3Set(handle, TJPARAM_BOTTOMUP, Data[0] & 1);
        tj3Set(handle, TJPARAM_FASTUPSAMPLE, Data[1] & 1);
        tj3Set(handle, TJPARAM_FASTDCT, Data[2] & 1);
        
        tj3Get(handle, TJPARAM_PRECISION);
        
        tj3Set(handle, TJPARAM_ARITHMETIC, Data[3] & 1);
        
        tjscalingfactor sf1 = {1, 1};
        tj3SetScalingFactor(handle, sf1);
        
        tjscalingfactor sf2 = {1, 2};
        tj3SetScalingFactor(handle, sf2);
        
        int width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        int height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        
        if (width > 0 && height > 0) {
            tjregion region1 = {0, 0, width/2, height/2};
            tj3SetCroppingRegion(handle, region1);
            
            tjregion region2 = {width/4, height/4, width/2, height/2};
            tj3SetCroppingRegion(handle, region2);
        }
        
        unsigned short *dst_buf = NULL;
        if (width > 0 && height > 0) {
            size_t alloc_size = width * height * 3 * sizeof(unsigned short);
            if (alloc_size > 0 && alloc_size < 100 * 1024 * 1024) {
                dst_buf = (unsigned short *)tj3Alloc(alloc_size);
            }
        }
        
        if (dst_buf) {
            int pitch = width * 3 * sizeof(unsigned short);
            tj3Decompress16(handle, jpeg_buf, file_size, dst_buf, pitch, TJPF_RGB);
            tj3Free(dst_buf);
        }
        
        free(jpeg_buf);
    }
    
    tj3Destroy(handle);
    remove_dummy_file();
    
    return 0;
}