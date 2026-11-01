// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3DecompressToYUV8 at turbojpeg.c:2338:15 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3GetScalingFactors at turbojpeg.c:1959:28 in turbojpeg.h
// tjGetScalingFactors at turbojpeg.c:1974:28 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetScalingFactor at turbojpeg.c:1981:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
// tj3SetCroppingRegion at turbojpeg.c:2006:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static void write_dummy_jpeg(const uint8_t *Data, size_t Size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write minimal dummy data if input is empty to avoid empty file issues
            const char dummy[] = "\xFF\xD8\xFF\xE0\x00\x10JFIF\x00\x01\x01\x00\x00\x01\x00\x01\x00\x00";
            fwrite(dummy, 1, sizeof(dummy) - 1, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Test tj3GetScalingFactors and tjGetScalingFactors (no handle needed)
    int numFactors = 0;
    tjscalingfactor *factors = tj3GetScalingFactors(&numFactors);
    if (factors && numFactors > 0) {
        // Use the factors later
    }
    
    int numFactorsOld = 0;
    tjscalingfactor *factorsOld = tjGetScalingFactors(&numFactorsOld);
    if (factorsOld && numFactorsOld > 0) {
        // Use the factors later
    }

    // 2. Initialize TurboJPEG instance for decompression
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS);
    if (!handle) {
        return 0;
    }

    // 3. Prepare dummy JPEG file if needed for header reading
    const char *dummyFile = "./dummy_file";
    write_dummy_jpeg(Data, Size, dummyFile);

    // 4. Read header to enable cropping and scaling setup
    // We need a valid JPEG buffer for header reading. 
    // If Data is not a valid JPEG, this might fail, which is expected in fuzzing.
    // We try to use the input data directly first.
    if (tj3DecompressHeader(handle, (const unsigned char *)Data, Size) != 0) {
        // If direct decompression header fails, try reading from file if possible
        // Note: tj3DecompressHeader takes a buffer, not a filename. 
        // We must load the file content if we want to test with the file.
        // However, for fuzzing, we can just rely on the buffer.
        // If the buffer is invalid, the function returns -1, which is fine.
        // We proceed to test other functions that might handle errors gracefully or fail safely.
    }

    // 5. Test tj3SetScalingFactor
    if (factors && numFactors > 0) {
        // Try setting a valid scaling factor from the list
        // Use modulo to pick a random factor from the list based on input data
        int idx = (Size > 0) ? (Data[0] % numFactors) : 0;
        tj3SetScalingFactor(handle, factors[idx]);
    }
    
    // Also try setting TJUNSCALED (usually {1, 1})
    // We construct a scaling factor manually if possible, but TJUNSCALED is a macro.
    // Assuming TJUNSCALED is defined in turbojpeg.h as {1, 1} or similar.
    // If not, we rely on the factors list.
    if (factors && numFactors > 0) {
        // Try the first factor again
        tj3SetScalingFactor(handle, factors[0]);
    }

    // 6. Test tj3SetCroppingRegion
    // We need a valid tjregion. 
    // TJUNCROPPED is usually {0, 0, 0, 0}.
    tjregion cropRegion = {0, 0, 0, 0}; // TJUNCROPPED
    tj3SetCroppingRegion(handle, cropRegion);

    // Try a non-zero cropping region (might fail if header not read or invalid, which is fine)
    if (Size >= 4) {
        int x = (Data[0] % 100);
        int y = (Data[1] % 100);
        int w = (Data[2] % 100);
        int h = (Data[3] % 100);
        tjregion cropRegion2 = {x, y, w, h};
        tj3SetCroppingRegion(handle, cropRegion2);
    }

    // 7. Test tj3DecompressToYUV8
    // We need a destination buffer.
    // Calculate size based on potential dimensions. 
    // Since we don't know the exact dimensions if header read failed, we allocate a safe size.
    // Or we can try to get dimensions from parameters if header was read.
    // For fuzzing, we allocate a fixed reasonable size or based on input size.
    // Let's allocate a buffer based on input size to ensure we have space.
    size_t dstSize = Size * 2; 
    if (dstSize == 0) dstSize = 1024;
    
    unsigned char *dstBuf = (unsigned char *)malloc(dstSize);
    if (dstBuf) {
        // Try different alignments
        int align = 1;
        if (Size > 0 && Data[0] % 4 == 0) align = 4;
        else if (Size > 0 && Data[0] % 8 == 0) align = 8;
        
        tj3DecompressToYUV8(handle, (const unsigned char *)Data, Size, dstBuf, align);
        
        // Try with alignment 1
        tj3DecompressToYUV8(handle, (const unsigned char *)Data, Size, dstBuf, 1);
        
        free(dstBuf);
    }

    // 8. Cleanup
    tj3Destroy(handle);
    
    // Remove dummy file
    remove(dummyFile);

    return 0;
}