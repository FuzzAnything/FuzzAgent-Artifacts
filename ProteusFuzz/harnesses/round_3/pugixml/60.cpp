// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
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
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_document proto;
    
    // Helper to generate random options and encoding from input data
    auto get_options = [](const uint8_t* d, size_t s) -> unsigned int {
        if (s < 4) return 0;
        return static_cast<unsigned int>(d[0]) | (static_cast<unsigned int>(d[1]) << 8) | 
               (static_cast<unsigned int>(d[2]) << 16) | (static_cast<unsigned int>(d[3]) << 24);
    };

    auto get_encoding = [](const uint8_t* d, size_t s) -> pugi::xml_encoding {
        if (s < 1) return pugi::encoding_auto;
        int val = d[0] % 10; // 0 to 9
        return static_cast<pugi::xml_encoding>(val);
    };

    // 1. Fuzz load_buffer
    pugi::xml_parse_result res1 = doc.load_buffer(Data, Size, get_options(Data, Size), get_encoding(Data, Size));
    (void)res1;

    // 2. Fuzz load_buffer_inplace
    std::vector<uint8_t> inplace_buf(Size);
    if (Size > 0) {
        std::memcpy(inplace_buf.data(), Data, Size);
    }
    pugi::xml_parse_result res2 = doc.load_buffer_inplace(inplace_buf.data(), Size, get_options(Data, Size), get_encoding(Data, Size));
    (void)res2;

    // 3. Fuzz load_string
    std::vector<char> str_buf(Size + 1);
    if (Size > 0) {
        std::memcpy(str_buf.data(), Data, Size);
    }
    str_buf[Size] = '\0';
    
    pugi::xml_parse_result res3 = doc.load_string(reinterpret_cast<pugi::char_t*>(str_buf.data()), get_options(Data, Size));
    (void)res3;

    // 4. Fuzz load_file
    write_dummy_file(Data, Size);
    
    // Construct a wide string path for load_file manually
    const wchar_t* wpath = L"./dummy_file";
    pugi::xml_parse_result res4 = doc.load_file(wpath, get_options(Data, Size), get_encoding(Data, Size));
    (void)res4;

    // 5. Fuzz load_buffer_inplace_own
    if (Size > 0) {
        void* own_buf = std::malloc(Size);
        if (own_buf) {
            std::memcpy(own_buf, Data, Size);
            pugi::xml_parse_result res5 = doc.load_buffer_inplace_own(own_buf, Size, get_options(Data, Size), get_encoding(Data, Size));
            (void)res5;
            // Do not free own_buf here, the document should own it.
        }
    }

    // 6. Fuzz reset with proto
    pugi::xml_parse_result res_proto = proto.load_string(reinterpret_cast<pugi::char_t*>(str_buf.data()), get_options(Data, Size));
    (void)res_proto;
    
    doc.reset(proto);

    return 0;
}