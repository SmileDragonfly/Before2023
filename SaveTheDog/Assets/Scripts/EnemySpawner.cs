using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemySpawner : MonoBehaviour
{
    public GameObject enemyPrefab;
    public int enemyNumber;
    public Vector2 spawnerBound;
    private GameObject[] enemies;

    private void Start()
    {
        enemies = new GameObject[enemyNumber];
        for(int i = 0; i < enemyNumber; i++)
        {
            Vector3 pos = new Vector3(Random.Range(transform.position.x - spawnerBound.x, transform.position.x + spawnerBound.x),
                                        Random.Range(transform.position.y - spawnerBound.y, transform.position.y + spawnerBound.y),
                                        0);
            var obj = Instantiate(enemyPrefab, pos, Quaternion.identity);
            enemies[i] = obj;
        }
    }
}
