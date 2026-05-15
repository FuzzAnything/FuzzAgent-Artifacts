#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>

namespace harness_013 {
/*
 * libtiff fuzzing harness for compression CODEC management APIs with 0% coverage
 * Targets compression CODEC management functions:
 * - TIFFGetConfiguredCODECs: Get list of configured codecs (built-in + registered)
 * - TIFFRegisterCODEC: Register a new compression codec
 * - TIFFUnRegisterCODEC: Unregister a compression codec
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Basic reading operations via TIFFStreamOpen
 * - harness_001: Basic writing operations (tiles/strips with some raw operations)
 * - harness_002: Multi-directory and compression
 * - harness_003: Directory writing lifecycle
 * - harness_004: File opening/client I/O
 * - harness_005: Advanced RGB reading operations
 * - harness_006: LogLuv color space transformation
 * - harness_007: Advanced compression codec configuration and testing
 * - harness_008: Directory and image writing
 * - harness_009: EXIF/GPS metadata reading
 * - harness_010: Complete writing subsystem lifecycle
 * - harness_011: Coordinate-based writing operations
 * - harness_012: EXIF/GPS directory manipulation
 * - harness_013: CODEC MANAGEMENT (registration, querying, unregistration)
 * 
 * This harness specifically targets CODEC management APIs that control
 * compression scheme registration and querying, a critical system-level
 * functionality not covered by existing harnesses.
 */


// Maximum string length for safety
const size_t MAX_CODEC_NAME_LENGTH = 256;
const size_t MAX_INITIAL_CODECS = 100;

// Dummy initialization function for custom codecs
static int DummyCodecInit(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 1; // Success
}

// Another dummy initialization function variant
static int DummyCodecInit2(TIFF *tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 0; // Failure
}

// Error handler that does nothing (silent)
void silent_error_handler(const char *module, const char *fmt, va_list ap) {
    (void)module;
    (void)fmt;
    (void)ap;
    return;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Set silent error handlers to avoid output noise
    TIFFSetErrorHandler(silent_error_handler);
    TIFFSetWarningHandler(silent_error_handler);
    
    // Minimum input size check
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Get initial configured CODECs
    TIFFCodec *initial_codecs = TIFFGetConfiguredCODECs();
    if (initial_codecs) {
        // Count how many codecs we have initially
        size_t initial_count = 0;
        TIFFCodec *codec_ptr = initial_codecs;
        while (codec_ptr->name != NULL) {
            initial_count++;
            codec_ptr++;
            if (initial_count > MAX_INITIAL_CODECS) {
                break; // Safety check
            }
        }
        
        // Clean up initial codecs list
        _TIFFfree(initial_codecs);
    }
    
    // Step 2: Consume parameters for codec registration testing
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    size_t num_codecs_to_register = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    bool test_unregister = fdp.ConsumeBool();
    bool test_multiple_registrations = fdp.ConsumeBool();
    
    // Array to store registered codecs for cleanup
    std::vector<TIFFCodec*> registered_codecs;
    
    // Step 3: Register custom codecs based on fuzzer input
    for (size_t i = 0; i < num_codecs_to_register && fdp.remaining_bytes() > 16; i++) {
        // Consume codec parameters from fuzzer input
        uint16_t scheme = fdp.ConsumeIntegral<uint16_t>();
        
        // Generate codec name from fuzzer input
        size_t name_length = fdp.ConsumeIntegralInRange<size_t>(1, MAX_CODEC_NAME_LENGTH);
        if (name_length > fdp.remaining_bytes()) {
            name_length = fdp.remaining_bytes();
        }
        std::string codec_name = fdp.ConsumeBytesAsString(name_length);
        
        if (codec_name.empty()) {
            codec_name = "DummyCodec";
        }
        
        // Choose initialization function based on fuzzer input
        TIFFInitMethod init_func = DummyCodecInit;
        if (fdp.ConsumeBool()) {
            init_func = DummyCodecInit2;
        }
        
        // Register the codec
        TIFFCodec *registered = TIFFRegisterCODEC(scheme, codec_name.c_str(), init_func);
        if (registered) {
            registered_codecs.push_back(registered);
        }
    }
    
    // Step 4: Test TIFFFindCODEC with various schemes
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 2; i++) {
        uint16_t scheme_to_find = fdp.ConsumeIntegral<uint16_t>();
        const TIFFCodec *found = TIFFFindCODEC(scheme_to_find);
        (void)found; // Use result to avoid unused variable warning
    }
    
