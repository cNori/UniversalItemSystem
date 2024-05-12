using FlaxEngine;
using System;

namespace UniversalItemSystem
{
    internal static class UISLogger
    {
        const string b = "\n[Universal Item System]\n";
        internal static void Ex(string Massange)
        {
            Debug.LogException(new Exception(b + Massange));
        }
    }
}
