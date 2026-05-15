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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_destroy","aom_codec_enc_init_ver","aom_codec_destroy","aom_codec_set_option","aom_codec_destroy","aom_codec_destroy","aom_img_alloc","aom_codec_set_cx_data_buf","aom_codec_encode","aom_uleb_encode","aom_img_free","aom_codec_set_cx_data_buf","aom_codec_encode","aom_codec_get_cx_data","aom_codec_enc_config_set","aom_codec_destroy","aom_codec_destroy"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_peek_stream_info","aom_codec_decode","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_codec_av1_cx","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_codec_set_option","aom_img_alloc","aom_codec_set_cx_data_buf","aom_codec_encode","aom_uleb_encode","aom_img_free","aom_codec_enc_config_set","aom_codec_destroy","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if input is too small for basic operations
    if (size < 4) {
	return 0;
	}
	

    // 1. Get libaom version
    const char* version = aom_codec_version_str();
    (void)version; // Use version to satisfy requirement

    // 2. Decode a ULEB value from the input
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size >= 1) {
        aom_uleb_decode(data, std::min(size, (size_t)10), &uleb_value, &uleb_length);
    }

    // 3. Create a FILE* for input using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 4. Setup decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t dec_cfg = {0};
    dec_cfg.threads = 1;
    dec_cfg.w = 128;
    dec_cfg.h = 128;
    dec_cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init_ver(&decoder, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION)) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Get stream info from the input
    aom_codec_stream_info_t stream_info = {0};
    if (aom_codec_peek_stream_info(decoder_iface, data, std::min(size, (size_t)1024), &stream_info) == AOM_CODEC_OK) {
        // Use the stream info
        (void)stream_info.w;
        (void)stream_info.h;
    }

    // 6. Decode the input data
    aom_codec_decode(&decoder, data, size, nullptr);

    // 7. Get decoded frames
    aom_codec_iter_t iter = nullptr;
    aom_image_t* img;
    std::vector<aom_image_t*> decoded_images;
    
    while ((img = aom_codec_get_frame(&decoder, &iter)) != nullptr) {
        decoded_images.push_back(img);
        
        // Use image plane utilities
        int plane_width = aom_img_plane_width(img, 0);
        int plane_height = aom_img_plane_height(img, 0);
        size_t metadata_count = aom_img_num_metadata(img);
        (void)plane_width;
        (void)plane_height;
        (void)metadata_count;
    }

    // 8. Create encoder context
    aom_codec_ctx_t encoder;
    aom_codec_iface_t* encoder_iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t enc_cfg;
    
    if (aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0)) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    enc_cfg.g_w = 128;
    enc_cfg.g_h = 128;
    enc_cfg.g_threads = 1;
    enc_cfg.rc_target_bitrate = 1000;
    enc_cfg.g_timebase.num = 1;
    enc_cfg.g_timebase.den = 30;

    if (aom_codec_enc_init_ver(&encoder, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION)) {
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 9. Set an encoder option
    aom_codec_set_option(&encoder, "cpu-used", "1");

    // 10. Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        aom_codec_destroy(&encoder);
        aom_codec_destroy(&decoder);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 11. Setup buffer for encoded data
    const size_t max_buffer_size = 1024 * 1024;
    std::vector<uint8_t> buffer(max_buffer_size);
    aom_fixed_buf_t fixed_buf;
    fixed_buf.buf = buffer.data();
    fixed_buf.sz = buffer.size();

    if (decoded_images.empty()) {
        // If no decoded images, create a test image
        aom_image_t* test_img = aom_img_alloc(nullptr, AOM_IMG_FMT_I420, 128, 128, 16);
        if (test_img) {
            // Set buffer for encoded data
            aom_codec_set_cx_data_buf(&encoder, &fixed_buf, 0, 0);
            
            // Encode the test image
            aom_codec_encode(&encoder, test_img, 0, 1, 0);
            
            // Encode a ULEB value
            uint8_t coded_value[10];
            size_t coded_size = 0;
            aom_uleb_encode(uleb_value, 10, coded_value, &coded_size);
            
            aom_img_free(test_img);
        }
    } else {
        // Encode decoded images
        for (size_t i = 0; i < decoded_images.size(); i++) {
            aom_image_t* frame = decoded_images[i];
            
            // Set buffer for encoded data
            aom_codec_set_cx_data_buf(&encoder, &fixed_buf, 0, 0);
            
            // Encode the frame
            aom_codec_encode(&encoder, frame, i, 1, (i == 0) ? AOM_EFLAG_FORCE_KF : 0);
            
            // Get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t* pkt;
            while ((pkt = aom_codec_get_cx_data(&encoder, &enc_iter)) != nullptr) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
            }
        }
    }

    // 12. Set encoder configuration (even if unchanged)
    aom_codec_enc_config_set(&encoder, &enc_cfg);

    // 13. Cleanup
    aom_codec_destroy(&encoder);
    aom_codec_destroy(&decoder);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // 14. Get file descriptors (optional, not used but demonstrating)
    if (in_file) {
        int fd_in = fuzz_fileno(in_file);
        (void)fd_in;
    }
    if (out_file) {
        int fd_out = fuzz_fileno(out_file);
        (void)fd_out;
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}