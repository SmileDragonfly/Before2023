using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnitType : MonoBehaviour
{
    public static UnitType instance;
    private void Awake()
    {
        instance = this;
    }
}

public enum Races
{
    Dog,
    Cat,
    Lion,
    Tiger,
    Pig,
    Buffalo,
    Cow,
    Cock
}

public enum Classes
{
    Farmer,
    Boss,
    Trader,
    Artist
}