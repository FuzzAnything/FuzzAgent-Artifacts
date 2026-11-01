// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Allocate output buffer for decompression
    uint8_t *output_buf = NULL;
    size_t output_size = 0;
    
    // Use input data as window for inflateBack
    uint8_t *window = NULL;
    
    // Determine operation based on first byte to diversify execution paths
    int op = Data[0] % 6;
    
    int ret = Z_OK;
    
    switch (op) {
        case 0: {
            // Test inflateBackInit_
            // Allocate window buffer if size allows
            if (Size >= 32) {
                window = malloc(32);
                if (window) {
                    memcpy(window, Data, 32);
                }
            }
            
            // Try various windowBits values (8-15)
            int windowBits = 8 + (Data[1] % 8);
            
            ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
            
            if (ret == Z_OK) {
                // Call inflateBackEnd to cleanup
                inflateBackEnd(&strm);
            }
            if (window) free(window);
            break;
        }
        
        case 1: {
            // Test inflateInit2_
            int windowBits = -15 + (Data[1] % 31); // Range -15 to 15
            
            ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
            
            if (ret == Z_OK) {
                // Setup input
                strm.next_in = Data;
                strm.avail_in = Size;
                
                // Allocate output buffer
                output_size = Size * 2 + 1024;
                output_buf = malloc(output_size);
                if (output_buf) {
                    strm.next_out = output_buf;
                    strm.avail_out = output_size;
                    
                    // Try to decompress
                    inflate(&strm, Z_NO_FLUSH);
                    
                    // Test inflatePrime with various bit values
                    // Ensure we have enough data before accessing Data[2] and Data[3]
                    int bits = (Data[2] % 17) - 1; // -1 to 15
                    int value = Data[3] % 256;
                    inflatePrime(&strm, bits, value);
                    
                    inflateEnd(&strm);
                }
            }
            break;
        }
        
        case 2: {
            // Test inflateInit_
            ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
            
            if (ret == Z_OK) {
                strm.next_in = Data;
                strm.avail_in = Size;
                
                output_size = Size * 2 + 1024;
                output_buf = malloc(output_size);
                if (output_buf) {
                    strm.next_out = output_buf;
                    strm.avail_out = output_size;
                    
                    inflate(&strm, Z_NO_FLUSH);
                    inflateEnd(&strm);
                }
            }
            break;
        }
        
        case 3: {
            // Test zError with various error codes
            // Generate error codes from input data
            int error_code = Data[0] % 20 - 10; // Range -10 to 9
            const char *msg = zError(error_code);
            (void)msg; // Use the result to avoid unused variable warning
            
            // Also test with known error codes
            msg = zError(Z_OK);
            msg = zError(Z_STREAM_ERROR);
            msg = zError(Z_DATA_ERROR);
            msg = zError(Z_MEM_ERROR);
            msg = zError(Z_VERSION_ERROR);
            break;
        }
        
        case 4: {
            // Test inflateBackInit_ with invalid parameters
            // Null window
            ret = inflateBackInit_(&strm, 15, NULL, ZLIB_VERSION, (int)sizeof(z_stream));
            if (ret == Z_OK) {
                inflateBackEnd(&strm);
            }
            
            // Invalid windowBits
            ret = inflateBackInit_(&strm, 5, NULL, ZLIB_VERSION, (int)sizeof(z_stream));
            if (ret == Z_OK) {
                inflateBackEnd(&strm);
            }
            
            ret = inflateBackInit_(&strm, 20, NULL, ZLIB_VERSION, (int)sizeof(z_stream));
            if (ret == Z_OK) {
                inflateBackEnd(&strm);
            }
            break;
        }
        
        case 5: {
            // Test inflatePrime on uninitialized stream
            int bits = Data[0] % 17;
            int value = Data[1] % 256;
            ret = inflatePrime(&strm, bits, value);
            (void)ret;
            
            // Test with negative bits
            ret = inflatePrime(&strm, -1, 0);
            (void)ret;
            
            // Test with zero bits
            ret = inflatePrime(&strm, 0, 0);
            (void)ret;
            break;
        }
    }
    
    // Cleanup
    if (output_buf) {
        free(output_buf);
    }
    
    return 0;
}