// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_71(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize random options and encoding based on input data
    unsigned int options = 0;
    if (Size >= 4) {
        options = *reinterpret_cast<const uint32_t*>(Data);
        // Ensure options are within reasonable bounds for pugixml flags
        options &= 0xFFFF; 
    }
    
    pugi::xml_encoding encoding = pugi::encoding_auto;
    if (Size >= 5) {
        int enc_idx = Data[4] % 10; // 0 to 9
        switch (enc_idx) {
            case 0: encoding = pugi::encoding_auto; break;
            case 1: encoding = pugi::encoding_utf8; break;
            case 2: encoding = pugi::encoding_utf16_le; break;
            case 3: encoding = pugi::encoding_utf16_be; break;
            case 4: encoding = pugi::encoding_utf16; break;
            case 5: encoding = pugi::encoding_utf32_le; break;
            case 6: encoding = pugi::encoding_utf32_be; break;
            case 7: encoding = pugi::encoding_utf32; break;
            case 8: encoding = pugi::encoding_wchar; break;
            case 9: encoding = pugi::encoding_latin1; break;
        }
    }

    // 1. Test load_buffer
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer(Data, Size, options, encoding);
        (void)result;
    }

    // 2. Test load (from stream)
    // Fix: Use std::string with explicit size to avoid reading past buffer end
    {
        pugi::xml_document doc;
        std::string str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(str, std::ios::binary);
        pugi::xml_parse_result result = doc.load(stream, options, encoding);
        (void)result;
    }

    // 3. Test load_file
    {
        pugi::xml_document doc;
        write_dummy_file(Data, Size);
        pugi::xml_parse_result result = doc.load_file(L"./dummy_file", options, encoding);
        (void)result;
    }

    // 4. Test load_buffer_inplace
    {
        pugi::xml_document doc;
        std::vector<char> buffer(Size);
        std::memcpy(buffer.data(), Data, Size);
        pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.data(), Size, options, encoding);
        (void)result;
    }

    // 5. Test load_buffer_inplace_own
    {
        pugi::xml_document doc;
        void* buffer = malloc(Size);
        if (buffer) {
            std::memcpy(buffer, Data, Size);
            pugi::xml_parse_result result = doc.load_buffer_inplace_own(buffer, Size, options, encoding);
            (void)result;
        }
    }

    // 6. Test load_string
    {
        pugi::xml_document doc;
        // Ensure null-termination for load_string
        std::vector<char> str_buffer(Size + 1);
        std::memcpy(str_buffer.data(), Data, Size);
        str_buffer[Size] = '\0';
        
        pugi::xml_parse_result result = doc.load_string(reinterpret_cast<const pugi::char_t*>(str_buffer.data()), options);
        (void)result;
    }

    return 0;
}