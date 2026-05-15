#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <tiffio.h>

namespace harness_010 {
/* Fuzzing harness for libtiff targeting codec registration and field management operations
 * Focus: TIFF codec registration and field management APIs with 229+ undiscovered branches
 * Strategy: Create semantically unique harness for codec extension and field management testing
 * Target APIs: TIFFGetConfiguredCODECs -> TIFFRegisterCODEC -> TIFFUnRegisterCODEC -> 
 *              TIFFSetClientInfo -> TIFFGetClientInfo -> TIFFFieldSetGetCountSize -> 
 *              TIFFMergeFieldInfo -> TIFFFieldWithName
 * Coverage Gap: These APIs are currently 0% covered despite being reached by fuzzers
 */


/* Error handler to suppress libtiff error messages during fuzzing */
extern "C" void handle_error(const char* unused, const char* unused2, va_list unused3) {
    // Suppress error messages during fuzzing
}

/* Dummy codec initialization function for testing TIFFRegisterCODEC */
static int dummy_codec_init(TIFF* tif, int scheme) {
    (void)tif;
    (void)scheme;
    return 1;  // Return success
}

/* Structure to hold client info for testing TIFFSetClientInfo/TIFFGetClientInfo */
struct ClientInfoData {
    uint32_t magic;
    uint32_t data1;
    uint64_t data2;
    char name[64];
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration, codec data, and field info
    if (size < 256) {
        return 0;
    }

    // Set error handlers to suppress libtiff messages
    TIFFSetErrorHandler(handle_error);
    TIFFSetWarningHandler(handle_error);

    FuzzedDataProvider fdp(data, size);

    // Consume operation mode to determine which combination of APIs to test
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0: codec only, 1: field only, 2: client info, 3: all
    
    // Consume codec configuration parameters
    uint16_t codec_scheme = fdp.ConsumeIntegral<uint16_t>();
    std::string codec_name = fdp.ConsumeRandomLengthString(32);
    
    // Consume client info parameters
    ClientInfoData client_data;
    client_data.magic = fdp.ConsumeIntegral<uint32_t>();
    client_data.data1 = fdp.ConsumeIntegral<uint32_t>();
    client_data.data2 = fdp.ConsumeIntegral<uint64_t>();
    std::string client_name = fdp.ConsumeRandomLengthString(32);
    strncpy(client_data.name, client_name.c_str(), sizeof(client_data.name) - 1);
    client_data.name[sizeof(client_data.name) - 1] = '\0';
    
    // Consume field info parameters
    uint32_t field_tag = fdp.ConsumeIntegral<uint32_t>();
    short field_readcount = fdp.ConsumeIntegral<short>();
    short field_writecount = fdp.ConsumeIntegral<short>();
    TIFFDataType field_type = static_cast<TIFFDataType>(fdp.ConsumeIntegralInRange<int>(1, 12)); // Valid TIFFDataType range
    unsigned short field_bit = fdp.ConsumeIntegral<unsigned short>();
    unsigned char field_oktochange = fdp.ConsumeIntegral<unsigned char>() % 2;
    unsigned char field_passcount = fdp.ConsumeIntegral<unsigned char>() % 2;
    std::string field_name_str = fdp.ConsumeRandomLengthString(32);
    
    // Use remaining data as TIFF file content
    std::string tiff_data = fdp.ConsumeRemainingBytesAsString();
    
    // Create a temporary file for TIFF operations
    char filename[] = "/tmp/tiff_fuzz_codec_XXXXXX.tif";
    int fd = mkstemps(filename, 4);
    if (fd < 0) {
        return 0;
    }
    
    // Write TIFF data to the temporary file
    if (write(fd, tiff_data.c_str(), tiff_data.size()) != (ssize_t)tiff_data.size()) {
        close(fd);
        unlink(filename);
        return 0;
    }
    
    close(fd);
    
    // Open TIFF file for reading
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        // If opening for reading fails, try to create a new file
        tif = TIFFOpen(filename, "w");
        if (!tif) {
            unlink(filename);
            return 0;
        }
    }
    
    // Test TIFFGetConfiguredCODECs - get list of configured codecs
    TIFFCodec* configured_codecs = TIFFGetConfiguredCODECs();
    if (configured_codecs) {
        // Iterate through codecs (not modifying, just reading)
        for (TIFFCodec* codec = configured_codecs; codec->name != NULL; codec++) {
            // Just read the codec info without modifying
            const char* name = codec->name;
            uint16_t scheme = codec->scheme;
            (void)name;
            (void)scheme;
        }
        // Free the codec list
        _TIFFfree(configured_codecs);
    }
    
