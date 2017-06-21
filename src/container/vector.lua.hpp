#ifndef UUID_E374ABCE_6BEB_482C_9506_4E4AE9174D22
#define UUID_E374ABCE_6BEB_482C_9506_4E4AE9174D22
#pragma once

#ifdef NEPTOOLS_WITHOUT_LUA
#define NEPTOOLS_STD_VECTOR_LUAGEN(name, ...)
#else

#include <vector>

#include "../lua/user_type.hpp"
#include "../lua/dynamic_object.hpp"

namespace Neptools::Lua
{

template <typename T, typename Allocator>
struct IsSmartObject<std::vector<T, Allocator>> : std::true_type {};

namespace Detail
{

#define NEPTOOLS_COMP_CHECK(comp, COMP, op)         \
    template <typename T, typename Enable = void>   \
    struct Is##comp##Comp : std::false_type {};     \
    template <typename T>                           \
    struct Is##comp##Comp<T, std::void_t<decltype(  \
        std::declval<T>() op std::declval<T>())>>   \
        : std::true_type {};                        \
    template <typename T>                           \
    constexpr bool IS_##COMP##_COMP = Is##comp##Comp<T>::value
NEPTOOLS_COMP_CHECK(Eq, EQ, ==);
NEPTOOLS_COMP_CHECK(Lt, LT, <);

}

template <typename T, typename Allocator = std::allocator<T>>
struct VectorBinding
{
    using Vect = std::vector<T, Allocator>;
    using size_type = typename Vect::size_type;

    static void Assign(Vect& v, const Vect& o) { v = o; }

    static RetNum Get0(StateRef vm, Vect& v, size_type i) noexcept
    {
        if (i < v.size()) vm.Push(v[i]);
        else lua_pushnil(vm);
        return 1;
    }

    static void Get1() noexcept {}

    static void Set(StateRef vm, Vect& v, size_type i, const T& val)
    {
        if (BOOST_UNLIKELY(i == std::numeric_limits<size_type>::max()))
            luaL_error(vm, "vector size overflow");
        if constexpr (std::is_default_constructible_v<T>)
        {
            if (i >= v.size()) v.resize(i+1);
            v[i] = val;
        }
        else
        {
            if (i < v.size()) v[i] = val;
            else if (i == v.size()) v.push_back(val);
            else luaL_error(vm, "set invalid index");
        }
    }

    static auto CheckedNthEnd(Vect& v, size_type i)
    {
        if (i <= v.size()) return v.begin() + i;
        else NEPTOOLS_THROW(std::out_of_range{"VectorBinding::CheckedNthEnd"});
    }

    static auto IndexOf(Vect& v, typename Vect::iterator it) noexcept
    { return it - v.begin(); }

    static auto Insert0(Vect& v, size_type pos, size_type count, const T& val)
    { return IndexOf(v, v.insert(CheckedNthEnd(v, pos), count, val)); }

    static auto Insert1(Vect& v, size_type pos, const T& val)
    { return IndexOf(v, v.insert(CheckedNthEnd(v, pos), val)); }


    static auto Erase0(StateRef vm, Vect& v, size_type s, size_type e)
    {
        if (s > e) luaL_error(vm, "Invalid range");
        return IndexOf(v, v.erase(CheckedNthEnd(v, s), CheckedNthEnd(v, e)));
    }

    static auto Erase1(Vect& v, size_type pos)
    { return IndexOf(v, v.erase(CheckedNthEnd(v, pos))); }

    // lua-compat: returns the erased element
    static T Remove(Vect& v, size_type i)
    {
        auto ret = v.at(i);
        v.erase(v.begin() + i);
        return ret;
    }

    static void PopBack(Vect& v) noexcept
    { if (!v.empty()) v.pop_back(); }

    static Lua::RetNum ToTable(Lua::StateRef vm, Vect& v)
    {
        auto size = v.size();
        lua_createtable(vm, size ? size-1 : size, 0);
        for (size_t i = 0; i < size; ++i)
        {
            vm.Push(v[i]);
            lua_rawseti(vm, -2, i);
        }
        return 1;
    }

    static void Register(TypeBuilder& bld)
    {
        if constexpr (std::is_default_constructible_v<T>)
            bld.AddFunction<
                &MakeShared<Vect, size_type, const T&>,
                &MakeShared<Vect, size_type>,
                &MakeShared<Vect, const Vect&>,
                &MakeShared<Vect>
            >("new");
        else
            bld.AddFunction<
                &MakeShared<Vect, size_type, const T&>,
                &MakeShared<Vect, const Vect&>,
                &MakeShared<Vect>
            >("new");

        bld.AddFunction<
            static_cast<void (Vect::*)(size_type, const T&)>(&Vect::assign),
            &Assign
        >("assign");

#define S(name) bld.AddFunction<&Vect::name>(#name)
        S(empty); S(size); S(max_size); S(reserve); S(capacity);
        S(shrink_to_fit); S(clear); S(swap);
#undef S
        bld.AddFunction<&Get0, &Get1>("get");
        bld.AddFunction<&Set>("set");
        bld.AddFunction<
            static_cast<T& (Vect::*)(size_t)>(&Vect::at)>("at");
        bld.AddFunction<&Vect::size>("__len");
        bld.AddFunction<&Insert0, &Insert1>("insert");
        bld.AddFunction<&Erase0, &Erase1>("erase");
        bld.AddFunction<&Remove>("remove");
        bld.AddFunction<
            static_cast<void (Vect::*)(const T&)>(&Vect::push_back)>("push_back");
        bld.AddFunction<&PopBack>("pop_back");
        if constexpr (std::is_default_constructible_v<T>)
            bld.AddFunction<
                static_cast<void (Vect::*)(size_type, const T&)>(&Vect::resize),
                static_cast<void (Vect::*)(size_type)>(&Vect::resize)
            >("resize");
        else
            bld.AddFunction<
                static_cast<void (Vect::*)(size_type, const T&)>(&Vect::resize)
            >("resize");
        bld.AddFunction<&ToTable>("to_table");

        if constexpr (Detail::IS_EQ_COMP<T>)
            bld.AddFunction<
                static_cast<bool (*)(const Vect&, const Vect&)>(&std::operator==)
            >("__eq");
        if constexpr (Detail::IS_LT_COMP<T>)
        {
            bld.AddFunction<
                static_cast<bool (*)(const Vect&, const Vect&)>(&std::operator<)
            >("__lt");
            bld.AddFunction<
                static_cast<bool (*)(const Vect&, const Vect&)>(&std::operator<=)
            >("__le");
        }

        luaL_getmetatable(bld, "neptools_ipairs");
        bld.SetField("__ipairs");
    }
};

}

#define NEPTOOLS_STD_VECTOR_LUAGEN(name, ...)                               \
    template<> void Neptools::Lua::TypeRegister::DoRegister<                \
        ::std::vector<__VA_ARGS__>>(TypeBuilder& bld)                       \
    { ::Neptools::Lua::VectorBinding<__VA_ARGS__>::Register(bld); }         \
                                                                            \
    static ::Neptools::Lua::TypeRegister::StateRegister<                    \
        ::std::vector<__VA_ARGS__>> reg_std_vector_##name;                  \
    template<> struct Neptools::Lua::TypeName<std::vector<__VA_ARGS__>>     \
    { static constexpr const char* TYPE_NAME = "neptools.vector_" #name; }

#endif
#endif
