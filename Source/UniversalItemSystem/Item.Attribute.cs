using FlaxEngine;
using FlaxEngine.Networking;
using System.Runtime.CompilerServices;
using System.Text;
namespace UniversalItemSystem;

public partial class Item
{
    public partial struct Attribute
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Attribute" /> struct.
        /// </summary>
        public Attribute() { }

        /// <summary>
        /// Gets or sets the network replicate flags.
        /// </summary>
        /// <value>
        /// The network replicate flags.
        /// </value>
        [NoSerialize]
        public NetworkReplicateFlags NetworkReplicateFlags
        {
            get => m_NetworkReplicate;
            set
            {
                m_NetworkReplicate = (NetworkReplicateFlags)(((ushort)value) & 0b0000000000111111);
                MarkDirty();
            }
        }

        /// <summary>
        /// Gets or sets the name.
        /// </summary>
        /// <value>
        /// The name.
        /// </value>
        [NoSerialize]
        public string Name
        {
            get => m_Name;
            set
            {
                m_Name = value;
                MarkNameDirty();
            }
        }

        /// <summary>
        /// Gets or sets the type.
        /// </summary>
        /// <value>
        /// The type.
        /// </value>
        [NoSerialize]
        public DataType Type
        {
            get => m_DataType;
            set
            {
                m_DataType = value;
            }
        }
        /// <summary>
        /// Gets or sets the data.
        /// </summary>
        /// <value>
        /// The data.
        /// </value>
        [NoSerialize]
        public string Data
        {
            get
            {
                return UIS.StringFromAttributeData(m_Data, m_DataType);
            }
            set
            {
                m_Data = UIS.StringToAttributeData(value, m_DataType);
                MarkDataDirty();
            }
        }

        /// <summary>
        /// Converts to string.
        /// </summary>
        /// <returns>
        /// A <see cref="System.String" /> that represents this instance.
        /// </returns>
        public override string ToString()
        {
            StringBuilder builder = new();
            builder.Append("NetworkReplicate    :"); builder.Append(m_NetworkReplicate);                                        builder.AppendLine();
            builder.Append("IsNetworkDirty      :"); builder.Append(m_NetworkReplicate.HasFlag(NetworkReplicateFlags.IsDirty)); builder.AppendLine();
            builder.Append("Name                :"); builder.Append(Name);                                                      builder.AppendLine();
            builder.Append("Data                :"); builder.Append(Data);                                                      builder.AppendLine();
            return base.ToString();
        }

