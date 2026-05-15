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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 3
//<Quality> {"density":11,"unique_branches":{"vp8_get_frame":[[530,19,530,49,0,0,4,1]],"vpx_codec_get_cx_data":[[257,14,257,57,0,0,4,0]],"vp8dx_get_raw_frame":[[380,7,380,35,0,0,4,0]]},"library_calls":["vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_codec_dec_init_ver","vpx_img_free","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_cx_data","vpx_codec_get_cx_data","vpx_codec_get_frame","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free"],"critical_calls":["vpx_codec_vp8_dx","vpx_codec_get_caps","vpx_codec_peek_stream_info","vpx_img_alloc","vpx_codec_dec_init_ver","vpx_codec_register_put_slice_cb","vpx_codec_decode","vpx_codec_get_cx_data","vpx_codec_get_cx_data","vpx_codec_get_frame","vpx_codec_get_frame","vpx_codec_destroy","vpx_img_free"],"visited":0}
/**/


extern "C" {
}

// Dummy put_slice callback function
static void  id_000005_dummy_put_slice_cb(void* user_priv,
                              const vpx_image_t* img,
                              const vpx_image_rect_t* slice,
                              const vpx_image_rect_t* updated) {
    // This is a dummy callback for fuzzing purposes
    (void)user_priv;
    (void)img;
    (void)slice;
    (void)updated;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd = fuzz_fileno(in_file);
    (void)fd; // Use fd if needed

    // Variables to track resources
    vpx_codec_ctx_t decoder = {0};
    vpx_image_t* image = nullptr;
    vpx_codec_iter_t iter = nullptr;
    const vpx_codec_cx_pkt_t* pkt = nullptr;
    
    // 1. Get codec capabilities
    vpx_codec_iface_t* decoder_iface = vpx_codec_vp8_dx();
    vpx_codec_caps_t caps = vpx_codec_get_caps(decoder_iface);
    (void)caps; // Use caps if needed

    // 2. Peek stream info from input data
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    
    vpx_codec_err_t err = vpx_codec_peek_stream_info(
        decoder_iface, data, (unsigned int)size, &si);
    
    // 3. Allocate an image based on stream info (or default values)
    unsigned int width = si.w > 0 ? si.w : 320;
    unsigned int height = si.h > 0 ? si.h : 240;
    
    image = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, width, height, fuzz_uint32_t_1);
    if (!image) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Initialize decoder
    err = vpx_codec_dec_init_ver(&decoder, decoder_iface, nullptr, fuzz_int64_t_2, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(image);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 5. Register put_slice callback
    err = vpx_codec_register_put_slice_cb(&decoder,  id_000005_dummy_put_slice_cb, nullptr);
    // Note: This may fail for some codecs, but we call it as requested

    // 6. Decode the input data
    err = vpx_codec_decode(&decoder, data, (unsigned int)size, nullptr, fuzz_int64_t_3);
    
    // 7. Get encoded data packets (cx_data) from decoder
    // Note: vpx_codec_get_cx_data is typically for encoders, but we call it anyway
    // as per the requirement. For decoders, we should use vpx_codec_get_frame.
    pkt = vpx_codec_get_cx_data(&decoder, &iter);
    while (pkt != nullptr) {
        // Process the packet if needed
        pkt = vpx_codec_get_cx_data(&decoder, &iter);
    }

    // Also get decoded frames (proper way for decoder)
    iter = nullptr;
    vpx_image_t* decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    while (decoded_frame != nullptr) {
        // Process decoded frame if needed
        decoded_frame = vpx_codec_get_frame(&decoder, &iter);
    }

    // Cleanup
    vpx_codec_destroy(&decoder);
    vpx_img_free(image);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}