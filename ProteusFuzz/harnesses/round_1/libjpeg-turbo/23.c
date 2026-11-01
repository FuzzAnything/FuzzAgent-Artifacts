// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3SaveImage16 at turbojpeg-mp.c:442:15 in turbojpeg.h
// tjGetErrorStr at turbojpeg.c:636:17 in turbojpeg.h
// tjGetErrorStr2 at turbojpeg.c:630:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3GetErrorStr at turbojpeg.c:618:17 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <turbojpeg.h>

static int write_dummy_ppm(const char *filename, const uint8_t *data, size_t size, int width, int height, int pixelFormat) {
    if (width <= 0 || height <= 0) return -1;
    
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;

    int channels = 1;
    if (pixelFormat == TJPF_RGB || pixelFormat == TJPF_CMYK || pixelFormat == TJPF_XRGB || pixelFormat == TJPF_BGR || pixelFormat == TJPF_XBGR) {
        channels = 3;
    } else if (pixelFormat == TJPF_RGBA || pixelFormat == TJPF_BGRA || pixelFormat == TJPF_ABGR || pixelFormat == TJPF_ARGB) {
        channels = 4;
    }

    // Write PPM/PGM header
    if (pixelFormat == TJPF_GRAY) {
        fprintf(f, "P2\n%d %d\n65535\n", width, height);
    } else {
        fprintf(f, "P6\n%d %d\n65535\n", width, height);
    }

    // Write dummy 16-bit data
    size_t total_samples = (size_t)width * (size_t)height * (size_t)channels;
    unsigned short *buffer = (unsigned short *)malloc(total_samples * sizeof(unsigned short));
    if (!buffer) {
        fclose(f);
        return -1;
    }

    memset(buffer, 0, total_samples * sizeof(unsigned short));
    
    // Fill with some pattern based on input data if available
    if (data && size > 0) {
        size_t fill_count = (size < total_samples) ? size : total_samples;
        for (size_t i = 0; i < fill_count; i++) {
            buffer[i] = (unsigned short)(data[i] * 257); // Scale 0-255 to 0-65535
        }
    }

    fwrite(buffer, sizeof(unsigned short), total_samples, f);
    free(buffer);
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize TurboJPEG instance
    int initTypes[] = {TJINIT_DECOMPRESS, TJINIT_COMPRESS, TJINIT_TRANSFORM};
    int numInitTypes = sizeof(initTypes) / sizeof(initTypes[0]);
    
    for (int i = 0; i < numInitTypes; i++) {
        tjhandle handle = tj3Init(initTypes[i]);
        if (!handle) {
            // Check error
            tj3GetErrorStr(handle);
            continue;
        }

        // Test tj3LoadImage16
        int width = 0, height = 0;
        int pixelFormats[] = {TJPF_UNKNOWN, TJPF_GRAY, TJPF_RGB, TJPF_CMYK, TJPF_BGR, TJPF_XRGB, TJPF_XBGR, TJPF_RGBA, TJPF_BGRA, TJPF_ABGR, TJPF_ARGB};
        int numPixelFormats = sizeof(pixelFormats) / sizeof(pixelFormats[0]);
        
        for (int pf = 0; pf < numPixelFormats; pf++) {
            int pixelFormat = pixelFormats[pf];
            int aligns[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
            int numAligns = sizeof(aligns) / sizeof(aligns[0]);
            
            for (int a = 0; a < numAligns; a++) {
                int align = aligns[a];
                
                // Create a dummy file with some data
                // Ensure dimensions are reasonable to prevent overflow
                int w = (Size > 10 ? Data[0] % 100 + 1 : 10);
                int h = (Size > 11 ? Data[1] % 100 + 1 : 10);
                
                if (w > 1000 || h > 1000) {
                    w = 100;
                    h = 100;
                }

                if (write_dummy_ppm("./dummy_file", Data, Size, w, h, pixelFormat) == 0) {
                    
                    unsigned short *buffer = tj3LoadImage16(handle, "./dummy_file", &width, align, &height, &pixelFormat);
                    
                    if (buffer) {
                        // Use the buffer to test tj3SaveImage16
                        // We must calculate the correct pitch based on the returned pixel format
                        // tj3LoadImage16 returns a buffer with row alignment 'align'
                        // The pitch returned by the library is width * pixelSize (aligned)
                        // However, tj3SaveImage16 expects pitch in samples, not bytes.
                        // If pitch is 0, it calculates width * pixelSize.
                        // The issue in the original code was passing 'width' as pitch which is incorrect for multi-channel formats.
                        // But more importantly, the buffer returned by tj3LoadImage16 has a specific pitch (samples per row).
                        // We should pass 0 to let the library calculate it based on width, OR pass the actual pitch.
                        // Since tj3LoadImage16 aligns rows, the actual pitch might be larger than width * channels.
                        // However, tj3SaveImage16 documentation says: "Setting this parameter to 0 is the equivalent of setting it to width * tjPixelSize[pixelFormat]".
                        // This implies it expects the standard pitch, not the padded one.
                        // If we pass 0, it might read beyond the allocated buffer if the buffer was padded.
                        // To be safe, we should pass the actual pitch used by tj3LoadImage16.
                        // But tj3LoadImage16 doesn't return the pitch.
                        // The safest approach for fuzzing is to pass 0 and ensure the buffer is large enough for the worst case,
                        // OR just pass 0 and let the library handle standard pitch.
                        // The crash indicates the library is reading beyond the buffer.
                        // This happens if the buffer allocated by tj3LoadImage16 has padding, but tj3SaveImage16 with pitch=0
                        // assumes no padding and reads more data than exists in the logical image, but within the allocated block?
                        // No, the crash is a heap-buffer-overflow.
                        // The buffer returned by tj3LoadImage16 is allocated with padding.
                        // If we pass pitch=0 to tj3SaveImage16, it calculates pitch = width * pixelSize.
                        // If the buffer was padded, the actual row size is larger.
                        // But tj3SaveImage16 writes to the buffer? No, it reads from it.
                        // Wait, tj3SaveImage16 saves TO disk. It reads FROM buffer.
                        // If the buffer has padding, and we tell tj3SaveImage16 the pitch is smaller (no padding),
                        // it will read the next row starting earlier than it should, potentially reading garbage or overlapping.
                        // But the crash is a READ overflow.
                        // The library is trying to read 'pitch' samples.
                        // If the buffer was allocated with padding, the total size is height * (aligned_pitch).
                        // If we pass pitch=0, it uses width * pixelSize.
                        // If width * pixelSize > aligned_pitch? No, aligned_pitch >= width * pixelSize.
                        // So reading width * pixelSize should be safe within the row.
                        // Why the overflow?
                        // Maybe the pixelFormat changed?
                        // tj3LoadImage16 returns a buffer with a specific pixelFormat.
                        // We are passing that buffer to tj3SaveImage16 with a DIFFERENT pixelFormat (saveFormat).
                        // If the formats differ, the pixelSize differs.
                        // If saveFormat has a larger pixelSize than the loaded format, pitch=0 will calculate a larger pitch.
                        // This will cause tj3SaveImage16 to read more bytes per row than were allocated.
                        // Example: Load as GRAY (1 sample), Save as RGB (3 samples).
                        // Buffer allocated: width * 1 * height (plus padding).
                        // tj3SaveImage16 with pitch=0 and RGB: expects width * 3 per row.
                        // It reads 3x the data, causing overflow.
                        // FIX: Only call tj3SaveImage16 with the SAME pixelFormat as returned by tj3LoadImage16.
                        
                        int loadedFormat = pixelFormat; // This is the format of the buffer
                        // We must use the loaded format for saving to avoid pitch mismatch
                        tj3SaveImage16(handle, "./dummy_save_file", buffer, width, 0, height, loadedFormat);
                        
                        // Also test with the same format but different pitch? 
                        // If we pass pitch=0, it's safe.
                        // Let's try to save with the loaded format only.
                        
                        tj3Free(buffer);
                    } else {
                        // Check error
                        char *err = tj3GetErrorStr(handle);
                        if (err) {
                            // Error message retrieved
                        }
                    }
                }
            }
        }
        
        // Test tj3SaveImage16 directly with generated data
        int testWidth = (Size > 0 ? Data[0] % 100 + 1 : 10);
        int testHeight = (Size > 1 ? Data[1] % 100 + 1 : 10);
        
        if (testWidth > 1000 || testHeight > 1000) {
            testWidth = 100;
            testHeight = 100;
        }

        int testPixelFormats[] = {TJPF_GRAY, TJPF_RGB, TJPF_CMYK};
        
        for (int spf = 0; spf < 3; spf++) {
            int saveFormat = testPixelFormats[spf];
            int testChannels = (saveFormat == TJPF_GRAY) ? 1 : 3;
            
            size_t bufSize = (size_t)testWidth * (size_t)testHeight * (size_t)testChannels * sizeof(unsigned short);
            unsigned short *testBuffer = (unsigned short *)malloc(bufSize);
            if (testBuffer) {
                memset(testBuffer, 0, bufSize);
                // Pass 0 for pitch to let library calculate standard pitch
                tj3SaveImage16(handle, "./dummy_save_file", testBuffer, testWidth, 0, testHeight, saveFormat);
                free(testBuffer);
            }
        }
        
        // Test error retrieval functions
        char *err1 = tjGetErrorStr();
        char *err2 = tjGetErrorStr2(handle);
        char *err3 = tj3GetErrorStr(handle);
        char *err4 = tj3GetErrorStr(NULL);
        
        // Cleanup
        tj3Destroy(handle);
    }
    
    return 0;
}