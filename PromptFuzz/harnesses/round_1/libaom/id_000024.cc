#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <aom/aom.h>
}
extern "C" {
#include <aom/aom_codec.h>
}
extern "C" {
#include <aom/aom_decoder.h>
}
extern "C" {
#include <aom/aom_encoder.h>
}
extern "C" {
#include <aom/aom_ext_ratectrl.h>
}
extern "C" {
#include <aom/aom_external_partition.h>
}
extern "C" {
#include <aom/aom_frame_buffer.h>
}
extern "C" {
#include <aom/aom_image.h>
}
extern "C" {
#include <aom/aomcx.h>
}
extern "C" {
#include <aom/aomdx.h>
}
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_get_frame","aom_codec_destroy","aom_img_num_metadata","aom_img_get_metadata","aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_enc_config_set","aom_codec_destroy","aom_codec_destroy","aom_codec_set_cx_data_buf","aom_codec_destroy","aom_codec_destroy","aom_codec_encode","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_uleb_encode","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_enc_config_set","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_encode","aom_codec_get_preview_frame","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small for basic operations
    if (size < 100) {
        return 0;
    }

    // Create a FILE* from the input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Read the entire input file into a buffer
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    std::vector<uint8_t> input_buffer(file_size);
    if (fread(input_buffer.data(), 1, file_size, in_file) != static_cast<size_t>(file_size)) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    assert_file_closed(&in_file);;

    // 1. First, decode the input data (assumed to be encoded AV1 stream)
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 4;
    dec_cfg.w = 640;
    dec_cfg.h = 480;
    dec_cfg.allow_lowbitdepth = 1;

    // Call aom_codec_dec_init_ver
    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION) != AOM_CODEC_OK) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Decode the input data
    if (aom_codec_decode(&decoder, input_buffer.data(), input_buffer.size(), nullptr) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get decoded frames
    aom_codec_iter_t iter = nullptr;
    const aom_image_t* decoded_frame = nullptr;
    std::vector<const aom_image_t*> frames;
    
    while ((decoded_frame = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        frames.push_back(decoded_frame);
    }

    if (frames.empty()) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Process metadata from the first decoded frame
    // Call aom_img_get_metadata
    size_t num_metadata = aom_img_num_metadata(frames[0]);
    for (size_t i = 0; i < num_metadata; ++i) {
        const aom_metadata_t* metadata = aom_img_get_metadata(frames[0], i);
        if (metadata) {
            // Do something with metadata
        }
    }

    // 3. ULEB encoding demonstration
    // Call aom_uleb_encode
    uint64_t uleb_value = static_cast<uint64_t>(size) % 1000;
    uint8_t uleb_buffer[10];
    size_t uleb_encoded_size = 0;
    if (aom_uleb_encode(uleb_value, sizeof(uleb_buffer), uleb_buffer, &uleb_encoded_size) == 0) {
        // Successfully encoded ULEB
    }

    // 4. Now encode the decoded frame back
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;

    // Call aom_codec_enc_config_default to get default configuration
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, AOM_USAGE_GOOD_QUALITY) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Configure encoder
    enc_cfg.g_w = frames[0]->d_w;
    enc_cfg.g_h = frames[0]->d_h;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_threads = 4;
    enc_cfg.g_pass = AOM_RC_ONE_PASS;
    enc_cfg.g_lag_in_frames = 0;
    enc_cfg.g_error_resilient = 0;

    // Call aom_codec_enc_init_ver
    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION) != AOM_CODEC_OK) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // Call aom_codec_enc_config_set
    enc_cfg.rc_target_bitrate = 500; // Change bitrate
    aom_codec_enc_config_set(&encoder, &enc_cfg);

    // 5. Set output buffer for encoded data
    // Call aom_codec_set_cx_data_buf
    const size_t output_buffer_size = 10 * 1024 * 1024; // 10MB buffer
    uint8_t* output_buffer = static_cast<uint8_t*>(calloc(1, output_buffer_size));
    if (!output_buffer) {
        aom_codec_destroy(&encoder);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    aom_fixed_buf_t output_buf;
    output_buf.buf = output_buffer;
    output_buf.sz = output_buffer_size;
    
    if (aom_codec_set_cx_data_buf(&encoder, &output_buf, 0, 0) != AOM_CODEC_OK) {
        free(output_buffer);
        aom_codec_destroy(&encoder);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Encode frames
    for (size_t i = 0; i < std::min(frames.size(), static_cast<size_t>(3)); ++i) {
        aom_codec_encode(&encoder, frames[i], i * 30, 30, 0);
    }

    // Flush encoder
    aom_codec_encode(&encoder, nullptr, 0, 0, 0);

    // 7. Get preview frame (if available)
    // Call aom_codec_get_preview_frame
    const aom_image_t* preview_frame = aom_codec_get_preview_frame(&encoder);
    if (preview_frame) {
        // Process preview frame if needed
    }

    // 8. Get encoded data
    aom_codec_iter_t enc_iter = nullptr;
    const aom_codec_cx_pkt_t* pkt;
    while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Encoded frame data available
        }
    }

    // 9. Write encoded output to file (optional)
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        size_t actual_output_size = output_buffer_size - output_buf.sz;
        fwrite(output_buffer, 1, actual_output_size, out_file);
        assert_file_closed(&out_file);;
    }

    // 10. Cleanup
    free(output_buffer);
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}