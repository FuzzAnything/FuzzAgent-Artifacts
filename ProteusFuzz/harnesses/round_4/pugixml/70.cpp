// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <vector>

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.append_child("root");
        return doc;
    }

    // Ensure null termination for string parsing
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';

    if (!doc.load_string(buffer.data())) {
        // If parsing fails, create a minimal valid document
        doc.append_child("root");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc = create_document_from_input(Data, Size);
    pugi::xml_node root = doc.child("root");
    
    // If no root exists, try to get any child
    if (!root) {
        root = doc.first_child();
    }
    
    // If still no node, create one
    if (!root) {
        root = doc.append_child("root");
    }

    // Extract potential attribute names from input data
    std::vector<std::string> names;
    std::vector<std::string> values;
    
    // Simple heuristic to extract strings from binary data
    size_t i = 0;
    while (i < Size) {
        if (Data[i] >= 32 && Data[i] < 127) {
            size_t start = i;
            while (i < Size && Data[i] >= 32 && Data[i] < 127) {
                i++;
            }
            if (i - start > 0) {
                std::string str(reinterpret_cast<const char*>(Data + start), i - start);
                if (str.length() > 0 && str.length() < 64) {
                    names.push_back(str);
                    values.push_back(str);
                }
            }
        } else {
            i++;
        }
    }
    
    // Ensure we have at least one name
    if (names.empty()) {
        names.push_back("fuzz_attr");
        values.push_back("fuzz_value");
    }

    // Helper to get a random name index
    auto get_name_idx = [&](size_t seed) -> size_t {
        return seed % names.size();
    };

    // Fuzz insert_attribute_before
    {
        pugi::xml_attribute ref_attr = root.attribute(names[get_name_idx(Size)].c_str());
        if (!ref_attr) {
            // Create a reference attribute first
            ref_attr = root.append_attribute(names[get_name_idx(Size + 1)].c_str());
            ref_attr.set_value(values[get_name_idx(Size + 2)].c_str());
        }
        if (ref_attr) {
            pugi::xml_attribute new_attr = root.insert_attribute_before(
                names[get_name_idx(Size + 3)].c_str(), 
                ref_attr
            );
            if (new_attr) {
                new_attr.set_value(values[get_name_idx(Size + 4)].c_str());
            }
        }
    }

    // Fuzz attribute
    {
        pugi::xml_attribute attr = root.attribute(names[get_name_idx(Size + 5)].c_str());
        (void)attr; // Suppress unused variable warning
    }

    // Fuzz insert_attribute_after
    {
        pugi::xml_attribute ref_attr = root.attribute(names[get_name_idx(Size + 6)].c_str());
        if (!ref_attr) {
            ref_attr = root.append_attribute(names[get_name_idx(Size + 7)].c_str());
            ref_attr.set_value(values[get_name_idx(Size + 8)].c_str());
        }
        if (ref_attr) {
            pugi::xml_attribute new_attr = root.insert_attribute_after(
                names[get_name_idx(Size + 9)].c_str(), 
                ref_attr
            );
            if (new_attr) {
                new_attr.set_value(values[get_name_idx(Size + 10)].c_str());
            }
        }
    }

    // Fuzz insert_copy_after
    {
        pugi::xml_attribute proto_attr = root.attribute(names[get_name_idx(Size + 11)].c_str());
        pugi::xml_attribute ref_attr = root.attribute(names[get_name_idx(Size + 12)].c_str());
        
        if (!proto_attr) {
            proto_attr = root.append_attribute(names[get_name_idx(Size + 13)].c_str());
            proto_attr.set_value(values[get_name_idx(Size + 14)].c_str());
        }
        
        if (!ref_attr) {
            ref_attr = root.append_attribute(names[get_name_idx(Size + 15)].c_str());
            ref_attr.set_value(values[get_name_idx(Size + 16)].c_str());
        }
        
        if (proto_attr && ref_attr) {
            pugi::xml_attribute copied = root.insert_copy_after(proto_attr, ref_attr);
            (void)copied;
        }
    }

    // Fuzz insert_copy_before
    {
        pugi::xml_attribute proto_attr = root.attribute(names[get_name_idx(Size + 17)].c_str());
        pugi::xml_attribute ref_attr = root.attribute(names[get_name_idx(Size + 18)].c_str());
        
        if (!proto_attr) {
            proto_attr = root.append_attribute(names[get_name_idx(Size + 19)].c_str());
            proto_attr.set_value(values[get_name_idx(Size + 20)].c_str());
        }
        
        if (!ref_attr) {
            ref_attr = root.append_attribute(names[get_name_idx(Size + 21)].c_str());
            ref_attr.set_value(values[get_name_idx(Size + 22)].c_str());
        }
        
        if (proto_attr && ref_attr) {
            pugi::xml_attribute copied = root.insert_copy_before(proto_attr, ref_attr);
            (void)copied;
        }
    }

    // Fuzz prepend_attribute
    {
        pugi::xml_attribute new_attr = root.prepend_attribute(names[get_name_idx(Size + 23)].c_str());
        if (new_attr) {
            new_attr.set_value(values[get_name_idx(Size + 24)].c_str());
        }
    }

    return 0;
}