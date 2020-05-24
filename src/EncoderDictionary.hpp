#pragma once

#include <string_view>
#include <memory>
#include <unordered_map>
#include <vector>
#include <climits>
#include <iostream>

#include "BaseDictionary.hpp"

class EncoderDictionary : BaseDictionary {
    std::unordered_map<std::string_view, uint32_t> m_dict;
    std::vector<std::string> m_data;

    void insert(char c, uint32_t code) override {
        insert(std::string(1, c));
    }

    void insert(const std::string_view &view) {
        m_data.emplace_back(view);
        std::string_view view_data(m_data.back());
        m_dict[view_data] = m_dict.size();
    }

    inline std::string_view createStringView(const char* data_ptr, int substr_start, int substr_len) const {
        const char* str_start_ptr = data_ptr + substr_start;
        return std::string_view(str_start_ptr, substr_len);
    }

public:
    EncoderDictionary() {
        initializeDictionary();
    }

    bool findOrInsert(const char* data_ptr, int substr_start, int substr_len, uint32_t &code) {
        std::string_view sequence = createStringView(data_ptr, substr_start, substr_len);
        
        if (m_dict.find(sequence) != m_dict.end()) {
            return false;
        } else {
            // if not found we must add it to dictionary
            insert(sequence);
            // now return code of previous string
            std::string_view prev_sequence = createStringView(data_ptr, substr_start, substr_len - 1);
            code = m_dict[prev_sequence];
            return true;
        }
    }

    bool find(const char* data_ptr, int substr_start, int substr_len, uint32_t &code) {
        std::string_view sequence = createStringView(data_ptr, substr_start, substr_len);
        code = m_dict[sequence];
        return true;
    }

    size_t size() const {
        return m_dict.size();
    }
};