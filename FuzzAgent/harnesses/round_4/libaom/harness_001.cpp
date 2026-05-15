#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"

namespace harness_001 {
/*
 * Fuzzing harness for libaom AV1 decoder API
 * Targets complete decoder lifecycle: stream inspection, initialization, decoding, frame retrieval
 * Focuses on decoder module which currently has 0% coverage
 */




int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for basic decoder operations
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume configuration parameters from fuzzer input
    unsigned int threads = fdp.ConsumeIntegralInRange<unsigned int>(1, 64);
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(16, 4096);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(16, 4096);
    int allow_lowbitdepth = fdp.ConsumeBool() ? 1 : 0;
    int flags = fdp.ConsumeIntegral<aom_codec_flags_t>();
    
    // Consume stream info parameters
    int is_annexb = fdp.ConsumeBool() ? 1 : 0;
    
    // Get remaining data for AV1 bitstream
    std::vector<uint8_t> bitstream_data = fdp.ConsumeRemainingBytes<uint8_t>();
    if (bitstream_data.size() < 16) {
        return 0;  // Not enough data for meaningful bitstream
    }

    // Get decoder interface
    aom_codec_iface_t *decoder = aom_codec_av1_dx();
    if (!decoder) {
        return 0;
    }

    // Step 1: Call aom_codec_peek_stream_info to inspect input data before decoder initialization
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.is_annexb = is_annexb;
    
    aom_codec_err_t peek_res = aom_codec_peek_stream_info(
        decoder, 
        bitstream_data.data(), 
        std::min((size_t)1024, bitstream_data.size()),  // Limit size for peek
        &stream_info
    );
    
    // We ignore peek_res as it may fail on invalid bitstreams (that's what fuzzing tests)

    // Step 2: Initialize decoder configuration
    aom_codec_dec_cfg_t cfg;
    cfg.threads = threads;
    cfg.w = width;
    cfg.h = height;
    cfg.allow_lowbitdepth = allow_lowbitdepth;

    // Step 3: Call aom_codec_dec_init_ver to initialize decoder context
    aom_codec_ctx_t codec;
    aom_codec_err_t init_res = aom_codec_dec_init_ver(
        &codec, 
        decoder, 
        &cfg, 
        flags, 
        AOM_DECODER_ABI_VERSION
    );
    
    if (init_res != AOM_CODEC_OK) {
        // If initialization fails, we can't proceed with other API calls
        return 0;
    }

    // Step 4: Call aom_codec_get_stream_info to get stream information
    aom_codec_stream_info_t actual_stream_info;
    memset(&actual_stream_info, 0, sizeof(actual_stream_info));
    actual_stream_info.is_annexb = is_annexb;
    
    aom_codec_err_t stream_info_res = aom_codec_get_stream_info(&codec, &actual_stream_info);
    // We ignore stream_info_res as it may fail on invalid or incomplete bitstreams

    // Step 5: Call aom_img_alloc to allocate image buffer for decoded frames
    // Use stream info if available, otherwise use configured dimensions
    unsigned int img_width = (actual_stream_info.w > 0) ? actual_stream_info.w : width;
    unsigned int img_height = (actual_stream_info.h > 0) ? actual_stream_info.h : height;
    
    // Limit dimensions to reasonable values to avoid excessive memory usage
    img_width = std::min(img_width, 4096u);
    img_height = std::min(img_height, 4096u);
    
    aom_image_t img;
    aom_image_t* img_ptr = aom_img_alloc(&img, AOM_IMG_FMT_I420, img_width, img_height, 16);
    
    // Check if allocation succeeded
    if (img_ptr == NULL) {
        // Allocation failed, clean up decoder and return
        aom_codec_destroy(&codec);
        return 0;
    }

    // Step 6: Call aom_codec_decode to decode input frame(s)
    // Use a portion of the bitstream data for decoding
    size_t decode_size = std::min(bitstream_data.size(), (size_t)65536);
    aom_codec_err_t decode_res = aom_codec_decode(
        &codec, 
        bitstream_data.data(), 
        decode_size, 
        nullptr
    );
    
    // We ignore decode_res as failed decoding is a valid test outcome

    // Step 7: Call aom_codec_get_frame to retrieve decoded frame(s)
    aom_codec_iter_t iter = nullptr;
    aom_image_t *decoded_frame = nullptr;
    int frame_count = 0;
    
    while ((decoded_frame = aom_codec_get_frame(&codec, &iter)) != nullptr && frame_count < 10) {
        frame_count++;
        // We could process the frame here, but for fuzzing we just retrieve it
        // to exercise the frame retrieval code path
    }

    // Step 8: Call aom_img_free to free allocated image buffer
    aom_img_free(&img);

    // Step 9: Call aom_codec_destroy to clean up decoder context
    aom_codec_destroy(&codec);

    return 0;
}

} // namespace harness_001
