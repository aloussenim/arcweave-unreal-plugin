// ArcscriptTranspilerTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <filesystem>
#include <fstream>
#include <map>
#include <string>
#include <any>
#include <nlohmann/json.hpp>
#include "ArcscriptTranspiler.h"

using json = nlohmann::json;

using namespace Arcweave;

char* getStrCopy(const std::string& source)
{
    const size_t bufferSize = source.size() + 1;
    std::unique_ptr<char[]> myBuffer = std::make_unique<char[]>(bufferSize);
    std::memcpy(myBuffer.get(), source.c_str(), bufferSize);
    return myBuffer.release();
}

UVariable* getInitialVars(json initialVarsJson) {
    UVariable* initVars = new UVariable[initialVarsJson.size()];
    int i = 0;
    for (json::iterator it = initialVarsJson.begin(); it != initialVarsJson.end(); ++it) {
        std::string id = it.value()["id"].template get<std::string>();
        std::string name = it.value()["name"].template get<std::string>();
        std::string type = it.value()["type"].template get<std::string>();

        initVars[i].id = getStrCopy(id);
        initVars[i].name = getStrCopy(name);
        initVars[i].type = VariableType::AW_ANY;
        if (type == "string") {
            initVars[i].type = VariableType::AW_STRING;
        }
        else if (type == "integer") {
            initVars[i].type = VariableType::AW_INTEGER;
        }
        else if (type == "double") {
            initVars[i].type = VariableType::AW_DOUBLE;
        }
        else if (type == "boolean") {
            initVars[i].type = VariableType::AW_BOOLEAN;
        }

        if (initVars[i].type == VariableType::AW_STRING) {
            initVars[i].string_val = getStrCopy(it.value()["value"].template get<std::string>().c_str());
        }
        else if (initVars[i].type == VariableType::AW_INTEGER) {
            initVars[i].int_val = it.value()["value"].template get<int>();
        }
        else if (initVars[i].type == VariableType::AW_DOUBLE) {
            initVars[i].double_val= it.value()["value"].template get<double>();
        }
        else if (initVars[i].type == VariableType::AW_BOOLEAN) {
            initVars[i].bool_val = it.value()["value"].template get<bool>();
        }
        i += 1;
    }

    return initVars;
}

UVisit* getVisits(json initVisits) {
    if (initVisits.size() == 0) return nullptr;
    UVisit* visits = new UVisit[initVisits.size()];
    int i = 0; 
    for (json::iterator it = initVisits.begin(); it != initVisits.end(); ++it) {
        visits[i].elId = getStrCopy(it.key().c_str());
        visits[i].visits = it.value().template get<int>();
        i += 1;
    }
    return visits;
}

int testFile(std::filesystem::path path) {
    std::ifstream f(path);
    json data = json::parse(f);
    // ssstd::cout << data << std::endl;
    json initVarsJson = data["initialVars"];
    UVariable* initVars = getInitialVars(initVarsJson);
    size_t initVarLen = initVarsJson.size();
    for (json::iterator it = data["cases"].begin(); it != data["cases"].end(); ++it) {
        const char* code = getStrCopy((*it)["code"].template get<std::string>().c_str());
        UVisit* visits = nullptr;
        size_t visitsLen = 0;
        const char* currentElement = nullptr;
        if ((*it).contains("elementId")) {
            currentElement = getStrCopy((*it)["elementId"].template get<std::string>().c_str());
        }
        else {
            currentElement = getStrCopy("TestElement");
        }
        if ((*it).contains("visits")) {
            visits = getVisits((*it)["visits"]);
            visitsLen = (*it)["visits"].size();
        }
        bool hasError = false;
        if ((*it).contains("error")) {
            hasError = true;
        }
        
        UTranspilerOutput* result = runScriptExport(code, currentElement, initVars, initVarLen, visits, visitsLen);
        if ((*it).contains("output"))
        {
            std::string output = (*it)["output"].template get<std::string>();
            if (output.compare(result->output) != 0)
            {
                std::cout << "DIFFERENT" << std::endl;
                std::cout << "EXPECTED: \"" << output << "\"" << std::endl << "ACTUAL: \"" << result->output << "\"" << std::endl;
            }
        }

        deallocateOutput(result);
        
        /*for (int i = 0; i < visitsLen; i++) {
            free((char*)visits[i].elId);
        }
        delete visits;
        free((char*)currentElement);
        free((char*)code);*/
    }
    /*for (int j = 0; j < initVarLen; j++) {
        free((char*)initVars[j].id);
        free((char*)initVars[j].name);
        if (initVars[j].type == VariableType::AW_STRING) {
            free((char*)initVars[j].string_val);
        }
    }
    delete initVars;*/
    return 0;
}



int main()
{
    const std::filesystem::path relativePath{ "..\\..\\..\\arcweave\\test\\valid.json" };

    testFile(std::filesystem::absolute(relativePath));

    system("pause");
}