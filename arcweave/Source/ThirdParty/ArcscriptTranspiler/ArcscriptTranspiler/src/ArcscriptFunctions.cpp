#include "ArcscriptFunctions.h"
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <variant>

#include "ArcscriptExpression.h"
#include "ArcscriptHelpers.h"

namespace Arcweave {
    std::map<std::string, ArcscriptFunctions::FunctionInfo> ArcscriptFunctions::functions = {
      { "abs", { 1, 1 } },
      { "max", { 2, -1 }},
      { "min", { 2, -1 }},
      { "random", { 0, 0 }},
      { "reset", { 1, -1 }},
      { "resetAll", { 0, -1} },
      { "roll", { 1, 2 } },
      { "round", { 1, 1} },
      { "show", { 1, -1 } },
      { "sqr", { 1, 1 } },
      { "sqrt", { 1, 1 } },
      { "visits", { 0, 1 } },
    };

    ArcscriptValue ArcscriptFunctions::Call(std::string functionName, const std::vector<std::any> &_args) {
        std::vector<ArcscriptValue> args;
        args.reserve(_args.size());

        for (const auto& arg : _args) {
            auto arcscriptArg = anyToArcscriptValue(arg);
            // Use the copy-based helper to safely unwrap Expressions
            if (auto optionalExp = getArcscriptValue<Expression>(arcscriptArg)) {
                args.push_back(optionalExp->getValue());
            }
            else {
                args.push_back(arcscriptArg);
            }
        }

        // The rest of the function remains the same, dispatching to the correct method.
        ArcscriptValue result;
        if (functionName == "sqrt") {
            result = this->Sqrt(args);
        }
        else if (functionName == "sqr") {
            result = this->Sqr(args);
        }
        else if (functionName == "abs") {
            result = this->Abs(args);
        }
        else if (functionName == "random") {
            result = this->Random(args);
        }
        else if (functionName == "roll") {
            result = this->Roll(args);
        }
        else if (functionName == "show") {
            result = this->Show(args);
        }
        else if (functionName == "reset") {
            this->Reset(args);
        }
        else if (functionName == "resetAll") {
            this->ResetAll(args);
        }
        else if (functionName == "round") {
            result = this->Round(args);
        }
        else if (functionName == "min") {
            result = this->Min(args);
        }
        else if (functionName == "max") {
            result = this->Max(args);
        }
        else if (functionName == "visits") {
            result = this->Visits(args);
        }
        return result;
    }

    ArcscriptValue ArcscriptFunctions::Sqrt(const std::vector<ArcscriptValue>& args) {
        if (auto p_int = getArcscriptValue<int>(args[0])) {
            return sqrt(static_cast<double>(*p_int));
        }
        if (auto p_double = getArcscriptValue<double>(args[0])) {
            return sqrt(*p_double);
        }
        return {}; // Return default on error
    }

    ArcscriptValue ArcscriptFunctions::Sqr(const std::vector<ArcscriptValue>& args) {
        if (auto p_int = getArcscriptValue<int>(args[0])) {
            return (*p_int) * (*p_int);
        }
        if (auto p_double = getArcscriptValue<double>(args[0])) {
            return (*p_double) * (*p_double);
        }
        return {}; // Return default on error
    }

    ArcscriptValue ArcscriptFunctions::Abs(const std::vector<ArcscriptValue>& args) {
        if (auto p_int = getArcscriptValue<int>(args[0])) {
            return abs(*p_int);
        }
        if (auto p_double = getArcscriptValue<double>(args[0])) {
            return abs(*p_double);
        }
        return {}; // Return default on error
    }

    ArcscriptValue ArcscriptFunctions::Random(const std::vector<ArcscriptValue>& args) {
        srand(time(NULL));
        return ((double)rand() / (RAND_MAX));
    }

