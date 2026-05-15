// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
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
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string_view>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* Data, size_t Size) {
    if (Size < 4) return 0;

    // Prepare dummy file
    writeDummyFile(Data, Size);

    // Initialize document and nodes
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    pugi::xml_node sibling = root.append_child(pugi::node_element);
    
    // Create string views from input data
    size_t part_size = Size / 4;
    std::string_view sv1(reinterpret_cast<const char*>(Data), part_size);
    std::string_view sv2(reinterpret_cast<const char*>(Data + part_size), part_size);
    std::string_view sv3(reinterpret_cast<const char*>(Data + 2 * part_size), part_size);
    std::string_view sv4(reinterpret_cast<const char*>(Data + 3 * part_size), Size - 3 * part_size);

    // Convert to null-terminated strings
    std::string str1(sv1.data(), sv1.length());
    std::string str2(sv2.data(), sv2.length());
    std::string str3(sv3.data(), sv3.length());
    std::string str4(sv4.data(), sv4.length());

    try {
        // 1. First insert_child_before
        pugi::xml_node inserted1 = root.insert_child_before(pugi::node_element, sibling);
        if (inserted1) {
            inserted1.set_name(str1.c_str());
        }

        // 2. first_child
        pugi::xml_node first1 = root.first_child();

        // 3. append_attribute
        if (first1) {
            pugi::xml_attribute attr = first1.append_attribute(str2.c_str());
            if (attr) {
                attr.set_value(str3.c_str());
            }
        }

        // 4. Second insert_child_before
        pugi::xml_node inserted2 = root.insert_child_before(pugi::node_pcdata, first1);
        
        // 5. Second first_child
        pugi::xml_node first2 = root.first_child();

        // 6. set_value
        if (first2) {
            first2.set_value(str4.c_str());
        }

        // 7. save
        std::stringstream ss;
        pugi::xml_writer_stream writer(ss);
        doc.save(writer, "  ", pugi::format_default, pugi::encoding_auto);

    } catch (...) {
        // Catch any exceptions to prevent crashes
    }

    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}