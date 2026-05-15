// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int read_int(const uint8_t *data, size_t size, size_t *pos, int default_val) {
    if (*pos + 4 <= size) {
        int val;
        memcpy(&val, data + *pos, 4);
        *pos += 4;
        return val;
    }
    return default_val;
}

static void set_header_from_data(gz_headerp header, const uint8_t *data, size_t size, size_t *pos) {
    memset(header, 0, sizeof(gz_header));
    
    header->text = read_int(data, size, pos, 0);
    header->time = read_int(data, size, pos, 0);
    header->xflags = read_int(data, size, pos, 0);
    header->os = read_int(data, size, pos, 0);
    header->hcrc = read_int(data, size, pos, 0);
    header->done = read_int(data, size, pos, 0);
    
    int extra_len = read_int(data, size, pos, 0);
    if (extra_len > 0 && *pos + extra_len <= size) {
        header->extra = (Bytef*)(data + *pos);
        header->extra_len = extra_len;
        *pos += extra_len;
    }
    
    int name_len = read_int(data, size, pos, 0);
    if (name_len > 0 && *pos + name_len <= size) {
        header->name = (Bytef*)(data + *pos);
        *pos += name_len;
    }
    
    int comment_len = read_int(data, size, pos, 0);
    if (comment_len > 0 && *pos + comment_len <= size) {
        header->comment = (Bytef*)(data + *pos);
        *pos += comment_len;
    }
    
    header->extra_max = 0;
    header->name_max = 0;
    header->comm_max = 0;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    int function_choice = Data[0] % 6;
    const uint8_t *params = Data + 1;
    size_t params_size = Size - 1;
    size_t pos = 0;
    
    switch (function_choice) {
        case 0: {
            z_stream stream;
            gz_header header;
            memset(&stream, 0, sizeof(z_stream));
            
            int ret = deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15+16, 8, Z_DEFAULT_STRATEGY);
            if (ret != Z_OK) break;
            
            set_header_from_data(&header, params, params_size, &pos);
            ret = deflateSetHeader(&stream, &header);
            
            deflateEnd(&stream);
            break;
        }
        
        case 1: {
            z_stream stream;
            memset(&stream, 0, sizeof(z_stream));
            
            int windowBits = read_int(params, params_size, &pos, 15+16);
            int ret = inflateInit2(&stream, windowBits);
            if (ret != Z_OK) break;
            
            if (pos < params_size) {
                stream.next_in = (Bytef*)(params + pos);
                stream.avail_in = params_size - pos;
                
                Bytef out[1024];
                stream.next_out = out;
                stream.avail_out = sizeof(out);
                
                ret = inflate(&stream, Z_NO_FLUSH);
            }
            
            inflateEnd(&stream);
            break;
        }
        
        case 2: {
            z_stream stream;
            memset(&stream, 0, sizeof(z_stream));
            
            int windowBits = read_int(params, params_size, &pos, 15+16);
            int stream_size = sizeof(z_stream);
            
            inflateInit2_(&stream, windowBits, ZLIB_VERSION, stream_size);
            
            inflateEnd(&stream);
            break;
        }
        
        case 3: {
            z_stream stream;
            gz_header header;
            memset(&stream, 0, sizeof(z_stream));
            
            int windowBits = read_int(params, params_size, &pos, 15+16);
            int ret = inflateInit2(&stream, windowBits);
            if (ret != Z_OK) break;
            
            set_header_from_data(&header, params, params_size, &pos);
            ret = inflateGetHeader(&stream, &header);
            
            inflateEnd(&stream);
            break;
        }
        
        case 4: {
            z_stream stream;
            memset(&stream, 0, sizeof(z_stream));
            
            int windowBits = read_int(params, params_size, &pos, 15+16);
            int ret = inflateInit2(&stream, windowBits);
            if (ret != Z_OK) break;
            
            int bits = read_int(params, params_size, &pos, 0);
            int value = read_int(params, params_size, &pos, 0);
            
            if (bits < 0) bits = -bits;
            if (bits > 16) bits = 16;
            
            ret = inflatePrime(&stream, bits, value);
            
            inflateEnd(&stream);
            break;
        }
        
        case 5: {
            z_stream stream;
            memset(&stream, 0, sizeof(z_stream));
            
            int windowBits = read_int(params, params_size, &pos, 15+16);
            int ret = inflateInit2(&stream, windowBits);
            if (ret != Z_OK) break;
            
            int new_windowBits = read_int(params, params_size, &pos, 15+16);
            ret = inflateReset2(&stream, new_windowBits);
            
            inflateEnd(&stream);
            break;
        }
    }
    
    return 0;
}