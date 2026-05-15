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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_error","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_error","aom_codec_get_global_headers","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_get_cx_data","aom_codec_destroy"],"critical_calls":["aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_cx_data_buf","aom_codec_decode","aom_codec_error","aom_codec_get_global_headers","aom_codec_get_preview_frame","aom_img_num_metadata","aom_img_get_metadata","aom_codec_get_cx_data","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create input file from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used

    // Initialize decoder context
    aom_codec_ctx_t decoder;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    
    // Get codec interface name
    const char *iface_name = aom_codec_iface_name(iface);
    (void)iface_name;  // Mark as used

    // Configure decoder
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    cfg.w = 128;
    cfg.h = 128;
    cfg.allow_lowbitdepth = 1;

    // Initialize decoder
    aom_codec_err_t init_result = aom_codec_dec_init_ver(&decoder, iface, &cfg, 0, 
                                                         AOM_DECODER_ABI_VERSION);
    if (init_result != AOM_CODEC_OK) {
        // Get error message
        const char *error_msg = aom_codec_error(&decoder);
        (void)error_msg;  // Mark as used
        
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create fixed buffer for cx data
    std::vector<uint8_t> cx_buffer(4096);
    aom_fixed_buf_t fixed_buf;
    fixed_buf.buf = cx_buffer.data();
    fixed_buf.sz = cx_buffer.size();

    // Set cx data buffer
    aom_codec_err_t set_result = aom_codec_set_cx_data_buf(&decoder, &fixed_buf, 0, 0);
    (void)set_result;  // Mark as used

    // Decode the input data
    aom_codec_err_t decode_result = aom_codec_decode(&decoder, data, size, NULL);
    
    // Get error message if decode failed
    if (decode_result != AOM_CODEC_OK) {
        const char *decode_error = aom_codec_error(&decoder);
        (void)decode_error;  // Mark as used
    }

    // Get global headers
    aom_fixed_buf_t *global_headers = aom_codec_get_global_headers(&decoder);
    if (global_headers) {
        // Write global headers to output file if they exist
        if (global_headers->buf && global_headers->sz > 0) {
            fwrite(global_headers->buf, 1, global_headers->sz, out_file);
        }
    }

    // Get preview frame
    const aom_image_t *preview_frame = aom_codec_get_preview_frame(&decoder);
    if (preview_frame) {
        // Get metadata from preview frame if available
        size_t num_metadata = aom_img_num_metadata(preview_frame);
        for (size_t i = 0; i < num_metadata; i++) {
            const aom_metadata_t *metadata = aom_img_get_metadata(preview_frame, i);
            (void)metadata;  // Mark as used
        }
    }

    // Iterate through compressed data packets
    aom_codec_iter_t iter = nullptr;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&decoder, &iter)) != nullptr) {
        // Process different packet types
        switch (pkt->kind) {
            case AOM_CODEC_CX_FRAME_PKT:
                // Write frame data to output file
                if (pkt->data.frame.buf && pkt->data.frame.sz > 0) {
                    fwrite(pkt->data.frame.buf, 1, pkt->data.frame.sz, out_file);
                }
                break;
            case AOM_CODEC_STATS_PKT:
                // Process stats packet
                if (pkt->data.twopass_stats.buf && pkt->data.twopass_stats.sz > 0) {
                    fwrite(pkt->data.twopass_stats.buf, 1, pkt->data.twopass_stats.sz, out_file);
                }
                break;
            case AOM_CODEC_PSNR_PKT:
                // Process PSNR packet
                (void)pkt->data.psnr;  // Mark as used
                break;
            default:
                break;
        }
    }

    // Cleanup
    aom_codec_err_t destroy_result = aom_codec_destroy(&decoder);
    (void)destroy_result;  // Mark as used

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}