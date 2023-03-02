using DigitalRuby.BowAndArrow;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class BowRotation : MonoBehaviour
{
    public FixedJoystick joystick;
    public BowScript bow;
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        // 02/10/2022-DatDT: Rotation object
        Vector3 moveVector = (Vector3.up * joystick.Horizontal + Vector3.left * joystick.Vertical);
        if (joystick.Horizontal != 0 || joystick.Vertical != 0)
        {
            transform.rotation = Quaternion.LookRotation(Vector3.forward, moveVector);
            bow.RenderBowString(Vector3.zero);
        }
    }
}