    ArcscriptValue ArcscriptFunctions::Roll(const std::vector<ArcscriptValue>& args) {
        int maxRoll = 0;
        int numRolls = 1;

        if (auto p_max = getArcscriptValue<int>(args[0])) {
            maxRoll = *p_max;
        }
        else { return {}; } // Invalid first argument

        if (args.size() == 2) {
            if (auto p_rolls = getArcscriptValue<int>(args[1])) {
                numRolls = *p_rolls;
            }
            else { return {}; } // Invalid second argument
        }

        int sum = 0;
        for (int i = 0; i < numRolls; i++) {
            int oneRoll = rand() % maxRoll + 1;
            sum += oneRoll;
        }
        return sum;
    }

    ArcscriptValue ArcscriptFunctions::Show(const std::vector<ArcscriptValue>& args) {
        std::string result;
        for (const auto& arg : args) {
            // std::visit is the cleanest way to handle multiple primitive types
            std::visit([&result](auto&& val) {
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    result += val;
                }
                else if constexpr (std::is_same_v<T, bool>) {
                    result += (val ? "true" : "false");
                }
                else if constexpr (std::is_same_v<T, int> || std::is_same_v<T, double>) {
                    result += std::to_string(val);
                }
                // Other complex types will be ignored, matching original logic
                }, arg);
        }
        _state->outputs.AddScriptOutput(result);
        return {};
    }

    ArcscriptValue ArcscriptFunctions::Round(const std::vector<ArcscriptValue>& args) {
        if (auto p_int = getArcscriptValue<int>(args[0])) {
            return round(static_cast<double>(*p_int));
        }
        if (auto p_double = getArcscriptValue<double>(args[0])) {
            return round(*p_double);
        }
        return {}; // Return default on error
    }

    ArcscriptValue ArcscriptFunctions::Min(const std::vector<ArcscriptValue>& args) {
        std::vector<double> casted;
        for (const auto& arg : args) {
            if (auto p_int = getArcscriptValue<int>(arg)) {
                casted.push_back(*p_int);
            }
            else if (auto p_double = getArcscriptValue<double>(arg)) {
                casted.push_back(*p_double);
            }
        }
        if (casted.empty()) return {};
        return *min_element(casted.begin(), casted.end());
    }

    ArcscriptValue ArcscriptFunctions::Max(const std::vector<ArcscriptValue>& args) {
        std::vector<double> casted;
        for (const auto& arg : args) {
            if (auto p_int = getArcscriptValue<int>(arg)) {
                casted.push_back(*p_int);
            }
            else if (auto p_double = getArcscriptValue<double>(arg)) {
                casted.push_back(*p_double);
            }
        }
        if (casted.empty()) return {};
        return *max_element(casted.begin(), casted.end());
    }

    void ArcscriptFunctions::Reset(const std::vector<ArcscriptValue>& args) {
        std::vector<Variable> variables;
        for (const auto& arg : args) {
            // Use the copy-based helper to safely get a Variable object
            if (auto var_opt = getArcscriptValue<Variable>(arg)) {
                variables.push_back(*var_opt);
            }
        }
        _state->resetVars(variables);
    }

    void ArcscriptFunctions::ResetAll(const std::vector<ArcscriptValue>& args) {
        std::vector<Variable> except;
        for (const auto& arg : args) {
            // Use the copy-based helper to safely get a Variable object
            if (auto var_opt = getArcscriptValue<Variable>(arg)) {
                except.push_back(*var_opt);
            }
        }
        _state->resetAllVars(except);
    }

    ArcscriptValue ArcscriptFunctions::Visits(const std::vector<ArcscriptValue>& args) {
        std::string nodeId = _state->currentElement;
        if (!args.empty()) {
            // Use the pointer-based helper to safely check for a Mention
            if (auto p_mention = getArcscriptValue<Mention>(args[0])) {
                if (p_mention->attrs.count("data-id")) {
                    nodeId = p_mention->attrs.at("data-id");
                }
            }
        }
        return _state->visits[nodeId];
    }
}