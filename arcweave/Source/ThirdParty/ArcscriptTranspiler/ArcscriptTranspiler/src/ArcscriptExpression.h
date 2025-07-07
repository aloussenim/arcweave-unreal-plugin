#include <iostream>
#include <string>
#include <cstring>
#include <any>
#include <variant>

#include "ArcscriptHelpers.h"

namespace Arcweave {

  // All the types that can be held in an expression.
  using ArcscriptBasicValue = std::variant <std::string, bool, int, double>;
  class Expression {
  private:
    struct NumberValues {
      double value1;
      double value2;
      bool hasDoubles = false;
    };

    static NumberValues doubleValues(ArcscriptBasicValue value1, ArcscriptBasicValue value2);

    static std::string valueToString(ArcscriptBasicValue value);
    static bool valueToBool(ArcscriptBasicValue value);
public:
  ArcscriptBasicValue value;
  Expression() {
    // empty value.
    value = ArcscriptBasicValue();
  }
  Expression(std::string _value) {
    value = _value;
  }
  Expression(bool _value) {
    value = _value;
  }
  Expression(int _value) {
    value = _value;
  }
  Expression(double _value) {
    value = _value;
  }
  Expression(const Expression &e) {
    value = e.value;
  }

  /**
    * Sets the internal value from a more general ArcscriptValue.
    * This will only perform the assignment if the incoming value holds a type
    * that ArcscriptBasicValue can also hold (string, bool, int, or double).
    */
  void setValue(const ArcscriptValue& _value) {
    std::visit([this](auto&& arg) {
      // Use a compile-time if to check if the type can be assigned
      if constexpr (std::is_assignable_v<ArcscriptBasicValue&, decltype(arg)>) {
        this->value = arg;
      }
      else {
        // Attempt to pass a type that is not supported in expressions.
      }
      }, _value);
  }

  bool setValue(const std::any& _value) {
    if (const auto* pVal = std::any_cast<std::string>(&_value)) {
      this->value = *pVal;
      return true;
    }
    if (const auto* pVal = std::any_cast<bool>(&_value)) {
      this->value = *pVal;
      return true;
    }
    if (const auto* pVal = std::any_cast<int>(&_value)) {
      this->value = *pVal;
      return true;
    }
    if (const auto* pVal = std::any_cast<double>(&_value)) {
      this->value = *pVal;
      return true;
    }

    // If none of the types match, the assignment fails.
    return false;
  }
  /**
    * Gets the value and promotes it to the more general ArcscriptValue type.
    */
  ArcscriptValue getValue() const {
    return std::visit([](auto&& arg) -> ArcscriptValue {
      return arg;
      }, value);
  }

  Expression operator+ (const Expression &other);
  Expression operator- (const Expression &other);
  Expression operator* (const Expression &other);
  Expression operator* (const int other);
  Expression operator/ (const Expression &other);
  Expression operator+= (const Expression &other);
  Expression operator-= (const Expression &other);
  Expression operator*= (const Expression &other);
  Expression operator/= (const Expression &other);

  bool operator== (const Expression &other);
  bool operator== (double other);
  bool operator== (int other);
  bool operator== (std::string other);
  bool operator== (bool other);
  bool operator!= (const Expression &other);
  bool operator!= (double other);
  bool operator!= (int other);
  bool operator!= (std::string other);
  bool operator!= (const char other[]);
  bool operator> (const Expression &other);
  bool operator> (int other);
  bool operator> (double other);
  bool operator>= (const Expression &other);
  bool operator< (const Expression &other);
  bool operator<= (const Expression &other);
  bool operator! ();
  bool operator&& (const Expression &other);
  bool operator|| (const Expression &other);
 };

}
std::ostream& operator<< (std::ostream& out, const Arcweave::Expression &e);