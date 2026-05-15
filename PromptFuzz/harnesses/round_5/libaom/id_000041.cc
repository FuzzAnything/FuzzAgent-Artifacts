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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 9
//<Quality> {"density":10,"unique_branches":{"img_alloc_helper":[[61,7,61,29,0,0,4,0]],"realloc_frame_buffer_aligned":[[108,11,108,59,0,0,4,0]],"read_metadata_itut_t35":[[630,7,630,14,0,0,4,0]],"is_valid_img_fmt":[[45,5,45,12,0,0,4,0]],"alloc_read_metadata":[[601,7,601,21,0,0,4,1]],"read_uncompressed_header":[[4841,11,4841,53,0,0,4,0],[4845,11,4845,27,0,0,4,1]],"av1_receive_compressed_data":[[511,7,511,31,0,0,4,1]],"aom_img_free":[[315,7,315,10,0,0,4,1]]},"library_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_destroy","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_free","aom_img_metadata_free","aom_codec_destroy"],"critical_calls":["aom_uleb_decode","aom_codec_av1_dx","aom_codec_iface_name","aom_codec_dec_init_ver","aom_codec_set_frame_buffer_functions","aom_codec_decode","aom_codec_get_frame","aom_img_num_metadata","aom_img_get_metadata","aom_img_metadata_alloc","aom_img_free","aom_img_metadata_free","aom_codec_destroy"],"visited":0}
/**/


extern "C" {
}

// Custom frame buffer context structure
struct FrameBufferContext {
    std::vector<aom_image_t*> allocated_images;
    std::vector<aom_metadata_t*> allocated_metadata;
    const uint8_t* fuzz_data;
    size_t fuzz_data_size;
    size_t data_offset;
};

// Frame buffer allocation callback
int  id_000041_get_frame_buffer(void* cb_priv, size_t min_size, aom_codec_frame_buffer_t* fb) {
    FrameBufferContext* ctx = static_cast<FrameBufferContext*>(cb_priv);
    
    // Use fuzz data to determine image parameters
    if (ctx->data_offset + 12 > ctx->fuzz_data_size) {
        return -1;
    }
    
    // Extract image parameters from fuzz data
    unsigned int width = (ctx->fuzz_data[ctx->data_offset] << 8) | ctx->fuzz_data[ctx->data_offset + 1];
    unsigned int height = (ctx->fuzz_data[ctx->data_offset + 2] << 8) | ctx->fuzz_data[ctx->data_offset + 3];
    aom_img_fmt_t fmt = static_cast<aom_img_fmt_t>(ctx->fuzz_data[ctx->data_offset + 4] % 13);
    unsigned int border = ctx->fuzz_data[ctx->data_offset + 5] % 64;
    
    ctx->data_offset += 6;
    width = (width % 1024) + 1;
    height = (height % 1024) + 1;
    
    // Allocate image with border using the specified API
    aom_image_t* img = aom_img_alloc_with_border(nullptr, fmt, width, height, 1, 1, border);
    if (!img) {
        return -1;
    }
    
    // Store the image for later cleanup
    ctx->allocated_images.push_back(img);
    
    // Set up frame buffer structure
    fb->data = img->img_data;
    fb->size = img->sz;
    fb->priv = img;
    
    return 0;
}

// Frame buffer release callback
int  id_000041_release_frame_buffer(void* cb_priv, aom_codec_frame_buffer_t* fb) {
    // We'll handle cleanup in the main function
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;
    }
    
    // Initialize context
    FrameBufferContext ctx;
    ctx.fuzz_data = data;
    ctx.fuzz_data_size = size;
    ctx.data_offset = 0;
    
    // 1. Use aom_uleb_decode to decode a value from the input
    uint64_t uleb_value;
    size_t uleb_length;
    if (data[0] < size) {
        aom_uleb_decode(data + 1, size - 1, &uleb_value, &uleb_length);
    }
    
    // 2. Create a decoder context
    aom_codec_ctx_t codec_ctx;
    aom_codec_iface_t* decoder_iface = aom_codec_av1_dx();
    
    // 3. Get codec interface name using the specified API
    const char* iface_name = aom_codec_iface_name(decoder_iface);
    
    // 4. Initialize decoder with default configuration
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    
    aom_codec_err_t res = aom_codec_dec_init_ver(&codec_ctx, decoder_iface, &cfg, 0, AOM_DECODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    // 5. Set frame buffer functions using the specified API
    res = aom_codec_set_frame_buffer_functions(&codec_ctx,  id_000041_get_frame_buffer,  id_000041_release_frame_buffer, &ctx);
    
    // 6. Create a FILE* from input data using fmemopen
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        aom_codec_destroy(&codec_ctx);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Read and decode data in chunks
    std::vector<uint8_t> buffer(4096);
    while (!feof(in_file) && ctx.data_offset < size) {
        size_t bytes_read = fread(buffer.data(), 1, std::min(buffer.size(), size - ctx.data_offset), in_file);
        if (bytes_read > 0) {
            aom_codec_decode(&codec_ctx, buffer.data(), bytes_read, nullptr);
            
            // Try to get frames
            aom_codec_iter_t iter = nullptr;
            aom_image_t* img;
            while ((img = aom_codec_get_frame(&codec_ctx, &iter)) != nullptr) {
                // 8. Get metadata from image using the specified API
                size_t num_metadata = aom_img_num_metadata(img);
                for (size_t i = 0; i < num_metadata; i++) {
                    const aom_metadata_t* metadata = aom_img_get_metadata(img, i);
                    if (metadata) {
                        // Metadata exists, we can use it
                    }
                }
                
                // 9. Create new metadata using the specified API
                if (ctx.data_offset + 10 < size) {
                    uint32_t metadata_type = *reinterpret_cast<const uint32_t*>(data + ctx.data_offset);
                    size_t metadata_size = data[ctx.data_offset + 4] % 256;
                    
                    if (ctx.data_offset + metadata_size < size) {
                        aom_metadata_t* new_metadata = aom_img_metadata_alloc(
                            metadata_type,
                            data + ctx.data_offset + 5,
                            metadata_size,
                            static_cast<aom_metadata_insert_flags_t>(data[ctx.data_offset + 5] % 6)
                        );
                        
                        if (new_metadata) {
                            ctx.allocated_metadata.push_back(new_metadata);
                        }
                        
                        ctx.data_offset += metadata_size + 5;
                    }
                }
            }
        }
    }
    
    // 10. Cleanup
    assert_file_closed(&in_file);;
    
    // Clean up allocated images
    for (aom_image_t* img : ctx.allocated_images) {
        aom_img_free(img);
    }
    
    // Clean up allocated metadata
    for (aom_metadata_t* metadata : ctx.allocated_metadata) {
        aom_img_metadata_free(metadata);
    }
    
    // Destroy codec context
    aom_codec_destroy(&codec_ctx);
    
    assert_file_closed(&in_file);
	return 0;
}