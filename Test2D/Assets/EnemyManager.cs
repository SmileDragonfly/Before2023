using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class EnemyManager : MonoBehaviour
{
    public Transform target;
    public float minSpeed = 0.2f;
    public float maxSpeed = 0.3f;
    public GameObject enemyPrefab;
    public int numberOfEnemy;

    private GameObject[] gameObjects;
    // Start is called before the first frame update
    void Start()
    {
        gameObjects = new GameObject[numberOfEnemy]; 
        if (enemyPrefab != null)
        {
            for(int i = 0; i < numberOfEnemy; i++)
            {
                Vector3 enemyPos = new Vector3(transform.position.x + Random.Range(-2f, 2f),
                    transform.position.y + Random.Range(-2f, 2f),
                    transform.position.z);
                gameObjects[i] = Instantiate(enemyPrefab, enemyPos, Quaternion.identity);
                gameObjects[i].GetComponent<EnemyController>().SetSpeed(Random.Range(minSpeed, maxSpeed));
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        foreach(GameObject go in gameObjects)
        {
            go.GetComponent<EnemyController>().FollowTarget(target.transform.position);
        }
    }
}
