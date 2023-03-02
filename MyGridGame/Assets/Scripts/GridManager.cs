using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridManager : MonoBehaviour
{
    public static GridManager Instance;
    [SerializeField] private int boardWidth, boardHeight;
    [SerializeField] private TileManager tile;
    [SerializeField] private Camera cam;
    private Dictionary<Vector2, TileManager> board;
    private void Awake()
    {
        Instance = this;
    }
    public void GenerateGrid()
    {
        board = new Dictionary<Vector2, TileManager>();
        for(int i = 0; i < boardWidth; i++)
        {
            for(int j = 0; j < boardHeight; j++)
            {
                TileManager spawnedTile = Instantiate(tile, new Vector3(i, j), Quaternion.identity);
                spawnedTile.name = $"Tile {i} {j}";
                spawnedTile.SetColor(i, j);
                board[new Vector2(i, j)] = spawnedTile;
            }
        }
        cam.transform.position = new Vector3((float)boardWidth / 2 - 0.5f, (float)boardHeight / 2 - 0.5f, -10);
        // Change state
        GameManager.Instance.ChangeState(GameState.SpawnAnimal);
    }

    public TileManager GetTile(int x, int y)
    {
        var tilePos = new Vector2(x, y);
        if(board.TryGetValue(tilePos, out var value))
        {
            return value;
        }
        else
        {
            return null;
        }
    }

    public Vector2 GetRandomPosition()
    {
        var x = Random.Range(0, boardWidth);
        var y = Random.Range(0, boardHeight);
        return new Vector2(x, y);
    }
}
