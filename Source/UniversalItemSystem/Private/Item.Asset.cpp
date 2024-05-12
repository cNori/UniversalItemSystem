#include "../Item.h"
#include "../UniversalItemSystem.h"
#include <Engine/Serialization/Serialization.h>
#include <Engine/Networking/NetworkStream.h>

void Item::Asset::Serialize(ISerializable::SerializeStream& stream, const void* otherObj)
{
    SERIALIZE_GET_OTHER_OBJ(Item::Asset);
    stream.Key("Name"); 
    Serialization::Serialize(stream, Name, other ? &other->Name : nullptr);
    stream.Key("Description"); 
    Serialization::Serialize(stream, Description, other ? &other->Description : nullptr);
    stream.Key("Icon"); 
    Serialization::Serialize(stream, Icon, other ? &other->Icon : nullptr);
    stream.Key("WorldPrefab"); 
    Serialization::Serialize(stream, WorldPrefab, other ? &other->WorldPrefab : nullptr);
    UIS::SerializeAttributes(stream, otherObj, Attributes);
}

void Item::Asset::Deserialize(ISerializable::DeserializeStream& stream, ISerializeModifier* modifier)
{
    DESERIALIZE_MEMBER(Name, Name);
    DESERIALIZE_MEMBER(Description, Description);
    DESERIALIZE_MEMBER(Icon, Icon);
    DESERIALIZE_MEMBER(WorldPrefab, WorldPrefab);
    UIS::DeserializeAttributes(stream, modifier, Attributes);
}