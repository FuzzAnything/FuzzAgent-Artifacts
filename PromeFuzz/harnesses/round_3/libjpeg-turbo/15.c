// This fuzz driver is generated for library libjpeg-turbo, aiming to fuzz the following functions:
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompress2 at turbojpeg.c:2059:15 in turbojpeg.h
// tjDestroy at turbojpeg.c:601:15 in turbojpeg.h
// tjInitDecompress at turbojpeg.c:1808:20 in turbojpeg.h
// tjBufSizeYUV2 at turbojpeg.c:999:25 in turbojpeg.h
// tjDecompressToYUV2 at turbojpeg.c:2401:15 in turbojpeg.h
// tjBufSizeYUV at turbojpeg.c:1007:25 in turbojpeg.h
// tjDecompressToYUV at turbojpeg.c:2449:15 in turbojpeg.h
// tjDecompressHeader3 at turbojpeg.c:1874:15 in turbojpeg.h
// tjDecompressHeader at turbojpeg.c:1914:15 in turbojpeg.h
// tjDecompressHeader2 at turbojpeg.c:1903:15 in turbojpeg.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "turbojpeg.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIMENSION 16384
#define MAX_BUFFER_SIZE (MAX_DIMENSION * MAX_DIMENSION * 4)

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    // Write data to dummy file for potential file-based operations
    write_dummy_file(Data, Size);
    
    tjhandle handle = tjInitDecompress();
    if (!handle) return 0;
    
    // Variables for header extraction
    int width = 0, height = 0, jpegSubsamp = 0, jpegColorspace = 0;
    unsigned char *dstBuf = NULL;
    int flags = 0;
    
    // Randomly select which API to test based on input data
    int selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            // tjDecompressToYUV2
            if (Size > 100) {
                width = (Data[1] * 256 + Data[2]) % MAX_DIMENSION + 1;
                height = (Data[3] * 256 + Data[4]) % MAX_DIMENSION + 1;
                int align = (Data[5] % 16) + 1;
                flags = Data[6] % 256;
                
                unsigned long yuvSize = tjBufSizeYUV2(width, align, height, TJSAMP_444);
                if (yuvSize > 0 && yuvSize < MAX_BUFFER_SIZE) {
                    dstBuf = (unsigned char *)malloc(yuvSize);
                    if (dstBuf) {
                        tjDecompressToYUV2(handle, Data, Size, dstBuf, 
                                          width, align, height, flags);
                        free(dstBuf);
                    }
                }
            }
            break;
        }
        
        case 1: {
            // tjDecompressToYUV
            if (Size > 50) {
                flags = Data[1] % 256;
                unsigned long yuvSize = tjBufSizeYUV(MAX_DIMENSION, MAX_DIMENSION, TJSAMP_444);
                if (yuvSize > 0 && yuvSize < MAX_BUFFER_SIZE) {
                    dstBuf = (unsigned char *)malloc(yuvSize);
                    if (dstBuf) {
                        tjDecompressToYUV(handle, (unsigned char *)Data, Size, 
                                         dstBuf, flags);
                        free(dstBuf);
                    }
                }
            }
            break;
        }
        
        case 2: {
            // tjDecompressHeader3
            if (Size > 10) {
                tjDecompressHeader3(handle, Data, Size, &width, &height, 
                                   &jpegSubsamp, &jpegColorspace);
            }
            break;
        }
        
        case 3: {
            // tjDecompressHeader
            if (Size > 10) {
                tjDecompressHeader(handle, (unsigned char *)Data, Size, 
                                  &width, &height);
            }
            break;
        }
        
        case 4: {
            // tjDecompressHeader2
            if (Size > 10) {
                tjDecompressHeader2(handle, (unsigned char *)Data, Size, 
                                   &width, &height, &jpegSubsamp);
            }
            break;
        }
        
        case 5: {
            // tjDecompress2
            if (Size > 100) {
                if (tjDecompressHeader3(handle, Data, Size, &width, &height, 
                                       &jpegSubsamp, &jpegColorspace) == 0) {
                    if (width > 0 && height > 0 && width < MAX_DIMENSION && height < MAX_DIMENSION) {
                        int pixelFormat = Data[1] % 10; // Limited to valid formats
                        int pitch = width * tjPixelSize[pixelFormat];
                        flags = Data[2] % 256;
                        
                        unsigned long bufSize = pitch * height;
                        if (bufSize > 0 && bufSize < MAX_BUFFER_SIZE) {
                            dstBuf = (unsigned char *)malloc(bufSize);
                            if (dstBuf) {
                                tjDecompress2(handle, Data, Size, dstBuf, 
                                             width, pitch, height, pixelFormat, flags);
                                free(dstBuf);
                            }
                        }
                    }
                }
            }
            break;
        }
    }
    
    tjDestroy(handle);
    return 0;
}