// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_codec.h"
#include "aom_decoder.h"

// Declare the AV1 decoder interface function
extern aom_codec_iface_t *aom_codec_av1_dx(void);

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Early return if no data
    if (Size == 0) {
        return 0;
    }

    // 1. Test aom_codec_peek_stream_info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    
    // Initialize required is_annexb field
    stream_info.is_annexb = (Data[0] & 1);
    
    // Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (iface) {
        aom_codec_err_t peek_err = aom_codec_peek_stream_info(
            iface, Data, Size, &stream_info);
        const char *err_str = aom_codec_err_to_string(peek_err);
        (void)err_str; // Prevent unused variable warning
    }

    // 2. Test decoder initialization and decoding
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Initialize decoder configuration
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = (Data[0] % 8) + 1; // Use 1-8 threads based on input
    
    // Initialize decoder
    aom_codec_err_t init_err = aom_codec_dec_init_ver(
        &ctx, 
        aom_codec_av1_dx(), 
        &cfg, 
        0, // flags
        AOM_DECODER_ABI_VERSION
    );
    
    if (init_err == AOM_CODEC_OK) {
        // 3. Test aom_codec_get_stream_info
        aom_codec_stream_info_t si;
        memset(&si, 0, sizeof(si));
        si.is_annexb = (Data[0] & 1);
        
        aom_codec_err_t stream_err = aom_codec_get_stream_info(&ctx, &si);
        const char *stream_err_str = aom_codec_err_to_string(stream_err);
        (void)stream_err_str;
        
        // 4. Test aom_codec_decode
        // Use a subset of data for decoding to avoid buffer overflows
        size_t decode_size = Size;
        if (Size > 1024 * 1024) { // Limit to 1MB for fuzzing
            decode_size = 1024 * 1024;
        }
        
        aom_codec_err_t decode_err = aom_codec_decode(
            &ctx, 
            Data, 
            decode_size, 
            NULL // user_priv
        );
        const char *decode_err_str = aom_codec_err_to_string(decode_err);
        (void)decode_err_str;
        
        // Try decoding again with different parameters
        if (Size > 1) {
            // Try with half the data
            aom_codec_decode(&ctx, Data, Size / 2, NULL);
            
            // Try with single byte
            if (Size > 10) {
                aom_codec_decode(&ctx, Data + 5, 1, NULL);
            }
        }
        
        // 5. Test aom_codec_destroy
        aom_codec_err_t destroy_err = aom_codec_destroy(&ctx);
        const char *destroy_err_str = aom_codec_err_to_string(destroy_err);
        (void)destroy_err_str;
    } else {
        // If initialization failed, still try to destroy
        aom_codec_destroy(&ctx);
        
        // Test error string conversion
        const char *init_err_str = aom_codec_err_to_string(init_err);
        (void)init_err_str;
    }

    // 6. Test aom_codec_err_to_string with various error codes
    for (int i = 0; i < 10; i++) {
        const char *err_str = aom_codec_err_to_string((aom_codec_err_t)i);
        (void)err_str;
    }

    // 7. Test with file-based approach if needed
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        // Write first 1KB of data or all if smaller
        size_t write_size = Size > 1024 ? 1024 : Size;
        fwrite(Data, 1, write_size, fp);
        fclose(fp);
        
        // Re-open and read back to test file-based decoding scenarios
        fp = fopen(filename, "rb");
        if (fp) {
            uint8_t file_buffer[1024];
            size_t read_size = fread(file_buffer, 1, sizeof(file_buffer), fp);
            fclose(fp);
            
            // Try decoding with file data
            if (read_size > 0 && iface) {
                aom_codec_ctx_t file_ctx;
                memset(&file_ctx, 0, sizeof(file_ctx));
                
                if (aom_codec_dec_init_ver(&file_ctx, iface, NULL, 0, 
                                           AOM_DECODER_ABI_VERSION) == AOM_CODEC_OK) {
                    aom_codec_decode(&file_ctx, file_buffer, read_size, NULL);
                    aom_codec_destroy(&file_ctx);
                }
            }
        }
    }

    return 0;
}