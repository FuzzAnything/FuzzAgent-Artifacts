// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <fstream>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <cstdlib>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static void writeDummyFileWide(const uint8_t* data, size_t size) {
    std::wofstream file("./dummy_file", std::ios::binary);
    if (file) {
        // Convert bytes to wchar_t (simple approach - may not be correct for all encodings)
        size_t wsize = size / sizeof(wchar_t);
        if (wsize > 0) {
            file.write(reinterpret_cast<const wchar_t*>(data), wsize);
        }
    }
}

static bool isValidEncoding(pugi::xml_encoding encoding) {
    return encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1;
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data for string operations
    if (Size == 0) {
        // Still test with empty input
        const char* empty_str = "";
        pugi::xml_document doc1;
        doc1.load_string(empty_str);
        doc1.reset();
        
        pugi::xml_document doc2;
        doc2.load(empty_str);
        return 0;
    }

    // Create null-terminated string from fuzzer input
    char* cstr = new char[Size + 1];
    memcpy(cstr, Data, Size);
    cstr[Size] = '\0';
    
    // Create wchar_t string for wide character operations
    wchar_t* wstr = nullptr;
    size_t wsize = Size / sizeof(wchar_t);
    if (wsize > 0) {
        wstr = new wchar_t[wsize + 1];
        memcpy(wstr, Data, Size);
        wstr[wsize] = L'\0';
    }

    // Generate random values for options and encoding from input data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= sizeof(unsigned int)) {
        memcpy(&options, Data, sizeof(unsigned int));
    } else {
        options = static_cast<unsigned int>(Data[0]);
    }
    
    if (Size >= 2) {
        uint8_t enc_byte = Data[Size % Size]; // Use modulo to avoid out of bounds
        encoding = static_cast<pugi::xml_encoding>(enc_byte % 10); // 0-9
    }

    try {
        // Test 1: xml_document::reset()
        pugi::xml_document doc1;
        doc1.reset(); // Reset empty document
        
        // Test reset with another document
        pugi::xml_document doc2;
        if (Size > 10) {
            doc2.load_string(cstr, options);
            doc1.reset(doc2); // Reset with another document
        }

        // Test 2: xml_document::load_string()
        pugi::xml_document doc3;
        pugi::xml_parse_result result1 = doc3.load_string(cstr, options);
        (void)result1; // Use result to avoid unused variable warning

        // Test 3: xml_document::load() - first overload (deprecated)
        pugi::xml_document doc4;
        pugi::xml_parse_result result2 = doc4.load(cstr, options);
        (void)result2;

        // Test 3: xml_document::load() - second overload (wchar_t stream)
        if (wstr != nullptr && wsize > 0) {
            pugi::xml_document doc5;
            std::wstringstream wstream;
            wstream.write(wstr, wsize);
            pugi::xml_parse_result result3 = doc5.load(wstream, options);
            (void)result3;
        }

        // Test 4: xml_document::load_file()
        // Write fuzzer data to dummy file
        writeDummyFile(Data, Size);
        
        pugi::xml_document doc6;
        if (isValidEncoding(encoding)) {
            pugi::xml_parse_result result4 = doc6.load_file("./dummy_file", options, encoding);
            (void)result4;
        } else {
            // Use default encoding if generated encoding is invalid
            pugi::xml_parse_result result4 = doc6.load_file("./dummy_file", options);
            (void)result4;
        }

        // Test 5: xml_document::save_file()
        // First load some content to save
        pugi::xml_document doc7;
        if (Size > 0) {
            doc7.load_string(cstr, options);
            
            // Generate random flags from input data
            unsigned int flags = pugi::format_default;
            if (Size >= 4) {
                memcpy(&flags, Data + (Size % (Size-3)), sizeof(unsigned int));
            }
            
            // Generate indent string from input data
            char indent[32] = "\t";
            if (Size >= 8) {
                size_t indent_len = std::min(Size % 31, (size_t)30);
                memcpy(indent, Data, indent_len);
                indent[indent_len] = '\0';
            }
            
            if (isValidEncoding(encoding)) {
                bool save_result = doc7.save_file("./dummy_file_save", indent, flags, encoding);
                (void)save_result;
            } else {
                bool save_result = doc7.save_file("./dummy_file_save", indent, flags);
                (void)save_result;
            }
            
            // Also test with default parameters
            bool save_result2 = doc7.save_file("./dummy_file_save2");
            (void)save_result2;
        }

        // Clean up temporary files
        std::remove("./dummy_file");
        std::remove("./dummy_file_save");
        std::remove("./dummy_file_save2");

    } catch (...) {
        // Catch any exceptions to prevent crashes
    }

    // Clean up
    delete[] cstr;
    if (wstr != nullptr) {
        delete[] wstr;
    }

    return 0;
}