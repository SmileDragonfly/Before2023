using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyController : MonoBehaviour
{
    GameObject player;
    public Transform target;
    Rigidbody2D myRigidbody;
    public float minSpeed = 0.1f;
    public float maxSpeed = 0.4f;
    public float moveSpeed;

    // Start is called before the first frame update
    void Start()
    {
        player = GameObject.FindGameObjectWithTag("Player");
        myRigidbody = GetComponent<Rigidbody2D>();
        target = GameObject.FindGameObjectWithTag("Player").GetComponent<Transform>();
        moveSpeed = Random.Range(minSpeed, maxSpeed);
    }

    // Update is called once per frame
    void Update()
    {
        Follow();
    }

    //void OnTriggerExit2D(Collider2D collision)
    //{
    //    transform.localScale = new Vector2(-(Mathf.Sign(myRigidbody.velocity.x)) * transform.localScale.x, transform.localScale.y);
    //}

    void Follow()
    {
        Vector3 direction = target.position - transform.position;
        RotateFollow(direction);
        MoveFollow(direction);
    }

    void RotateFollow(Vector3 direction)
    {
        float angle = Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg;
        transform.rotation = Quaternion.Euler(new Vector3(0, 0, angle));
    }

    void MoveFollow(Vector3 direction)
    {
        transform.Translate(direction * Time.deltaTime * moveSpeed, Space.World);
    }
}