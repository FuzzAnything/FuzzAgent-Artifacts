// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test tjGetScalingFactors (old API)
    int num_factors_old = 0;
    tjscalingfactor *factors_old = tjGetScalingFactors(&num_factors_old);
    
    // Test tj3GetScalingFactors (new API)
    int num_factors_new = 0;
    tjscalingfactor *factors_new = tj3GetScalingFactors(&num_factors_new);
    
    // Create decompressor handle
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) return 0;
    
    // Test with various parameters using fuzz data
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // Test tj3Set with various parameters
    if (offset < Size) {
        int param = Data[offset] % 32;  // Map to valid parameter range
        int value = (offset + 1 < Size) ? Data[offset + 1] : 0;
        tj3Set(handle, param, value);
        offset += 2;
    }
    
    // Test tj3Get with various parameters
    if (offset < Size) {
        int param = Data[offset] % 32;
        tj3Get(handle, param);
        offset++;
    }
    
    // Test scaling factor setting if we have factors
    if (factors_new && num_factors_new > 0 && offset < Size) {
        int idx = Data[offset] % num_factors_new;
        tjscalingfactor sf = factors_new[idx];
        tj3SetScalingFactor(handle, sf);
        offset++;
        
        // Also test with TJUNSCALED
        if (offset < Size && (Data[offset] & 1)) {
            tj3SetScalingFactor(handle, TJUNSCALED);
        }
        offset++;
    }
    
    // Write fuzz data to file for header reading
    write_dummy_file(Data, Size);
    
    // Try to read JPEG header if file has data
    FILE *test = fopen("./dummy_file", "rb");
    if (test) {
        fseek(test, 0, SEEK_END);
        long file_size = ftell(test);
        fseek(test, 0, SEEK_SET);
        
        if (file_size > 0) {
            unsigned char *jpeg_buf = (unsigned char *)malloc(file_size);
            if (jpeg_buf) {
                fread(jpeg_buf, 1, file_size, test);
                if (tj3DecompressHeader(handle, jpeg_buf, file_size) == 0) {
                    // Header read successfully, test cropping region
                    if (offset + 3 < Size) {
                        tjregion region;
                        region.x = Data[offset] * 8;  // Align to 8-pixel boundary
                        region.y = Data[offset + 1];
                        region.w = Data[offset + 2];
                        region.h = Data[offset + 3];
                        tj3SetCroppingRegion(handle, region);
                        offset += 4;
                        
                        // Also test with TJUNCROPPED
                        if (offset < Size && (Data[offset] & 1)) {
                            tj3SetCroppingRegion(handle, TJUNCROPPED);
                        }
                        offset++;
                    }
                }
                free(jpeg_buf);
            }
        }
        fclose(test);
    }
    
    // Clean up
    remove_dummy_file();
    tj3Destroy(handle);
    
    return 0;
}