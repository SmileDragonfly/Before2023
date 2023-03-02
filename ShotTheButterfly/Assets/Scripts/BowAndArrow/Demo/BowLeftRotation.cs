using DigitalRuby.BowAndArrow;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BowLeftRotation : MonoBehaviour
{
    public BowScript bow;
    private void OnMouseDrag()
    {
        bow.RotateLeft();
    }
}