    // Test codec registration operations if enabled
    if (operation_mode == 0 || operation_mode == 3) {
        // Check if codec already exists
        const TIFFCodec* existing_codec = TIFFFindCODEC(codec_scheme);
        
        if (existing_codec == NULL) {
            // Register a dummy codec
            TIFFCodec* dummy_codec = TIFFRegisterCODEC(codec_scheme, codec_name.c_str(), dummy_codec_init);
            if (dummy_codec) {
                // Verify registration by finding the codec
                const TIFFCodec* found_codec = TIFFFindCODEC(codec_scheme);
                if (found_codec) {
                    // Check if the codec is configured
                    int is_configured = TIFFIsCODECConfigured(codec_scheme);
                    (void)is_configured;
                }
                
                // Get configured codecs again to see our new one
                TIFFCodec* updated_codecs = TIFFGetConfiguredCODECs();
                if (updated_codecs) {
                    _TIFFfree(updated_codecs);
                }
                
                // Unregister the codec
                TIFFUnRegisterCODEC(dummy_codec);
            }
        }
    }
    
    // Test field management operations if enabled
    if (operation_mode == 1 || operation_mode == 3) {
        // Create a TIFFFieldInfo array for testing
        TIFFFieldInfo field_info[] = {
            {
                field_tag,                // field_tag
                field_readcount,          // field_readcount
                field_writecount,         // field_writecount
                field_type,               // field_type
                field_bit,                // field_bit
                field_oktochange,         // field_oktochange
                field_passcount,          // field_passcount
                field_name_str.c_str()    // field_name
            }
        };
        
        // Test TIFFMergeFieldInfo - merge custom field info into TIFF structure
        int merge_result = TIFFMergeFieldInfo(tif, field_info, 1);
        (void)merge_result;
        
        // Test TIFFFieldWithName - find field by name
        const TIFFField* field_by_name = TIFFFieldWithName(tif, field_name_str.c_str());
        if (field_by_name) {
            // Test various field accessor functions
            uint32_t tag = TIFFFieldTag(field_by_name);
            const char* name = TIFFFieldName(field_by_name);
            TIFFDataType data_type = TIFFFieldDataType(field_by_name);
            int pass_count = TIFFFieldPassCount(field_by_name);
            int read_count = TIFFFieldReadCount(field_by_name);
            int write_count = TIFFFieldWriteCount(field_by_name);
            int set_get_size = TIFFFieldSetGetSize(field_by_name);
            int count_size = TIFFFieldSetGetCountSize(field_by_name);
            int is_anonymous = TIFFFieldIsAnonymous(field_by_name);
            
            (void)tag;
            (void)name;
            (void)data_type;
            (void)pass_count;
            (void)read_count;
            (void)write_count;
            (void)set_get_size;
            (void)count_size;
            (void)is_anonymous;
        }
    }
    
    // Test client info operations if enabled
    if (operation_mode == 2 || operation_mode == 3) {
        // Set client info with our custom data
        TIFFSetClientInfo(tif, &client_data, client_name.c_str());
        
        // Get client info back and verify
        ClientInfoData* retrieved_data = static_cast<ClientInfoData*>(TIFFGetClientInfo(tif, client_name.c_str()));
        if (retrieved_data) {
            // Verify data matches (just checking pointer, not deep comparison)
            if (retrieved_data == &client_data) {
                // Data pointer matches, which is expected
            }
        }
        
        // Test with different client name (should return NULL)
        void* non_existent = TIFFGetClientInfo(tif, "non_existent_client");
        (void)non_existent;
    }
    
    // Test TIFFAccessTagMethods - get tag methods structure
    TIFFTagMethods* tag_methods = TIFFAccessTagMethods(tif);
    if (tag_methods) {
        // Access the methods (just reading, not calling)
        TIFFVSetMethod vsetfield = tag_methods->vsetfield;
        TIFFVGetMethod vgetfield = tag_methods->vgetfield;
        TIFFPrintMethod printdir = tag_methods->printdir;
        (void)vsetfield;
        (void)vgetfield;
        (void)printdir;
    }
    
    // Clean up
    TIFFClose(tif);
    unlink(filename);
    
    // Test TIFFCleanup for final cleanup (this is global, use carefully)
    // Note: TIFFCleanup is global and might affect other tests, so we only call it
    // in a specific operation mode to avoid interfering with other harnesses
    if (operation_mode == 3 && fdp.ConsumeBool()) {
        // Only call TIFFCleanup occasionally to avoid global state issues
        TIFFCleanup(tif);
    }
    
    return 0;
}

} // namespace harness_010
