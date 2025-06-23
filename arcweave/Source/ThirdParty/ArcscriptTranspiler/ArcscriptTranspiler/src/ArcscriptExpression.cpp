#include "ArcscriptExpression.h"
#include <sstream>

namespace Arcweave
{
    std::string Expression::valueToString(SupportedValueType value)
    {
        return std::visit([](auto &&arg) -> std::string
                          {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>)
        {
            return arg;
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return arg ? "true" : "false";
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            return std::to_string(arg);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            std::stringstream ss;
            ss << arg;
            return ss.str();
        }
        else 
        {
            // Handle types not covered by above, if your variant can contain them
            throw std::runtime_error("Unhandled type in Expression::valueToString");
        } }, value);
    }

    Expression::NumberValues Expression::doubleValues(SupportedValueType value1, SupportedValueType value2)
    {
        Expression::NumberValues returnVal;
        returnVal.hasDoubles = false;

        std::visit([&](auto &&arg)
                   {
                       using T = std::decay_t<decltype(arg)>;
                       if constexpr (std::is_same_v<T, int>)
                       {
                           returnVal.value1 = static_cast<double>(arg);
                       }
                       else if constexpr (std::is_same_v<T, double>)
                       {
                           returnVal.value1 = arg;
                           returnVal.hasDoubles = true;
                       }
                       else if constexpr (std::is_same_v<T, bool>)
                       {
                           returnVal.value1 = arg ? 1.0 : 0.0;
                       }
                       // Add more 'else if constexpr' for other types in your variant
                       // Make sure all types in SupportedValueType are handled here
                   },
                   value1);

        std::visit([&](auto &&arg)
                   {
                       using T = std::decay_t<decltype(arg)>;
                       if constexpr (std::is_same_v<T, int>)
                       {
                           returnVal.value2 = static_cast<double>(arg);
                       }
                       else if constexpr (std::is_same_v<T, double>)
                       {
                           returnVal.value2 = arg;
                           returnVal.hasDoubles = true;
                       }
                       else if constexpr (std::is_same_v<T, bool>)
                       {
                           returnVal.value2 = arg ? 1.0 : 0.0;
                       }
                       // Add more 'else if constexpr' for other types in your variant
                   },
                   value2);

        return returnVal;
    }

    bool Expression::valueToBool(SupportedValueType value)
    {
        return std::visit([](auto &&arg) -> bool
                          {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
            {
                return (arg != 0);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return (std::fabs(arg) > 0.0000001);
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return !arg.empty();
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return arg;
            }
            else {
                // Unhandled type, default to false or throw error
                throw std::runtime_error("Unhandled type in Expression::valueToBool");
            } }, value);
    }

    Expression Expression::operator+(const Expression &other)
    {
        if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(other.value))
        {
            return Expression(std::get<std::string>(value) + std::get<std::string>(other.value));
        }
        else
        {
            NumberValues values = doubleValues(value, other.value);
            if (!values.hasDoubles)
            {
                return Expression(static_cast<int>(values.value1 + values.value2));
            }
            else
            {
                return Expression(values.value1 + values.value2);
            }
        }
    }

    Expression Expression::operator-(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        if (!values.hasDoubles)
        {
            return Expression(static_cast<int>(values.value1 - values.value2));
        }
        else
        {
            return Expression(values.value1 - values.value2);
        }
    }

    Expression Expression::operator*(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        if (!values.hasDoubles)
        {
            return Expression(static_cast<int>(values.value1 * values.value2));
        }
        else
        {
            return Expression(values.value1 * values.value2);
        }
    }

    Expression Expression::operator*(const int other)
    {
        NumberValues values = doubleValues(value, other); // Implicit conversion of int to SupportedValueType for doubleValues
        if (!values.hasDoubles)
        {
            return Expression(static_cast<int>(values.value1 * values.value2));
        }
        else
        {
            return Expression(values.value1 * values.value2);
        }
    }

    Expression Expression::operator/(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        if (values.value2 == 0.0)
        {
            // Handle division by zero, e.g., return a default value or throw an exception
            std::cerr << "Error: Division by zero!" << std::endl;
            return Expression(0.0);
        }
        if (!values.hasDoubles)
        {
            return Expression(static_cast<int>(values.value1 / values.value2));
        }
        else
        {
            return Expression(values.value1 / values.value2);
        }
    }

    Expression Expression::operator+=(const Expression &other)
    {
        if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(other.value))
        {
            value = std::get<std::string>(value) + std::get<std::string>(other.value);
        }
        else
        {
            NumberValues values = doubleValues(value, other.value);
            if (!values.hasDoubles)
            {
                value = static_cast<int>(values.value1 + values.value2);
            }
            else
            {
                value = values.value1 + values.value2;
            }
        }
        return *this;
    }

    Expression Expression::operator-=(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        if (!values.hasDoubles)
        {
            value = static_cast<int>(values.value1 - values.value2);
        }
        else
        {
            value = values.value1 - values.value2;
        }
        return *this;
    }

    Expression Expression::operator*=(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        if (!values.hasDoubles)
        {
            value = static_cast<int>(values.value1 * values.value2);
        }
        else
        {
            value = values.value1 * values.value2;
        }
        return *this;
    }

    Expression Expression::operator/=(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        if (values.value2 == 0.0)
        {
            // Handle division by zero
            std::cerr << "Error: Division by zero in operator/=!" << std::endl;
            value = 0.0; // Set to a default or error value
        }
        else if (!values.hasDoubles)
        {
            value = static_cast<int>(values.value1 / values.value2);
        }
        else
        {
            value = values.value1 / values.value2;
        }
        return *this;
    }

    bool Expression::operator==(const Expression &other)
    {
        return std::visit([&](auto &&arg1, auto &&arg2) -> bool
                          {
            using T1 = std::decay_t<decltype(arg1)>;
            using T2 = std::decay_t<decltype(arg2)>;

            if constexpr (std::is_same_v<T1, std::string> || std::is_same_v<T2, std::string>) {
                return valueToString(value) == valueToString(other.value);
            } else {
                NumberValues values = doubleValues(value, other.value);
                // Compare doubles with a small epsilon for floating point accuracy
                return std::fabs(values.value1 - values.value2) < 0.0000001;
            } }, value, other.value);
    }

    bool Expression::operator==(double other)
    {
        NumberValues values = doubleValues(value, other);
        return std::fabs(values.value1 - values.value2) < 0.0000001;
    }

    bool Expression::operator==(int other)
    {
        NumberValues values = doubleValues(value, other);
        return std::fabs(values.value1 - values.value2) < 0.0000001;
    }

    bool Expression::operator==(std::string other)
    {
        return valueToString(value) == other;
    }

    bool Expression::operator==(bool other)
    {
        return valueToBool(value) == other;
    }

    bool Expression::operator!=(const Expression &other)
    {
        return !(*this == other);
    }

    bool Expression::operator!=(double other)
    {
        return !(*this == other);
    }

    bool Expression::operator!=(int other)
    {
        return !(*this == other);
    }

    bool Expression::operator!=(std::string other)
    {
        return !(*this == other);
    }

    bool Expression::operator!=(const char other[])
    {
        return !(*this == std::string(other));
    }

    bool Expression::operator>(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        return values.value1 > values.value2;
    }

    bool Expression::operator>(int other)
    {
        NumberValues values = doubleValues(value, other);
        return values.value1 > values.value2;
    }

    bool Expression::operator>(double other)
    {
        NumberValues values = doubleValues(value, other);
        return values.value1 > values.value2;
    }

    bool Expression::operator>=(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        return values.value1 >= values.value2;
    }

    bool Expression::operator<(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        return values.value1 < values.value2;
    }

    bool Expression::operator<=(const Expression &other)
    {
        NumberValues values = doubleValues(value, other.value);
        return values.value1 <= values.value2;
    }

    bool Expression::operator!()
    {
        return !valueToBool(value);
    }

    bool Expression::operator&&(const Expression &other)
    {
        return valueToBool(value) && valueToBool(other.value);
    }

    bool Expression::operator||(const Expression &other)
    {
        return valueToBool(value) || valueToBool(other.value);
    }
}

std::ostream &operator<<(std::ostream &out, const Arcweave::Expression &e)
{
    std::visit([&](auto &&arg)
               {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>)
        {
            out << arg;
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            out << (arg ? "true" : "false");
        }
        else if constexpr (std::is_same_v<T, int>)
        {
            out << arg;
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            out << arg;
        }
        else
        {
            // Fallback for unhandled types
            out << "[Unknown Type]";
        } }, e.value);
    return out;
}
