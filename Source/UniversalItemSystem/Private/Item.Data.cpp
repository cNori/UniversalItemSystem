#include "../Item.h"
#include "../UniversalItemSystem.h"
#include <Engine/Serialization/Serialization.h>
#include <Engine\Networking\NetworkStream.h>

Item::Data::Data()
{
    m_NetworkReplicate = Item::NetworkReplicateFlags::None;
    m_Name = "New Item Data";
    m_ItemAsset = nullptr;
    m_Attributes = Array<Item::Attribute>();
}

void Item::Data::Serialize(SerializeStream& stream, const void* otherObj)
{
    SERIALIZE_GET_OTHER_OBJ(Item::Data);
    SERIALIZE_MEMBER(NetworkReplicate, m_NetworkReplicate);
    SERIALIZE_MEMBER(Name, m_Name);
    SERIALIZE_MEMBER(Asset, m_ItemAsset);
    UIS::SerializeAttributes(stream, otherObj, m_Attributes);
}

void Item::Data::Deserialize(DeserializeStream& stream, ISerializeModifier* modifier)
{
    DESERIALIZE_MEMBER(NetworkReplicate, m_NetworkReplicate);
    DESERIALIZE_MEMBER(Name, m_Name);
    DESERIALIZE_MEMBER(Asset, m_ItemAsset);
    UIS::DeserializeAttributes(stream, modifier, m_Attributes);
}

void Item::Data::SetName(const String& name)
{
    m_Name = name;
    MarkNameDirty();
}

inline const String& Item::Data::GetName() const
{
    return m_Name;
}

void Item::Data::SetItemAsset(const AssetReference<JsonAsset>& ItemAsset)
{
    m_ItemAsset = ItemAsset;
    MarkAssetDataDirty();
}

inline const AssetReference<JsonAsset>& Item::Data::GetItemAsset() const
{
    return m_ItemAsset;
}

void Item::Data::SetAttributes(const Array<Item::Attribute>& Attributes)
{
    m_Attributes = Attributes;
    MarkAttributesDataDirty();
}
inline const Array<Item::Attribute>& Item::Data::GetAttributes() const
{
    return m_Attributes;
}

void Item::Data::MarkDirty()
{
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate))
        m_NetworkReplicate |= Item::NetworkReplicateFlags::IsDirty;
}

void Item::Data::MarkAssetDataDirty()
{
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate | Item::NetworkReplicateFlags::Value))
        m_NetworkReplicate |= Item::NetworkReplicateFlags::IsDirty | Item::NetworkReplicateFlags::Bit6;
}

void Item::Data::MarkAttributesDataDirty()
{
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate | Item::NetworkReplicateFlags::Value))
        m_NetworkReplicate |= Item::NetworkReplicateFlags::IsDirty | Item::NetworkReplicateFlags::Bit7;
}

void Item::Data::MarkNameDirty()
{
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate | Item::NetworkReplicateFlags::Value))
        m_NetworkReplicate |= Item::NetworkReplicateFlags::IsDirty | Item::NetworkReplicateFlags::Bit5;
}

bool Item::Data::IsDirty()
{
    return EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate | Item::NetworkReplicateFlags::IsDirty);
}

void Item::Data::ClearDirtyFlags()
{
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate))
        m_NetworkReplicate = m_NetworkReplicate & ~(
            Item::NetworkReplicateFlags::IsDirty |
            Item::NetworkReplicateFlags::Bit5 |
            Item::NetworkReplicateFlags::Bit6 |
            Item::NetworkReplicateFlags::Bit7 );
}

void Item::Data::Serialize(NetworkStream* stream)
{
    if (IsDirty())
    {
        stream->WriteUint16(m_NetworkReplicate);
        if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit5 | Item::NetworkReplicateFlags::Value))
        {
            stream->Write(m_ItemAsset);
        }
        if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit6 | Item::NetworkReplicateFlags::NameTag))
        {
            stream->Write(m_Name);
        }
        if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit7 | Item::NetworkReplicateFlags::Value))
        {
            stream->WriteInt32(m_Attributes.Count());
            for (int32 i = 0; i < m_Attributes.Count(); i++)
                stream->Write(m_Attributes[i]);
        }
        ClearDirtyFlags();
    }
}
void Item::Data::Deserialize(NetworkStream* stream)
{
    unsigned short NRandDT = 0;
    stream->ReadUint16(&NRandDT);
    m_NetworkReplicate = (Item::NetworkReplicateFlags)NRandDT;
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit5 | Item::NetworkReplicateFlags::Value))
    {
        stream->Read(m_ItemAsset);
    }
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit6 | Item::NetworkReplicateFlags::NameTag))
    {
        stream->Read(m_Name);
    }
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit7 | Item::NetworkReplicateFlags::Value))
    {
        int32 size;
        stream->ReadInt32(&size);
        m_Attributes.Resize(size, true);
        for (int32 i = 0; i < size; i++)
            stream->Read(m_Attributes[i]);
    }
    ClearDirtyFlags();
}
