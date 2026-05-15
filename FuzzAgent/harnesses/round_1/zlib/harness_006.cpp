#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_006 {
/*
 * Fuzzing harness for zlib stream control functions and deflate/inflate control APIs
 * Targets advanced stream control APIs with 756+ points of undiscovered complexity:
 * - inflateBack, inflateSyncPoint, inflateMark, inflateValidate, inflateGetHeader
 * - deflatePending, deflateUsed, deflateSetHeader
 * Focus: Testing stream control, synchronization points, header management, and advanced state queries
 *        with various data patterns and state transitions. Semantically different from:
 *        harness_000 (core compression), harness_001 (gzip ops), harness_002 (inflateBack callback),
 *        harness_003 (gzip reading), harness_004 (gzip output), harness_005 (dictionary manipulation)
 */


// Callback context structures for inflateBack
struct InputContext {
    const uint8_t* data;
    size_t size;
    size_t pos;
};

struct OutputContext {
    std::vector<uint8_t> buffer;
    size_t max_size;
};

// Input callback function for inflateBack
static unsigned input_callback(void* desc, z_const unsigned char FAR* FAR* buf) {
    InputContext* ctx = (InputContext*)desc;
    
    if (ctx->pos >= ctx->size) {
        return 0;  // No more data available
    }
    
    // Return pointer to remaining data
    *buf = (z_const unsigned char FAR*)&ctx->data[ctx->pos];
    
    // Determine how much to return (1-512 bytes at a time)
    size_t available = ctx->size - ctx->pos;
    size_t to_return = available > 512 ? 512 : available;
    
    ctx->pos += to_return;
    return (unsigned)to_return;
}

