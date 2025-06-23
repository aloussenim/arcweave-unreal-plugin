#include <iostream>
#include <string>
#include <cstring>
#include <any>

namespace Arcweave {

using SupportedValueType = std::variant<std::string, bool, int, double>;

class Expression {

private:
  struct NumberValues {
    double value1;
    double value2;
    bool hasDoubles = false;
  };

  static NumberValues doubleValues(SupportedValueType value1, SupportedValueType value2) ;

  static std::string valueToString(SupportedValueType value);
  static bool valueToBool(SupportedValueType value);
public:
  SupportedValueType value;
  Expression() {
    value = SupportedValueType();
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

    void setValue(std::any _value) {
        if (_value.has_value()) {
            if (std::string* pVal = std::any_cast<std::string>(&_value))
            {
                value = *pVal;
            }
            else if (bool* pVal = std::any_cast<bool>(&_value))
            {
                value = *pVal;
            }
            else if (int* pVal = std::any_cast<int>(&_value))
            {
                value = *pVal;
            }
            else if (double* pVal = std::any_cast<double>(&_value)) {
                value = *pVal;
            }
            else {
                // if RTTI is off
                    // __PRETTY_FUNCTION__ can give context for debugging
                    std::cerr << "WARNING: Arcweave::Expression::setValue(std::any) received unsupported value '";
                value = std::string(); // Set to a default value (e.g., empty string)
            }
        } else {
            value = std::string(); // Default to empty string
        }
    }

  void setValue(SupportedValueType _value) {
    value = _value;
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