    // Step 5: Test TIFFIsCODECConfigured
    for (int i = 0; i < 10 && fdp.remaining_bytes() > 2; i++) {
        uint16_t scheme_to_check = fdp.ConsumeIntegral<uint16_t>();
        int is_configured = TIFFIsCODECConfigured(scheme_to_check);
        (void)is_configured; // Use result to avoid unused variable warning
    }
    
    // Step 6: Get configured CODECs after registration
    TIFFCodec *post_registration_codecs = TIFFGetConfiguredCODECs();
    if (post_registration_codecs) {
        // Count codecs after registration
        size_t post_count = 0;
        TIFFCodec *codec_ptr = post_registration_codecs;
        while (codec_ptr->name != NULL) {
            post_count++;
            codec_ptr++;
            if (post_count > MAX_INITIAL_CODECS + 100) {
                break; // Safety check
            }
        }
        
        // Clean up
        _TIFFfree(post_registration_codecs);
    }
    
    // Step 7: Test unregistration if requested
    if (test_unregister && !registered_codecs.empty()) {
        // Unregister some or all codecs based on fuzzer input
        size_t num_to_unregister = fdp.ConsumeIntegralInRange<size_t>(0, registered_codecs.size());
        
        for (size_t i = 0; i < num_to_unregister; i++) {
            // Choose which codec to unregister
            size_t index = i % registered_codecs.size();
            if (registered_codecs[index] != nullptr) {
                TIFFUnRegisterCODEC(registered_codecs[index]);
                registered_codecs[index] = nullptr;
            }
        }
        
        // Get codecs after unregistration
        TIFFCodec *post_unregistration_codecs = TIFFGetConfiguredCODECs();
        if (post_unregistration_codecs) {
            _TIFFfree(post_unregistration_codecs);
        }
    }
    
    // Step 8: Test multiple registration scenarios
    if (test_multiple_registrations && fdp.remaining_bytes() > 32) {
        // Try to register the same scheme multiple times
        uint16_t duplicate_scheme = fdp.ConsumeIntegral<uint16_t>();
        std::string name1 = fdp.ConsumeRandomLengthString(64);
        std::string name2 = fdp.ConsumeRandomLengthString(64);
        
        if (name1.empty()) name1 = "CodecA";
        if (name2.empty()) name2 = "CodecB";
        
        TIFFCodec *codec1 = TIFFRegisterCODEC(duplicate_scheme, name1.c_str(), DummyCodecInit);
        TIFFCodec *codec2 = TIFFRegisterCODEC(duplicate_scheme, name2.c_str(), DummyCodecInit2);
        
        if (codec1) {
            registered_codecs.push_back(codec1);
        }
        if (codec2) {
            registered_codecs.push_back(codec2);
        }
        
        // Find the codec (should return the first registered one)
        const TIFFCodec *found_duplicate = TIFFFindCODEC(duplicate_scheme);
        (void)found_duplicate;
    }
    
    // Step 9: Test with specific compression schemes from fuzzer input
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 4; i++) {
        uint16_t specific_scheme = fdp.ConsumeIntegral<uint16_t>();
        std::string custom_name = fdp.ConsumeRandomLengthString(128);
        
        if (custom_name.empty()) {
            custom_name = "CustomScheme";
        }
        // Register with 50% probability
        if (fdp.ConsumeBool()) {
            TIFFCodec *custom_codec = TIFFRegisterCODEC(
                specific_scheme, 
                custom_name.c_str(), 
                fdp.ConsumeBool() ? DummyCodecInit : DummyCodecInit2
            );
            
            if (custom_codec) {
                registered_codecs.push_back(custom_codec);
                
                // Immediately unregister with 50% probability
                if (fdp.ConsumeBool()) {
                    TIFFUnRegisterCODEC(custom_codec);
                    // Remove from our tracking vector
                    for (auto it = registered_codecs.begin(); it != registered_codecs.end(); ++it) {
                        if (*it == custom_codec) {
                            registered_codecs.erase(it);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Step 10: Final cleanup of any remaining registered codecs
    for (TIFFCodec *codec : registered_codecs) {
        if (codec != nullptr) {
            TIFFUnRegisterCODEC(codec);
        }
    }
    
    // Step 11: Final query to ensure cleanup worked
    TIFFCodec *final_codecs = TIFFGetConfiguredCODECs();
    if (final_codecs) {
        _TIFFfree(final_codecs);
    }
    
    return 0;
}

} // namespace harness_013
