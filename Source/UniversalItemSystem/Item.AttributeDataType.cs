namespace UniversalItemSystem;

public partial class Item
{
    /// <summary>
    /// </summary>
    internal struct AttributeDataType
    {
        /// <summary>
        /// Gets the type of the attribute.
        /// </summary>
        /// <value>
        /// The type of the attribute.
        /// </value>
        public readonly Attribute.DataType AttributeType { get;}
        /// <summary>
        /// Gets the size of.
        /// </summary>
        /// <value>
        /// The size of.
        /// </value>
        public readonly byte SizeOf { get;}
        /// <summary>
        /// Initializes a new instance of the <see cref="AttributeDataType"/> struct.
        /// </summary>
        /// <param name="type">The type.</param>
        /// <param name="size">The size.</param>
        public AttributeDataType(Attribute.DataType type,int size) 
        {
            SizeOf = (byte)size;
            AttributeType = type;
        }
    }



}