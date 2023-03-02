using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AnimalManager : MonoBehaviour
{
    private TileManager occuppiedTile;
    public void SetTile(TileManager tile)
    {
        if(occuppiedTile != tile)
        {
            // Clear occuppied unit on old tile
            if(occuppiedTile)
            {
                occuppiedTile.SetUnit(null);
            }
            // Set new tile
            occuppiedTile = tile;
            transform.position = occuppiedTile.transform.position;
        } 
    }
}
