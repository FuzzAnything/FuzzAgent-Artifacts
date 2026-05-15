// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_integer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"

static void test_uleb_encode(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t) + sizeof(size_t)) return;
    
    uint64_t value;
    memcpy(&value, data, sizeof(uint64_t));
    data += sizeof(uint64_t);
    size -= sizeof(uint64_t);
    
    size_t available;
    memcpy(&available, data, sizeof(size_t));
    data += sizeof(size_t);
    size -= sizeof(size_t);
    
    // Cap available to avoid excessive allocations
    if (available > 1024) available = 1024;
    
    uint8_t *coded_value = (uint8_t *)malloc(available);
    if (!coded_value) return;
    
    size_t coded_size = 0;
    aom_uleb_encode(value, available, coded_value, &coded_size);
    
    free(coded_value);
}

static void test_uleb_encode_fixed_size(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t) + 2 * sizeof(size_t)) return;
    
    uint64_t value;
    memcpy(&value, data, sizeof(uint64_t));
    data += sizeof(uint64_t);
    size -= sizeof(uint64_t);
    
    size_t available;
    memcpy(&available, data, sizeof(size_t));
    data += sizeof(size_t);
    size -= sizeof(size_t);
    
    size_t pad_to_size;
    memcpy(&pad_to_size, data, sizeof(size_t));
    
    // Cap sizes to avoid excessive allocations
    if (available > 1024) available = 1024;
    if (pad_to_size > 1024) pad_to_size = 1024;
    
    uint8_t *coded_value = (uint8_t *)malloc(available);
    if (!coded_value) return;
    
    size_t coded_size = 0;
    aom_uleb_encode_fixed_size(value, available, pad_to_size, coded_value, &coded_size);
    
    free(coded_value);
}

static void test_uleb_decode(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uint64_t value = 0;
    size_t length = 0;
    
    // Use the fuzzed data directly as input buffer
    aom_uleb_decode(data, size, &value, &length);
}

static void test_uleb_size_in_bytes(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    uint64_t value;
    memcpy(&value, data, sizeof(uint64_t));
    
    size_t encoded_size = aom_uleb_size_in_bytes(value);
    (void)encoded_size; // Use result to avoid unused variable warning
}

static void test_peek_stream_info(const uint8_t *data, size_t size) {
    // Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return;
    
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    
    // Initialize is_annexb as required by the API
    si.is_annexb = (size > 0) ? (data[0] & 1) : 0;
    
    aom_codec_peek_stream_info(iface, data, size, &si);
}

static void test_decode(const uint8_t *data, size_t size) {
    // Get AV1 decoder interface
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return;
    
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg = {0};
    
    // Initialize decoder with default configuration
    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return;
    }
    
    // Use fuzzed data as compressed video frame
    aom_codec_decode(&ctx, data, size, NULL);
    
    // Clean up
    aom_codec_destroy(&ctx);
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = (Size > 1) ? Size - 1 : 0;
    
    switch (selector % 6) {
        case 0:
            test_uleb_encode(fuzz_data, fuzz_size);
            break;
        case 1:
            test_uleb_encode_fixed_size(fuzz_data, fuzz_size);
            break;
        case 2:
            test_peek_stream_info(fuzz_data, fuzz_size);
            break;
        case 3:
            test_uleb_size_in_bytes(fuzz_data, fuzz_size);
            break;
        case 4:
            test_decode(fuzz_data, fuzz_size);
            break;
        case 5:
            test_uleb_decode(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}