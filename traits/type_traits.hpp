#pragma once

#include <string_view>

namespace Infra {
    template <typename T>
    inline constexpr std::string_view CalcName() {
        std::string_view pretty = __FUNCSIG__;
        std::string_view prefix = "class std::basic_string_view<char,struct std::char_traits<char> > __cdecl Infra::CalcName<struct ";
        std::string_view suffix = ">(void)";
        pretty.remove_prefix(prefix.size());
        pretty.remove_suffix(suffix.size());
        return pretty;
    }

    template <typename T>
    struct TTypeTraits {
        static inline constexpr std::string_view Name() {
           return CalcName<T>();
        }
    };
}