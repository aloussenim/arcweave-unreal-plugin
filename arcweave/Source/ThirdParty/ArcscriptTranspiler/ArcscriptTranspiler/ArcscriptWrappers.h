#pragma once
#include "ArcscriptTranspiler.h"
/**
* Mainly created to respect c++20 standard in a friendly cross-platform way. These wrappers are a simple memory management tool
* To Receive U* structures and handle memory management by leveraging stl.
 */
namespace Arcweave {
	/**
	* T classes are wrappers to allow better memory management for U structures.
	* This allows for a centralized memory management.
	*/
    template <typename T>
    class UStructWrapper {
    public:
        UStructWrapper() = default;

        const T& getStruct() {
            return u_struct;
        }

    protected:
        mutable T u_struct;

        virtual void populateUStruct(T& _u_struct) const = 0;

        const char* getStringCStr(const std::string& s) const {
            return s.empty() ? nullptr : s.c_str();
        }

        char* getMutableStringCStr(const std::string& s) const {
            return s.empty() ? nullptr : const_cast<char*>(s.c_str());
        }

    private:
        UStructWrapper(const UStructWrapper&) = delete;
        UStructWrapper& operator=(const UStructWrapper&) = delete;
        UStructWrapper(UStructWrapper&&) noexcept = default;
        UStructWrapper& operator=(UStructWrapper&&) noexcept = default;
    };

    class UVariableChangeWrapper : public UStructWrapper<UVariableChange> {
    private:
        std::string varId_str;
        std::string string_result_str;

    public:
        UVariableChangeWrapper(
            const std::string& varId,
            VariableType type,
            int int_result = 0,
            double double_result = 0.0,
            const std::string& string_result = "",
            bool bool_result = false)
            : varId_str(varId),
            string_result_str(string_result) {

            u_struct.type = type;
            u_struct.int_result = int_result;
            u_struct.double_result = double_result;
            u_struct.bool_result = bool_result;
        }

    protected:
        void populateUStruct(UVariableChange& _u_struct) const override {
            _u_struct.varId = getMutableStringCStr(varId_str);
            _u_struct.type = u_struct.type;
            _u_struct.int_result = u_struct.int_result;
            _u_struct.double_result = u_struct.double_result;
            _u_struct.string_result = getMutableStringCStr(string_result_str);
            _u_struct.bool_result = u_struct.bool_result;
        }

    public:
        std::string getVarId() const { return varId_str; }
        void setVarId(const std::string& id) { varId_str = id; }
    };

    class UVariableWrapper : public UStructWrapper<UVariable> {
    private:
        std::string id_str;
        std::string name_str;
        std::string string_val_str;

    public:
        UVariableWrapper(
            const std::string& id,
            const std::string& name,
            const std::string& var_type,
            const std::string& string_val
            )
            : id_str(id), name_str(name), string_val_str(string_val) {
            intiValues(var_type, string_val);
        }

    protected:
        void populateUStruct(UVariable& _u_struct) const override {
            _u_struct.id = getStringCStr(id_str);
            _u_struct.name = getStringCStr(name_str);
            _u_struct.type = u_struct.type;
            _u_struct.int_val = u_struct.int_val;
            _u_struct.double_val = u_struct.double_val;
            _u_struct.string_val = getStringCStr(string_val_str);
            _u_struct.bool_val = u_struct.bool_val;
        }
    private:
        void intiValues(const std::string& var_type, const std::string& string_val) {
            if (var_type == "string") {
                u_struct.string_val = getStringCStr(string_val);
                u_struct.type = VariableType::AW_STRING;
            }
            else if (var_type == "integer") {
                u_struct.type = VariableType::AW_INTEGER;
                u_struct.int_val = std::stoi(string_val);
            }
            else if (var_type == "double") {
                u_struct.type = VariableType::AW_DOUBLE;
                u_struct.double_val = std::stod(string_val);
            }
            else if (var_type == "boolean") {
                u_struct.type = VariableType::AW_BOOLEAN;
                u_struct.bool_val = (string_val == "true");
            }
            else {
                u_struct.type = VariableType::AW_ANY;
            }
        }
    };

    class UVisitWrapper : public UStructWrapper<UVisit> {
    private:
        std::string elId_str;

    public:
        UVisitWrapper(
            const std::string& elId,
            int visits = 0)
            : elId_str(elId) {
            u_struct.visits = visits;
        }

        std::string getElId() const { return elId_str; }
        void setElId(const std::string& id) { elId_str = id; }
        int getVisits() const { return u_struct.visits; }
        void setVisits(int v) { u_struct.visits = v; }

    protected:
        void populateUStruct(UVisit& _u_struct) const override {
            _u_struct.elId = getStringCStr(elId_str);
            _u_struct.visits = u_struct.visits;
        }
    };

};


