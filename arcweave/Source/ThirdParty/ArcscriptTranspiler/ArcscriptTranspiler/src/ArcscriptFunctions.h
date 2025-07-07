#pragma once

#include "ArcscriptHelpers.h"
#include <typeindex>
#include <iostream>
#include <functional>

namespace Arcweave {
class ArcscriptFunctions {
private:
  ArcscriptState *_state;

public:
  struct FunctionInfo {
    int minArgs;
    int maxArgs;
  };

  static std::map<std::string, FunctionInfo> functions;

  ArcscriptFunctions(ArcscriptState* state) : _state(state) {
    
  }

  ArcscriptValue Call(std::string functionName, const std::vector<std::any>& _args);

  ArcscriptValue Abs(const std::vector<ArcscriptValue> &args);
  ArcscriptValue Max(const std::vector<ArcscriptValue> &args);
  ArcscriptValue Min(const std::vector<ArcscriptValue> &args);
  ArcscriptValue Random(const std::vector<ArcscriptValue> &args);
  void Reset(const std::vector<ArcscriptValue> &args);
  void ResetAll(const std::vector<ArcscriptValue> &args);
  ArcscriptValue Roll(const std::vector<ArcscriptValue> &args);
  ArcscriptValue Round(const std::vector<ArcscriptValue> &args);
  ArcscriptValue Show(const std::vector<ArcscriptValue> &args);
  ArcscriptValue Sqr(const std::vector<ArcscriptValue> &args);
  ArcscriptValue Sqrt(const std::vector<ArcscriptValue> &args);
  ArcscriptValue Visits(const std::vector<ArcscriptValue> &args);
};
}