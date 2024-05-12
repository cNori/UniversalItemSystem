using System;
using FlaxEditor.Content;
using FlaxEditor;
using FlaxEngine;

namespace UniversalItemSystem
{
    /// <summary>
    /// The sample game plugin.
    /// </summary>
    /// <seealso cref="FlaxEngine.GamePlugin" />
    public class UniversalItemSystem : GamePlugin
    {
        /// <inheritdoc />
        public UniversalItemSystem()
        {
            _description = new PluginDescription
            {
                Name = "Universal Item System",
                Category = "Systems",
                Author = "cNori",
                AuthorUrl = null,
                HomepageUrl = null,
                RepositoryUrl = "https://github.com/FlaxEngine/UniversalItemSystem",
                Description = "Universal Item System Fits any game needing a item system.",
                Version = new Version(0, 0, 1),
                IsAlpha = true,
                IsBeta = false,
            };
        }

        /// <inheritdoc />
        public override void Initialize()
        {
            base.Initialize();
        }

        /// <inheritdoc />
        public override void Deinitialize()
        {
            // Use it to cleanup data
            base.Deinitialize();
        }
    }

    /// <summary>
    /// </summary>
    /// <seealso cref="FlaxEditor.EditorPlugin" />
    public class UniversalItemSystemEditor : EditorPlugin
    {
        private ItemAssetProxy _proxy;

        /// <inheritdoc />
        public override void InitializeEditor()
        {
            _proxy = new ItemAssetProxy();
            Editor.ContentDatabase.AddProxy(_proxy, true);
        }

        /// <inheritdoc />
        public override void DeinitializeEditor()
        {
            Editor.ContentDatabase.RemoveProxy(_proxy, true);
            _proxy = null;

            base.DeinitializeEditor();
        }
    }
}
