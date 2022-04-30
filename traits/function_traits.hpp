#pragma once

#include <type_traits>
#include <functional>

namespace Infra {
    enum FunctionKind {
        kNotFunction = 0,
        kFunction,
        kMethod,
        kBind,
        kStdFunction,
        kLambda,
    };

    // is T a std::function?
    template<typename T>
    struct TStdFunctionChecker : public std::false_type {
    };

    template<typename T>
    struct TStdFunctionChecker<std::function<T>> : public std::true_type {
    };

    template<typename T>
    constexpr bool is_std_function_v = TStdFunctionChecker<T>::value;

    // is T a lambda?
    template<typename T, typename = void>
    struct TCallableClassChecker : public std::false_type {
    };

    template<typename T>
    struct TCallableClassChecker<T, std::enable_if_t<std::is_same_v<decltype(void(&T::operator())), void>>>
            : public std::true_type {

    };

    template<typename T>
    constexpr bool is_callable_class_v = TCallableClassChecker<T>::value;

    // traits corresponding to enum `FunctionType`
    template<typename T, typename = void>
    struct TFunctionTraits {
        static constexpr FunctionKind kind = FunctionKind::kNotFunction;
    };

    template<typename T>
    struct TFunctionTraits<T, std::enable_if_t<std::is_function_v<T>>> {
        // in `void(int,double) like` form
        static constexpr FunctionKind kind = FunctionKind::kFunction;
    };

    template<typename T>
    struct TFunctionTraits<T, std::enable_if_t<std::is_member_function_pointer_v<T>>> {
        // method in a class
        static constexpr FunctionKind kind = FunctionKind::kMethod;
    };

    template<typename T>
    struct TFunctionTraits<T, std::enable_if_t<std::is_bind_expression_v<T>>> {
        // std::bind
        static constexpr FunctionKind kind = FunctionKind::kBind;
    };

    template<typename T>
    struct TFunctionTraits<T, std::enable_if_t<is_std_function_v<T>>> {
        // std::function
        static constexpr FunctionKind kind = FunctionKind::kStdFunction;
    };

    template<typename T>
    struct TFunctionTraits<T, std::enable_if_t<!is_std_function_v<T> && is_callable_class_v<T>>> {
        // lambda
        static constexpr FunctionKind kind = FunctionKind::kLambda;
    };
}