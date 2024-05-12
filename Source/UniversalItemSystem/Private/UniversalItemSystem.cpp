#include "../UniversalItemSystem.h"
#include <iostream>
#include <string>
#include <Engine/Serialization/JsonWriters.h>
#include <Engine/Serialization/Serialization.h>
#include "Engine/Core/Cache.h"
#include "Engine/Content/Content.h"
#include "Engine/Core/ISerializable.h"

Array<byte> UIS::StringToAttributeData(String& str, Item::Attribute::DataType bExpectedType)
{
	Item::Attribute::DataType ExpectedType = bExpectedType;
	std::wstring s = std::wstring(str.Get(), str.Length());
	Array<byte> ar = Array<byte>();
	switch (ExpectedType)
	{
	case Item::Attribute::DataType::Bool:
		ar.Add(0);
		if (!str.IsEmpty()) 
		{
			if (str[0] == '1')
			{
				ar[0] = 1;
			}
			else
			{
				str.Resize(5);
				if (str.Contains(TEXT("true"), StringSearchCase::IgnoreCase))
				{
					ar[0] = 1;
				}
			}
		}
		break;
	case Item::Attribute::DataType::Decimal:
	{
		double value = 0.0;
		try
		{
			value = std::stod(s);
		}catch (const std::exception&){}

		if (value < 10000 && value > -10000)
		{
			ar.Resize(sizeof(float), false);
			auto fv = (float)value;
			memcpy_s(ar.Get(), ar.Count(), &fv, ar.Count());
		}
		else
		{
			ar.Resize(sizeof(double), false);
			memcpy_s(ar.Get(), ar.Count(), &value, ar.Count());
		}
	}
		break;
	case Item::Attribute::DataType::Text:
		ar.Resize(str.Length());
		for (auto i = 0; i < ar.Count(); i++)
		{
			ar[i] = (byte)str[i];
		}
		break;
	case Item::Attribute::DataType::UnsindedInteger:
		try
		{
			// string -> ull
			auto v = std::stoull(s);
			if (v < UINT8_MAX)
				ar.Resize(sizeof(uint8), false);
			else if (v < UINT16_MAX)
				ar.Resize(sizeof(uint16), false);
			else if (v < UINT32_MAX)
				ar.Resize(sizeof(uint32), false);
			else if (v < UINT64_MAX)
				ar.Resize(sizeof(uint64), false);
			memcpy_s(ar.Get(), ar.Count(), &v, ar.Count());
		}
		catch (const std::exception&)
		{
			uint8 v = 0;
			ar.Resize(sizeof(v), false);
			memcpy_s(ar.Get(), ar.Count(), &v, ar.Count());
		}
		
		break;
	case Item::Attribute::DataType::Integer:
		try
		{
			// string -> ull
			auto v = std::stoll(s);
			if (v < INT8_MAX)
				ar.Resize(sizeof(int8), false);
			else if (v < INT16_MAX)
				ar.Resize(sizeof(int16), false);
			else if (v < INT32_MAX)
				ar.Resize(sizeof(int32), false);
			else if (v < INT64_MAX)
				ar.Resize(sizeof(int64), false);
			memcpy_s(ar.Get(), ar.Count(), &v, ar.Count());
		}
		catch (const std::exception&)
		{
			int8 v = 0;
			ar.Resize(sizeof(v), false);
			memcpy_s(ar.Get(), ar.Count(), &v, ar.Count());
		}
		break;
	case Item::Attribute::DataType::Null:
		break;
	case Item::Attribute::DataType::Custom:
		ar.Resize(str.Length());
		for (auto i = 0; i < ar.Count(); i++)
		{
			ar[i] = (byte)str[i];
		}
		break;
	default:
		break;
	}
	if (ar.Count() > 127)
	{
		LOG_STR(Warning,
			L"\n[Universal Item System]\n"
			"Buffer Overflow\n"
			"Data will be Croped to 127 bytes");

		ar.Resize(127);
	}

	return ar;
}
String UIS::StringFromAttributeData(Array<byte>& Data, Item::Attribute::DataType bExpectedType)
{
	Item::Attribute::DataType ExpectedType = (Item::Attribute::DataType)bExpectedType;
	String str = L"";

	union converter
	{
		byte bytes[sizeof(uint64)];
		uint64 ui;
		int64 i;
		double d;
		float f;
	};
	converter c{};
	c.ui = 0;
	for (auto i = 0; i < Data.Count(); i++)
	{
		if (i == sizeof(uint64))
			break;
		c.bytes[i] = Data[i];
	}

	switch (ExpectedType)
	{
	case Item::Attribute::DataType::Bool:
		str.Append(Data[0] ? TEXT("true") : TEXT("false"));
		break;
	case Item::Attribute::DataType::Decimal:
		if (Data.Count() == sizeof(float)) 
		{
			str = String(std::to_string(c.f).c_str());
		}
		else
		{
			str = String(std::to_string(c.d).c_str());
		}
		break;
	case Item::Attribute::DataType::Text:
		str.Resize(Data.Count());
		for (auto i = 0; i < Data.Count(); i++)
		{
			str[i] = Data[i];
		}
		break;
	case Item::Attribute::DataType::UnsindedInteger:
		str = String(std::to_string(c.ui).c_str());
		break;
	case Item::Attribute::DataType::Integer:
		str = String(std::to_string(c.i).c_str());
		break;
	case Item::Attribute::DataType::Null:
		break;
	case Item::Attribute::DataType::Custom:
		str.Resize(Data.Count());
		for (auto i = 0; i < Data.Count(); i++)
		{
			str[i] = Data[i];
		}
		break;
	default:
		break;
	}
	return str;
}

void UIS::SaveItemAsset(JsonAsset& Source, Item::Asset& Instance)
{
	rapidjson_flax::StringBuffer dataBuffer;
	PrettyJsonWriter stream(dataBuffer);
	stream.StartObject();
	Instance.Serialize(stream, &Instance);
	stream.EndObject();
	Source.SetData(String(dataBuffer.GetString()));
	Source.Save();
}

void UIS::CallItemDataNetworkSerialize(Item::Data& Instance, NetworkStream* Stream)
{
	Instance.Serialize(Stream);
}
void UIS::CallItemDataNetworkDeserialize(Item::Data& Instance, NetworkStream* Stream)
{
	Instance.Deserialize(Stream);
}

void UIS::LoadItemAsset(JsonAsset& Source, Item::Asset& Instance)
{
	Source.WaitForLoaded();
	//auto modifier = Cache::ISerializeModifier.Get();
	ISerializable::DeserializeStream& stream = (*Source.Data);
	Instance.Deserialize(stream, nullptr);
}

void UIS::SaveItemDataToAsset(Item::Data& Instance)
{
	JsonAsset* SourcePtr = Instance.GetItemAsset().Get();
	if (SourcePtr)
	{
		JsonAsset& Source = *SourcePtr;
		Item::Asset as{};
		UIS::LoadItemAsset(Source, as);
		as.Attributes = Instance.GetAttributes();
		as.Name = Instance.GetName();
		SaveItemAsset(Source, as);
	}
}

void UIS::LoadItemDataFromAsset(Item::Data& Instance)
{
	JsonAsset* SourcePtr = Instance.GetItemAsset().Get();
	if (SourcePtr)
	{
		JsonAsset& Source = *SourcePtr;
		Source.WaitForLoaded();
		Item::Asset as{};
		UIS::LoadItemAsset(Source, as);
		Instance.m_Attributes = as.Attributes;
		Instance.m_Name = as.Name;
	}
}