// Output callback function for inflateBack
static int output_callback(void* desc, unsigned char FAR* buf, unsigned len) {
    OutputContext* ctx = (OutputContext*)desc;
    
    // Check if we exceed max buffer size
    if (ctx->buffer.size() + len > ctx->max_size) {
        return 1;  // Buffer overflow
    }
    
    // Append data to buffer
    ctx->buffer.insert(ctx->buffer.end(), buf, buf + len);
    return 0;  // Success
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check - need enough for configuration
    if (size < 128) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size parameters first
    int windowBits = fdp.ConsumeIntegralInRange<int>(8, 15);
    int level = fdp.ConsumeIntegralInRange<int>(0, 9);
    int strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
    int memLevel = fdp.ConsumeIntegralInRange<int>(1, 9);
    
    // Operation selection flags
    bool test_inflate_back = fdp.ConsumeBool();
    bool test_sync_point = fdp.ConsumeBool();
    bool test_stream_mark = fdp.ConsumeBool();
    bool test_validate = fdp.ConsumeBool();
    bool test_header_ops = fdp.ConsumeBool();
    bool test_deflate_state = fdp.ConsumeBool();
    bool test_gzip_header = fdp.ConsumeBool();
    bool test_raw_stream = fdp.ConsumeBool();
    
    // Parameter values
    int validate_mode = fdp.ConsumeIntegralInRange<int>(0, 2);
    unsigned bits = fdp.ConsumeIntegralInRange<unsigned>(0, 16);
    int flush_mode = fdp.ConsumeIntegralInRange<int>(0, 5);
    
    // Determine mode based on raw flag
    int actual_windowBits = windowBits;
    if (test_raw_stream) {
        actual_windowBits = -windowBits;  // Negative for raw mode
    }
    
    // Consume test data for compression/decompression
    size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(128, 8192);
    if (fdp.remaining_bytes() < test_data_size) {
        return 0;
    }
    std::vector<uint8_t> test_data = fdp.ConsumeBytes<uint8_t>(test_data_size);
    
    // Consume header data if needed
    size_t header_data_size = 0;
    std::vector<uint8_t> header_data;
    if (test_header_ops || test_gzip_header) {
        header_data_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
        if (fdp.remaining_bytes() < header_data_size) {
            return 0;
        }
        header_data = fdp.ConsumeBytes<uint8_t>(header_data_size);
    }
    
    // Initialize deflate stream for testing deflate state functions
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    int deflate_ret = Z_STREAM_ERROR;
    if (test_deflate_state || test_header_ops) {
        deflate_ret = deflateInit2(&deflate_strm, level, Z_DEFLATED, actual_windowBits, memLevel, strategy);
    }
    
    // Test deflate state functions if deflate stream initialized successfully
    if (deflate_ret == Z_OK) {
        // Setup input data for deflate
        size_t compr_size = deflateBound(&deflate_strm, (uLong)test_data.size());
        Bytef* compr = (Bytef*)malloc(compr_size);
        if (compr) {
            deflate_strm.next_in = test_data.data();
            deflate_strm.avail_in = (uInt)test_data.size();
            deflate_strm.next_out = compr;
            deflate_strm.avail_out = (uInt)compr_size;
            
            // Perform partial compression to test state functions
            int ret = deflate(&deflate_strm, Z_PARTIAL_FLUSH);
            if (ret == Z_OK || ret == Z_BUF_ERROR) {
                // Test deflatePending - check pending output
                unsigned pending;
                int bits_pending;
                deflatePending(&deflate_strm, &pending, &bits_pending);
                
                // Test deflateUsed - check bytes used
                int used_bits;
                deflateUsed(&deflate_strm, &used_bits);
                
                // Test deflateSetHeader if header data available
                if (test_header_ops && header_data.size() > 0) {
                    gz_header header;
                    memset(&header, 0, sizeof(header));
                    header.extra = header_data.data();
                    header.extra_len = (uInt)header_data.size();
                    header.name = (Bytef*)"test.gz";
                    header.name_max = 0;
                    header.comment = (Bytef*)"fuzzer test";
                    header.comm_max = 0;
                    header.hcrc = fdp.ConsumeBool() ? 1 : 0;
                    header.done = 0;
                    
                    deflateSetHeader(&deflate_strm, &header);
                }
                
                // Continue compression to finish
                deflate_strm.avail_in = 0;
                ret = deflate(&deflate_strm, Z_FINISH);
                
                // Test state functions again after completion
                if (ret == Z_STREAM_END) {
                    deflatePending(&deflate_strm, &pending, &bits_pending);
                    deflateUsed(&deflate_strm, &used_bits);
                }
            }
            
            free(compr);
        }
        
        // Clean up deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // Initialize inflate stream for testing inflate control functions
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    // First, create some compressed data to decompress
    size_t small_compr_size = 4096;
    Bytef* small_compr = (Bytef*)malloc(small_compr_size);
    Bytef* uncompr = (Bytef*)malloc(test_data.size());
    
    if (small_compr && uncompr && test_data.size() > 0) {
        // Create a simple compressed stream
        z_stream tmp_strm;
        memset(&tmp_strm, 0, sizeof(z_stream));
        tmp_strm.zalloc = Z_NULL;
        tmp_strm.zfree = Z_NULL;
        tmp_strm.opaque = Z_NULL;
        
        if (deflateInit2(&tmp_strm, level, Z_DEFLATED, actual_windowBits, memLevel, strategy) == Z_OK) {
            tmp_strm.next_in = test_data.data();
            tmp_strm.avail_in = (uInt)test_data.size();
            tmp_strm.next_out = small_compr;
            tmp_strm.avail_out = (uInt)small_compr_size;
            
            int ret = deflate(&tmp_strm, Z_FINISH);
            if (ret == Z_STREAM_END) {
                size_t compressed_size = small_compr_size - tmp_strm.avail_out;
                
                // Now initialize inflate stream with the compressed data
                if (inflateInit2(&inflate_strm, actual_windowBits) == Z_OK) {
                    inflate_strm.next_in = small_compr;
                    inflate_strm.avail_in = (uInt)compressed_size;
                    inflate_strm.next_out = uncompr;
                    inflate_strm.avail_out = (uInt)test_data.size();
                    
                    // Test inflateSyncPoint - check if at sync point
                    if (test_sync_point) {
                        int sync_result = inflateSyncPoint(&inflate_strm);
                        // Test at various points during decompression
                    }
                    
                    // Test inflateMark - get current position mark
                    if (test_stream_mark) {
                        long mark = inflateMark(&inflate_strm);
                        // Use mark for possible reset (though we won't actually reset in fuzzer)
                    }
                    
                    // Test inflateValidate - validate stream
                    if (test_validate) {
                        int validate_result = inflateValidate(&inflate_strm, validate_mode);
                    }
                    
                    // Test inflateGetHeader - get gzip header if present
                    gz_header header;
                    memset(&header, 0, sizeof(header));
                    if (test_gzip_header) {
                        inflateGetHeader(&inflate_strm, &header);
                    }
                    
                    // Perform partial decompression
                    ret = inflate(&inflate_strm, Z_SYNC_FLUSH);
                    
                    // Test functions again during decompression
                    if (ret == Z_OK || ret == Z_BUF_ERROR) {
                        if (test_sync_point) {
                            inflateSyncPoint(&inflate_strm);
                        }
                        if (test_stream_mark) {
                            inflateMark(&inflate_strm);
                        }
                        if (test_validate) {
                            inflateValidate(&inflate_strm, validate_mode);
                        }
                        
                        // Continue decompression
                        inflate_strm.avail_in = 0;
                        ret = inflate(&inflate_strm, Z_FINISH);
                        
                        // Test after completion
                        if (ret == Z_STREAM_END) {
                            if (test_sync_point) inflateSyncPoint(&inflate_strm);
                            if (test_stream_mark) inflateMark(&inflate_strm);
                        }
                    }
                    
                    inflateEnd(&inflate_strm);
                }
            }
            deflateEnd(&tmp_strm);
        }
    }
    
    if (small_compr) free(small_compr);
    if (uncompr) free(uncompr);
    
    // Test inflateBack if selected
    if (test_inflate_back && test_data.size() > 0) {
        // Create window buffer
        size_t window_size = 1U << windowBits;
        unsigned char* window = (unsigned char*)malloc(window_size);
        if (window) {
            memset(window, 0, window_size);
            
            z_stream back_strm;
            memset(&back_strm, 0, sizeof(z_stream));
            back_strm.zalloc = Z_NULL;
            back_strm.zfree = Z_NULL;
            back_strm.opaque = Z_NULL;
            
            // Initialize inflateBack
            if (inflateBackInit(&back_strm, windowBits, window) == Z_OK) {
                // Setup callback contexts
                InputContext in_ctx;
                in_ctx.data = test_data.data();
                in_ctx.size = test_data.size();
                in_ctx.pos = 0;
                
                OutputContext out_ctx;
                out_ctx.max_size = test_data.size() * 4;  // Allow expansion
                
                // Prepare initial input if available
                std::vector<uint8_t> initial_input;
                size_t initial_input_size = fdp.ConsumeIntegralInRange<size_t>(0, 256);
                if (initial_input_size > 0 && fdp.remaining_bytes() >= initial_input_size) {
                    initial_input = fdp.ConsumeBytes<uint8_t>(initial_input_size);
                    back_strm.next_in = initial_input.data();
                    back_strm.avail_in = (uInt)initial_input.size();
                }
                
                // Perform inflateBack with callbacks
                int back_ret = inflateBack(&back_strm, input_callback, &in_ctx,
                                          output_callback, &out_ctx);
                
                // Test inflate control functions on the back stream if possible
                if (test_sync_point) {
                    // Note: inflateSyncPoint may not work on inflateBack streams
                    // but we call it to test error paths
                    inflateSyncPoint(&back_strm);
                }
                
                // Clean up
                inflateBackEnd(&back_strm);
            }
            
            free(window);
        }
    }
    
    return 0;
}

} // namespace harness_006
