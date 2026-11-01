// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test 1: zlibVersion
    const char *version = zlibVersion();
    if (version == NULL) {
        return 0;
    }

    // Test 2: zlibCompileFlags
    uLong flags = zlibCompileFlags();
    (void)flags;

    // Prepare buffer for dictionary and input data
    const Bytef *dictionary = NULL;
    uInt dictLength = 0;
    
    if (Size > 0) {
        dictionary = (const Bytef *)Data;
        dictLength = (uInt)Size;
        // Limit dictionary size to avoid huge allocations in fuzzing
        if (dictLength > 65535) {
            dictLength = 65535;
        }
    }

    // Allocate output buffer
    Byte *out_buf = (Byte *)malloc(Size + 256);
    if (out_buf == NULL) {
        return 0;
    }

    // Test 3: deflateInit_
    z_stream strm_def = {0};
    int ret = deflateInit_(&strm_def, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Test 4: deflateSetDictionary
        if (dictionary != NULL && dictLength > 0) {
            deflateSetDictionary(&strm_def, dictionary, dictLength);
        }

        // Setup input for deflate
        strm_def.next_in = (const Bytef *)Data;
        strm_def.avail_in = (uInt)Size;
        strm_def.next_out = out_buf;
        strm_def.avail_out = (uInt)(Size + 256);

        // Perform deflate
        deflate(&strm_def, Z_FINISH);
        
        // Cleanup deflate
        deflateEnd(&strm_def);
    }

    // Test 5: deflateInit2_
    z_stream strm_def2 = {0};
    // Fuzz some parameters safely
    int level = (Size % 10); // 0-9
    int method = (Size % 2 == 0) ? Z_DEFLATED : 0;
    int windowBits = (Size % 16) - 8; // -8 to 7
    int memLevel = (Size % 9) + 1; // 1-9
    int strategy = (Size % 3); // 0-2

    ret = deflateInit2_(&strm_def2, level, method, windowBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        if (dictionary != NULL && dictLength > 0) {
            deflateSetDictionary(&strm_def2, dictionary, dictLength);
        }
        deflateEnd(&strm_def2);
    }

    // Test 6: inflateInit_
    z_stream strm_inf = {0};
    ret = inflateInit_(&strm_inf, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Try to decompress the output from the first deflate if it produced data
        if (strm_def.total_out > 0) {
            strm_inf.next_in = out_buf;
            strm_inf.avail_in = (uInt)strm_def.total_out;
            
            Byte *in_buf = (Byte *)malloc(Size + 256);
            if (in_buf != NULL) {
                strm_inf.next_out = in_buf;
                strm_inf.avail_out = (uInt)(Size + 256);
                
                inflate(&strm_inf, Z_FINISH);
                free(in_buf);
            }
        }
        inflateEnd(&strm_inf);
    }

    free(out_buf);
    return 0;
}