using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public float fSpeed = 50f, fMaxSpeed = 3f, fJumPow = 220f;
    public Rigidbody2D r2;
    public bool bGrounded = true;
    public Animator anim;
    public bool bFaceRight = true;
    public bool bDoubleJump = false;

    // Start is called before the first frame update
    void Start()
    {
        r2 = gameObject.GetComponent<Rigidbody2D>();
        anim = gameObject.GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        anim.SetBool("Grounded", bGrounded);
        anim.SetFloat("Speed", Mathf.Abs(r2.velocity.x));

        if(Input.GetKeyDown(KeyCode.Space))
        {
            if(bGrounded)
            {
                bGrounded = false;
                bDoubleJump = true;
                r2.AddForce(Vector2.up * fJumPow);
            }
            else
            {
                if(bDoubleJump)
                {
                    bDoubleJump = false;
                    r2.velocity = new Vector2(r2.velocity.x, 0);
                    r2.AddForce(Vector2.up * fJumPow * 0.7f);
                }
            }
        }
    }

    void FixedUpdate()
    {
        float h = Input.GetAxis("Horizontal");
        r2.AddForce((Vector2.right) * fSpeed * h);

        if(r2.velocity.x > fMaxSpeed)
        {
            r2.velocity = new Vector2(fMaxSpeed, r2.velocity.y);
        }
        if(r2.velocity.x < -fMaxSpeed)
        {
            r2.velocity = new Vector2(-fMaxSpeed, r2.velocity.y);
        }

        if((h > 0) && !bFaceRight)
        {
            Flip();
        }

        if((h < 0) && bFaceRight)
        {
            Flip();
        }
    }

    void Flip()
    {
        bFaceRight = !bFaceRight;
        Vector3 scale;
        scale = transform.localScale;
        scale.x *= -1;
        transform.localScale = scale;
    }
}
