// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Decompress16 at turbojpeg-mp.c:148:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Compress16 at turbojpeg-mp.c:71:15 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Init at turbojpeg.c:538:20 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3LoadImage16 at turbojpeg-mp.c:293:21 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Free at turbojpeg.c:890:16 in turbojpeg.h
// tj3Destroy at turbojpeg.c:580:16 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3DecompressHeader at turbojpeg.c:1815:15 in turbojpeg.h
// tj3Alloc at turbojpeg.c:877:17 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <turbojpeg.h>

static int fuzzer_test_one_input(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Initialize TurboJPEG instance for both compression and decompression
    tjhandle handle = tj3Init(TJINIT_DECOMPRESS | TJINIT_COMPRESS);
    if (!handle) {
        return 0;
    }

    // Use the first byte to determine the test path
    int test_mode = Data[0] % 4;
    size_t data_offset = 1;
    size_t remaining_size = Size - data_offset;

    // Prepare dummy file for tj3LoadImage16 if needed
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f) {
        // Write minimal PGM header and some data to create a valid-ish file
        fprintf(f, "P6\n%d %d\n255\n", 10, 10);
        // Write at least some binary data
        size_t write_len = remaining_size > 300 ? 300 : remaining_size;
        if (write_len > 0) {
            fwrite(Data + data_offset, 1, write_len, f);
        }
        fclose(f);
    }

    unsigned char *jpegBuf = NULL;
    size_t jpegSize = 0;
    unsigned short *dstBuf = NULL;
    unsigned short *srcBuf = NULL;
    int width = 0, height = 0, pixelFormat = 0;
    int pitch = 0;

    // Ensure we have enough data for buffer operations if needed
    if (remaining_size < 2) {
        goto cleanup;
    }

    switch (test_mode) {
        case 0: {
            // Test tj3DecompressHeader
            // Use remaining data as JPEG buffer
            const unsigned char *jpegData = Data + data_offset;
            size_t jpegLen = remaining_size;

            // Try with different parameters if data allows
            if (jpegLen > 0) {
                tj3DecompressHeader(handle, jpegData, jpegLen);
            }

            // Try with size 0
            tj3DecompressHeader(handle, jpegData, 0);

            // Try with NULL buffer (should error)
            tj3DecompressHeader(handle, NULL, jpegLen);

            // Try with very large size
            tj3DecompressHeader(handle, jpegData, SIZE_MAX);

            break;
        }

        case 1: {
            // Test tj3Decompress16
            // Need a destination buffer
            if (remaining_size < 4) {
                goto cleanup;
            }

            // Use data to determine dimensions and format
            int w = (Data[data_offset] % 100) + 1;
            int h = (Data[data_offset + 1] % 100) + 1;
            int pf = (Data[data_offset + 2] % 5); // 0-4
            int p = (Data[data_offset + 3] % 10) + 1;

            // Allocate destination buffer
            size_t buf_size = (size_t)w * h * 3 * sizeof(unsigned short);
            if (buf_size > 10 * 1024 * 1024) {
                buf_size = 10 * 1024 * 1024;
                w = (size_t)buf_size / (h * 3 * sizeof(unsigned short));
            }

            dstBuf = (unsigned short *)tj3Alloc(buf_size);
            if (!dstBuf) {
                goto cleanup;
            }

            // Use remaining data as JPEG buffer
            const unsigned char *jpegData = Data + data_offset + 4;
            size_t jpegLen = remaining_size - 4;

            // Call decompress with various parameters
            tj3Decompress16(handle, jpegData, jpegLen, dstBuf, w * 3, pf);
            tj3Decompress16(handle, NULL, jpegLen, dstBuf, w * 3, pf);
            tj3Decompress16(handle, jpegData, 0, dstBuf, w * 3, pf);
            tj3Decompress16(handle, jpegData, jpegLen, NULL, w * 3, pf);
            tj3Decompress16(handle, jpegData, jpegLen, dstBuf, 0, pf);

            tj3Free(dstBuf);
            dstBuf = NULL;

            break;
        }

        case 2: {
            // Test tj3Compress16
            if (remaining_size < 4) {
                goto cleanup;
            }

            int w = (Data[data_offset] % 100) + 1;
            int h = (Data[data_offset + 1] % 100) + 1;
            int pf = (Data[data_offset + 2] % 5);
            int p = (Data[data_offset + 3] % 10) + 1;

            // Allocate source buffer
            size_t buf_size = (size_t)w * h * 3 * sizeof(unsigned short);
            if (buf_size > 10 * 1024 * 1024) {
                buf_size = 10 * 1024 * 1024;
                w = (size_t)buf_size / (h * 3 * sizeof(unsigned short));
            }

            srcBuf = (unsigned short *)tj3Alloc(buf_size);
            if (!srcBuf) {
                goto cleanup;
            }

            // Fill source buffer with test data
            memset(srcBuf, 0, buf_size);
            for (size_t i = 0; i < buf_size && i < remaining_size - 4; i++) {
                srcBuf[i] = (unsigned short)(Data[data_offset + 4 + i] * 256);
            }

            unsigned char *outBuf = NULL;
            size_t outSize = 0;

            // Test compression
            tj3Compress16(handle, srcBuf, w, p * 3, h, pf, &outBuf, &outSize);
            tj3Compress16(handle, srcBuf, w, 0, h, pf, &outBuf, &outSize);
            tj3Compress16(handle, NULL, w, p * 3, h, pf, &outBuf, &outSize);
            tj3Compress16(handle, srcBuf, 0, p * 3, h, pf, &outBuf, &outSize);

            if (outBuf) {
                tj3Free(outBuf);
                outBuf = NULL;
            }

            tj3Free(srcBuf);
            srcBuf = NULL;

            break;
        }

        case 3: {
            // Test tj3Get and tj3LoadImage16
            int param = (Data[data_offset] % 10);
            tj3Get(handle, param);
            tj3Get(handle, -1);
            tj3Get(handle, 1000);

            // Test load image
            int align = 1 << (Data[data_offset + 1] % 4);
            int pf = (Data[data_offset + 2] % 5);

            unsigned short *loaded = tj3LoadImage16(handle, dummy_file, &width, align, &height, &pixelFormat);
            if (loaded) {
                tj3Free(loaded);
            }

            // Test with different formats
            int pf_test[] = {TJPF_UNKNOWN, TJPF_GRAY, TJPF_CMYK, TJPF_RGB, TJPF_BGR};
            for (int i = 0; i < 5; i++) {
                loaded = tj3LoadImage16(handle, dummy_file, &width, align, &height, &pf_test[i]);
                if (loaded) {
                    tj3Free(loaded);
                }
            }

            // Test with invalid file
            loaded = tj3LoadImage16(handle, "./nonexistent_file", &width, align, &height, &pixelFormat);
            if (loaded) {
                tj3Free(loaded);
            }

            break;
        }
    }

cleanup:
    if (jpegBuf) {
        tj3Free(jpegBuf);
    }
    if (dstBuf) {
        tj3Free(dstBuf);
    }
    if (srcBuf) {
        tj3Free(srcBuf);
    }
    if (handle) {
        tj3Destroy(handle);
    }
    // Clean up dummy file
    remove(dummy_file);

    return 0;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    return fuzzer_test_one_input(Data, Size);
}