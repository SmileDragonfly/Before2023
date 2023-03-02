using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public static GameManager Instance;
    private void Awake()
    {
        Instance = this;
    }
    private void Start()
    { 
        ChangeState(GameState.GenerateGrid);
    }
    public void ChangeState(GameState state)
    {
        switch (state)
        {
            case GameState.GenerateGrid:
                GridManager.Instance.GenerateGrid();
                break;
            case GameState.StartUI:
                break;
            case GameState.SpawnAnimal:
                UnitManager.Instance.SpawnAnimalAtRandomTile();
                UnitManager.Instance.SpawnAnimalAtRandomTile();
                ChangeState(GameState.InGame);
                break;
            case GameState.InGame:
                break;
            case GameState.EndGame:
                break;
            default:
                break;
        }
    }
}

public enum GameState
{
    GenerateGrid,
    StartUI,
    SpawnAnimal,
    InGame,
    EndGame
}
