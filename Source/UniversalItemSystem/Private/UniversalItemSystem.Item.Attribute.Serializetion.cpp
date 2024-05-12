#include "../UniversalItemSystem.h"
#include <Engine/Core/Log.h>
#include <Engine/Debug/DebugLog.h>
#include <Engine/Serialization/JsonWriters.h>
#include <Engine/Serialization/Serialization.h>

void UIS::SerializeAttribute(ISerializable::SerializeStream& stream,
    const String& InATName,
    const Item::Attribute::DataType& InATType,
    const Array<byte>& InATData
)
{
    union converter
    {
        byte bytes[sizeof(uint64)];
        bool b;
        uint64 ui;
        int64 i;
        double d;
        float f;
    };
    converter c{};
    c.ui = 0;
    for (auto i = 0; i < InATData.Count(); i++)
    {
        if (i == sizeof(uint64))
            break;
        c.bytes[i] = InATData[i];
    }


    //if (m_Name.IsEmpty()) 
    //    m_Name = Guid::New().ToString();
    stream.Key(InATName);
    switch (InATType)
    {
    case Item::Attribute::DataType::Null:
        stream.String("", 0);
        break;
    case Item::Attribute::DataType::Bool:
        stream.Bool(c.b);
        break;
    case Item::Attribute::DataType::Decimal:
        stream.Double(InATData.Count() == sizeof(double) ? c.d : (double)c.f);
        break;
    case Item::Attribute::DataType::Text:
    {
        String value = L"";
        int32 size = InATData.Count() / sizeof(Char);
        value.Resize(size);
        memcpy_s(value.Get(), size, InATData.Get(), size);
        stream.String(value);
    }
    break;
    case Item::Attribute::DataType::UnsindedInteger:
        stream.Uint64(c.ui);
        break;
    case Item::Attribute::DataType::Integer:
        stream.Int64(c.i);
        break;
    case Item::Attribute::DataType::Custom:
        String s = String();
        auto custom = InATData;
        s.Resize(custom.Count() + 1);
        s[0] = '*';
        for (auto i = 0; i < custom.Count(); i++)
        {
            s[i + 1] = (custom[i] + 1);
        }
        stream.String(s);
        break;
    }
}

