// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static void write_dummy_jpeg(const uint8_t *Data, size_t Size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid JPEG header if input is empty to avoid immediate crash on empty input
            const unsigned char header[] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 'J', 'F', 'I', 'F', 0x00};
            fwrite(header, 1, sizeof(header), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test tjGetScalingFactors (Legacy API)
    int numScalingFactorsLegacy = 0;
    tjscalingfactor *factorsLegacy = tjGetScalingFactors(&numScalingFactorsLegacy);
    if (factorsLegacy) {
        // Just access to ensure no crash, logic is static
        for (int i = 0; i < numScalingFactorsLegacy; i++) {
            (void)factorsLegacy[i];
        }
    }

    // Test tj3GetScalingFactors
    int numScalingFactors = 0;
    tjscalingfactor *factors = tj3GetScalingFactors(&numScalingFactors);
    if (!factors) {
        return 0;
    }

    // Initialize TurboJPEG instance for decompression
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        return 0;
    }

    // Prepare a dummy JPEG file
    const char *dummyFile = "./dummy_file";
    write_dummy_jpeg(Data, Size, dummyFile);

    // Load the dummy JPEG into a buffer
    FILE *f = fopen(dummyFile, "rb");
    if (!f) {
        tj3Destroy(handle);
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *jpegBuf = NULL;
    if (fsize > 0) {
        jpegBuf = (unsigned char *)malloc(fsize);
        if (jpegBuf) {
            size_t bytesRead = fread(jpegBuf, 1, fsize, f);
            if (bytesRead != (size_t)fsize) {
                free(jpegBuf);
                jpegBuf = NULL;
            }
        }
    }
    fclose(f);

    if (!jpegBuf) {
        tj3Destroy(handle);
        return 0;
    }

    // Try to set scaling factors from the retrieved list
    for (int i = 0; i < numScalingFactors; i++) {
        // Fuzz: Try setting scaling factor
        int ret = tj3SetScalingFactor(handle, factors[i]);
        (void)ret; // Ignore return for fuzzing exploration
    }

    // Try setting a random scaling factor derived from input data (if valid)
    if (Size > 0) {
        tjscalingfactor fuzzFactor = {
            .num = Data[0] % 8 + 1, // 1 to 8
            .denom = 8
        };
        if (fuzzFactor.num <= fuzzFactor.denom) {
            tj3SetScalingFactor(handle, fuzzFactor);
        }
    }

    // Try setting cropping region
    // Note: tj3SetCroppingRegion might fail if header not read, which is expected behavior to test
    tjregion cropRegion = {0};
    if (Size >= 4) {
        // Accessing members based on standard struct layout for tjregion (left, top, width, height)
        // Since the struct definition is opaque in the provided snippet, we rely on the standard definition
        // which is: typedef struct { int left; int top; int width; int height; } tjregion;
        // If the compiler still complains, it implies the header is different, but standard libjpeg-turbo uses these names.
        // To be safe against opaque struct errors in some builds, we cast to a known compatible struct if needed,
        // but standard usage is direct access.
        // Re-declaring a compatible struct to ensure member access works if the header is incomplete in the fuzz env
        struct { int left; int top; int width; int height; } tempRegion;
        tempRegion.left = Data[0];
        tempRegion.top = Data[1];
        tempRegion.width = Data[2];
        tempRegion.height = Data[3];
        
        // Copy to the actual region variable
        memcpy(&cropRegion, &tempRegion, sizeof(cropRegion));
    }
    tj3SetCroppingRegion(handle, cropRegion);

    // Try to decompress to YUV
    // We need to allocate a destination buffer. Since we don't know the exact size without reading header,
    // we allocate a reasonable buffer size based on input size or a fixed max.
    // tj3DecompressToYUV8 will fail if buffer is too small, which is a valid test case.
    size_t maxBufSize = 1024 * 1024; // 1MB max for fuzzing
    unsigned char *dstBuf = (unsigned char *)malloc(maxBufSize);
    if (dstBuf) {
        int align = 1;
        if (Size > 0) {
            align = (Data[0] & 0x0F) + 1; // 1 to 16
            // Ensure power of 2
            if (align & (align - 1)) {
                align = 1;
            }
        }
        
        // Call decompress
        tj3DecompressToYUV8(handle, jpegBuf, fsize, dstBuf, align);
        
        free(dstBuf);
    }

    // Cleanup
    free(jpegBuf);
    tj3Destroy(handle);
    return 0;
}