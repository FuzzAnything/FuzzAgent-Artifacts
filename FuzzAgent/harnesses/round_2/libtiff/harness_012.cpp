#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <fuzzer/FuzzedDataProvider.h>
#include <tiffio.h>
#include <tiffio.hxx>

namespace harness_012 {

/* stolen from tiffiop.h, which is a private header so we can't just include it */
/* safe multiply returns either the multiplied value or 0 if it overflowed */
#define __TIFFSafeMultiply(t, v, m)                                            \
    ((((t)(m) != (t)0) && (((t)(((v) * (m)) / (m))) == (t)(v)))                \
         ? (t)((v) * (m))                                                      \
         : (t)0)

const uint64_t MAX_SIZE = 500000000;

extern "C" void handle_error(const char *unused, const char *unused2,
                             va_list unused3)
{
    return;
}

// Simple structure to use as client data
struct TestClientData {
    int id;
    float value;
    char name[32];
};

// Custom compression codec initialization function
static int custom_codec_init(TIFF* tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 1; // Success
}

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
#ifndef STANDALONE
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);
#endif

#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
    // libjpeg-turbo has issues with MSAN and SIMD code
    // See https://bugs.chromium.org/p/oss-fuzz/issues/detail?id=7547
    // and https://github.com/libjpeg-turbo/libjpeg-turbo/pull/365
    setenv("JSIMD_FORCENONE", "1", 1);
