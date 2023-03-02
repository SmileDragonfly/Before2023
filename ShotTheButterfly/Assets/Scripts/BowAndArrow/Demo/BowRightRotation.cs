using DigitalRuby.BowAndArrow;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BowRightRotation : MonoBehaviour
{
    public BowScript bow;
    private void OnMouseDrag()
    {
        bow.RotateRight();
    }
}
