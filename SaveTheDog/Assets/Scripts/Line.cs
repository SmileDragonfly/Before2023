using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Line : MonoBehaviour
{
    public LineRenderer lineRenderer;
    public EdgeCollider2D edgeCollider;
    public Rigidbody2D rb;

    [HideInInspector] public List<Vector2> points = new List<Vector2>();
    [HideInInspector] public int pointsCount = 0;
    private float pointsMinDistance = 0.1f;
    private float circleColliderRadius;

    public Vector2 GetLastPoint()
    {
        return (Vector2)lineRenderer.GetPosition(pointsCount - 1);
    }

    public void UsePhysics(bool usePhysics, bool lineMassByLength)
    {
        rb.isKinematic = !usePhysics;
        if(usePhysics && lineMassByLength)
        {
            rb.mass = GetLineLength();
        }
    }

    public void SetLineColor(Gradient lineColor)
    {
        lineRenderer.colorGradient = lineColor;
    }

    public void SetLineWidth(float width)
    {
        lineRenderer.startWidth = width;
        lineRenderer.endWidth = width;
        edgeCollider.edgeRadius = width / 2f;
        circleColliderRadius = width / 2f;
    }

    public void AddPoint(Vector2 newPoint)
    {
        if(pointsCount >= 1 && Vector2.Distance(newPoint, GetLastPoint()) < pointsMinDistance)
        {
            return;
        }
        points.Add(newPoint);
        pointsCount++;

        // Add circle collider to the point
        CircleCollider2D circleCollider2D = this.gameObject.AddComponent<CircleCollider2D>();
        circleCollider2D.offset = newPoint;
        circleCollider2D.radius = circleColliderRadius;

        // LineRenderer
        lineRenderer.positionCount = pointsCount;
        lineRenderer.SetPosition(pointsCount - 1, newPoint);

        // EdgeCollider
        if(pointsCount > 1)
        {
            edgeCollider.points = points.ToArray();
        }
    }

    public void SetPointsMinDistance(float distance)
    {
        pointsMinDistance = distance;
    }

    public float GetLineLength()
    {
        float iLength = 0;
        for(int i = 0; i < lineRenderer.positionCount - 1; i++)
        {
            iLength += Vector2.Distance(lineRenderer.GetPosition(i), lineRenderer.GetPosition(i + 1));
        }
        return iLength;
    }

    private void OnBecameInvisible()
    {
        Destroy(gameObject);
    }
}