#endif
#endif

    // Need sufficient data for client info operations
    if (Size < 32) {
        return 0;  // Minimum: basic parameters + client data
    }

    FuzzedDataProvider fdp(Data, Size);

    // Create a memory stream for operations (avoid file I/O dependencies)
    std::ostringstream os;
    TIFF *tif = TIFFStreamOpen("MemTIFF", &os);
    if (!tif) {
        return 0;
    }

    // Set basic TIFF tags to create a valid directory
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32_t)1);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32_t)1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16_t)8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16_t)3);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    
    // Write the directory to initialize the TIFF structure
    if (!TIFFWriteDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Test 1: TIFFSetClientInfo and TIFFGetClientInfo with various data types
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (test_scenario) {
        case 0: {
            // Test with integer data
            int* int_data = new int;
            *int_data = fdp.ConsumeIntegral<int>();
            
            std::string client_name = fdp.ConsumeRandomLengthString(32);
            TIFFSetClientInfo(tif, int_data, client_name.c_str());
            
            void* retrieved = TIFFGetClientInfo(tif, client_name.c_str());
            if (retrieved) {
                int* retrieved_int = static_cast<int*>(retrieved);
                // Verify the data matches (at least check it's not null)
                if (*retrieved_int != *int_data) {
                    // Data mismatch - this is OK for fuzzing, just continue
                }
            }
            
            delete int_data;
            break;
        }
        
        case 1: {
            // Test with string data
            std::string str_data = fdp.ConsumeRandomLengthString(128);
            char* str_copy = new char[str_data.size() + 1];
            strcpy(str_copy, str_data.c_str());
            
            std::string client_name = fdp.ConsumeRandomLengthString(32);
            TIFFSetClientInfo(tif, str_copy, client_name.c_str());
            
            void* retrieved = TIFFGetClientInfo(tif, client_name.c_str());
            if (retrieved) {
                char* retrieved_str = static_cast<char*>(retrieved);
                // Could compare strings here, but just accessing is enough for coverage
            }
            
            delete[] str_copy;
            break;
        }
        
        case 2: {
            // Test with structure data
            TestClientData* struct_data = new TestClientData;
            struct_data->id = fdp.ConsumeIntegral<int>();
            struct_data->value = fdp.ConsumeFloatingPoint<float>();
            std::string name_str = fdp.ConsumeRandomLengthString(31);
            strncpy(struct_data->name, name_str.c_str(), 31);
            struct_data->name[31] = '\0';
            
            std::string client_name = fdp.ConsumeRandomLengthString(32);
            TIFFSetClientInfo(tif, struct_data, client_name.c_str());
            
            void* retrieved = TIFFGetClientInfo(tif, client_name.c_str());
            if (retrieved) {
                TestClientData* retrieved_struct = static_cast<TestClientData*>(retrieved);
                // Access structure members to exercise code
                (void)retrieved_struct->id;
                (void)retrieved_struct->value;
                (void)retrieved_struct->name;
            }
            
            delete struct_data;
            break;
        }
        
        case 3: {
            // Test with array data
            size_t array_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
            int* array_data = new int[array_size];
            for (size_t i = 0; i < array_size; i++) {
                array_data[i] = fdp.ConsumeIntegral<int>();
            }
            
            std::string client_name = fdp.ConsumeRandomLengthString(32);
            TIFFSetClientInfo(tif, array_data, client_name.c_str());
            
            void* retrieved = TIFFGetClientInfo(tif, client_name.c_str());
            if (retrieved) {
                int* retrieved_array = static_cast<int*>(retrieved);
                // Access array element to exercise code
                if (array_size > 0) {
                    (void)retrieved_array[0];
                }
            }
            
            delete[] array_data;
            break;
        }
    }

    // Test 2: Multiple client info entries with same and different names
    if (fdp.remaining_bytes() > 50) {
        std::string name1 = fdp.ConsumeRandomLengthString(16);
        std::string name2 = fdp.ConsumeRandomLengthString(16);
        
        int data1 = fdp.ConsumeIntegral<int>();
        int data2 = fdp.ConsumeIntegral<int>();
        
        // Set two different client info entries
        TIFFSetClientInfo(tif, &data1, name1.c_str());
        TIFFSetClientInfo(tif, &data2, name2.c_str());
        
        // Retrieve both
        void* retrieved1 = TIFFGetClientInfo(tif, name1.c_str());
        void* retrieved2 = TIFFGetClientInfo(tif, name2.c_str());
        
        // Test overwriting existing entry
        int data3 = fdp.ConsumeIntegral<int>();
        TIFFSetClientInfo(tif, &data3, name1.c_str()); // Overwrite name1
        
        // Retrieve overwritten entry
        void* retrieved3 = TIFFGetClientInfo(tif, name1.c_str());
        
        // Test non-existent entry
        std::string non_existent = "non_existent_" + fdp.ConsumeRandomLengthString(10);
        void* should_be_null = TIFFGetClientInfo(tif, non_existent.c_str());
        (void)should_be_null; // Should be NULL
    }

    // Test 3: TIFFAccessTagMethods
    // This function provides access to the tag methods structure
    TIFFTagMethods* tag_methods = TIFFAccessTagMethods(tif);
    if (tag_methods) {
        // We can't call these methods without proper context, but accessing
        // the structure exercises the TIFFAccessTagMethods function
        (void)tag_methods->vsetfield;
        (void)tag_methods->vgetfield;
        (void)tag_methods->printdir;
    }

    // Test 4: Optional codec registration/unregistration
    // Only test if we have enough remaining bytes
    if (fdp.remaining_bytes() > 100) {
        // Create a custom codec name from fuzzer input
        std::string codec_name = "CustomCodec_" + fdp.ConsumeRandomLengthString(20);
        
        // Choose a scheme number (should be in valid range)
        uint16_t scheme = fdp.ConsumeIntegralInRange<uint16_t>(32768, 65535); // User-defined range
        
        // Register the custom codec
        TIFFCodec* registered_codec = TIFFRegisterCODEC(scheme, codec_name.c_str(), custom_codec_init);
        
        if (registered_codec) {
            // Codec was registered successfully
            // We could attempt to use it, but that requires more complex setup
            
            // Test unregistering the codec
            TIFFUnRegisterCODEC(registered_codec);
            
            // Test registering again (should work)
            TIFFCodec* re_registered = TIFFRegisterCODEC(scheme, codec_name.c_str(), custom_codec_init);
            if (re_registered) {
                // Clean up again
                TIFFUnRegisterCODEC(re_registered);
            }
        }
        
        // Test with invalid/null init method
        if (fdp.ConsumeBool()) {
            TIFFCodec* null_codec = TIFFRegisterCODEC(scheme + 1, "NullCodec", nullptr);
            if (null_codec) {
                TIFFUnRegisterCODEC(null_codec);
            }
        }
    }

    // Test 5: Edge cases for client info
    if (fdp.remaining_bytes() > 10) {
        // Test with empty name
        int empty_name_data = fdp.ConsumeIntegral<int>();
        TIFFSetClientInfo(tif, &empty_name_data, "");
        
        void* empty_retrieved = TIFFGetClientInfo(tif, "");
        (void)empty_retrieved;
        
        // Test with NULL data pointer
        std::string null_data_name = "null_data_" + fdp.ConsumeRandomLengthString(10);
        TIFFSetClientInfo(tif, nullptr, null_data_name.c_str());
        
        void* null_retrieved = TIFFGetClientInfo(tif, null_data_name.c_str());
        (void)null_retrieved; // Should be NULL
    }

    // Clean up
    TIFFClose(tif);
    return 0;
}

} // namespace harness_012
