using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LineDrawer : MonoBehaviour
{
    public GameObject linePrefabs;
    public LayerMask cantDrawOverLayer;
    int cantDrawOverLayerIndex;
    public float linePointsMinDistance;
    public float lineWidth;
    public Gradient lineColor;
    public bool lineMassByLength;
    public bool oneLineOnly;
    public GameObject player;

    Line currentLine;
    private bool isDrew = false;

    private void Start()
    {
        cantDrawOverLayerIndex = LayerMask.NameToLayer("CantDrawOver");
    }

    private void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            if(!isDrew)
            {
                BeginDraw();
            }
        }
        if (currentLine != null)
        {
            Draw();
        }
        if (Input.GetMouseButtonUp(0))
        {
            EndDraw();
        }
    }

    private void BeginDraw()
    {
        currentLine = Instantiate(linePrefabs, this.transform).GetComponent<Line>();
        currentLine.SetLineColor(lineColor);
        currentLine.SetLineWidth(lineWidth);
        currentLine.SetPointsMinDistance(linePointsMinDistance);
        currentLine.UsePhysics(false, lineMassByLength);
    }

    private void Draw()
    {
        Vector2 mousePos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        //RaycastHit2D hit = Physics2D.CircleCast(mousePos, lineWidth / 3f, Vector2.zero, 1f, cantDrawOverLayer);
        if(currentLine.pointsCount > 0)
        {
            RaycastHit2D hit = Physics2D.Linecast(currentLine.GetLastPoint(), mousePos, cantDrawOverLayer);
            if (hit)
            {
                //EndDraw();
                return;
            }
            else
            {
                currentLine.AddPoint(mousePos);
            }
        }
        else
        {
            currentLine.AddPoint(mousePos);
        }
    }

    private void EndDraw()
    {
        if (currentLine != null)
        {
            if(currentLine.pointsCount < 2)
            {
                Destroy(currentLine.gameObject);
            }
            else
            {
                if(oneLineOnly)
                {
                    isDrew = true;
                }
                currentLine.gameObject.layer = cantDrawOverLayerIndex;
                currentLine.UsePhysics(true, lineMassByLength);
                currentLine = null;
                // Start player
                player.GetComponent<PlayerController>().StartPlayer();
            }
        }
    }
}
