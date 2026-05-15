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
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/turbojpeg.h"

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Initialize with decompression context
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        return 0;
    }

    // Write input data to dummy file for potential file-based operations
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Use the input data as JPEG buffer
    const unsigned char *jpegBuf = Data;
    size_t jpegSize = Size;

    // 1. Decompress header
    if (tj3DecompressHeader(handle, jpegBuf, jpegSize) == 0) {
        // 2-4. Three tj3Get calls with various parameters
        tj3Get(handle, TJPARAM_JPEGWIDTH);
        tj3Get(handle, TJPARAM_JPEGHEIGHT);
        tj3Get(handle, TJPARAM_SUBSAMP);

        // 5-7. Three tj3Set calls
        tj3Set(handle, TJPARAM_BOTTOMUP, 0);
        tj3Set(handle, TJPARAM_FASTDCT, 1);
        tj3Set(handle, TJPARAM_FASTUPSAMPLE, 1);

        // 8. Another tj3Get
        tj3Get(handle, TJPARAM_PRECISION);

        // 9-10. Two more tj3Set calls - using valid parameters
        tj3Set(handle, TJPARAM_STOPONWARNING, 0);
        tj3Set(handle, TJPARAM_PROGRESSIVE, 0);

        // 11-12. Two tj3SetScalingFactor calls
        tjscalingfactor sf1 = {1, 1};  // No scaling
        tj3SetScalingFactor(handle, sf1);
        
        tjscalingfactor sf2 = {1, 2};  // Half scaling
        tj3SetScalingFactor(handle, sf2);

        // 13-14. Two tj3SetCroppingRegion calls
        tjregion crop1 = {0, 0, 0, 0};  // No cropping
        tj3SetCroppingRegion(handle, crop1);
        
        // Get image dimensions for sensible cropping
        int width = tj3Get(handle, TJPARAM_JPEGWIDTH);
        int height = tj3Get(handle, TJPARAM_JPEGHEIGHT);
        
        if (width > 0 && height > 0) {
            // Create a reasonable cropping region (quarter of the image)
            tjregion crop2 = {width/4, height/4, width/2, height/2};
            tj3SetCroppingRegion(handle, crop2);
        }

        // 15. Allocate buffer for decompression
        // Calculate buffer size for 16-bit RGB
        int pixelFormat = TJPF_RGB;
        int pitch = 0;  // Let library calculate pitch
        unsigned short *dstBuf = NULL;
        
        if (width > 0 && height > 0) {
            size_t bufferSize = width * height * 3 * sizeof(unsigned short);
            dstBuf = (unsigned short *)tj3Alloc(bufferSize);
            
            if (dstBuf) {
                // 16. Decompress to 16-bit buffer
                tj3Decompress16(handle, jpegBuf, jpegSize, dstBuf, pitch, pixelFormat);
                
                // Free allocated buffer (using tj3Free if available, otherwise free())
                // Note: tj3Free is not in the provided API list, so we use standard free
                free(dstBuf);
            }
        }
    }

    // 17. Destroy the handle
    tj3Destroy(handle);
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}