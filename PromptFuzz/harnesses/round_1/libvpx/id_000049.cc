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
#include <vpx/vp8cx.h>
}
extern "C" {
#include <vpx/vp8dx.h>
}
extern "C" {
#include <vpx/vpx_decoder.h>
}
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"vpx_codec_get_cx_data":[[257,14,257,57,0,0,4,0]]},"library_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"critical_calls":["vpx_codec_vp9_dx","vpx_codec_vp8_cx","vpx_codec_iface_name","vpx_codec_iface_name","vpx_codec_get_caps","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_codec_dec_init_ver","vpx_img_alloc","vpx_codec_register_put_frame_cb","vpx_codec_set_cx_data_buf","vpx_codec_get_global_headers","vpx_codec_decode","vpx_codec_get_stream_info","vpx_codec_get_frame","vpx_codec_get_cx_data","vpx_img_free","vpx_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Callback function for put_frame
static void  id_000049_put_frame_callback(void* user_priv, const vpx_image_t* img) {
    // This is just a stub callback for demonstration
    (void)user_priv;
    (void)img;
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0; // Too small to be meaningful
    }

    // Create FILE* for input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int input_fd = fuzz_fileno(in_file);

    // 1. Get decoder and encoder interfaces
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp9_dx();
    vpx_codec_iface_t* encoder_iface = vpx_codec_vp8_cx();

    // 2. Get interface names
    const char* decoder_name = vpx_codec_iface_name(decoder_iface);
    const char* encoder_name = vpx_codec_iface_name(encoder_iface);
    (void)decoder_name; // Suppress unused warning
    (void)encoder_name; // Suppress unused warning

    // 3. Get capabilities of both interfaces
    vpx_codec_caps_t decoder_caps = vpx_codec_get_caps(decoder_iface);
    vpx_codec_caps_t encoder_caps = vpx_codec_get_caps(encoder_iface);
    (void)decoder_caps; // Suppress unused warning
    (void)encoder_caps; // Suppress unused warning

    // 4. Peek stream information from input data
    vpx_codec_stream_info_t stream_info;
    memset(&stream_info, 0, sizeof(stream_info));
    stream_info.sz = sizeof(stream_info);
    
    // Try to peek stream info - this will work for valid VP8/VP9 streams
    vpx_codec_err_t peek_err = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)size, &stream_info);
    (void)peek_err; // Continue even if peek fails

    // 5. Initialize decoder
    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.w = 640;
    dec_cfg.h = 480;

    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(
        &decoder, decoder_iface, &dec_cfg, 0, VPX_DECODER_ABI_VERSION);
    
    if (init_err != VPX_CODEC_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
    }

    // 6. Allocate an image for potential use
    vpx_image_t* img = vpx_img_alloc(
        nullptr, VPX_IMG_FMT_I420, 640, 480, 1);
    
    if (img) {
        // 7. Register put frame callback
        vpx_codec_register_put_frame_cb(&decoder,  id_000049_put_frame_callback, nullptr);

        // 8. Prepare fixed buffer for cx_data_buf
        vpx_fixed_buf_t fixed_buf;
        uint8_t buffer[4096];
        fixed_buf.buf = buffer;
        fixed_buf.sz = sizeof(buffer);
        
        // Try to set cx data buffer (may fail for decoder, but we call it)
        vpx_codec_set_cx_data_buf(&decoder, &fixed_buf, 0, 0);

        // 9. Try to get global headers (may return nullptr for decoder)
        vpx_fixed_buf_t* global_headers = vpx_codec_get_global_headers(&decoder);
        (void)global_headers; // Suppress unused warning

        // 10. Decode the input data
        vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, 0);

        // 11. Get stream info after decoding
        vpx_codec_stream_info_t post_stream_info;
        memset(&post_stream_info, 0, sizeof(post_stream_info));
        post_stream_info.sz = sizeof(post_stream_info);
        vpx_codec_get_stream_info(&decoder, &post_stream_info);

        // 12. Get decoded frames
        vpx_codec_iter_t iter = nullptr;
        const vpx_image_t* decoded_frame;
        while ((decoded_frame = vpx_codec_get_frame(&decoder, &iter)) != nullptr) {
            // Frame received - we could process it here
            (void)decoded_frame; // Suppress unused warning
        }

        // 13. Try to get cx data (for encoder output - will likely fail for decoder)
        vpx_codec_iter_t cx_iter = nullptr;
        const vpx_codec_cx_pkt_t* pkt;
        pkt = vpx_codec_get_cx_data(&decoder, &cx_iter);
        (void)pkt; // Suppress unused warning

        // 14. Clean up allocated image
        vpx_img_free(img);
    }

    // 15. Destroy decoder context
    vpx_codec_destroy(&decoder);

    // Close input file
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
}