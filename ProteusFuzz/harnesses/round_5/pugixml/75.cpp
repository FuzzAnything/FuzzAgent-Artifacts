// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7704:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
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
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <sstream>
#include <random>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static pugi::xml_encoding get_random_encoding() {
    static std::mt19937 rng(0);
    static std::uniform_int_distribution<int> dist(0, 9);
    return static_cast<pugi::xml_encoding>(dist(rng));
}

static unsigned int get_random_options() {
    static std::mt19937 rng(0);
    static std::uniform_int_distribution<unsigned int> dist(0, 0xFFFFFFFF);
    return dist(rng);
}

extern "C" int LLVMFuzzerTestOneInput_75(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    pugi::xml_document proto;
    pugi::xml_parse_result result;

    // Helper to ensure null-termination for string functions
    std::vector<char> buffer(Size + 1);
    if (Size > 0) {
        std::memcpy(buffer.data(), Data, Size);
    }
    buffer[Size] = '\0';

    // 1. Fuzz load_buffer_inplace
    // We need to ensure the buffer persists. We allocate a vector on heap to simulate this.
    std::vector<uint8_t> inplace_buffer(Size);
    if (Size > 0) std::memcpy(inplace_buffer.data(), Data, Size);
    
    // Try different encodings and options
    for (int i = 0; i < 3; ++i) {
        result = doc.load_buffer_inplace(
            inplace_buffer.data(), 
            inplace_buffer.size(), 
            get_random_options(), 
            get_random_encoding()
        );
        (void)result; // Suppress unused warning
    }

    // 2. Fuzz load (std::basic_istream<wchar_t>)
    // Convert input to wchar_t string for stream
    std::vector<wchar_t> wbuffer(Size + 1);
    if (Size > 0) {
        for (size_t j = 0; j < Size; ++j) {
            wbuffer[j] = static_cast<wchar_t>(Data[j]);
        }
    }
    wbuffer[Size] = L'\0';
    
    std::wstringstream wstream;
    wstream.write(wbuffer.data(), Size);
    wstream.clear(); // Clear error flags if any
    wstream.seekg(0);
    
    result = doc.load(wstream, get_random_options());
    (void)result;

    // 3. Fuzz load_file
    // Write data to dummy file
    write_dummy_file(Data, Size);
    
    // Use wide string path
    const wchar_t* dummy_path = L"./dummy_file";
    result = doc.load_file(dummy_path, get_random_options(), get_random_encoding());
    (void)result;

    // 4. Fuzz reset
    // First, load some valid-ish data into proto to have something to copy
    // Use load_string on proto with the null-terminated buffer
    proto.load_string(buffer.data(), get_random_options());
    
    // Reset doc with proto
    doc.reset(proto);
    
    // Also test simple reset
    doc.reset();

    // 5. Fuzz load_buffer
    // This copies data, so we can use the raw input directly
    result = doc.load_buffer(Data, Size, get_random_options(), get_random_encoding());
    (void)result;

    // 6. Fuzz load_string
    // Ensure null-termination is handled (we did this in 'buffer')
    // Note: If the input contains embedded nulls, load_string will stop early, which is expected behavior.
    result = doc.load_string(buffer.data(), get_random_options());
    (void)result;

    return 0;
}