using System;
using System.Runtime.CompilerServices;

namespace UniversalItemSystem;

public partial class Item
{
    /// <summary>
    /// Gets Attribute with type of <typeparamref name="T"/> and nameof <paramref name="InName"/>
    /// </summary>
    /// <typeparam name="T">type of <see cref="Attribute.Data"/></typeparam>
    /// <param name="InName"><see cref="Attribute.Name"/></param>
    /// <returns><typeparamref name="T"/> or <see langword="default"/> if not found or is invalid</returns>
    public T GetAttribute<T>(string InName) where T : unmanaged
    {
        var id = Internal_GetAttributeID<T>(InName, out AttributeDataType? type);

        T val = default;
        if (id == -1) return val;
        if (Unsafe.SizeOf<T>() == type.Value.SizeOf)
        {
            ByteArrToObj(ref val, ref ItemData.Attributes[id].m_Data);
        }
        return val;
    }

    /// <summary>
    /// Gets Attribute with type of <typeparamref name="T"/> and ID of <paramref name="ID"/>
    /// </summary>
    /// <typeparam name="T">type of <see cref="Attribute.Data"/></typeparam>
    /// <param name="ID"><see cref="Attribute"/> index</param>
    /// <returns><typeparamref name="T"/> or <see langword="default"/> if not found or is invalid</returns>
    public T GetAttribute<T>(int ID) where T : unmanaged
    {
        var id = ID;
        T val = default;
        if (id == -1) return val;
        if (Unsafe.SizeOf<T>() <= ItemData.Attributes[id].m_Data.Length)
        {
            ByteArrToObj(ref val, ref ItemData.Attributes[id].m_Data);
        }
        return val;
    }

    /// <summary>
    /// finds attribute index by name
    /// </summary>
    /// <param name="InName"><see cref="Attribute.Name"/></param>
    /// <returns><see cref="Attribute"/> ID or -1 if not found</returns>
    public int GetAttributeID(string InName)
    {
        if (string.IsNullOrEmpty(InName))
            return -1;
        for (int i = 0; i < ItemData.Attributes.Length; i++)
        {
            if ((   ItemData.Attributes[i].Name == InName))
                return i;
        }
        return -1;
    }

    /// <summary>
    /// Gets Attribute with type of <typeparamref name="T"/> and nameof <paramref name="InName"/>
    /// </summary>
    /// <typeparam name="T">type of <see cref="Attribute.Data"/></typeparam>
    /// <param name="NewData"><see cref="Attribute.Data"/></param>
    /// <param name="InName"><see cref="Attribute.Name"/></param>
    /// <returns><see langword="true"/> on success else <see langword="false"/></returns>
    public bool SetAttribute<T>(T NewData, string InName) where T : unmanaged
    {
        var id = Internal_GetAttributeID<T>(InName, out AttributeDataType? type);
        if (id == -1) return false;
        return SetAttribute(NewData, id);
    }
    /// <summary>
    /// Sets Attribute of <paramref name="ID"/> with <paramref name="NewData"/>
    /// </summary>
    /// <param name="NewData"><see cref="Attribute.Data"/></param>
    /// <param name="ID"></param>
    /// <returns><see langword="true"/> on success else <see langword="false"/></returns>
    public bool SetAttribute<T>(T NewData, int ID) where T : unmanaged
    {
        
        if (ID >= 0 && ID < ItemData.Attributes.Length)
        {
            Type Ttype = typeof(T);
            if (Ttype == typeof(Enum))
            {
                Ttype = Enum.GetUnderlyingType(Ttype);
            }
            if (LookUpTable.TryGetValue(Ttype, out AttributeDataType t))
            {
                ItemData.Attributes[ID].m_Data = new byte[t.SizeOf];
                ObjToByteArr(ref NewData, ref ItemData.Attributes[ID].m_Data);
                return true;
            }
        }
        return false;
    }

    /// <summary>
    /// Sets Attribute of <paramref name="ID"/> with <paramref name="NewData"/>
    /// </summary>
    /// <param name="NewData"><see cref="Attribute.Data"/></param>
    /// <param name="ID"></param>
    /// <returns><see langword="true"/> on success else <see langword="false"/></returns>
    public bool SetAttribute(ref readonly Attribute NewData, int ID)
    {
        if (ID >= 0 && ID < ItemData.Attributes.Length)
        {
            ItemData.Attributes[ID] = NewData;
            return true;
        }
        return false;
    }
}