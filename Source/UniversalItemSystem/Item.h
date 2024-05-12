#pragma once

#include "Engine/Scripting/Script.h"
#include "Engine/Networking/INetworkSerializable.h"
#include "Engine/Core/Log.h"
#include "Engine/Scripting/ScriptingObjectReference.h"
#include <Engine/Content/JsonAsset.h>
#include <Engine/Content/Assets/Texture.h>
#include <Engine/Content/Assets/Model.h>
#include <Engine/Content/AssetReference.h>
#include <Engine/Level/Prefabs/Prefab.h>



/// <summary>
/// </summary>
API_CLASS() class UNIVERSALITEMSYSTEM_API Item : public Script
{
    DECLARE_SCRIPTING_TYPE(Item);
    API_AUTO_SERIALIZATION();
public:
    /// <summary>
    /// </summary>
    API_ENUM(Attributes = "System.Flags") enum UNIVERSALITEMSYSTEM_API NetworkReplicateFlags : uint16
    {
        /// <summary>
        /// Dont replicate accros the network
        /// </summary>
        None = 0,
        /// <summary>
        /// The replicate object accros the network
        /// </summary>
        Replicate = 1,
        /// <summary>
        /// if flag is set the name will be included in replication process
        /// </summary>
        NameTag = 2,
        /// <summary>
        /// if flag is set the values will be included in replication process
        /// </summary>
        Value = 4,
        /// <summary>
        /// Set by the item system if the object needs a Network sync,
        /// u can force replication by seting this flag
        /// </summary>
        API_ENUM(Attributes = "HideInEditor") IsDirty = 8,

        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit5 = 16,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit6 = 32,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit7 = 64,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit8 = 128,

        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit9 = 256,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit10 = 512,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit11 = 1024,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit12 = 2048,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit13 = 4096,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit14 = 8192,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit15 = 16384,
        ///<summary>
        /// DON'T MODIFY DIRECTLY unless u know what are u doing 
        ///</summary>
        API_ENUM(Attributes = "HideInEditor") Bit16 = 32768,
    };
public:
    /// <summary>
    /// </summary>
    API_STRUCT(NoPod, NoDefault) struct UNIVERSALITEMSYSTEM_API Attribute : public ISerializable, public INetworkSerializable
    {
        DECLARE_SCRIPTING_TYPE_STRUCTURE(Attribute)
    public:
        /// <summary> 
        /// </summary>
        API_ENUM() enum UNIVERSALITEMSYSTEM_API DataType : byte
        {
            /// <summary>
            /// The null
            /// </summary>
            Null,
            /// <summary>
            /// The bool
            /// </summary>
            Bool,
            /// <summary>
            /// The decimal
            /// </summary>
            Decimal,
            /// <summary>
            /// The text
            /// </summary>
            Text,
            /// <summary>
            /// The unsinded integer
            /// </summary>
            UnsindedInteger,
            /// <summary>
            /// The integer
            /// </summary>
            Integer,
            /// <summary>
            /// The custom
            /// </summary>
            Custom,
        };

        /// <summary>
        /// <para> The data type                                                              </para>
        /// <para> DON'T MODIFY DIRECTLY unless u know what are u doing                       </para>
        /// <para> is exposed only because the c# needs it and flax cant have hiden fields    </para>
        /// <para>                                                                            </para>
        /// <para> remarks:                                                                   </para>
        /// <para> flax cant have hiden fields for Interoperability right now                 </para>
        /// <para> and propertys or funcions on structs                                       </para>
        /// </summary>
        API_FIELD(internal) Item::Attribute::DataType m_DataType;

        /// <summary>
        /// <para> The data                                                                   </para>
        /// <para> DON'T MODIFY DIRECTLY unless u know what are u doing                       </para>
        /// <para> is exposed only because the c# needs it and flax cant have hiden fields    </para>
        /// <para>                                                                            </para>
        /// <para> remarks:                                                                   </para>
        /// <para> flax cant have hiden fields for Interoperability right now                 </para>
        /// <para> and propertys or funcions on structs                                       </para>
        /// </summary>
        API_FIELD(internal) Array<byte> m_Data;
        bool GetBool();                         void Set(bool value);
        char GetSByte();                        void Set(char value);
        int16 GetInt16();                       void Set(int16 value);
        int32 GetInt32();                       void Set(int32 value);
        int64 GetInt64();                       void Set(int64 value);
        byte GetByte();                         void Set(byte value);
        uint16 GetUint16();                     void Set(uint16 value);
        uint32 GetUint32();                     void Set(uint32 value);
        uint64 GetUint64();                     void Set(uint64 value);
        float GetFloat();                       void Set(float value);
        double GetDouble();                     void Set(double value);
        const String GetString();               void Set(const String& value);
        const Array<byte> GetCustom() const;    void Set(const Array<byte>& value);
        /// <summary>
        /// Attribute network replicate flags
        /// DON'T MODIFY DIRECTLY unless u know what are u doing
        /// is exposed only because the c# needs it and flax cant have hiden fields
        /// remarks:
        /// flax cant have hiden fields for Interoperability right now
        /// and propertys or funcions on structs
        /// 
        /// <para>used bits                                         </para>
        /// <para>bit5 is if Name     IsDirty flag                  </para>
        /// <para>bit6 is if Data     IsDirty flag                  </para>
        /// <para>bit(7  to 9)  is Type   (range 0 to 7)            </para>
        /// <para>bit(10 to 16) is Data.Count (range 0 to 127)      </para>
        /// </summary>
        API_FIELD(internal) NetworkReplicateFlags m_NetworkReplicate;

        /// <summary>
        /// <para> Attribute name                                                                     </para>
        /// <para> DON'T MODIFY DIRECTLY unless u know what are u doing                               </para>
        /// <para> is exposed only because the c# needs it and flax cant have hiden fields            </para>
        /// <para> remarks:                                                                           </para>
        /// <para> flax cant have hiden fields for Interoperability right now                         </para>
        /// <para> and propertys or funcions on structs                                               </para>
        /// </summary>
        API_FIELD(internal) String m_Name;

        void SetName(const String& name);
        inline const String& GetName() const;

        void MarkDirty();
        void MarkDataDirty();
        void MarkNameDirty();
        bool IsDirty();
        void ClearDirtyFlags();
    public:
        // Inherited via ISerializable
        void Serialize(SerializeStream& stream, const void* otherObj) override;
        void Deserialize(DeserializeStream& stream, ISerializeModifier* modifier) override;
    public:
        // Inherited via INetworkSerializable
        void Serialize(NetworkStream* stream) override;
        void Deserialize(NetworkStream* stream) override;
    private:
        void ValidateData();
    };
public:
    /// <summary>
    /// </summary>
    API_STRUCT(NoPod, NoDefault) struct UNIVERSALITEMSYSTEM_API Data : public ISerializable, public INetworkSerializable
    {
        DECLARE_SCRIPTING_TYPE_STRUCTURE(Data)
    public:
        Data();

        /// <summary>
        /// <para> DON'T MODIFY DIRECTLY unless u know what are u doing                       </para>
        /// <para> is exposed only because the c# needs it and flax cant have hiden fields    </para>
        /// <para>                                                                            </para>
        /// <para> remarks:                                                                   </para>
        /// <para> flax cant have hiden fields for Interoperability right now                 </para>
        /// <para> and propertys or funcions on structs                                       </para>
        /// </summary>
        API_FIELD(internal) Item::NetworkReplicateFlags m_NetworkReplicate;

        /// <summary>
        /// <para> DON'T MODIFY DIRECTLY unless u know what are u doing                       </para>
        /// <para> is exposed only because the c# needs it and flax cant have hiden fields    </para>
        /// <para>                                                                            </para>
        /// <para> remarks:                                                                   </para>
        /// <para> flax cant have hiden fields for Interoperability right now                 </para>
        /// <para> and propertys or funcions on structs                                       </para>
        /// </summary>
        API_FIELD(internal) String m_Name;

        /// <summary>
        /// <para> DON'T MODIFY DIRECTLY unless u know what are u doing                       </para>
        /// <para> is exposed only because the c# needs it and flax cant have hiden fields    </para>
        /// <para>                                                                            </para>
        /// <para> remarks:                                                                   </para>
        /// <para> flax cant have hiden fields for Interoperability right now                 </para>
        /// <para> and propertys or funcions on structs                                       </para>
        /// </summary>
        API_FIELD(internal,Attributes = "AssetReference(typeof(Item.Asset))") AssetReference<JsonAsset> m_ItemAsset;

        /// <summary>
        /// <para> DON'T MODIFY DIRECTLY unless u know what are u doing                       </para>
        /// <para> is exposed only because the c# needs it and flax cant have hiden fields    </para>
        /// <para>                                                                            </para>
        /// <para> remarks:                                                                   </para>
        /// <para> flax cant have hiden fields for Interoperability right now                 </para>
        /// <para> and propertys or funcions on structs                                       </para>
        /// </summary>
        API_FIELD(internal) Array<Item::Attribute> m_Attributes;

        void SetName(const String& name);
        inline const String& GetName() const;
        void SetItemAsset(const AssetReference<JsonAsset>& name);
        inline const AssetReference<JsonAsset>& GetItemAsset() const;
        void SetAttributes(const Array<Item::Attribute>& name);
        inline const Array<Item::Attribute>& GetAttributes() const;

        void MarkDirty();
        void MarkAssetDataDirty();
        void MarkAttributesDataDirty();
        void MarkNameDirty();
        bool IsDirty();
        void ClearDirtyFlags();
    private:
        friend class UIS;

        // Inherited via ISerializable
        void Serialize(SerializeStream& stream, const void* otherObj) override;
        void Deserialize(ISerializable::DeserializeStream& stream, ISerializeModifier* modifier) override;

        // Inherited via INetworkSerializable
        void Serialize(NetworkStream* stream) override;
        void Deserialize(NetworkStream* stream) override;
    };
public:
    /// <summary>
    /// </summary>
    API_STRUCT(NoPod, NoDefault) struct UNIVERSALITEMSYSTEM_API Asset : public ISerializable
    {
        DECLARE_SCRIPTING_TYPE_STRUCTURE(Asset);
    public:

        /// <summary>
        /// The name
        /// </summary>
        API_FIELD() String Name;

        /// <summary>
        /// The description
        /// </summary>
        API_FIELD() String Description;

        /// <summary>
        /// The icon
        /// </summary>
        API_FIELD(Attributes = "AssetReference(typeof(Texture))")
            AssetReference<Texture> Icon;

        /// <summary>
        /// The world prefab the prefab representation when dropped
        /// </summary>
        API_FIELD(Attributes = "AssetReference(typeof(Prefab))")
            AssetReference<Prefab> WorldPrefab;

        /// <summary>
        /// The attributes
        /// </summary>
        API_FIELD() Array<Item::Attribute> Attributes;
    private:
        friend class UIS;

        // Inherited via ISerializable
        void Serialize(ISerializable::SerializeStream& stream, const void* otherObj) override;
        void Deserialize(ISerializable::DeserializeStream& stream, ISerializeModifier* modifier) override;
    };
public:
    API_FIELD() Item::Data ItemData;
};
DECLARE_ENUM_OPERATORS(Item::NetworkReplicateFlags)