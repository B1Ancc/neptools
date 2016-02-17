#ifndef UUID_3D8C2AC2_6B76_435C_94AE_18A797C4BE4D
#define UUID_3D8C2AC2_6B76_435C_94AE_18A797C4BE4D
#pragma once

#include "../raw_item.hpp"
#include <boost/endian/arithmetic.hpp>

namespace Cl3
{

struct Header
{
    char magic[4];
    boost::endian::little_uint32_t field_04;
    boost::endian::little_uint32_t field_08;
    boost::endian::little_uint32_t num_sections;
    boost::endian::little_uint32_t secs_offset;
    boost::endian::little_uint32_t field_14;

    bool IsValid(size_t file_size) const noexcept;
};
static_assert(sizeof(Header) == 0x18, "");

class SectionsItem;
class HeaderItem final : public Item
{
public:
    HeaderItem(Key k, Context* ctx)
        : Item{k, ctx}, sections{nullptr} {}
    HeaderItem(Key k, Context* ctx, const Header* hdr);
    static HeaderItem* CreateAndInsert(ItemPointer ptr);

    void Dump(std::ostream& os) const override;
    void PrettyPrint(std::ostream& os) const override;
    size_t GetSize() const noexcept override { return sizeof(Header); }

    const SectionsItem& GetSections() const noexcept { return GetSectionsInt(); }
    SectionsItem& GetSections() noexcept { return GetSectionsInt(); }

    uint32_t field_14;
    const Label* sections;

private:
    SectionsItem& GetSectionsInt() const noexcept;
};

}

#endif