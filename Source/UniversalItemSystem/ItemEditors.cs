
using FlaxEngine;
using FlaxEditor.CustomEditors.Editors;
using UniversalItemSystem;
using FlaxEditor.CustomEditors;
using FlaxEngine.Utilities;
using FlaxEditor.CustomEditors.Elements;

namespace UniversalItemSystemEditor;

/// <summary>
/// 
/// </summary>
/// <seealso cref="FlaxEditor.CustomEditors.Editors.GenericEditor" />
[CustomEditor(typeof(Item.Attribute))]
public class ItemAttributeEditor : GenericEditor
{
    CustomEditor TypeEd;

    TextBoxElement textBoxElement;
    /// <summary>
    /// </summary>
    /// <param name="layout"></param>
    /// <inheritdoc />
    public override void Initialize(LayoutElementsContainer layout)
    {

        if (Values == null)
            return;
        if (Values[0] == null)
            return;
        var items = GetItemsForType(TypeUtils.GetObjectType(Values[0]),true,true);
        if (items.Count == 0)
            return;

        for (int i = 0; i < items.Count; i++)
        {
            if (items[i].Info.Name == "Data")
            {
                var l= layout.AddPropertyItem(items[i].Info.Name);
                textBoxElement = l.TextBox();
                if (Values[0] is Item.Attribute at)
                {
                    textBoxElement.TextBox.Text = at.Data;
                }
                textBoxElement.TextBox.EditEnd += TextBox_EditEnd;
                //layout.
                continue;
            }
            if (items[i].Info.Name == "Type")
            {
                TypeEd = layout.Property(items[i].Info.Name, items[i].GetValues(Values));
            }
            else
            {
                SpawnProperty(layout, items[i].GetValues(Values), items[i]);
                //layout.Property();
            }

        }

        //base.Initialize(layout);
    }

    private void TextBox_EditEnd()
    {
        if (textBoxElement == null)
            return;

        if (Values[0] is Item.Attribute at)
        {
            at.Data = textBoxElement.TextBox.Text;
            SynchronizeValue(at);
            SetValue(at);
            textBoxElement.TextBox.Text = at.Data;
            Refresh();
        }
    }
}
/// <summary>
/// </summary>
/// <seealso cref="FlaxEditor.CustomEditors.Editors.GenericEditor" />
[CustomEditor(typeof(Item.Data))]
public class ItemDataEditor : GenericEditor
{
    /// <summary>
    /// Gets the custom editor style.
    /// </summary>
    public override DisplayStyle Style => DisplayStyle.InlineIntoParent;
    /// <summary>
    /// </summary>
    /// <param name="layout"></param>
    /// <inheritdoc />
    public override void Initialize(LayoutElementsContainer layout)
    {

        if (Values == null)
            return;
        if (Values[0] == null)
            return;
        var items = GetItemsForType(TypeUtils.GetObjectType(Values[0]), true, false);
        if (items.Count == 0)
            return;

        //var vp = layout.VerticalPanel();
        //vp.Panel.AnchorPreset = FlaxEngine.GUI.AnchorPresets.HorizontalStretchTop;
        //var vp = layout.VerticalPanel();
        //var hp = layout.HorizontalPanel();
        //hp.Panel.AnchorPreset = FlaxEngine.GUI.AnchorPresets.HorizontalStretchTop;
        layout.Button("Load").Button.Clicked += LoadFromAsset;
        layout.Button("Save").Button.Clicked += SaveToAsset;

        for (int i = 0; i < items.Count; i++)
        {
            SpawnProperty(layout, items[i].GetValues(Values), items[i]);
        }
    }

    private void LoadFromAsset()
    {
        if (Values[0] is Item.Data dat)
        {
            if (dat.ItemAsset == null) return;
            Debug.Log(dat);
            dat.LoadFromAsset();
            Debug.Log(dat);
            SetValue(dat);
            Refresh();
        }
    }
    private void SaveToAsset()
    {
        if (Values[0] is Item.Data dat)
        {
            if (dat.ItemAsset == null) return;
            dat.SaveToAsset();
            SetValue(dat);
            Refresh();
        }
    }
}