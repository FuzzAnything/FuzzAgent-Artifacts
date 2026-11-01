// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_version at aom_codec.c:26:5 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
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
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom API functions.
 * Targets: aom_codec_version, aom_codec_set_option.
 */

static void setup_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_175(const uint8_t *Data, size_t Size) {
    /* Step 1: Environment Preparation */
    
    /* 1.1 Test aom_codec_version (No setup required, always safe) */
    int version = aom_codec_version();
    (void)version; 

    /* 1.2 Test aom_codec_set_option */
    /* We need a valid codec context to test this. We will initialize an encoder. */
    /* Use aom_codec_av1_cx() for encoder interface. */
    const aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) {
        /* If encoder interface is not available, try decoder interface */
        iface = aom_codec_av1_dx();
    }

    if (!iface) {
        /* If no interface is available, we can only test version and dummy file creation */
        setup_dummy_file(Data, Size);
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_err_t res;

    /* Initialize the context as an encoder */
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    
    if (res != AOM_CODEC_OK) {
        /* If encoder init fails, try decoder init */
        res = aom_codec_dec_init(&ctx, iface, NULL, 0);
        if (res != AOM_CODEC_OK) {
            /* If both fail, just test version and exit */
            setup_dummy_file(Data, Size);
            return 0;
        }
    }

    /* Step 2: Fuzzing Logic for aom_codec_set_option */
    /* We will use the fuzz input to generate key-value pairs */
    if (Size >= 2) {
        /* Create null-terminated strings from input data */
        size_t key_len = (Size / 2) + 1;
        size_t val_len = Size - key_len + 1;
        
        /* Ensure lengths are reasonable to avoid huge allocations */
        if (key_len > 256) key_len = 256;
        if (val_len > 256) val_len = 256;

        char *key = (char *)malloc(key_len);
        char *val = (char *)malloc(val_len);

        if (key && val) {
            /* Copy data, ensuring null termination */
            memcpy(key, Data, key_len - 1);
            key[key_len - 1] = '\0';
            
            memcpy(val, Data + (Size / 2), val_len - 1);
            val[val_len - 1] = '\0';

            /* Invoke the target function */
            aom_codec_set_option(&ctx, key, val);

            /* Cleanup allocated memory */
            free(key);
            free(val);
        }
    }

    /* Step 3: Cleanup */
    /* Destroy the codec context */
    aom_codec_destroy(&ctx);

    /* Step 4: Dummy file creation (as per instructions if file needed) */
    setup_dummy_file(Data, Size);

    return 0;
}