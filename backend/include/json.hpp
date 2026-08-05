#ifndef MINIMAL_JSON_HPP
#define MINIMAL_JSON_HPP

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace Json {

inline std::string escape_string(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if ((unsigned char)c < 32) {
                    char buf[10];
                    snprintf(buf, sizeof(buf), "\\u%04x", (unsigned char)c);
                    out += buf;
                } else {
                    out += c;
                }
                break;
        }
    }
    return out;
}

class Value {
public:
    enum Type { NullType, BoolType, NumberType, StringType, ArrayType, ObjectType };

    Type type = NullType;
    bool bool_val = false;
    double number_val = 0.0;
    std::string string_val;
    std::vector<Value> array_val;
    std::map<std::string, Value> object_val;

    Value() : type(NullType) {}
    Value(bool b) : type(BoolType), bool_val(b) {}
    Value(int n) : type(NumberType), number_val((double)n) {}
    Value(size_t n) : type(NumberType), number_val((double)n) {}
    Value(double n) : type(NumberType), number_val(n) {}
    Value(const char* s) : type(StringType), string_val(s ? s : "") {}
    Value(const std::string& s) : type(StringType), string_val(s) {}

    void set(const std::string& key, const Value& val) {
        type = ObjectType;
        object_val[key] = val;
    }

    void push(const Value& val) {
        type = ArrayType;
        array_val.push_back(val);
    }

    std::string dump() const {
        std::ostringstream ss;
        if (type == NullType) {
            ss << "null";
        } else if (type == BoolType) {
            ss << (bool_val ? "true" : "false");
        } else if (type == NumberType) {
            if (number_val == (long long)number_val) {
                ss << (long long)number_val;
            } else {
                ss << number_val;
            }
        } else if (type == StringType) {
            ss << "\"" << escape_string(string_val) << "\"";
        } else if (type == ArrayType) {
            ss << "[";
            for (size_t i = 0; i < array_val.size(); ++i) {
                if (i > 0) ss << ",";
                ss << array_val[i].dump();
            }
            ss << "]";
        } else if (type == ObjectType) {
            ss << "{";
            size_t idx = 0;
            for (const auto& kv : object_val) {
                if (idx > 0) ss << ",";
                ss << "\"" << escape_string(kv.first) << "\":" << kv.second.dump();
                idx++;
            }
            ss << "}";
        }
        return ss.str();
    }
};

// Helper simple parser for flat/nested string objects from client requests
inline std::map<std::string, std::string> parse_object_flat(const std::string& json_str) {
    std::map<std::string, std::string> result;
    if (json_str.empty()) return result;

    bool in_string = false;
    bool is_key = true;
    std::string current_key = "";
    std::string current_val = "";
    bool escaped = false;

    for (size_t i = 0; i < json_str.size(); ++i) {
        char c = json_str[i];

        if (escaped) {
            if (is_key) current_key += c;
            else current_val += c;
            escaped = false;
            continue;
        }

        if (c == '\\') {
            escaped = true;
            continue;
        }

        if (c == '"') {
            in_string = !in_string;
            continue;
        }

        if (in_string) {
            if (is_key) current_key += c;
            else current_val += c;
            continue;
        }

        if (c == ':') {
            is_key = false;
            current_val.clear();
            continue;
        }

        if (c == ',' || c == '}') {
            if (!current_key.empty()) {
                // trim value
                auto s = current_val.find_first_not_of(" \t\r\n");
                if (s != std::string::npos) {
                    auto e = current_val.find_last_not_of(" \t\r\n");
                    current_val = current_val.substr(s, e - s + 1);
                }
                result[current_key] = current_val;
            }
            current_key.clear();
            current_val.clear();
            is_key = true;
            continue;
        }
    }
    return result;
}

} // namespace Json

#endif // MINIMAL_JSON_HPP
