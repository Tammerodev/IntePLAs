#pragma once

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <fstream>
#include <string>

class JsonReader {
public:

    ~JsonReader() {
        json_stream_read.close();
    }

    bool open(const std::string &path) {
        json_stream_read.open(path);

        return json_stream_read.good();
    }

    bool init() {
        return reader.parse(json_stream_read, actualJson);
    }

    const std::string readParameterAsString(const std::string& param) {
        return actualJson[param].asString();
    }
    
private:
    std::ifstream json_stream_read;

    Json::Value actualJson;
    Json::Reader reader;
};

class JsonWriter {
public:

    

    bool open(const std::string &path) {
        json_stream_write.open(path);

        return json_stream_write.is_open();
    }
 
    bool init() {
        return true; // You can add initialization logic here if needed
    }

    bool writeParameter(const std::string& param, const std::string& new_val) {
        Json::Value root;

        // Read existing JSON content from the file, if any
        json_stream_write >> root;

        // Update or add the parameter in the JSON
        root[param] = new_val;

        // Clear the content of the file before writing
        json_stream_write.clear();
        json_stream_write.seekp(0);
        json_stream_write << root;

        return true;
    }

    void close() {
        json_stream_write.flush();
        json_stream_write.close();
    }

private:
    std::fstream json_stream_write;
};