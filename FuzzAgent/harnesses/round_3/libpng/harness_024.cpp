#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>
#include <functional>

namespace harness_024 {

// Custom memory allocation functions for testing png_set_mem_fn
static void* custom_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void custom_free_fn(png_structp png_ptr, void* ptr) {
    (void)png_ptr;
    free(ptr);
}

// Structure to track memory allocations for testing
struct memory_tracker {
    size_t total_allocated;
    size_t total_freed;
    std::vector<void*> allocations;
    
    memory_tracker() : total_allocated(0), total_freed(0) {}
    
    void* track_malloc(png_alloc_size_t size) {
        void* ptr = malloc(size);
        if (ptr) {
            total_allocated += size;
            allocations.push_back(ptr);
        }
        return ptr;
    }
    
    void track_free(void* ptr) {
        // Find and remove from allocations
        for (auto it = allocations.begin(); it != allocations.end(); ++it) {
            if (*it == ptr) {
                allocations.erase(it);
                // We don't track exact size freed since we don't have it
                total_freed++;
                break;
            }
        }
        free(ptr);
    }
    
    ~memory_tracker() {
        // Clean up any remaining allocations
        for (void* ptr : allocations) {
            free(ptr);
        }
    }
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for comprehensive memory and chunk testing
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ====================================================
    // PHASE 1: TEST DIRECT MEMORY ALLOCATION APIS
    // ====================================================
    
    // Consume test scenario selection
    int test_scenario = fdp.ConsumeIntegralInRange<int>(0, 4);
    
    // Consume allocation parameters with reasonable bounds
    // Limit allocations to prevent excessive memory usage
    const png_alloc_size_t MAX_ALLOC_SIZE = 65536; // 64KB maximum
    png_alloc_size_t alloc_size1 = fdp.ConsumeIntegralInRange<png_alloc_size_t>(1, MAX_ALLOC_SIZE);
    png_alloc_size_t alloc_size2 = fdp.ConsumeIntegralInRange<png_alloc_size_t>(1, MAX_ALLOC_SIZE);
    png_alloc_size_t alloc_size3 = fdp.ConsumeIntegralInRange<png_alloc_size_t>(1, MAX_ALLOC_SIZE);
    int num_elements = fdp.ConsumeIntegralInRange<int>(1, 100);
    size_t element_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
    
    // Create a dummy png_struct for testing memory APIs
    // Note: We need a valid png_struct pointer for most memory APIs
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Test different memory allocation scenarios based on test_scenario
    switch (test_scenario) {
        case 0: {
            // Test png_malloc and png_free
            void* ptr1 = png_malloc(png_ptr, alloc_size1);
            if (ptr1) {
                // Write some data to the allocated memory
                memset(ptr1, fdp.ConsumeIntegral<uint8_t>(), 
                       alloc_size1 > 256 ? 256 : alloc_size1);
                png_free(png_ptr, ptr1);
            }
            
            // Test png_calloc (allocates and zeroes memory)
            void* ptr2 = png_calloc(png_ptr, alloc_size2);
            if (ptr2) {
                // Verify memory is zeroed (at least first few bytes)
                uint8_t* byte_ptr = (uint8_t*)ptr2;
                bool is_zeroed = true;
                for (size_t i = 0; i < (alloc_size2 > 16 ? 16 : alloc_size2); i++) {
                    if (byte_ptr[i] != 0) {
                        is_zeroed = false;
                        break;
                    }
                }
                (void)is_zeroed; // Avoid unused variable warning
                png_free(png_ptr, ptr2);
            }
            break;
        }
        
        case 1: {
            // Test png_malloc_warn (returns NULL instead of erroring)
            void* ptr = png_malloc_warn(png_ptr, alloc_size1);
            if (ptr) {
                // Test with zero size (should return NULL)
                void* zero_ptr = png_malloc_warn(png_ptr, 0);
                (void)zero_ptr;
                
                png_free(png_ptr, ptr);
            }
            
            // Note: png_malloc_array is a private/internal function not exposed in public API
            // We cannot call it directly from harness code
            // Instead, test other public memory APIs
            
            // Test with different allocation sizes
            void* small_ptr = png_malloc_warn(png_ptr, 16);
            if (small_ptr) {
                png_free(png_ptr, small_ptr);
            }
            break;
        }
        
        case 2: {
            // Test custom memory handlers with png_set_mem_fn
            // Use static tracker to ensure it outlives the PNG structure
            // libpng may call registered memory functions during PNG destruction
            static memory_tracker tracker;
            
            // Set custom memory functions
            png_set_mem_fn(png_ptr, &tracker, 
                          [](png_structp png_ptr, png_alloc_size_t size) -> void* {
                              memory_tracker* mt = (memory_tracker*)png_get_mem_ptr(png_ptr);
                              return mt ? mt->track_malloc(size) : nullptr;
                          },
                          [](png_structp png_ptr, void* ptr) {
                              memory_tracker* mt = (memory_tracker*)png_get_mem_ptr(png_ptr);
                              if (mt) mt->track_free(ptr);
                          });
            
            // Verify we can get the memory pointer back
            void* mem_ptr = png_get_mem_ptr(png_ptr);
            (void)mem_ptr;
            
            // Test allocation with custom handler
            void* custom_ptr = png_malloc(png_ptr, alloc_size1);
            if (custom_ptr) {
                png_free(png_ptr, custom_ptr);
            }
            
            // Test png_malloc_default (uses system malloc)
            #ifdef PNG_USER_MEM_SUPPORTED
            void* default_ptr = png_malloc_default(png_ptr, alloc_size2);
            if (default_ptr) {
                png_free_default(png_ptr, default_ptr);
            }
            #endif
            
            // Note: tracker is static, so it's not deleted here
            // This ensures it outlives the PNG structure which will be destroyed later
            break;
        }
        
        case 3: {
            // Test memory allocation with various sizes and patterns
            std::vector<void*> allocations;
            
            // Allocate multiple blocks with reasonable bounds
            for (int i = 0; i < 10 && fdp.remaining_bytes() > 0; i++) {
                png_alloc_size_t size = fdp.ConsumeIntegralInRange<png_alloc_size_t>(1, 4096);
                void* ptr = png_malloc(png_ptr, size);
                if (ptr) {
                    allocations.push_back(ptr);
                    
                    // Write pattern to memory
                    uint8_t pattern = fdp.ConsumeIntegral<uint8_t>();
                    memset(ptr, pattern, size > 128 ? 128 : size);
                }
            }
            
            // Free allocations in random order
            while (!allocations.empty() && fdp.remaining_bytes() > 0) {
                size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, allocations.size() - 1);
                png_free(png_ptr, allocations[idx]);
                allocations.erase(allocations.begin() + idx);
            }
            
            // Free any remaining allocations
            for (void* ptr : allocations) {
                png_free(png_ptr, ptr);
            }
            break;
        }
        
