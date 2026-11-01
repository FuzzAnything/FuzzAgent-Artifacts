// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void parse_fuzz_data(const uint8_t *data, size_t size, 
                            aom_codec_enc_cfg_t *enc_cfg,
                            aom_codec_dec_cfg_t *dec_cfg,
                            char **option_name, size_t *opt_name_len,
                            char **option_value, size_t *opt_val_len) {
    size_t offset = 0;
    
    // Helper macro to safely read 2 bytes
    #define READ_2_BYTES(val) \
        (offset + 2 <= size) ? ((uint16_t)data[offset] | ((uint16_t)data[offset+1] << 8)) : 0

    // Parse Encoder Config
    if (offset < size) enc_cfg->g_threads = data[offset++];
    if (offset + 2 <= size) {
        enc_cfg->g_w = READ_2_BYTES(enc_cfg->g_w);
        offset += 2;
    }
    if (offset + 2 <= size) {
        enc_cfg->g_h = READ_2_BYTES(enc_cfg->g_h);
        offset += 2;
    }
    if (offset < size) enc_cfg->g_bit_depth = data[offset++];
    if (offset < size) enc_cfg->g_input_bit_depth = data[offset++];
    if (offset + 2 <= size) {
        enc_cfg->rc_target_bitrate = READ_2_BYTES(enc_cfg->rc_target_bitrate);
        offset += 2;
    }
    if (offset < size) enc_cfg->g_timebase.num = data[offset++];
    if (offset < size) enc_cfg->g_timebase.den = data[offset++];
    if (offset < size) enc_cfg->rc_min_quantizer = data[offset++];
    if (offset < size) enc_cfg->rc_max_quantizer = data[offset++];
    if (offset + 2 <= size) {
        enc_cfg->kf_min_dist = READ_2_BYTES(enc_cfg->kf_min_dist);
        offset += 2;
    }
    if (offset + 2 <= size) {
        enc_cfg->kf_max_dist = READ_2_BYTES(enc_cfg->kf_max_dist);
        offset += 2;
    }
    
    // Parse Decoder Config
    if (offset < size) dec_cfg->threads = data[offset++];
    if (offset + 2 <= size) {
        dec_cfg->w = READ_2_BYTES(dec_cfg->w);
        offset += 2;
    }
    if (offset + 2 <= size) {
        dec_cfg->h = READ_2_BYTES(dec_cfg->h);
        offset += 2;
    }
    
    // Parse Option Name
    if (offset < size) *opt_name_len = data[offset++];
    if (*opt_name_len > 100) *opt_name_len = 100;
    if (offset + *opt_name_len <= size) {
        *option_name = malloc(*opt_name_len + 1);
        if (*option_name) {
            memcpy(*option_name, data + offset, *opt_name_len);
            (*option_name)[*opt_name_len] = '\0';
            offset += *opt_name_len;
        }
    } else {
        *option_name = NULL;
    }

    // Parse Option Value
    if (offset < size) *opt_val_len = data[offset++];
    if (*opt_val_len > 100) *opt_val_len = 100;
    if (offset + *opt_val_len <= size) {
        *option_value = malloc(*opt_val_len + 1);
        if (*option_value) {
            memcpy(*option_value, data + offset, *opt_val_len);
            (*option_value)[*opt_val_len] = '\0';
            offset += *opt_val_len;
        }
    } else {
        *option_value = NULL;
    }
    
    #undef READ_2_BYTES
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_enc_cfg_t enc_cfg;
    memset(&enc_cfg, 0, sizeof(enc_cfg));
    
    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));

    char *opt_name = NULL;
    size_t opt_name_len = 0;
    char *opt_value = NULL;
    size_t opt_val_len = 0;

    parse_fuzz_data(Data, Size, &enc_cfg, &dec_cfg, &opt_name, &opt_name_len, &opt_value, &opt_val_len);

    // Ensure valid dimensions to avoid immediate crashes in init
    if (enc_cfg.g_w == 0) enc_cfg.g_w = 64;
    if (enc_cfg.g_h == 0) enc_cfg.g_h = 64;
    if (dec_cfg.w == 0) dec_cfg.w = 64;
    if (dec_cfg.h == 0) dec_cfg.h = 64;

    // Test Decoder Path
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (dec_iface) {
        aom_codec_err_t ret = aom_codec_dec_init_ver(&ctx, dec_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
        if (ret == AOM_CODEC_OK) {
            // Try setting options
            if (opt_name && opt_value) {
                aom_codec_set_option(&ctx, opt_name, opt_value);
            }
            // Try getting error string
            const char *err_str = aom_codec_error(&ctx);
            (void)err_str; // Suppress unused warning
            
            // Try getting global headers (might fail for decoder, but safe to call)
            aom_fixed_buf_t *hdr = aom_codec_get_global_headers(&ctx);
            if (hdr) {
                free(hdr->buf);
                free(hdr);
            }
            
            aom_codec_destroy(&ctx);
        }
    }

    // Test Encoder Path
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (enc_iface) {
        memset(&ctx, 0, sizeof(ctx));
        aom_codec_err_t ret = aom_codec_enc_init_ver(&ctx, enc_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        if (ret == AOM_CODEC_OK) {
            // Try setting options
            if (opt_name && opt_value) {
                aom_codec_set_option(&ctx, opt_name, opt_value);
            }
            // Try getting error string
            const char *err_str = aom_codec_error(&ctx);
            (void)err_str;

            // Try getting global headers
            aom_fixed_buf_t *hdr = aom_codec_get_global_headers(&ctx);
            if (hdr) {
                free(hdr->buf);
                free(hdr);
            }

            aom_codec_destroy(&ctx);
        }
    }

    // Cleanup allocated strings
    if (opt_name) free(opt_name);
    if (opt_value) free(opt_value);

    return 0;
}