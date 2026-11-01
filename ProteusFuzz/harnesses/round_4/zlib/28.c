// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress_z at uncompr.c:92:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Allocate buffers for decompression
    // Destination buffer: 4x input size to handle worst-case decompression
    size_t dest_size = Size * 4 + 1024;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (dest == NULL) {
        return 0;
    }

    // Test 1: uncompress_z
    z_size_t destLen_z = dest_size;
    z_size_t sourceLen_z = Size;
    int ret_z = uncompress_z(dest, &destLen_z, (const Bytef *)Data, sourceLen_z);
    // zError is safe to call with any int, including error codes
    (void)zError(ret_z);

    // Test 2: uncompress2_z
    destLen_z = dest_size;
    z_size_t sourceLen_z2 = Size;
    int ret_z2 = uncompress2_z(dest, &destLen_z, (const Bytef *)Data, &sourceLen_z2);
    (void)zError(ret_z2);

    // Test 3: uncompress2
    uLong destLen_u = dest_size;
    uLong sourceLen_u = Size;
    int ret_u2 = uncompress2(dest, &destLen_u, (const Bytef *)Data, &sourceLen_u);
    (void)zError(ret_u2);

    // Test 4: uncompress
    destLen_u = dest_size;
    uLong sourceLen = Size;
    int ret_u = uncompress(dest, &destLen_u, (const Bytef *)Data, sourceLen);
    (void)zError(ret_u);

    // Test 5: inflate (incremental decompression)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.next_in = (Bytef const *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = dest;
    strm.avail_out = (uInt)dest_size;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    int ret_init = inflateInit(&strm);
    if (ret_init == Z_OK) {
        int ret_inf = inflate(&strm, Z_NO_FLUSH);
        (void)zError(ret_inf);
        
        // Try with different flush modes if stream is still active
        if (ret_inf == Z_OK && strm.avail_out > 0) {
            ret_inf = inflate(&strm, Z_SYNC_FLUSH);
            (void)zError(ret_inf);
        }
        
        inflateEnd(&strm);
    }

    // Test 6: Edge case for uncompress2_z with NULL dest to query size
    z_size_t req_size = 0;
    int ret_query = uncompress2_z(NULL, &req_size, (const Bytef *)Data, &sourceLen_z2);
    (void)zError(ret_query);

    free(dest);
    return 0;
}