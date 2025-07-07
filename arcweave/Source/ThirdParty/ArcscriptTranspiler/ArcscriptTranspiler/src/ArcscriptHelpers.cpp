#include "ArcscriptHelpers.h"

// Include the full definitions of classes needed for copying.
// This is the key to fixing the error.
#include "ArcscriptExpression.h" 

#include <cstring> // For std::memcpy

namespace Arcweave {

  ArcscriptValue anyToArcscriptValue(const std::any& anyValue) {
    if (!anyValue.has_value()) {
      return std::monostate{};
    }

    // Using std::any_cast directly, as this function's body is in the .cpp file
    // where the full definitions for Expression, etc., are known.
    if (const auto* pVal = std::any_cast<std::monostate>(&anyValue)) {
      return *pVal;
    }
    if (const auto* pVal = std::any_cast<std::string>(&anyValue)) {
      return *pVal;
    }
    if (const auto* pVal = std::any_cast<bool>(&anyValue)) {
      return *pVal;
    }
    if (const auto* pVal = std::any_cast<int>(&anyValue)) {
      return *pVal;
    }
    if (const auto* pVal = std::any_cast<double>(&anyValue)) {
      return *pVal;
    }
    if (const auto* pVal = std::any_cast<Variable>(&anyValue)) {
      return std::make_shared<Variable>(*pVal);
    }
    if (const auto* pVal = std::any_cast<Mention>(&anyValue)) {
      return std::make_shared<Mention>(*pVal);
    }
    if (const auto* pVal = std::any_cast<Expression>(&anyValue)) {
      return std::make_shared<Expression>(*pVal);
    }
    if (const auto* pVal = std::any_cast<ConditionalSection>(&anyValue)) {
      return std::make_shared<ConditionalSection>(*pVal);
    }

    // Fallback for any unhandled types
    return std::monostate{};
  }

  std::any arcscriptValueToAny(const ArcscriptValue& value) {
    return std::visit([](auto&& arg) -> std::any {
      return std::any(arg);
      }, value);
  }

  char* copyAndAssignCharPointer(const std::string& source)
  {
    const size_t bufferSize = source.size() + 1;
    std::unique_ptr<char[]> myBuffer = std::make_unique<char[]>(bufferSize);
    std::memcpy(myBuffer.get(), source.c_str(), bufferSize);
    return myBuffer.release();
  }

} // namespace Arcweave