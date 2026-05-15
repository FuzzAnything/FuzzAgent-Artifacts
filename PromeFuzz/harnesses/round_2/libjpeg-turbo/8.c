// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitCompress at turbojpeg.c:1157:20 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Set at turbojpeg.c:671:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tj3Get at turbojpeg.c:807:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "src/turbojpeg.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Write input to dummy file for potential file-based operations
    write_dummy_file(Data, Size);

    // Test tjInitCompress
    tjhandle compress_handle = tjInitCompress();
    if (!compress_handle) return 0;

    // Test tj3Set with various parameters using fuzzed data
    // Use first few bytes to determine parameter values
    int param1 = Data[0] % 30;  // Limit to reasonable parameter range
    int value1 = (int)Data[1];
    tj3Set(compress_handle, param1, value1);

    int param2 = Data[2] % 30;
    int value2 = (int)Data[3];
    tj3Set(compress_handle, param2, value2);

    // Test tj3Get for various parameters
    tj3Get(compress_handle, param1);
    tj3Get(compress_handle, param2);
    tj3Get(compress_handle, TJPARAM_QUALITY);
    tj3Get(compress_handle, TJPARAM_SUBSAMP);

    // Test decompression functions if we have enough data
    if (Size > 100) {
        // Create a decompression handle (using tjInitCompress as base)
        // In real usage, we'd use tjInitDecompress, but we're testing given APIs
        unsigned char *jpegBuf = (unsigned char *)Data;
        unsigned long jpegSize = Size;
        int width = 0, height = 0;

        // Test tjDecompressHeader
        int header_result = tjDecompressHeader(compress_handle, jpegBuf, jpegSize, &width, &height);
        
        if (header_result == 0 && width > 0 && height > 0) {
            // Allocate buffer for decompression
            int pixelFormat = TJPF_RGB;
            int pitch = width * 3;  // 3 bytes per pixel for RGB
            unsigned char *dstBuf = (unsigned char *)malloc(height * pitch);
            
            if (dstBuf) {
                // Test tjDecompress2
                tjDecompress2(compress_handle, jpegBuf, jpegSize, dstBuf, 
                             width, pitch, height, pixelFormat, 0);
                free(dstBuf);
            }
        }
    }

    // Test tjDestroy
    tjDestroy(compress_handle);

    // Additional test: create another handle and test parameter setting/getting
    tjhandle handle2 = tjInitCompress();
    if (handle2) {
        // Set some parameters with different values
        for (int i = 0; i < 5 && i * 2 + 1 < Size; i++) {
            int param = Data[i * 2] % 30;
            int value = (int)Data[i * 2 + 1];
            tj3Set(handle2, param, value);
            
            // Get the value back
            tj3Get(handle2, param);
        }
        
        // Test with known parameters
        tj3Set(handle2, TJPARAM_QUALITY, 85);
        tj3Set(handle2, TJPARAM_SUBSAMP, TJSAMP_420);
        
        tj3Get(handle2, TJPARAM_QUALITY);
        tj3Get(handle2, TJPARAM_SUBSAMP);
        
        tjDestroy(handle2);
    }

    return 0;
}