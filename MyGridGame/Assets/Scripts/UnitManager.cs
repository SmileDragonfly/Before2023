using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UnitManager : MonoBehaviour
{
    public static UnitManager Instance;
    public List<AnimalManager> availableAnimals;
    public Dictionary<string, AnimalManager> dictAnimalsLv1;
    public Dictionary<string, AnimalManager> dictAnimalsLv2;
    private AnimalManager selectedAnimal;
    private void Awake()
    {
        Instance = this;
    }
    // This script use to spawn any unit in the board
    public void SpawnAnimalAtRandomTile()
    {
        var pos = GridManager.Instance.GetRandomPosition();
        var random = Random.Range(0, availableAnimals.Count);
        var randomAnimal = availableAnimals[random];
        var spawnedAnimal = Instantiate(randomAnimal, new Vector3(pos.x, pos.y), Quaternion.identity);
        var tile = GridManager.Instance.GetTile((int)pos.x, (int)pos.y);
        if(tile)
        {
            Debug.Log($"SpawnAnimalAtRandomTile: pos:{pos.x}, {pos.y}");
            tile.SetUnit(spawnedAnimal);
        }
        //GameManager.Instance.ChangeState(GameState.InGame);
    }

    public void SetSelectedAnimal(AnimalManager animal)
    {
        selectedAnimal = animal;
    }

    public AnimalManager GetSelectedAnimal()
    {
        return selectedAnimal;
    }
}
