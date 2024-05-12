﻿using Flax.Build;

public class UniversalItemSystemTarget : GameProjectTarget
{
    /// <inheritdoc />
    public override void Init()
    {
        base.Init();

        // Reference the modules for game
        Modules.Add("UniversalItemSystem");
    }
}
