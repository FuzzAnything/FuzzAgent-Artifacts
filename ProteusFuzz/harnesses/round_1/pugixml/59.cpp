// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
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

static pugi::xml_encoding get_safe_encoding(int value) {
    // Valid enum values are 0 (encoding_auto) to 9 (encoding_latin1)
    // Ensure the value is within the valid range to prevent assertion failure
    if (value >= 0 && value <= 9) {
        return static_cast<pugi::xml_encoding>(value);
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;

    // Random options and encoding for variety
    uint32_t random_opts = 0;
    int random_encoding_val = 0;

    // Safely extract random_opts if enough data exists
    if (Size >= sizeof(random_opts)) {
        std::memcpy(&random_opts, Data, sizeof(random_opts));
    }
    
    // Safely extract random_encoding_val if enough data exists after random_opts
    if (Size >= sizeof(random_opts) + sizeof(int)) {
        std::memcpy(&random_encoding_val, Data + sizeof(random_opts), sizeof(int));
    }

    pugi::xml_encoding safe_encoding = get_safe_encoding(random_encoding_val);

    // 1. load_buffer
    {
        pugi::xml_document doc_buf;
        doc_buf.load_buffer(Data, Size, random_opts, safe_encoding);
    }

    // 2. load (char stream)
    {
        pugi::xml_document doc_stream_char;
        std::string str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(str, std::ios::binary);
        doc_stream_char.load(stream, random_opts, safe_encoding);
    }

    // 2. load (wchar stream)
    {
        pugi::xml_document doc_stream_wchar;
        std::vector<wchar_t> wbuf(Size);
        for (size_t i = 0; i < Size; ++i) {
            wbuf[i] = static_cast<wchar_t>(Data[i]);
        }
        std::wstring wstr(wbuf.begin(), wbuf.end());
        std::wistringstream wstream(wstr, std::ios::binary);
        // Note: load(wstream) only takes options, encoding is ignored or auto-detected for wchar streams
        doc_stream_wchar.load(wstream, random_opts);
    }

    // 3. load_file
    {
        pugi::xml_document doc_file;
        write_dummy_file(Data, Size);
        doc_file.load_file("./dummy_file", random_opts, safe_encoding);
    }

    // 4. load_buffer_inplace
    {
        pugi::xml_document doc_inplace;
        std::vector<uint8_t> buffer_copy(Data, Data + Size);
        doc_inplace.load_buffer_inplace(buffer_copy.data(), Size, random_opts, safe_encoding);
    }

    // 5. load_string
    {
        pugi::xml_document doc_string;
        // Ensure null termination for load_string
        std::vector<uint8_t> null_terminated_buffer(Data, Data + Size);
        null_terminated_buffer.push_back(0);
        doc_string.load_string(reinterpret_cast<const pugi::char_t*>(null_terminated_buffer.data()), random_opts);
    }

    return 0;
}