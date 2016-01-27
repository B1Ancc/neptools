#ifndef STCM_HEADER_HPP
#define STCM_HEADER_HPP
#pragma once

#include "../raw_item.hpp"
#include <boost/endian/arithmetic.hpp>

namespace Stcm
{

struct Header
{
    union
    {
        char msg[0x20];
        struct
        {
            char magic[5];
            char version;
            char rest[0x20-5-1];
        } parts;
    };
    boost::endian::little_uint32_t export_offset;
    boost::endian::little_uint32_t export_count;
    boost::endian::little_uint32_t field_28;
    boost::endian::little_uint32_t collection_link_offset;
};
static_assert(sizeof(Header) == 0x30, "");

class HeaderItem : public Item
{
public:
    HeaderItem(const RawItem& it) : HeaderItem{it.GetPtr(), it.GetSize()} {}
    HeaderItem(const Byte* data, size_t len);

    void Dump(std::ostream& os) const;
    size_t GetSize() const noexcept { return sizeof(Header); }
private:
    Header raw;
};

}

#endif
