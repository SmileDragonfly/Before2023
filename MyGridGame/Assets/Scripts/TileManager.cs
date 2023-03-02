using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TileManager : MonoBehaviour
{
    [SerializeField] private Color darkColor, lightColor;
    [SerializeField] private SpriteRenderer render;
    private AnimalManager occuppiedAnimal;
    public void SetColor(int x, int y)
    {
        var isDark = (x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1);
        Color color = isDark ? darkColor : lightColor;
        render.color = color;
    }

    private void OnMouseOver()
    {
        if(Input.GetMouseButton(0))
        {
            //Debug.Log(name);
            var selectedAnimal = UnitManager.Instance.GetSelectedAnimal();
            if(selectedAnimal)
            {
                SetUnit(selectedAnimal);
            }
        }
    }

    private void OnMouseDown()
    {
        if(occuppiedAnimal)
        {
            UnitManager.Instance.SetSelectedAnimal(occuppiedAnimal);
        }
        else
        {
            UnitManager.Instance.SetSelectedAnimal(null);
        }
    }

    public void SetUnit(AnimalManager animal)
    {
        occuppiedAnimal = animal;
        if(occuppiedAnimal)
        {
            occuppiedAnimal.SetTile(this);
        }
    }
}