void UIS::DeserializeAttribute(ISerializable::DeserializeStream& stream,
    String& OutATName,
    Item::Attribute::DataType& OutATType,
    Array<byte>& OutATData
)
{
    if (!stream.IsObject())
        return;
    if (stream.IsNull())
        return;
    auto const& name = stream.MemberBegin()->name;
    auto const& value = stream.MemberBegin()->value;
    OutATName = name.GetStringAnsiView().GetText();
    if (!value.IsNull())
    {
        if (value.IsBool()) {
            OutATData.Resize(sizeof(bool), false);
            auto b = bool(value.GetBool());
            memcpy_s(OutATData.Get(), OutATData.Count(), &b, OutATData.Count());
            OutATType = Item::Attribute::DataType::Bool;
        }
        else  if (value.IsDouble())
        {
            //Set(double());
            auto v = value.GetDouble();
            if (v < 50000 && v > -50000)
            {
                OutATType = Item::Attribute::DataType::Decimal;
                OutATData.Resize(sizeof(float), false);
                auto fv = (float)v;
                memcpy_s(OutATData.Get(), OutATData.Count(), &fv, OutATData.Count());
            }
            else
            {
                OutATType = Item::Attribute::DataType::Decimal;
                OutATData.Resize(sizeof(double), false);
                memcpy_s(OutATData.Get(), OutATData.Count(), &v, OutATData.Count());
            }
        }
        else if (value.IsUint64())
        {
            auto v = value.GetUint64();
            if (v < UINT8_MAX)
                OutATData.Resize(sizeof(uint8), false);
            else if (v < UINT16_MAX)
                OutATData.Resize(sizeof(uint16), false);
            else if (v < UINT32_MAX)
                OutATData.Resize(sizeof(uint32), false);
            else if (v < UINT64_MAX)
                OutATData.Resize(sizeof(uint64), false);
            memcpy_s(OutATData.Get(), OutATData.Count(), &v, OutATData.Count());
            OutATType = Item::Attribute::DataType::Integer;
        }
        else if (value.IsInt64())
        {
            auto v = value.GetUint64();
            if (v < INT8_MAX)
                OutATData.Resize(sizeof(int8), false);
            else if (v < INT16_MAX)
                OutATData.Resize(sizeof(int16), false);
            else if (v < INT32_MAX)
                OutATData.Resize(sizeof(int32), false);
            else if (v < INT64_MAX)
                OutATData.Resize(sizeof(int64), false);
            memcpy_s(OutATData.Get(), OutATData.Count(), &v, OutATData.Count());
            OutATType = Item::Attribute::DataType::UnsindedInteger;
        }
        else if (value.IsString())
        {
            //auto cstring = value.GetString();
            auto const& v = String(value.GetString());
            if (v.HasChars())
            {
                if (v.Get()[0] == '*')
                {
                    auto str = v.Substring(1);
                    Array<byte> bytes;
                    for (auto i = 0; i < str.Length(); i++)
                    {
                        bytes.Add((byte)(str[i] - 1));
                    }
                    OutATData = bytes;
                    if (OutATData.Count() > 127)
                    {
                        LOG(Warning,
                            "\n[Universal Item System]\n"
                            "Buffer Overflow on Attribute: {0}\n"
                            "Data will be Croped to 127 bytes"
                            "\nStackTrace:\n{1}", OutATName, DebugLog::GetStackTrace());

                        OutATData.Resize(127);
                    }
                    OutATType = Item::Attribute::DataType::Custom;
                }
            }
            else
            {
                int32 size = v.Length() * sizeof(Char);
                OutATData.Resize(size);
                memcpy_s(OutATData.Get(), OutATData.Count(), v.Get(), size);
                if (OutATData.Count() > 127)
                {
                    LOG(Warning,
                        "\n[Universal Item System]\n"
                        "Buffer Overflow on Attribute: {0}\n"
                        "Data will be Croped to 127 bytes"
                        "\nStackTrace:\n{1}", OutATName, DebugLog::GetStackTrace());

                    OutATData.Resize(127);
                }
                OutATType = Item::Attribute::DataType::Text;
            }
        }
    }
}

void UIS::SerializeAttributes(ISerializable::SerializeStream& stream, const void* otherObj,
    Array<Item::Attribute>& InAT
)
{
    SERIALIZE_GET_OTHER_OBJ(Item::Attribute);
    stream.Key("Attributes", (sizeof("Attributes") / sizeof("Attributes"[0])) - 1);
    stream.StartArray();
    for (int32 i = 0; i < InAT.Count(); i++)
    {
        stream.StartObject();
        SerializeAttribute(stream, InAT[i].m_Name, InAT[i].m_DataType, InAT[i].m_Data);
        InAT[i].Serialize(stream, &InAT[i]);
        stream.EndObject();
    }
    stream.EndArray();
}

void UIS::DeserializeAttributes(ISerializable::DeserializeStream& stream, ISerializeModifier* modifier,
    Array<Item::Attribute>& OutAT
)
{
    const auto e = stream.FindMember(rapidjson_flax::Value("Attributes", (sizeof("Attributes") / sizeof("Attributes"[0])) - 1));
    if (e != stream.MemberEnd())
    {
        if (e->value.IsArray())
        {
            const auto& streamArray = e->value.GetArray();
            OutAT.Resize(streamArray.Size());
            for (int32 i = 0; i < OutAT.Count(); i++)
            {
                DeserializeAttribute(streamArray[i], OutAT[i].m_Name, OutAT[i].m_DataType, OutAT[i].m_Data);
                OutAT[i].Deserialize(streamArray[i], modifier);
            }
        }
    }
}