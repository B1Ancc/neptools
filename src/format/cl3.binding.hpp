// Auto generated code, do not edit. See gen_binding in project root.
#include "lua/user_type.hpp"

namespace Neptools
{
namespace Lua
{

// class neptools.cl3
template<>
void TypeRegister::DoRegister<Neptools::Cl3>(StateRef vm, TypeBuilder& bld)
{
    (void) vm;
    bld.Inherit<Neptools::Cl3, Neptools::Dumpable>();

    bld.Add<
        Overload<decltype(&::Neptools::MakeSmart<Neptools::Cl3>), &::Neptools::MakeSmart<Neptools::Cl3>>,
        Overload<decltype(&::Neptools::MakeSmart<Neptools::Cl3, LuaGetRef<Neptools::Source>>), &::Neptools::MakeSmart<Neptools::Cl3, LuaGetRef<Neptools::Source>>>
    >("new");
    bld.Add<
        decltype(&::Neptools::Lua::GetMember<Neptools::Cl3, uint32_t, &Neptools::Cl3::field_14>), &::Neptools::Lua::GetMember<Neptools::Cl3, uint32_t, &Neptools::Cl3::field_14>
    >("get_field_14");
    bld.Add<
        decltype(&::Neptools::Lua::SetMember<Neptools::Cl3, uint32_t, &Neptools::Cl3::field_14>), &::Neptools::Lua::SetMember<Neptools::Cl3, uint32_t, &Neptools::Cl3::field_14>
    >("set_field_14");
    bld.Add<
        uint32_t (Neptools::Cl3::*)(const WeakSmartPtr<Neptools::Cl3::Entry> &) const, &Neptools::Cl3::IndexOf
    >("index_of");
    bld.Add<
        Neptools::Cl3::Entry & (Neptools::Cl3::*)(StringView), &Neptools::Cl3::GetOrCreateFile
    >("get_or_create_file");
    bld.Add<
        void (Neptools::Cl3::*)(const boost::filesystem::path &) const, &Neptools::Cl3::ExtractTo
    >("extract_to");
    bld.Add<
        void (Neptools::Cl3::*)(const boost::filesystem::path &), &Neptools::Cl3::UpdateFromDir
    >("update_from_dir");
    bld.Add<
        Stcm::File & (Neptools::Cl3::*)(), &Neptools::Cl3::GetStcm
    >("get_stcm");

}
static TypeRegister::StateRegister<Neptools::Cl3> reg_neptools_cl3;

}
}

const char Neptools::Cl3::TYPE_NAME[] = "neptools.cl3";

namespace Neptools
{
namespace Lua
{

// class neptools.cl3.entry
template<>
void TypeRegister::DoRegister<Neptools::Cl3::Entry>(StateRef vm, TypeBuilder& bld)
{
    (void) vm;
    bld.Inherit<Neptools::Cl3::Entry, Neptools::Lua::DynamicObject>();

    bld.Add<
        decltype(&::Neptools::Lua::GetMember<Neptools::Cl3::Entry, std::string, &Neptools::Cl3::Entry::name>), &::Neptools::Lua::GetMember<Neptools::Cl3::Entry, std::string, &Neptools::Cl3::Entry::name>
    >("get_name");
    bld.Add<
        decltype(&::Neptools::Lua::SetMember<Neptools::Cl3::Entry, std::string, &Neptools::Cl3::Entry::name>), &::Neptools::Lua::SetMember<Neptools::Cl3::Entry, std::string, &Neptools::Cl3::Entry::name>
    >("set_name");
    bld.Add<
        decltype(&::Neptools::Lua::GetMember<Neptools::Cl3::Entry, uint32_t, &Neptools::Cl3::Entry::field_200>), &::Neptools::Lua::GetMember<Neptools::Cl3::Entry, uint32_t, &Neptools::Cl3::Entry::field_200>
    >("get_field_200");
    bld.Add<
        decltype(&::Neptools::Lua::SetMember<Neptools::Cl3::Entry, uint32_t, &Neptools::Cl3::Entry::field_200>), &::Neptools::Lua::SetMember<Neptools::Cl3::Entry, uint32_t, &Neptools::Cl3::Entry::field_200>
    >("set_field_200");
    bld.Add<
        decltype(&::Neptools::Lua::GetMember<Neptools::Cl3::Entry, SmartPtr<Neptools::Dumpable>, &Neptools::Cl3::Entry::src>), &::Neptools::Lua::GetMember<Neptools::Cl3::Entry, SmartPtr<Neptools::Dumpable>, &Neptools::Cl3::Entry::src>
    >("get_src");
    bld.Add<
        decltype(&::Neptools::Lua::SetMember<Neptools::Cl3::Entry, SmartPtr<Neptools::Dumpable>, &Neptools::Cl3::Entry::src>), &::Neptools::Lua::SetMember<Neptools::Cl3::Entry, SmartPtr<Neptools::Dumpable>, &Neptools::Cl3::Entry::src>
    >("set_src");
    bld.Add<
        Overload<decltype(&::Neptools::MakeSmart<Neptools::Cl3::Entry, LuaGetRef<std::string>>), &::Neptools::MakeSmart<Neptools::Cl3::Entry, LuaGetRef<std::string>>>,
        Overload<decltype(&::Neptools::MakeSmart<Neptools::Cl3::Entry, LuaGetRef<std::string>, LuaGetRef<uint32_t>, LuaGetRef<SmartPtr<Neptools::Dumpable>>>), &::Neptools::MakeSmart<Neptools::Cl3::Entry, LuaGetRef<std::string>, LuaGetRef<uint32_t>, LuaGetRef<SmartPtr<Neptools::Dumpable>>>>
    >("new");

}
static TypeRegister::StateRegister<Neptools::Cl3::Entry> reg_neptools_cl3_entry;

}
}

const char Neptools::Cl3::Entry::TYPE_NAME[] = "neptools.cl3.entry";

