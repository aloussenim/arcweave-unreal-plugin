#pragma once

#include <iostream>
#include <any>
#include <string>
#include <cstring>
#include <map>
#include <vector>
#include <set>
#include <variant>
#include <memory>
#include "ArcscriptErrorExceptions.h"
#include "ArcscriptOutputs.h"
#include <optional>

namespace Arcweave {

    class Expression; // Forward definition of the class defined in ArcscriptExpression.h

    enum VariableType {
        AW_STRING,
        AW_INTEGER,
        AW_DOUBLE,
        AW_BOOLEAN,
        AW_ANY
    };
    class Mention {
    public:
        std::string label;
        std::map<std::string, std::string> attrs;
        Mention(std::string _label, std::map<std::string, std::string> _attrs) {
            label = _label;
            attrs = _attrs;
        }
        Mention(const Mention& m) {
            label = m.label;
            attrs = m.attrs;
        }
    };

    // All Arcscript value types, with possible recursions stored in shared ptrs.
    using ArcscriptValue = std::variant<std::monostate, std::string, bool, int, double,
        std::shared_ptr<struct Variable>,
        std::shared_ptr<Mention>,
        std::shared_ptr<struct ConditionalSection>,
        std::shared_ptr<Expression>
    >;

    struct Variable {
        std::string id;
        std::string name;
        VariableType type;
        ArcscriptValue value;
    };

    struct ConditionalSection {
        bool clause;
        ArcscriptValue script;
    };

    class ArcscriptState {
    public:
        std::map<std::string, ArcscriptValue> variableChanges;
        std::map<std::string, Variable> variableValues;
        std::map<std::string, std::string> varNameToID;
        ArcscriptOutputs outputs;
        std::string currentElement;
        std::map<std::string, int> visits;

        ArcscriptState(std::string elementId, std::map<std::string, Variable> varValues, std::map<std::string, int> _visits) {
            currentElement = elementId;
            variableValues = varValues;
            for (const auto var : variableValues) {
                varNameToID[var.second.name] = var.first;
            }
            visits = _visits;
        };

        inline Variable getVar(std::string name) {
            std::string varId = varNameToID[name];
            return variableValues[varId];
        }

        inline ArcscriptValue getVarValue(const std::string &name) {
            std::string varId = varNameToID[name];
            if(variableChanges.count(varId)) {
                return variableChanges.at(varId);
            }
            return variableValues[varId].value;
        }
        inline VariableType getVarType(std::string name) {
            return variableValues[varNameToID[name]].type;
        }
        inline void setVarValue(const std::string &name, ArcscriptValue value) {
            std::string varId = varNameToID[name];
            variableChanges[varId] = value;
        }
        inline void setVarValues(std::vector<std::string> names, const std::vector<ArcscriptValue> &values) {
            for (int i = 0; i < names.size(); i++) {
                variableChanges[names[i]] = values[i];
            }
        }

        inline void resetVars(const std::vector<Variable> &vars) {
            for (const Variable &var : vars) {
                variableChanges[var.id] = var.value;
            }
        }

        inline void resetAllVars(std::vector<Variable> except) {
            std::set<std::string> exceptVariableIds;
            for (const Variable &var : except) {
                exceptVariableIds.insert(var.id);
            }
            std::map<std::string, Variable>::iterator it = variableValues.begin();
            while (it != variableValues.end())
            {
                if (exceptVariableIds.find(it->first) == exceptVariableIds.end()) { // not in except vars
                    variableChanges[it->first] = it->second.value;
                }
                it++;
            }
        }
    };

    /*
    * Helper method: Converts any values to ArcscriptValue without RTTI.
    **/
    ArcscriptValue anyToArcscriptValue(const std::any& anyValue);
    
    // This function takes any ArcscriptValue and returns a std::any containing the same value. This is to avoid issues with all
    // the std::any usage that comes with using antlr4
    std::any arcscriptValueToAny(const ArcscriptValue& value);

    /**
     * Generic function to copy and assign a char pointer.
     *
     * This is a workaround for strdup and other methods deemed unsafe by the compiler.
     *
     */
    char* copyAndAssignCharPointer(const std::string& source);

    /**
     * @brief Safely gets a copy of a value of type T from an ArcscriptValue.
     *
     * This function handles both direct values (like std::string) and values
     * managed by a std::shared_ptr (like Expression or Variable).
     *
     * @tparam T The type of the value to extract (e.g., Expression, std::string).
     * @param value The ArcscriptValue variant to inspect.
     * @return An std::optional<T> containing a copy of the object if found, otherwise std::nullopt.
     *
     * @example
     * ArcscriptValue val1 = std::make_shared<Expression>();
     * if (auto expr_opt = get_value_copy<Expression>(val1)) {
     * Expression expr = *expr_opt; // expr is a copy
     * }
     */
    template <typename T, typename Variant>
    struct is_variant_alternative;

    template <typename T, typename... Types>
    struct is_variant_alternative<T, std::variant<Types...>>
        : std::disjunction<std::is_same<T, Types>...> {};

    template <typename T>
    std::optional<T> getArcscriptValue(const ArcscriptValue& value) {
        // Version 1: For types managed by a shared_ptr (e.g., Expression).
        if constexpr (is_variant_alternative<std::shared_ptr<T>, ArcscriptValue>::value) {
            if (auto p_ptr = std::get_if<std::shared_ptr<T>>(&value)) {
                // If the shared_ptr exists and is not null, return a copy of the object it points to.
                if (p_ptr && *p_ptr) {
                    return **p_ptr; // Dereference twice to get the object and copy it
                }
            }
        }
        // Version 2: For types stored directly (e.g., std::string).
        else if constexpr (is_variant_alternative<T, ArcscriptValue>::value) {
            if (auto p_val = std::get_if<T>(&value)) {
                // If the value exists, return a copy of it.
                return *p_val;
            }
        }

        // If the type wasn't found in any form, return an empty optional.
        return std::nullopt;
    }

}