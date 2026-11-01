// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
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
#include <random>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Seed random number generator with input data for deterministic behavior per input
    std::mt19937 gen(Data[0]);
    std::uniform_int_distribution<unsigned int> options_dist(0, 0xFFFFFFFF);
    std::uniform_int_distribution<int> encoding_dist(0, 9); // 0 to 9 to cover enum range safely

    pugi::xml_document doc;
    pugi::xml_document proto_doc;

    // 1. Fuzz load_buffer (Primary target for raw binary data)
    {
        pugi::xml_document doc1;
        unsigned int opts = options_dist(gen);
        int enc_val = encoding_dist(gen);
        pugi::xml_encoding enc = static_cast<pugi::xml_encoding>(enc_val);
        
        // Ensure encoding is within valid enum range if possible, though casting to int is safe for fuzzing
        if (enc_val > 9) enc = pugi::encoding_auto;

        doc1.load_buffer(Data, Size, opts, enc);
    }

    // 2. Fuzz load_buffer_inplace
    {
        pugi::xml_document doc2;
        // We need a mutable buffer. Create a copy.
        std::vector<uint8_t> buffer(Size);
        std::memcpy(buffer.data(), Data, Size);
        
        unsigned int opts = options_dist(gen);
        int enc_val = encoding_dist(gen);
        pugi::xml_encoding enc = static_cast<pugi::xml_encoding>(enc_val);
        if (enc_val > 9) enc = pugi::encoding_auto;

        doc2.load_buffer_inplace(buffer.data(), Size, opts, enc);
    }

    // 3. Fuzz load_string (Requires null-termination)
    {
        pugi::xml_document doc3;
        // Create a null-terminated copy if the input doesn't end with null
        if (Size > 0 && Data[Size-1] == 0) {
            doc3.load_string(reinterpret_cast<const pugi::char_t*>(Data), options_dist(gen));
        } else {
            // Try to find a null byte or just truncate and append one
            size_t null_pos = Size;
            for (size_t i = 0; i < Size; ++i) {
                if (Data[i] == 0) {
                    null_pos = i;
                    break;
                }
            }
            // If no null found, we can't safely use load_string without copying and appending.
            if (null_pos < Size) {
                 doc3.load_string(reinterpret_cast<const pugi::char_t*>(Data), options_dist(gen));
            } else {
                // No null byte found, create a safe buffer with null terminator
                std::vector<uint8_t> safe_buf(Size + 1);
                std::memcpy(safe_buf.data(), Data, Size);
                safe_buf[Size] = 0;
                doc3.load_string(reinterpret_cast<const pugi::char_t*>(safe_buf.data()), options_dist(gen));
            }
        }
    }

    // 4. Fuzz load (std::istream)
    // FIX: std::istringstream constructor taking (const char*, size_t) is not standard.
    // We must use a std::string which handles the size internally.
    {
        pugi::xml_document doc4;
        std::string str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(str, std::ios::binary);
        doc4.load(stream, options_dist(gen));
    }

    // 5. Fuzz load (std::wistream)
    {
        pugi::xml_document doc5;
        // Convert input to wstring for wistream
        std::vector<wchar_t> wide_data(Size);
        for (size_t i = 0; i < Size; ++i) {
            wide_data[i] = static_cast<wchar_t>(Data[i]);
        }
        // Construct wstring from vector, then pass to wistringstream
        std::wstring wstr(wide_data.begin(), wide_data.end());
        std::wistringstream wstream(wstr, std::ios::binary);
        doc5.load(wstream, options_dist(gen));
    }

    // 6. Fuzz load_file (wchar_t path)
    {
        pugi::xml_document doc6;
        write_dummy_file(Data, Size);
        unsigned int opts = options_dist(gen);
        int enc_val = encoding_dist(gen);
        pugi::xml_encoding enc = static_cast<pugi::xml_encoding>(enc_val);
        if (enc_val > 9) enc = pugi::encoding_auto;
        
        doc6.load_file(L"./dummy_file", opts, enc);
    }

    // 7. Fuzz reset(const xml_document& proto)
    {
        pugi::xml_document doc7;
        // Load some data into proto_doc first to make reset meaningful
        proto_doc.load_buffer(Data, Size, options_dist(gen));
        
        doc7.reset(proto_doc);
        
        // Also test reset on empty doc
        pugi::xml_document empty_doc;
        empty_doc.reset(proto_doc);
    }

    // 8. Fuzz reset() (no args)
    {
        pugi::xml_document doc8;
        doc8.load_buffer(Data, Size);
        doc8.reset();
    }

    // Cleanup: remove dummy file
    std::remove("./dummy_file");

    return 0;
}