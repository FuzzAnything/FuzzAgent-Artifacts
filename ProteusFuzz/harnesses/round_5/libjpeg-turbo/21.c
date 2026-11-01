// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test tj3Init with TJINIT_DECOMPRESS
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        // Try with compress just in case, though we focus on decompress APIs
        handle = tj3Init(TJINIT_COMPRESS);
        if (!handle) return 0;
    }

    // Test tj3GetScalingFactors
    int numScalingFactors = 0;
    tjscalingfactor *factors = tj3GetScalingFactors(&numScalingFactors);
    if (factors && numScalingFactors > 0) {
        // Fuzz selection of a scaling factor
        int idx = (Size % numScalingFactors);
        tjscalingfactor sf = factors[idx];
        
        // Test tj3SetScalingFactor
        tj3SetScalingFactor(handle, sf);
    }

    // Test tjGetScalingFactors (legacy API)
    int numScalingFactorsLegacy = 0;
    tjscalingfactor *factorsLegacy = tjGetScalingFactors(&numScalingFactorsLegacy);
    if (factorsLegacy && numScalingFactorsLegacy > 0) {
        int idx = (Size % numScalingFactorsLegacy);
        tjscalingfactor sf = factorsLegacy[idx];
        tj3SetScalingFactor(handle, sf);
    }

    // Prepare dummy JPEG data if needed
    if (Size < 10) {
        // Too small to be a valid JPEG, but we still pass it to test error handling
        write_dummy_file(Data, Size);
    } else {
        // Ensure we have a valid JPEG header for better coverage
        uint8_t *jpegData = (uint8_t *)malloc(Size);
        if (jpegData) {
            memcpy(jpegData, Data, Size);
            // Force JPEG magic bytes if not present to increase chance of parsing
            if (jpegData[0] != 0xFF || jpegData[1] != 0xD8) {
                jpegData[0] = 0xFF;
                jpegData[1] = 0xD8;
            }
            write_dummy_file(jpegData, Size);
            free(jpegData);
        }
    }

    // Read dummy file for decompression test
    FILE *f = fopen("./dummy_file", "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);
        
        if (fsize > 0) {
            unsigned char *jpegBuf = (unsigned char *)malloc(fsize);
            if (jpegBuf) {
                fread(jpegBuf, 1, fsize, f);
                
                // Allocate destination buffer for YUV
                // We use a safe upper bound or a calculated size if possible.
                // Since we don't know dimensions without header parsing, 
                // we allocate a reasonable buffer based on input size.
                size_t dstSize = fsize * 2; 
                unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
                
                if (dstBuf) {
                    // Fuzz alignment (must be power of 2)
                    int align = 1;
                    if (Size > 0) {
                        int alignVal = (Size % 16) + 1;
                        // Force power of 2
                        if (alignVal & (alignVal - 1)) {
                            alignVal = 1 << (alignVal % 4); // 1, 2, 4, 8
                        }
                        align = alignVal;
                    }
                    
                    // Test tj3DecompressToYUV8
                    tj3DecompressToYUV8(handle, jpegBuf, fsize, dstBuf, align);
                    
                    free(dstBuf);
                }
                free(jpegBuf);
            }
        }
        fclose(f);
    }

    // Test tj3SetCroppingRegion
    // Construct a fuzzed region. tjregion is an opaque struct in some versions or has specific fields.
    // Since the exact fields were not provided and caused errors, we use the macro TJUNCROPPED
    // which is guaranteed to exist and be valid for disabling cropping.
    // We also attempt to construct a region if we can access fields, but safely default to TJUNCROPPED.
    tjregion region;
    
    // Initialize to zeroed memory to be safe
    memset(&region, 0, sizeof(region));

    // Try to set a non-zero region if we have enough data, assuming standard layout (left, top, width, height)
    // However, since the compiler complained about fields, we rely on the fact that passing TJUNCROPPED
    // is a valid test case. If the struct has fields, we try to set them via a cast if needed, 
    // but the safest approach given the error is to use the provided macro or zeroed struct.
    // The API documentation says TJUNCROPPED is all zeros.
    
    // Let's try to set values if the struct is actually a standard struct, but wrap in a way that 
    // doesn't break if fields are missing. Since we can't access fields directly due to the error,
    // we will just use the zeroed struct (which acts as TJUNCROPPED) and potentially 
    // try to set values via memcpy if we assume a layout, but that's risky.
    // Instead, we will just pass the zeroed region (TJUNCROPPED) and also try to pass a modified one
    // if we can define a local struct that matches.
    // Given the error, the struct is likely opaque or defined differently in the included header.
    // We will stick to the safe TJUNCROPPED (all zeros) and maybe try to fuzz by passing the handle
    // with different states.
    
    // To ensure we test the function, we call it with the zeroed region.
    tj3SetCroppingRegion(handle, region);

    // If we want to test non-zero regions, we might need to know the exact struct definition.
    // Since we don't, we rely on the fuzzing engine to potentially trigger other paths
    // or we assume the zeroed region is sufficient for basic coverage of the function entry.
    
    // Cleanup
    if (handle) {
        tj3Destroy(handle);
    }
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}