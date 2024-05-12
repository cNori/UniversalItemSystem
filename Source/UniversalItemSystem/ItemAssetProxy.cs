using FlaxEditor;
using FlaxEditor.Content;
using FlaxEditor.CustomEditors;
using FlaxEditor.GUI;
using FlaxEditor.Windows;
using FlaxEditor.Windows.Assets;
using FlaxEngine;
using FlaxEngine.GUI;
using FlaxEngine.Json;
using System;

namespace UniversalItemSystem
{

    /// <summary>
    /// 
    /// </summary>
    /// <seealso cref="FlaxEditor.Content.JsonAssetItem" />
    public class ItemAssetJsonAssetItem : JsonAssetItem
    {
        /// <inheritdoc />
        public ItemAssetJsonAssetItem(string path, Guid id, string typeName)
        : base(path, id, typeName)
        {
            // Use custom icon (Sprite)
            //_thumbnail = Editor.Instance.Icons.Document128;
        }
    }
    /// <summary>
    /// 
    /// </summary>
    /// <seealso cref="FlaxEditor.Content.JsonAssetProxy" />
    [ContentContextMenu("New/Universal Item System/Item Asset")]
    public sealed class ItemAssetProxy : SpawnableJsonAssetProxy<Item.Asset>
    {
        /// <inheritdoc />
        public override Color AccentColor => Color.FromRGB(0xd14f67);

        /// <inheritdoc />
        public override EditorWindow Open(Editor editor, ContentItem item)
        {
            return new ItemAssetEditor(editor, (JsonAssetItem)item);
        }

        /// <inheritdoc />
        public override AssetItem ConstructItem(string path, string typeName, ref Guid id)
        {
            return new ItemAssetJsonAssetItem(path, id, typeName);
        }
        //public override void Create(string outputPath, object arg)
        //{
        //    base.Create(outputPath, arg);
        //}
        /// <summary>
        /// Gets the full name of the asset type (stored data format).
        /// </summary>
        public override string TypeName => typeof(Item.Asset).FullName;
    }

    internal class ItemAssetEditor : AssetEditorWindowBase<JsonAsset>
    {
        private readonly CustomEditorPresenter _presenter;
        private readonly ToolStripButton _saveButton;
        private readonly ToolStripButton _undoButton;
        private readonly ToolStripButton _redoButton;
        private readonly Undo _undo;
        private Label _typeText;
        Item.Asset assetInstance = new Item.Asset();

        /// <inheritdoc />
        public ItemAssetEditor(Editor editor, AssetItem item)
        : base(editor, item)
        {
            var inputOptions = Editor.Options.Options.Input;

            // Undo
            _undo = new Undo();
            _undo.UndoDone += OnUndoRedo;
            _undo.RedoDone += OnUndoRedo;
            _undo.ActionDone += OnUndoRedo;

            // Toolstrip
            _saveButton = (ToolStripButton)_toolstrip.AddButton(editor.Icons.Save64, Save).LinkTooltip("Save");
            _toolstrip.AddSeparator();
            _undoButton = (ToolStripButton)_toolstrip.AddButton(Editor.Icons.Undo64, _undo.PerformUndo).LinkTooltip($"Undo ({inputOptions.Undo})");
            _redoButton = (ToolStripButton)_toolstrip.AddButton(Editor.Icons.Redo64, _undo.PerformRedo).LinkTooltip($"Redo ({inputOptions.Redo})");

            // Panel
            var panel = new Panel(ScrollBars.Vertical)
            {
                AnchorPreset = AnchorPresets.StretchAll,
                Offsets = new Margin(0, 0, _toolstrip.Bottom, 0),
                Parent = this
            };

            // Properties
            _presenter = new CustomEditorPresenter(_undo, "Loading...");
            _presenter.Panel.Parent = panel;
            _presenter.Modified += MarkAsEdited;

            // Setup input actions
            InputActions.Add(options => options.Undo, _undo.PerformUndo);
            InputActions.Add(options => options.Redo, _undo.PerformRedo);
        }

        private void OnUndoRedo(IUndoAction action)
        {
            MarkAsEdited();
            UpdateToolstrip();
        }

        /// <inheritdoc />
        public override void Save()
        {
            if (!IsEdited)
                return;
            if (_asset.WaitForLoaded())
                return;
            UIS.SaveItemAsset(Asset, (Item.Asset)_presenter.Selection[0]);

            ClearEditedFlag();
        }

        /// <inheritdoc />
        protected override void UpdateToolstrip()
        {
            _saveButton.Enabled = IsEdited;
            _undoButton.Enabled = _undo.CanUndo;
            _redoButton.Enabled = _undo.CanRedo;

            base.UpdateToolstrip();
        }

        /// <inheritdoc />
        protected override void OnAssetLoaded()
        {
            UIS.LoadItemAsset(Asset,ref assetInstance);
            
            _presenter.Select(assetInstance);

            if (_typeText != null)
                _typeText.Dispose();
            var typeText = new ClickableLabel
            {
                Text = $"{Asset.DataTypeName}",
                TooltipText = "Asset data type (full name)",
                AnchorPreset = AnchorPresets.TopRight,
                AutoWidth = true,
                Parent = this,
            };
            typeText.LocalX += -(typeText.Width + 4);
            typeText.LocalY += (_toolstrip.Height - typeText.Height) * 0.5f;
            typeText.RightClick = () => Clipboard.Text = Asset.DataTypeName;
            _typeText = typeText;

            _undo.Clear();
            ClearEditedFlag();
            base.OnAssetLoaded();
        }

        /// <inheritdoc />
        protected override void OnAssetLoadFailed()
        {
            _presenter.NoSelectionText = "Failed to load the asset.";

            base.OnAssetLoadFailed();
        }

        /// <inheritdoc />
        public override void OnItemReimported(ContentItem item)
        {
            // Refresh the properties (will get new data in OnAssetLoaded)
            _presenter.Deselect();
            ClearEditedFlag();

            base.OnItemReimported(item);
        }

        /// <inheritdoc />
        public override void OnDestroy()
        {
            _typeText = null;
            base.OnDestroy();
        }
    }
}
