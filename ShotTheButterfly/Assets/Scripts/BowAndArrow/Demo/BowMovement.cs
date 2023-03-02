using DigitalRuby.BowAndArrow;
using Mono.Cecil;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.UIElements;

public class BowMovement : MonoBehaviour
{
    public BowScript bow;
    private bool isRotate = false;
    private int leftClicked = 0;
    private Vector3 mouseReference;
    // Update is called once per frame
    void Update()
    {
        if(Input.GetMouseButtonUp(0))
        {
            if(!isRotate)
            {
                var mousePos = Input.mousePosition;
                Debug.Log("Update: Mouse position: " + mousePos.ToString());
                transform.position = Camera.main.ScreenToWorldPoint(mousePos);
                transform.position = new Vector3(transform.position.x, transform.position.y, 0);
                bow.RenderBowString(Vector3.zero);
            }
            isRotate = false;
            leftClicked = 0;
        }
    }

    private void OnMouseDown()
    {
        Vector3 mousePos = new Vector3(Input.mousePosition.x, Input.mousePosition.y, 0);
        Vector3 lookPos = Camera.main.ScreenToWorldPoint(mousePos);
        lookPos = lookPos - transform.position;
        if (lookPos.x < 0)
        {
            leftClicked = -1;
        }
        else
        {
            leftClicked = 1;
        }
    }

    private void OnMouseDrag()
    {
        if(leftClicked == 0)
        {
            return;
        }
        Vector3 mousePos = new Vector3(Input.mousePosition.x, Input.mousePosition.y, 0);
        Vector3 lookPos = Camera.main.ScreenToWorldPoint(mousePos);
        lookPos = lookPos - transform.position;
        float angle;
        if (leftClicked == -1)
        {
            angle = Mathf.Atan2(-lookPos.y, -lookPos.x) * Mathf.Rad2Deg;
        }
        else
        {
            angle = Mathf.Atan2(lookPos.y, lookPos.x) * Mathf.Rad2Deg;
        }
        Debug.Log("OnMouseDrag: Angle = " + angle);
        transform.rotation = Quaternion.Euler(0, 0, angle);
        //transform.rotation = Quaternion.AngleAxis(angle, Vector3.back); 
        //transform.rotation = Quaternion.AngleAxis(angle, Vector3.forward);
        isRotate = true;
        bow.RenderBowString(Vector3.zero);
    }
}


