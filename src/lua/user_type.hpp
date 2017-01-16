#ifndef UUID_365580B3_AF64_4E79_8FC1_35F50DFF840F
#define UUID_365580B3_AF64_4E79_8FC1_35F50DFF840F
#pragma once

#include "dynamic_object.hpp"
#include "function_call.hpp"
#include "type_traits.hpp"
#include "../shared_ptr.hpp"
#include <type_traits>

namespace Neptools
{

template <typename T> class NotNull;

namespace Lua
{

// When specifying T as template argument for vararg &&... argument,
// std::forward will try to convert it to T&&. If TypeTraits::Get returns a T&,
// it'll fail, even though normally it'd copy it. Thus we need to specify T& in
// this case (or const T&).

namespace Detail
{
template <typename T, typename = void> struct LuaGetRefHlp;
template <typename T>
struct LuaGetRefHlp<T, std::enable_if_t<std::is_reference<T>::value>>
{ using Type = T; };

template <typename T>
struct LuaGetRefHlp<T, std::enable_if_t<!std::is_reference<T>::value>>
{
    using Type = std::conditional_t<
        !std::is_reference<
            decltype(TypeTraits<T>::Get(std::declval<StateRef>(), false, 0))>::value ||
        std::is_rvalue_reference<
            decltype(TypeTraits<T>::Get(std::declval<StateRef>(), false, 0))>::value,
        T, T&>;
};
}

template <typename T>
using LuaGetRef = typename Detail::LuaGetRefHlp<T>::Type;

namespace Detail
{
template <typename Class, typename T, T Class::* Member, typename Enable = void>
struct GetMemberHlp
{
    inline static T& Get(Class& cls) { return cls.*Member; }
};

template <typename Class, typename T, T Class::* Member>
struct GetMemberHlp<Class, T, Member, std::enable_if_t<
    std::is_base_of_v<RefCounted, Class> &&
    is_normal_smart_object_v<T>>>
{
    inline static NotNull<SharedPtr<T>> Get(Class& cls)
    { return NotNull<SharedPtr<T>>{&cls, &(cls.*Member), true}; }
};
}

template <typename Class, typename T, T Class::* Member>
decltype(auto) GetMember(Class& cls)
{ return Detail::GetMemberHlp<Class, T, Member>::Get(cls); }

template <typename Class, typename T, T Class::* member>
void SetMember(Class& cls, const T& val) { cls.*member = val; }

class TypeBuilder
{
public:
    TypeBuilder(StateRef vm, void* type_tag, const char* name);
    template <typename T>
    void Init()
    {
        using UT = UserTypeTraits<T>;
        UT::MetatableCreate(vm);
        Add<decltype(&UT::GcFun), &UT::GcFun>("__gc");
    }

    void Done();

    template <typename Deriv, typename... Base>
    void Inherit() { InheritHelp<Deriv, Base...>::Do(*this); }

    template <typename T, T fun>
    void Add(const char* name)
    {
        vm.Push<T, fun>();
        SetField(name);
    }

    template <typename... Args>
    void Add(const char* name)
    {
        vm.Push<Args...>();
        SetField(name);
    }

    template <typename T>
    void Add(const char* name, T&& t)
    {
        vm.Push(std::forward<T>(t));
        SetField(name);
    }

    // low-level, pops value from lua stack
    void SetField(const char* name);

private:
    template <typename Deriv, typename... Base>
    struct InheritHelp;

    void DoInherit(ptrdiff_t offs);

    StateRef vm;
    bool has_get_ = false, has_get = false, has_set_ = false, has_set = false;
};

class TypeRegister
{
public:
    template <typename Class>
    static void Register(StateRef vm)
    {
        NEPTOOLS_LUA_GETTOP(vm, top);

        void* type_tag = &TYPE_TAG<Class>;
        lua_rawgetp(vm, LUA_REGISTRYINDEX, type_tag);
        if (lua_isnil(vm, -1))
        {
            lua_pop(vm, 1);
            TypeBuilder bld{vm, type_tag, TYPE_NAME<Class>};
            bld.Init<Class>();
            DoRegister<Class>(vm, bld);
            bld.Done();
        }

        NEPTOOLS_LUA_CHECKTOP(vm, top+1);
    }

    template <typename... Args>
    static void MultiRegister(StateRef vm)
    {
        using Swallow = int[];
        (void) Swallow{ 0, (Register<Args>(vm), lua_pop(vm, 1), 0)... };
    }

    template <typename... Classes>
    struct StateRegister : State::Register
    {
        StateRegister() : Register{&TypeRegister::MultiRegister<Classes...>} {}
    };

private:
    template <typename Class>
    static void DoRegister(StateRef vm, TypeBuilder& bld);
};

template <typename Deriv>
struct TypeBuilder::InheritHelp<Deriv>
{ static void Do(TypeBuilder&) {} };

template <typename Deriv, typename Head, typename... Rest>
struct TypeBuilder::InheritHelp<Deriv, Head, Rest...>
{
    static void Do(TypeBuilder& bld)
    {
        TypeRegister::Register<Head>(bld.vm);
        bld.DoInherit(reinterpret_cast<ptrdiff_t>(static_cast<Head*>(static_cast<Deriv*>(nullptr))));
        InheritHelp<Deriv, Rest...>::Do(bld);
    }
};


}
}

#endif
