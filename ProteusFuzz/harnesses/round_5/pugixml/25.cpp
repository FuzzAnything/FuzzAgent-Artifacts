// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    if (!doc.load_string("<?xml version=\"1.0\"?><root><child/></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child("child");
    if (!child_node) {
        // Try to create a child if not found to proceed with fuzzing
        child_node = root.append_child("child");
        if (!child_node) {
            return 0;
        }
    }

    // 2. pugi::xml_node::append_attribute (1st)
    pugi::xml_attribute attr1 = child_node.append_attribute("attr1");
    if (!attr1) {
        return 0;
    }

    // 3. pugi::xml_attribute::set_value (3 overloads)
    // Overload: int
    attr1.set_value(static_cast<int>(Data[0]));
    
    // Overload: unsigned int - Check bounds before accessing Data[1]
    if (Size >= 2) {
        attr1.set_value(static_cast<unsigned int>(Data[1] + 1));
    } else {
        attr1.set_value(static_cast<unsigned int>(Data[0] + 1));
    }
    
    // Overload: const char_t*
    std::string str_val(reinterpret_cast<const char*>(Data), Size);
    // Ensure null termination for safety if used as C-string
    std::string safe_str = str_val;
    safe_str.resize(100); // Limit size to prevent huge allocations
    safe_str.push_back('\0');
    attr1.set_value(safe_str.c_str());

    // 4. pugi::xml_node::append_attribute (2nd)
    pugi::xml_attribute attr2 = child_node.append_attribute("attr2");
    if (!attr2) {
        return 0;
    }

    // 5. pugi::xml_node::append_attribute (3rd)
    pugi::xml_attribute attr3 = child_node.append_attribute("attr3");
    if (!attr3) {
        return 0;
    }

    // 6. pugi::xml_attribute::set_value (3 overloads again for attr3)
    // Overload: int
    attr3.set_value(static_cast<int>(Data[0] ^ 0xFF));
    // Overload: unsigned int
    attr3.set_value(static_cast<unsigned int>(Size));
    // Overload: const char_t*
    std::string str_val2(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(50)));
    str_val2.push_back('\0');
    attr3.set_value(str_val2.c_str());

    // 7. pugi::xml_node::append_attribute (4th)
    pugi::xml_attribute attr4 = child_node.append_attribute("attr4");
    if (!attr4) {
        return 0;
    }

    // 8. pugi::xml_node::append_attribute (5th)
    pugi::xml_attribute attr5 = child_node.append_attribute("attr5");
    if (!attr5) {
        return 0;
    }

    // 9. pugi::xml_attribute::set_value (3 overloads again for attr5)
    // Overload: int
    attr5.set_value(static_cast<int>(Size % 1000));
    // Overload: unsigned int
    attr5.set_value(static_cast<unsigned int>(Size % 1000));
    // Overload: const char_t*
    std::string str_val3(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(20)));
    str_val3.push_back('\0');
    attr5.set_value(str_val3.c_str());

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}