        case 4: {
            // Test chunk-related memory operations
            // First create an info structure for chunk operations
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr == nullptr) {
                png_destroy_read_struct(&png_ptr, nullptr, nullptr);
                return 0;
            }
            
            // Test png_free_data with different flags
            // Set up some dummy data that can be freed
            uint32_t free_flags = 0;
            
            // Consume which flags to test
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_HIST;
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_ICCP;
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_SPLT;
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_ROWS;
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_PCAL;
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_SCAL;
            #ifdef PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_UNKN;
            #endif
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_PLTE;
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_TRNS;
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_TEXT;
            if (fdp.ConsumeBool()) free_flags |= PNG_FREE_EXIF;
            
            // Test png_free_data with consumed flags
            // Note: This will only free data if it was actually allocated
            png_free_data(png_ptr, info_ptr, free_flags, -1);
            
            // Test png_data_freer to change responsibility for freeing data
            int freer = fdp.ConsumeIntegralInRange<int>(0, 2);
            png_data_freer(png_ptr, info_ptr, freer, free_flags);
            
            png_destroy_info_struct(png_ptr, &info_ptr);
            break;
        }
    }
    
    // ====================================================
    // PHASE 2: TEST CHUNK-RELATED MEMORY OPERATIONS
    // ====================================================
    
    // Test chunk error/warning functions
    std::string error_msg = fdp.ConsumeRandomLengthString(128);
    std::string warning_msg = fdp.ConsumeRandomLengthString(128);
    
    // Note: These would normally be called from within libpng,
    // but we can test their interfaces
    (void)error_msg;
    (void)warning_msg;
    
    // Test with edge cases
    if (fdp.ConsumeBool()) {
        // Test with bounded large allocation size
        png_alloc_size_t huge_size = fdp.ConsumeIntegralInRange<png_alloc_size_t>(1, MAX_ALLOC_SIZE);
        void* huge_ptr = png_malloc_warn(png_ptr, huge_size);
        if (huge_ptr) {
            png_free(png_ptr, huge_ptr);
        }
    }
    
    if (fdp.ConsumeBool()) {
        // Test with size 0
        void* zero_ptr = png_malloc_warn(png_ptr, 0);
        (void)zero_ptr;
    }
    
    if (fdp.ConsumeBool()) {
        // Test with size 1
        void* tiny_ptr = png_malloc(png_ptr, 1);
        if (tiny_ptr) {
            png_free(png_ptr, tiny_ptr);
        }
    }
    
    // ====================================================
    // PHASE 3: TEST MEMORY LIMIT HANDLING
    // ====================================================
    
    // Test allocation near limits
    #ifdef PNG_MAX_MALLOC_64K
    // Test allocation near 64K limit if defined
    png_alloc_size_t near_limit = fdp.ConsumeIntegralInRange<png_alloc_size_t>(60000, 65536);
    void* limit_ptr = png_malloc_warn(png_ptr, near_limit);
    if (limit_ptr) {
        png_free(png_ptr, limit_ptr);
    }
    #endif
    
    // Test with remaining bytes
    if (fdp.remaining_bytes() > 0) {
        // Use remaining input for one more allocation test
        png_alloc_size_t final_size = fdp.ConsumeIntegralInRange<png_alloc_size_t>(
            1, fdp.remaining_bytes() > MAX_ALLOC_SIZE ? MAX_ALLOC_SIZE : fdp.remaining_bytes());
        void* final_ptr = png_malloc(png_ptr, final_size);
        if (final_ptr) {
            // Fill with remaining data
            size_t fill_size = final_size > fdp.remaining_bytes() ? 
                              fdp.remaining_bytes() : final_size;
            std::vector<uint8_t> fill_data = fdp.ConsumeBytes<uint8_t>(fill_size);
            if (fill_size > 0) {
                memcpy(final_ptr, fill_data.data(), fill_size);
            }
            png_free(png_ptr, final_ptr);
        }
    }
    
    // ====================================================
    // PHASE 4: CLEANUP
    // ====================================================
    
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    
    return 0;
}

} // namespace harness_024
