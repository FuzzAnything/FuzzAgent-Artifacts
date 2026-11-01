// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>

/* Helper to initialize a dummy AV1 bitstream file for fuzzing */
static int create_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    
    /* Write a minimal valid AV1 header if data is empty or too small */
    if (size < 10) {
        /* Minimal AV1 OBU header sequence (simplified) */
        uint8_t header[] = {0x42, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        fwrite(header, 1, sizeof(header), f);
    } else {
        fwrite(data, 1, size, f);
    }
    
    fclose(f);
    return 0;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_stream_info_t si;
    aom_fixed_buf_t *global_headers = NULL;
    aom_codec_err_t res;
    
    /* Initialize config */
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    
    /* Get AV1 decoder interface using the correct function from aomdx.h */
    iface = aom_codec_av1_dx();
    if (!iface) return 0;
    
    /* Initialize decoder context */
    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;
    
    /* Create dummy file for bitstream */
    if (create_dummy_file(Data, Size) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    /* Open the file for reading */
    FILE *f = fopen("./dummy_file", "rb");
    if (!f) {
        aom_codec_destroy(&ctx);
        return 0;
    }
    
    /* Read data into a buffer */
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    uint8_t *buf = NULL;
    if (fsize > 0) {
        buf = (uint8_t *)malloc(fsize);
        if (buf) {
            fread(buf, 1, fsize, f);
        }
    }
    fclose(f);
    
    /* Submit data to decoder if we have data */
    if (buf && fsize > 0) {
        res = aom_codec_decode(&ctx, buf, fsize, NULL);
        /* Continue to test other functions even on decode error */
    }
    
    /* Test 1: aom_codec_get_stream_info */
    memset(&si, 0, sizeof(si));
    res = aom_codec_get_stream_info(&ctx, &si);
    /* Check return value, but don't fail on error as it's expected for malformed input */
    
    /* Test 2: aom_codec_get_global_headers (only for encoder, but we test decoder context too) */
    /* This function is primarily for encoders, but we call it to test error handling */
    global_headers = aom_codec_get_global_headers(&ctx);
    if (global_headers) {
        if (global_headers->buf) {
            free(global_headers->buf);
        }
        free(global_headers);
        global_headers = NULL;
    }
    
    /* Cleanup */
    if (buf) free(buf);
    aom_codec_destroy(&ctx);
    
    return 0;
}