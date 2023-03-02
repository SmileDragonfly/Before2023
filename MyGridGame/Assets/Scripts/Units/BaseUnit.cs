using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BaseUnit : MonoBehaviour
{
    [SerializeField] protected Races raceName;
    [SerializeField] protected Classes className;
    [SerializeField] protected string unitName;
    [SerializeField] protected int star;             // current level 1star*3->2star*3->3star
    [SerializeField] protected int hp;               // health point
    [SerializeField] protected int atk;              // attack point
    [SerializeField] protected int armor;            // armor point
    [SerializeField] protected double atkSpeed;      // attack speed point
    [SerializeField] protected int magRes;           // magic resistance point
    [SerializeField] protected int range;            // attack range
    [SerializeField] protected int hpRegen;          // hp regeneration
    [SerializeField] public string background;       // story about character
}
