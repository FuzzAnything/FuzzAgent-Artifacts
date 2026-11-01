// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Allocate buffers for decompression
    // Use a large buffer to handle potential expansion
    size_t dest_size = Size * 10 + 1024;
    if (dest_size > 100 * 1024 * 1024) { // Limit max allocation to 100MB
        dest_size = 100 * 1024 * 1024;
    }
    
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (dest == NULL) {
        return 0;
    }

    // Prepare source data
    const Bytef *source = (const Bytef *)Data;
    uLong sourceLen_uLong = (uLong)Size;
    z_size_t sourceLen_zSize = (z_size_t)Size;
    uLongf destLen_uLong = (uLongf)dest_size;
    z_size_t destLen_zSize = (z_size_t)dest_size;

    // Test uncompress
    destLen_uLong = dest_size;
    int ret = uncompress(dest, &destLen_uLong, source, sourceLen_uLong);
    if (ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR && ret != Z_STREAM_ERROR) {
        // Unexpected error, but we continue
    }

    // Test uncompress2
    destLen_uLong = dest_size;
    uLong sourceLen_uLong_copy = sourceLen_uLong;
    ret = uncompress2(dest, &destLen_uLong, source, &sourceLen_uLong_copy);
    if (ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR && ret != Z_STREAM_ERROR) {
        // Unexpected error
    }

    // Test uncompress_z (z_size_t version)
    destLen_zSize = dest_size;
    ret = uncompress_z(dest, &destLen_zSize, source, sourceLen_zSize);
    if (ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR && ret != Z_STREAM_ERROR) {
        // Unexpected error
    }

    // Test uncompress2_z (z_size_t version)
    destLen_zSize = dest_size;
    z_size_t sourceLen_zSize_copy = sourceLen_zSize;
    ret = uncompress2_z(dest, &destLen_zSize, source, &sourceLen_zSize_copy);
    if (ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR && ret != Z_STREAM_ERROR) {
        // Unexpected error
    }

    // Test zError with various error codes
    const char *msg;
    msg = zError(Z_OK);
    (void)msg;
    msg = zError(Z_STREAM_ERROR);
    (void)msg;
    msg = zError(Z_DATA_ERROR);
    (void)msg;
    msg = zError(Z_BUF_ERROR);
    (void)msg;
    msg = zError(Z_MEM_ERROR);
    (void)msg;
    msg = zError(Z_VERSION_ERROR);
    (void)msg;
    msg = zError(-1); // Invalid code
    (void)msg;
    msg = zError(100); // Invalid code
    (void)msg;

    // Test inflate
    z_stream strm = {0};
    strm.next_in = source;
    strm.avail_in = (uInt)Size;
    strm.next_out = dest;
    strm.avail_out = (uInt)dest_size;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    int init_ret = inflateInit(&strm);
    if (init_ret == Z_OK) {
        // Try different flush modes
        int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK};
        for (int i = 0; i < 5; i++) {
            // Reset stream for each flush mode test to avoid state issues
            inflateEnd(&strm);
            strm.next_in = source;
            strm.avail_in = (uInt)Size;
            strm.next_out = dest;
            strm.avail_out = (uInt)dest_size;
            strm.zalloc = Z_NULL;
            strm.zfree = Z_NULL;
            strm.opaque = Z_NULL;
            
            if (inflateInit(&strm) == Z_OK) {
                ret = inflate(&strm, flush_modes[i]);
                if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR && ret != Z_STREAM_ERROR) {
                    // Unexpected error
                }
            }
            inflateEnd(&strm);
        }
    }

    // Cleanup
    inflateEnd(&strm);
    free(dest);

    return 0;
}