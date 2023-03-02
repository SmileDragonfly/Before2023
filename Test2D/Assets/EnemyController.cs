using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class EnemyController : MonoBehaviour
{
    public float speed;
    // Update is called once per frame
    //void Update()
    //{
    //    FollowMouse();
    //}

    public void SetSpeed(float fSpeed)
    {
        speed = fSpeed;
    }

    private void FollowMouse()
    {
        Vector3 mousePos = Input.mousePosition;
        mousePos = Camera.main.ScreenToWorldPoint(mousePos);
        Debug.Log($"Mouse position: {mousePos.x}-{mousePos.y}");
        transform.position = Vector2.MoveTowards(transform.position, mousePos, speed * Time.deltaTime);
    }

    public void FollowTarget(Vector3 targetPos)
    {
        Debug.Log($"Target position: {targetPos.x}-{targetPos.y}");
        RotateFollowTarget(targetPos);
        transform.position = Vector2.MoveTowards(transform.position, targetPos, speed * Time.deltaTime);
    }

    private void RotateFollowTarget(Vector3 targetPos)
    {
        Vector3 direction = targetPos - transform.position;
        float angle = Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg - transform.rotation.eulerAngles.z;
        transform.Rotate(0, 0, angle);
    }
}
