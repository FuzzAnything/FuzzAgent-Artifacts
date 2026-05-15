#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <vector>
#include <string>

namespace harness_015 {
/*
 * Fuzzing harness for libtiff library focusing on CODEC management functions
 * Targets completely uncovered CODEC APIs: TIFFGetConfiguredCODECs, TIFFRegisterCODEC, 
 * TIFFUnRegisterCODEC, TIFFFindCODEC, TIFFIsCODECConfigured with 47+ undiscovered branches
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Basic TIFF file operations and reading
 * - harness_001: TIFF writing operations with compression
 * - harness_002: RGBA image reading functions  
 * - harness_014: Tile writing operations
 * - harness_015: Specifically targets CODEC management and registration system,
 *   testing the internal compression scheme registry, custom codec registration,
 *   and codec discovery mechanisms
 * 
 * CRITICAL FIX: Removed dangerous edge case tests that pass nullptr to 
 * TIFFUnRegisterCODEC and TIFFRegisterCODEC with null name, as these functions
 * do not handle NULL pointers safely (TIFFUnRegisterCODEC dereferences c->name,
 * TIFFRegisterCODEC uses strlen(name)).
 */


// Error handler to suppress library error messages during fuzzing
extern "C" void handle_error(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

// Simple stub initialization function for custom codecs
// Returns success (1) to indicate initialization succeeded
extern "C" int stub_codec_init(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 1; // Success
}

// Alternative stub that returns failure (0) for error path testing
extern "C" int stub_codec_init_fail(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 0; // Failure
}

// Stub that returns -1 for more error testing
extern "C" int stub_codec_init_error(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return -1; // Error
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size check - need at least some data for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    // Initialize FuzzedDataProvider for structured input processing
    FuzzedDataProvider fdp(data, size);
    
    // Set error handlers to suppress library messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
    
    // Phase 1: Test TIFFGetConfiguredCODECs - get list of currently configured codecs
    TIFFCodec *configured_codecs = TIFFGetConfiguredCODECs();
    if (configured_codecs) {
        // Iterate through the list (terminated by NULL entry)
        int codec_count = 0;
        for (TIFFCodec *codec = configured_codecs; codec->name != NULL; codec++) {
            codec_count++;
            
            // Test TIFFFindCODEC for each scheme
            const TIFFCodec *found = TIFFFindCODEC(codec->scheme);
            (void)found; // Use result to prevent optimization
            
            // Test TIFFIsCODECConfigured for each scheme
            int is_configured = TIFFIsCODECConfigured(codec->scheme);
            (void)is_configured; // Use result to prevent optimization
        }
        
        // Free the allocated codec list (caller responsibility per documentation)
        _TIFFfree(configured_codecs);
    }
    
    // Phase 2: Test TIFFFindCODEC with random compression schemes
    // Valid compression schemes range from 1 to 65535, but common ones are:
    // 1 (NONE), 2 (CCITTRLE), 3 (CCITTFAX3), 4 (CCITTFAX4), 5 (LZW), 
    // 6 (OJPEG), 7 (JPEG), 8 (ADOBE_DEFLATE), 9 (T85), 10 (T43),
    // 32773 (PACKBITS), 32946 (DEFLATE), 50000 (ZSTD), 50001 (WEBP)
    
    uint16_t num_scheme_tests = fdp.ConsumeIntegralInRange<uint16_t>(1, 20);
    for (uint16_t i = 0; i < num_scheme_tests && fdp.remaining_bytes() > 2; i++) {
        uint16_t scheme = fdp.ConsumeIntegral<uint16_t>();
        
        // Test TIFFFindCODEC
        const TIFFCodec *found_codec = TIFFFindCODEC(scheme);
        (void)found_codec;
        
        // Test TIFFIsCODECConfigured
        int is_configured = TIFFIsCODECConfigured(scheme);
        (void)is_configured;
    }
    
    // Phase 3: Test custom codec registration and unregistration
    // Determine how many custom codecs to register based on remaining input
    uint8_t num_custom_codecs = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    std::vector<TIFFCodec*> registered_codecs;
    
    for (uint8_t i = 0; i < num_custom_codecs && fdp.remaining_bytes() > 10; i++) {
        // Consume scheme number (ensure it's not 0)
        uint16_t scheme = fdp.ConsumeIntegral<uint16_t>();
        if (scheme == 0) scheme = 1; // Ensure non-zero scheme
        
        // Consume codec name (limited length)
        size_t name_length = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::string codec_name = fdp.ConsumeBytesAsString(name_length);
        if (codec_name.empty()) {
            codec_name = "CustomCodec";
        }
        
        // Select initialization function based on fuzzed input
        TIFFInitMethod init_func = nullptr;
        uint8_t init_selector = fdp.ConsumeIntegral<uint8_t>() % 3;
        switch (init_selector) {
            case 0:
                init_func = stub_codec_init;
                break;
            case 1:
                init_func = stub_codec_init_fail;
                break;
            case 2:
                init_func = stub_codec_init_error;
                break;
        }
        
        // Register the custom codec
        TIFFCodec *registered = TIFFRegisterCODEC(scheme, codec_name.c_str(), init_func);
        if (registered) {
            registered_codecs.push_back(registered);
            
            // Verify it can be found
            const TIFFCodec *found = TIFFFindCODEC(scheme);
            (void)found;
            
            // Verify it's reported as configured
            int is_configured = TIFFIsCODECConfigured(scheme);
            (void)is_configured;
        }
        
        // Test duplicate registration (should return existing codec)
        if (fdp.ConsumeBool() && registered) {
            TIFFCodec *duplicate = TIFFRegisterCODEC(scheme, "DuplicateCodec", stub_codec_init);
            (void)duplicate;
        }
    }
    
    // Phase 4: Test TIFFGetConfiguredCODECs again after registration
    // This should now include our custom codecs
    TIFFCodec *updated_codecs = TIFFGetConfiguredCODECs();
    if (updated_codecs) {
        int updated_count = 0;
        for (TIFFCodec *codec = updated_codecs; codec->name != NULL; codec++) {
            updated_count++;
            
            // Verify we can find each codec
            const TIFFCodec *found = TIFFFindCODEC(codec->scheme);
            (void)found;
        }
        _TIFFfree(updated_codecs);
    }
    
    // Phase 5: Unregister some or all custom codecs
    while (!registered_codecs.empty() && fdp.remaining_bytes() > 0) {
        // Decide whether to unregister based on fuzzed input
        if (fdp.ConsumeBool()) {
            TIFFCodec *codec_to_remove = registered_codecs.back();
            registered_codecs.pop_back();
            
            // Get scheme before unregistering for verification
            uint16_t scheme = codec_to_remove->scheme;
            
            // Unregister the codec
            TIFFUnRegisterCODEC(codec_to_remove);
            
            // Verify it's no longer found (may still exist as built-in)
            const TIFFCodec *found_after = TIFFFindCODEC(scheme);
            (void)found_after;
            
            // Check if still configured
            int still_configured = TIFFIsCODECConfigured(scheme);
            (void)still_configured;
        } else {
            // Skip this codec
            registered_codecs.pop_back();
        }
    }
    
    // Phase 6: Test edge cases
    
    // Test with NULL name (should handle gracefully)
    // NOTE: TIFFRegisterCODEC uses strlen(name) which would crash with nullptr.
    // Removing this test to avoid NULL pointer dereference.
    // if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
    //     uint16_t scheme = fdp.ConsumeIntegral<uint16_t>();
    //     TIFFCodec *null_name_codec = TIFFRegisterCODEC(scheme, nullptr, stub_codec_init);
    //     (void)null_name_codec;
    // }
    
    // Test with NULL init function
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 2) {
        uint16_t scheme = fdp.ConsumeIntegral<uint16_t>();
        std::string name = fdp.ConsumeRandomLengthString(20);
        TIFFCodec *null_init_codec = TIFFRegisterCODEC(scheme, name.c_str(), nullptr);
        (void)null_init_codec;
    }
    
    // Test unregistering NULL (should handle gracefully)
    // NOTE: TIFFUnRegisterCODEC does not check for NULL pointer, so calling it
    // with nullptr would cause NULL pointer dereference. Removing this test.
    // if (fdp.ConsumeBool()) {
    //     TIFFUnRegisterCODEC(nullptr);
    // }
    
    // Test unregistering already unregistered codec (should error)
    if (fdp.ConsumeBool() && !registered_codecs.empty()) {
        // Create a dummy codec struct to attempt unregistration
        TIFFCodec dummy_codec;
        dummy_codec.name = "Dummy";
        dummy_codec.scheme = 9999;
        dummy_codec.init = stub_codec_init;
        TIFFUnRegisterCODEC(&dummy_codec);
    }
    
    // Phase 7: Final verification - get codecs one more time
    TIFFCodec *final_codecs = TIFFGetConfiguredCODECs();
    if (final_codecs) {
        int final_count = 0;
        for (TIFFCodec *codec = final_codecs; codec->name != NULL; codec++) {
            final_count++;
        }
        _TIFFfree(final_codecs);
    }
    
    return 0;
}

} // namespace harness_015
