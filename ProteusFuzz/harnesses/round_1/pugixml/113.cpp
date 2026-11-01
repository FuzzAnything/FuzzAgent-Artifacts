// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a random seed from input data for options and encoding
    uint32_t seed = 0;
    if (Size >= 4) {
        std::memcpy(&seed, Data, 4);
    } else {
        std::memcpy(&seed, Data, Size);
    }

    unsigned int options = seed & 0xFFFF;
    // Ensure options are within reasonable bounds for pugixml flags
    // Common flags: parse_default (0), parse_escapes, parse_wconv_attribute, etc.
    // We mask to keep it somewhat valid but still random
    options = options & 0x3F; 

    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(seed & 0x0F);
    // Clamp encoding to valid range if necessary, though casting to enum is generally safe for fuzzing
    if (encoding > pugi::encoding_latin1) {
        encoding = static_cast<pugi::xml_encoding>(encoding % (pugi::encoding_latin1 + 1));
    }

    pugi::xml_document doc;
    pugi::xml_document proto_doc;

    // 1. Test load_buffer
    {
        pugi::xml_parse_result result = doc.load_buffer(Data, Size, options, encoding);
        (void)result; // Ignore result to avoid compiler warnings, but execution path is covered
        doc.reset();
    }

    // 2. Test load_buffer_inplace
    // We need a mutable buffer. Allocate a copy.
    {
        std::vector<uint8_t> buffer(Size);
        if (Size > 0) {
            std::memcpy(buffer.data(), Data, Size);
        }
        pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.data(), Size, options, encoding);
        (void)result;
        doc.reset();
    }

    // 3. Test load_string
    // Ensure null termination. We create a temporary buffer with a null terminator.
    // If the input already has a null terminator, we might find it, but we must ensure safety.
    {
        std::vector<char> str_buffer(Size + 1);
        if (Size > 0) {
            std::memcpy(str_buffer.data(), Data, Size);
        }
        str_buffer[Size] = '\0'; // Force null termination

        // Cast to char_t. If PUGIXML_WCHAR_MODE is on, this might be a narrowing conversion, 
        // which is acceptable for fuzzing to trigger potential issues or just test ASCII paths.
        pugi::xml_parse_result result = doc.load_string(reinterpret_cast<pugi::char_t*>(str_buffer.data()), options);
        (void)result;
        doc.reset();
    }

    // 4. Test load (deprecated, but still available)
    // Similar to load_string, ensure null termination.
    {
        std::vector<char> str_buffer(Size + 1);
        if (Size > 0) {
            std::memcpy(str_buffer.data(), Data, Size);
        }
        str_buffer[Size] = '\0';

        pugi::xml_parse_result result = doc.load(reinterpret_cast<pugi::char_t*>(str_buffer.data()), options);
        (void)result;
        doc.reset();
    }

    // 5. Test load_file
    // Write data to a dummy file first
    {
        write_dummy_file(Data, Size);
        pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
        (void)result;
        doc.reset();
    }

    // 6. Test reset with proto
    // First, populate proto_doc with some data to make reset meaningful
    {
        // Try to load into proto_doc first
        proto_doc.load_buffer(Data, Size, options, encoding);
        
        // Now reset doc with proto_doc
        doc.reset(proto_doc);
        
        // Cleanup proto_doc
        proto_doc.reset();
    }

    // 7. Test reset without proto (simple reset)
    {
        doc.load_buffer(Data, Size, options, encoding);
        doc.reset();
    }

    // 8. Test stream loading (std::istringstream)
    // pugixml has an overload for std::istream, but the target list specifically mentions 
    // load(const char_t*) and load_file. The prompt mentions "load parses XML from a stream" 
    // but the signature provided is the deprecated string one. 
    // However, to be thorough with "stream" concept mentioned in description, we can try 
    // to use the string version which is the one explicitly requested in the signature list.
    // If the library version supports stream overload not listed in the specific signature block, 
    // we stick to the provided signatures to avoid compilation errors with specific versions.
    // The provided signature for load is: load(const char_t* contents, ...).
    // We already tested this in step 4.

    return 0;
}