        void MarkDirty()
        {
            if (m_NetworkReplicate.HasFlag(NetworkReplicateFlags.Replicate))
                m_NetworkReplicate |= NetworkReplicateFlags.IsDirty;
        }
        void MarkDataDirty()
        {
            if (m_NetworkReplicate.HasFlag(NetworkReplicateFlags.Replicate | NetworkReplicateFlags.Value))
                m_NetworkReplicate |= NetworkReplicateFlags.IsDirty | NetworkReplicateFlags.Bit6;
        }
        void MarkNameDirty()
        {
            if (m_NetworkReplicate.HasFlag(NetworkReplicateFlags.Replicate | NetworkReplicateFlags.NameTag))
                m_NetworkReplicate |= NetworkReplicateFlags.IsDirty | NetworkReplicateFlags.Bit5;
        }
    }


    public partial struct Data : INetworkSerializable
    {
        /// <summary>
        /// Gets or sets the network replicate flags.
        /// </summary>
        /// <value>
        /// The network replicate flags.
        /// </value>
        [NoSerialize]
        public NetworkReplicateFlags NetworkReplicate
        {
            readonly get => m_NetworkReplicate;
            set
            {
                m_NetworkReplicate = value;
                MarkDirty();
            }
        }

        /// <summary>
        /// Gets or sets the name.
        /// </summary>
        /// <value>
        /// The name.
        /// </value>
        [NoSerialize]
        public string Name
        {
            readonly get => m_Name;
            set
            {
                m_Name = value;
                MarkNameDirty();
            }
        }

        /// <summary>
        /// Gets or sets the item asset.
        /// </summary>
        /// <value>
        /// The item asset.
        /// </value>
        [AssetReference(typeof(Item.Asset)), NoSerialize]
        public JsonAsset ItemAsset
        {
            readonly get => m_ItemAsset;
            set
            {
                m_ItemAsset = value;
                MarkAssetItemDataDirty();
            }
        }

        /// <summary>
        /// Sets the attributes.
        /// </summary>
        /// <value>
        /// The attributes.
        /// </value>
        [NoSerialize]
        public Item.Attribute[] Attributes
        {
            readonly get => m_Attributes;
            set
            {
                m_Attributes = value;
                MarkAttributesDataDirty();
            }
        }

        /// <summary>
        /// Saves to asset.
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void SaveToAsset()
        {
            UIS.SaveItemDataToAsset(ref this);
        }

        /// <summary>
        /// Loads from asset.
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void LoadFromAsset()
        {
            UIS.LoadItemDataFromAsset(ref this);
        }


        void MarkDirty()
        {
            if (m_NetworkReplicate.HasFlag(NetworkReplicateFlags.Replicate))
                m_NetworkReplicate |= NetworkReplicateFlags.IsDirty;
        }

        void MarkAssetItemDataDirty()
        {
            if (m_NetworkReplicate.HasFlag(NetworkReplicateFlags.Replicate | NetworkReplicateFlags.Value))
                m_NetworkReplicate |= NetworkReplicateFlags.IsDirty | NetworkReplicateFlags.Bit6;
        }

        void MarkAttributesDataDirty()
        {
            if (m_NetworkReplicate.HasFlag(NetworkReplicateFlags.Replicate | NetworkReplicateFlags.Value))
                m_NetworkReplicate |= NetworkReplicateFlags.IsDirty | NetworkReplicateFlags.Bit7;
        }

        void MarkNameDirty()
        {
            if (m_NetworkReplicate.HasFlag(NetworkReplicateFlags.Replicate | NetworkReplicateFlags.NameTag))
                m_NetworkReplicate |= NetworkReplicateFlags.IsDirty | NetworkReplicateFlags.Bit5;
        }

        /// <summary>
        /// Serializes object to the output stream.
        /// </summary>
        /// <param name="stream">The output stream to write serialized data.</param>
        public void Serialize(NetworkStream stream)
        {
            UIS.CallItemDataNetworkSerialize(ref this, stream);
        }

        /// <summary>
        /// Deserializes object from the input stream.
        /// </summary>
        /// <param name="stream">The input stream to read serialized data.</param>
        public void Deserialize(NetworkStream stream)
        {
            UIS.CallItemDataNetworkDeserialize(ref this, stream);
        }

        /// <inheritdoc/>
        public override string ToString()
        {
            StringBuilder builder = new();
            builder.AppendLine();
            builder.Append("NetworkReplicate      :"); builder.Append(NetworkReplicate); builder.AppendLine();
            builder.Append("Name                  :"); builder.Append(Name); builder.AppendLine();
            builder.AppendLine();
            for (int i = 0; i < Attributes.Length; i++)
            {
                builder.Append("ID "); builder.Append(i);                                                                       builder.AppendLine();
                builder.Append("--NetworkReplicate    :"); builder.Append(Attributes[i].m_NetworkReplicate);                                        builder.AppendLine();
                builder.Append("--IsNetworkDirty      :"); builder.Append(Attributes[i].m_NetworkReplicate.HasFlag(NetworkReplicateFlags.IsDirty)); builder.AppendLine();
                builder.Append("--Name                :"); builder.Append(Attributes[i].Name);                                                      builder.AppendLine();
                builder.Append("--Data                :"); builder.Append(Attributes[i].Data);                                                      builder.AppendLine();
            }
            return builder.ToString();
        }
    }
}