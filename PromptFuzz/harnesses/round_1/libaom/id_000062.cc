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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"estimate_screen_content":[[2097,35,2097,72,0,0,4,0],[2098,35,2098,68,0,0,4,1]]},"library_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_error","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_iface_name","aom_img_alloc","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"critical_calls":["aom_codec_version_str","aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_peek_stream_info","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_get_stream_info","aom_codec_get_frame","aom_img_plane_width","aom_img_plane_height","aom_img_num_metadata","aom_img_remove_metadata","aom_img_add_metadata","aom_codec_av1_cx","aom_codec_iface_name","aom_img_alloc","aom_codec_enc_config_default","aom_codec_enc_init_ver","aom_img_add_metadata","aom_codec_encode","aom_codec_get_cx_data","aom_codec_destroy","aom_codec_destroy","aom_img_free"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Get library version
    const char *version = aom_codec_version_str();
    (void)version; // Use version to avoid unused warning

    // 2. Test uleb decode from input data
    uint64_t uleb_value = 0;
    size_t uleb_length = 0;
    if (size > 0) {
        aom_uleb_decode(data, size, &uleb_value, &uleb_length);
    }

    // 3. Decoder path: decode and analyze stream
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    const char *decoder_name = aom_codec_iface_name(decoder_iface);
    (void)decoder_name;

    // 4. Peek stream info
    aom_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    if (size > 0) {
        aom_codec_peek_stream_info(decoder_iface, data, size, &stream_info);
    }

    // 5. Initialize decoder context
    aom_codec_ctx_t decoder_ctx;
    aom_codec_dec_cfg_t dec_cfg = {0, 0, 0};
    aom_codec_err_t err = aom_codec_dec_init_ver(&decoder_ctx, decoder_iface, &dec_cfg, 0, AOM_DECODER_ABI_VERSION);
    if (err != AOM_CODEC_OK) {
        const char *error_detail = aom_codec_error(&decoder_ctx);
        (void)error_detail;
    } else {
        // 6. Decode the input
        if (size > 0) {
            aom_codec_decode(&decoder_ctx, data, size, nullptr);
        }

        // 7. Get stream info from decoder
        aom_codec_get_stream_info(&decoder_ctx, &stream_info);

        // 8. Get decoded frames
        aom_codec_iter_t iter = nullptr;
        aom_image_t *decoded_img = nullptr;
        while ((decoded_img = aom_codec_get_frame(&decoder_ctx, &iter)) != nullptr) {
            // Test image utilities
            int plane_width = aom_img_plane_width(decoded_img, 0);
            int plane_height = aom_img_plane_height(decoded_img, 0);
            (void)plane_width;
            (void)plane_height;

            // Test metadata APIs
            size_t metadata_count = aom_img_num_metadata(decoded_img);
            if (metadata_count > 0) {
                aom_img_remove_metadata(decoded_img);
            }

            // Add metadata using input data
            if (size > 4) {
                aom_img_add_metadata(decoded_img, 
                                    static_cast<uint32_t>(data[0]), 
                                    data + 1, 
                                    size - 1, 
                                    AOM_MIF_ANY_FRAME);
            }
        }
    }

    // 9. Encoder path: encode a test image
    aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
    const char *encoder_name = aom_codec_iface_name(encoder_iface);
    (void)encoder_name;

    // 10. Allocate test image
    aom_image_t test_img;
    memset(&test_img, 0, sizeof(test_img));
    aom_img_alloc(&test_img, AOM_IMG_FMT_I420, 64, 64, 1);

    if (test_img.planes[0]) {
        // Fill with some data from input
        size_t fill_size = size < 64*64 ? size : 64*64;
        for (size_t i = 0; i < fill_size; i++) {
            test_img.planes[0][i] = data[i % size];
        }

        // 11. Initialize encoder
        aom_codec_ctx_t encoder_ctx;
        aom_codec_enc_cfg_t enc_cfg;
        aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
        enc_cfg.g_w = 64;
        enc_cfg.g_h = 64;
        enc_cfg.rc_target_bitrate = 256;
        
        err = aom_codec_enc_init_ver(&encoder_ctx, encoder_iface, &enc_cfg, 0, AOM_ENCODER_ABI_VERSION);
        
        if (err == AOM_CODEC_OK) {
            // 12. Add metadata to image before encoding
            if (size > 8) {
                aom_img_add_metadata(&test_img, 
                                    static_cast<uint32_t>(data[0] << 8 | data[1]), 
                                    data + 2, 
                                    size > 10 ? 8 : size - 2, 
                                    AOM_MIF_KEY_FRAME);
            }

            // 13. Encode the image
            aom_codec_encode(&encoder_ctx, &test_img, 0, 1, 0);

            // 14. Get encoded data
            aom_codec_iter_t enc_iter = nullptr;
            const aom_codec_cx_pkt_t *pkt = nullptr;
            while ((pkt = aom_codec_get_cx_data(&encoder_ctx, &enc_iter)) != nullptr) {
                // Process packet if needed
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Could write to file or process further
                    // FILE* out_file = fopen("output_file", "wb");
                    // if (out_file) {
                    //     fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                    //     assert_file_closed(&out_file);;
                    // }
                }
            }

            // Clean up encoder
            aom_codec_destroy(&encoder_ctx);
        }
    }

    // 15. Clean up decoder if initialized
    if (err == AOM_CODEC_OK) {
        aom_codec_destroy(&decoder_ctx);
    }

    // 16. Clean up image
    if (test_img.planes[0]) {
        aom_img_free(&test_img);
    }

    return 0;
}