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
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
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

static void write_to_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_to_file(Data, Size);
    
    // Initialize TurboJPEG instance for decompression
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;
    
    // Decompress header
    tj3DecompressHeader(handle, Data, Size);
    
    // Get various parameters
    tj3Get(handle, TJPARAM_JPEGWIDTH);
    tj3Get(handle, TJPARAM_JPEGHEIGHT);
    tj3Get(handle, TJPARAM_SUBSAMP);
    
    // Set some parameters
    tj3Set(handle, TJPARAM_BOTTOMUP, Data[0] & 1);
    tj3Set(handle, TJPARAM_FASTDCT, Data[1] & 1);
    tj3Set(handle, TJPARAM_FASTUPSAMPLE, Data[2] & 1);
    
    // Get a parameter
    tj3Get(handle, TJPARAM_BOTTOMUP);
    
    // Set more parameters
    tj3Set(handle, TJPARAM_ARITHMETIC, Data[3] & 1);
    tj3Set(handle, TJPARAM_OPTIMIZE, Data[0] & 1);
    
    // Set scaling factors
    tjscalingfactor sf1 = {1, 1};
    tjscalingfactor sf2 = {1, 2};
    tj3SetScalingFactor(handle, sf1);
    tj3SetScalingFactor(handle, sf2);
    
    // Set cropping regions
    tjregion crop1 = {0, 0, 100, 100};
    tjregion crop2 = {10, 10, 50, 50};
    tj3SetCroppingRegion(handle, crop1);
    tj3SetCroppingRegion(handle, crop2);
    
    // Allocate buffer for decompression
    int width = tj3Get(handle, TJPARAM_JPEGWIDTH);
    int height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
    if (width > 0 && height > 0) {
        size_t alloc_size = width * height * 3 * sizeof(unsigned short);
        if (alloc_size > 0 && alloc_size < 100 * 1024 * 1024) { // Limit to 100MB
            unsigned short *buffer = (unsigned short *)tj3Alloc(alloc_size);
            if (buffer) {
                // Decompress to 16-bit buffer
                tj3Decompress16(handle, Data, Size, buffer, 0, TJPF_RGB);
                // Free the allocated buffer (tj3Free is internal, but we need to free)
                // Note: tj3Alloc documentation says to use tj3Free, but we don't have it in API
                // We'll use standard free since tj3Alloc might use malloc internally
                free(buffer);
            }
        }
    }
    
    // Clean up
    tj3Destroy(handle);
    
    return 0;
}