#pragma once
#include <Engine/Core/Collections/Dictionary.h>
#include "Item.h"

/// <summary>
/// </summary>
API_CLASS(Static) class UNIVERSALITEMSYSTEM_API UIS
{
	DECLARE_SCRIPTING_TYPE_NO_SPAWN(UIS)
public:
	/// <summary>
	/// Saves the item data to assined asset.
	/// </summary>
	/// <param name="Instance">The instance.</param>
	API_FUNCTION() static void SaveItemDataToAsset(API_PARAM(ref) Item::Data& Instance);

	/// <summary>
	/// Loads the item data from assined asset.
	/// </summary>
	/// <param name="Instance">The instance.</param>
	API_FUNCTION() static void LoadItemDataFromAsset(API_PARAM(ref) Item::Data& Instance);

private:
	API_FUNCTION(internal) static Array<byte> StringToAttributeData(String& str, Item::Attribute::DataType ExpectedType);
	API_FUNCTION(internal) static String StringFromAttributeData(Array<byte>& Data, Item::Attribute::DataType ExpectedType);
#pragma region GlueCode
	API_FUNCTION(internal) static void SaveItemAsset(JsonAsset& Source, Item::Asset& Instance);
	API_FUNCTION(internal) static void CallItemDataNetworkSerialize(API_PARAM(ref)Item::Data& Instance, NetworkStream* Stream);
	API_FUNCTION(internal) static void CallItemDataNetworkDeserialize(API_PARAM(ref)Item::Data& Instance, NetworkStream* Stream);
	API_FUNCTION(internal) static void LoadItemAsset(JsonAsset& Source,API_PARAM(ref) Item::Asset& Instance);
#pragma endregion
private:

	friend class Item;
	static void SerializeAttributes(ISerializable::SerializeStream& stream, const void* otherObj, Array<Item::Attribute>& InAT);
	static void DeserializeAttributes(ISerializable::DeserializeStream& stream, ISerializeModifier* modifier, Array<Item::Attribute>& OutAT);
	static void SerializeAttribute(ISerializable::SerializeStream& stream, const String& InATName, const Item::Attribute::DataType& InATType, const Array<byte>& InATData);
	static void DeserializeAttribute(ISerializable::DeserializeStream& stream, String& OutATName, Item::Attribute::DataType& OutATType, Array<byte>& OutATData);
};