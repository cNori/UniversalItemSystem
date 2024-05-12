#include "../Item.h"
#include <Engine/Serialization/Serialization.h>
#include <Engine\Networking\NetworkStream.h>
#include <Engine\Debug\DebugLog.h>
#include <Engine/Serialization/JsonWriters.h>

void Item::Attribute::SetName(const String& name)
{
    m_Name = name;
    MarkNameDirty();
}

inline const String& Item::Attribute::GetName() const
{
    return m_Name;
}

void Item::Attribute::MarkDirty()
{
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate))
        m_NetworkReplicate |= Item::NetworkReplicateFlags::IsDirty;
}

void Item::Attribute::MarkDataDirty()
{
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate | Item::NetworkReplicateFlags::Value))
        m_NetworkReplicate |= Item::NetworkReplicateFlags::IsDirty | Item::NetworkReplicateFlags::Bit6;
}

void Item::Attribute::MarkNameDirty()
{
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate | Item::NetworkReplicateFlags::Value))
        m_NetworkReplicate |= Item::NetworkReplicateFlags::IsDirty | Item::NetworkReplicateFlags::Bit5;
}

bool Item::Attribute::IsDirty()
{
    return EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate | Item::NetworkReplicateFlags::IsDirty);
}

void Item::Attribute::ClearDirtyFlags()
{
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Replicate))
        m_NetworkReplicate = m_NetworkReplicate & ~(
            Item::NetworkReplicateFlags::IsDirty |
            Item::NetworkReplicateFlags::Bit5 |
            Item::NetworkReplicateFlags::Bit6);
}
void Item::Attribute::ValidateData()
{
    if (m_Data.Count() > 127)
    {
        LOG(Warning,
            "\n[Universal Item System]\n"
            "Buffer Overflow on Attribute: {0}\n"
            "Data will be Croped to 127 bytes"
            "\nStackTrace:\n{1}", m_Name, DebugLog::GetStackTrace());

        m_Data.Resize(127);
    }
}

#define ImpGeterSeter(EType,FName,SysType)\
SysType Item::Attribute::Get##FName##()\
{\
    if (m_DataType != EType || m_Data.Count() != sizeof(SysType))\
    {\
        DebugLog::LogError(TEXT("Invalid Data size"));\
        return 0;\
    }\
    SysType value;\
    memcpy_s(&value, m_Data.Count(), m_Data.Get(), m_Data.Count());\
    return value;\
}\
void Item::Attribute::Set(SysType value)\
{\
    m_Data.Resize(sizeof(SysType), false);\
    memcpy_s(m_Data.Get(), m_Data.Count(), &value, m_Data.Count());\
    m_DataType = EType;\
    MarkDataDirty();\
}


ImpGeterSeter(Bool, Bool, bool)

ImpGeterSeter(Integer, SByte, char)
ImpGeterSeter(Integer, Int16, int16)
ImpGeterSeter(Integer, Int32, int32)
ImpGeterSeter(Integer, Int64, int64)

ImpGeterSeter(UnsindedInteger, Byte, byte)
ImpGeterSeter(UnsindedInteger, Uint16, uint16)
ImpGeterSeter(UnsindedInteger, Uint32, uint32)
ImpGeterSeter(UnsindedInteger, Uint64, uint64)

ImpGeterSeter(Decimal, Float, float)
ImpGeterSeter(Decimal, Double, double)

const Array<byte> Item::Attribute::GetCustom() const
{
    return m_Data;
}
void Item::Attribute::Set(const Array<byte>& value)
{
    m_Data = value;
    ValidateData();
    m_DataType = Custom;
    MarkDataDirty();
}
const String Item::Attribute::GetString()
{
    if (m_DataType != Text || m_Data.Count() < 1)
    {
        return String();
    }
    String value = L"";
    int32 size = m_Data.Count() / sizeof(Char);
    value.Resize(size);
    memcpy_s(value.Get(), size, m_Data.Get(), size);
    return value;
}
void Item::Attribute::Set(const String& value)
{
    int32 size = value.Length() * sizeof(Char);
    m_Data.Resize(size);
    memcpy_s(m_Data.Get(), m_Data.Count(), value.Get(), size);
    ValidateData();
    m_DataType = Text;
    MarkDataDirty();
}

void Item::Attribute::Serialize(NetworkStream* stream)
{
    unsigned short MaskA = 0b0000000000111111;
    unsigned short MaskB = 0b0000000111000000;
    unsigned short MaskC = 0b1111111000000000;
    unsigned short NRandDT = (m_NetworkReplicate & MaskA) << 0;
    unsigned short B = (((unsigned short)m_DataType) & MaskA) << 6;
    unsigned short C = (((unsigned short)m_Data.Count()) & MaskA) << 9;
    NRandDT = (NRandDT | B | C);
    stream->WriteUint16(NRandDT);

    if (IsDirty())
    {
        ValidateData();
        if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit6 | Item::NetworkReplicateFlags::NameTag))
        {
            stream->WriteString(m_Name);
        }
        if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit5 | Item::NetworkReplicateFlags::Value))
        {
            stream->WriteBytes(m_Data.Get(), m_Data.Count());
        }
    }
    ClearDirtyFlags();
}

void Item::Attribute::Deserialize(NetworkStream* stream)
{
    unsigned short NRandDT = 0;
    stream->ReadUint16(&NRandDT);
    unsigned short MaskA = 0b0000000000111111;
    unsigned short MaskB = 0b0000000111000000;
    unsigned short MaskC = 0b1111111000000000;
    m_NetworkReplicate = (Item::NetworkReplicateFlags)((NRandDT & MaskA) << 0);
    m_DataType = (Item::Attribute::DataType)((NRandDT & MaskA) >> 6);
    uint16 DataCount = (Item::NetworkReplicateFlags)((NRandDT & MaskA) >> 9);

    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit5 | Item::NetworkReplicateFlags::NameTag))
    {
        stream->ReadString(&m_Name);
    }
    if (EnumHasAnyFlags(m_NetworkReplicate, Item::NetworkReplicateFlags::Bit6 | Item::NetworkReplicateFlags::Value))
    {
        m_Data.Resize(DataCount);
        stream->ReadBytes(m_Data.Get(), DataCount);
    }
}

void Item::Attribute::Serialize(SerializeStream& stream, const void* otherObj)
{
    SERIALIZE_GET_OTHER_OBJ(Item::Attribute);
    stream.Key("NetworkReplicate");
    stream.Enum(m_NetworkReplicate);
}

void Item::Attribute::Deserialize(DeserializeStream& stream, ISerializeModifier* modifier)
{
    DESERIALIZE_MEMBER(NetworkReplicate, m_NetworkReplicate);
}