// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7806:34 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_node::print at pugixml.cpp:6846:30 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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

static std::mt19937 rng(0);

static uint32_t random_uint() {
    return static_cast<uint32_t>(rng());
}

static pugi::xml_encoding random_encoding() {
    return static_cast<pugi::xml_encoding>(random_uint() % 10);
}

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    const char* dummy_path = "./dummy_file";

    // Write input data to dummy file
    FILE* f = fopen(dummy_path, "wb");
    if (!f) {
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Fuzz load_file
    uint32_t options = random_uint();
    pugi::xml_encoding encoding = random_encoding();
    doc.load_file(dummy_path, options, encoding);

    // Fuzz save_file (char* overload)
    const pugi::char_t* indent = PUGIXML_TEXT("\t");
    uint32_t flags = random_uint();
    pugi::xml_encoding save_enc = random_encoding();
    doc.save_file(dummy_path, indent, flags, save_enc);

    // Fuzz save_file (wchar_t* overload)
    std::wstring wpath = L"./dummy_file";
    doc.save_file(wpath.c_str(), indent, flags, save_enc);

    // Fuzz save (std::basic_ostream<wchar_t>& overload)
    {
        std::wofstream wstream(dummy_path, std::ios::out);
        if (wstream.is_open()) {
            doc.save(wstream, indent, flags);
            // Explicit flush before scope exit to avoid potential issues
            if (wstream.good()) {
                wstream.flush();
            }
        }
        // Let destructor handle closing, but ensure we don't access after close if needed
    }

    // Fuzz save (xml_writer& overload)
    {
        FILE* wf = fopen(dummy_path, "wb");
        if (wf) {
            pugi::xml_writer_file writer(wf);
            doc.save(writer, indent, flags, save_enc);
            fclose(wf);
        }
    }

    // Fuzz print (xml_writer& overload)
    {
        FILE* pf = fopen(dummy_path, "wb");
        if (pf) {
            pugi::xml_writer_file writer(pf);
            doc.print(writer, indent, flags, save_enc, 0);
            fclose(pf);
        }
    }

    // Cleanup
    doc.reset();

    // Remove dummy file
    std::remove(dummy_path);

    return 0;
}