using Flax.Build;

public class UniversalItemSystemEditorTarget : GameProjectEditorTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // Reference the modules for editor
        Modules.Add("UniversalItemSystem");
        Modules.Add("UniversalItemSystemEditor");
    }
}
