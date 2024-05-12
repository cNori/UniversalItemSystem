using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace UniversalItemSystem
{
    public partial class Item
    {
#pragma warning disable format // @formatter:off
    
        internal static Dictionary<Type, AttributeDataType> LookUpTable = new()
        {
            { typeof(byte)  , new AttributeDataType(Attribute.DataType.UnsindedInteger ,Unsafe.SizeOf<byte  >())},
            { typeof(ushort), new AttributeDataType(Attribute.DataType.UnsindedInteger ,Unsafe.SizeOf<ushort>())},
            { typeof(uint)  , new AttributeDataType(Attribute.DataType.UnsindedInteger ,Unsafe.SizeOf<uint  >())},
            { typeof(ulong) , new AttributeDataType(Attribute.DataType.UnsindedInteger ,Unsafe.SizeOf<ulong >())},
            { typeof(sbyte) , new AttributeDataType(Attribute.DataType.Integer         ,Unsafe.SizeOf<sbyte >())},
            { typeof(short) , new AttributeDataType(Attribute.DataType.Integer         ,Unsafe.SizeOf<short >())},
            { typeof(int)   , new AttributeDataType(Attribute.DataType.Integer         ,Unsafe.SizeOf<int   >())},
            { typeof(long)  , new AttributeDataType(Attribute.DataType.Integer         ,Unsafe.SizeOf<long  >())},
            { typeof(float) , new AttributeDataType(Attribute.DataType.Decimal         ,Unsafe.SizeOf<float >())},
            { typeof(double), new AttributeDataType(Attribute.DataType.Decimal         ,Unsafe.SizeOf<double>())},
            { typeof(string), new AttributeDataType(Attribute.DataType.Text            ,Unsafe.SizeOf<string>())},
            { typeof(bool)  , new AttributeDataType(Attribute.DataType.Bool            ,Unsafe.SizeOf<bool  >())},
        };
#pragma warning restore format // @formatter:on

        internal static unsafe void ObjToByteArr<T>(ref T obj, ref byte[] dest) where T : unmanaged
        {
            fixed (void* pSrc = &obj, pDest = dest)
            {
                Buffer.MemoryCopy(pSrc, pDest, dest.Length, sizeof(T));
            }
        }
        internal static unsafe void ByteArrToObj<T>(ref T obj, ref byte[] dest) where T : unmanaged
        {
            fixed (void* pSrc = dest, pDest = &obj)
            {
                Buffer.MemoryCopy(pSrc, pDest, dest.Length, sizeof(T));
            }
        }
        internal int Internal_GetAttributeID<T>(string InName, out AttributeDataType? OutType)
        {
            OutType = null;
            if (string.IsNullOrEmpty(InName))
                return -1;

            Type Ttype = typeof(T);
            if (Ttype == typeof(Enum))
            {
                Ttype = Enum.GetUnderlyingType(Ttype);
            }
            if (LookUpTable.TryGetValue(Ttype, out AttributeDataType t))
            {
                OutType = t;
                for (int i = 0; i < ItemData.Attributes.Length; i++)
                {
                    if (ItemData.Attributes[i].Type == OutType?.AttributeType)
                    {
                        if ((ItemData.Attributes[i].Name == InName))
                        {
                            return i;
                        }
                    }
                }
            }
            return -1;
        }
    }
}
