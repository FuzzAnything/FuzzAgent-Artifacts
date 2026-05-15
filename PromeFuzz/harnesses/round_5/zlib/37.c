// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void init_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

static void cleanup_stream(z_streamp strm) {
    if (strm->state != Z_NULL) {
        inflateEnd(strm);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use first byte to control fuzzing behavior
    uint8_t selector = Data[0];
    const uint8_t* compressed_data = Data + 1;
    size_t compressed_size = Size - 1;

    // Create dummy compressed file if needed
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (compressed_size > 0) {
            write(fd, compressed_data, compressed_size);
        }
        close(fd);
    }

    z_stream strm1, strm2;
    int ret;
    unsigned long codes_used;

    // Initialize first stream
    init_stream(&strm1);
    
    // Test inflateInit2_ with different windowBits values
    int windowBits = (selector & 0x0F) + 8; // 8-23
    if (selector & 0x10) windowBits = -windowBits; // Negative for raw inflate
    
    ret = inflateInit2_(&strm1, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        cleanup_stream(&strm1);
        return 0;
    }

    // Set up input data for inflation
    strm1.next_in = (Bytef*)compressed_data;
    strm1.avail_in = compressed_size > 1024 ? 1024 : compressed_size;
    
    // Allocate output buffer
    Bytef out_buf[4096];
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);

    // Test inflatePrime with different bits and values
    int bits = selector & 0x0F;
    int value = (selector >> 4) & 0xFF;
    if (bits > 16) bits = 16;
    inflatePrime(&strm1, bits, value);

    // Test inflateUndermine
    int undermine = selector & 0x01;
    inflateUndermine(&strm1, undermine);

    // Perform inflation with different flush modes
    int flush_modes[] = {Z_NO_FLUSH, Z_PARTIAL_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK};
    int flush_idx = selector % (sizeof(flush_modes)/sizeof(flush_modes[0]));
    
    do {
        ret = inflate(&strm1, flush_modes[flush_idx]);
        if (ret == Z_STREAM_ERROR || ret == Z_NEED_DICT || ret == Z_DATA_ERROR || 
            ret == Z_MEM_ERROR || ret == Z_BUF_ERROR) {
            break;
        }
        
        // Reset output buffer if full
        if (strm1.avail_out == 0) {
            strm1.next_out = out_buf;
            strm1.avail_out = sizeof(out_buf);
        }
    } while (ret != Z_STREAM_END && strm1.avail_in > 0);

    // Test inflateCodesUsed
    codes_used = inflateCodesUsed(&strm1);

    // Test inflateCopy if stream is valid
    if (strm1.state != Z_NULL) {
        init_stream(&strm2);
        ret = inflateCopy(&strm2, &strm1);
        if (ret == Z_OK) {
            // Use the copied stream
            Bytef out_buf2[1024];
            strm2.next_out = out_buf2;
            strm2.avail_out = sizeof(out_buf2);
            
            // Try inflating with copied stream
            inflate(&strm2, Z_NO_FLUSH);
            inflateCodesUsed(&strm2);
            
            cleanup_stream(&strm2);
        }
    }

    // Cleanup
    cleanup_stream(&strm1);

    // Remove dummy file
    unlink("./dummy_file");

    return 0;
}