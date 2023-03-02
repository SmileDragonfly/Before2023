using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PanelSettingController : MonoBehaviour
{
    private GameObject prevPanel;
    public void Active(GameObject gameObj)
    {
        gameObject.SetActive(true);
        gameObj.SetActive(false);
        prevPanel = gameObj;
    }

    public void DeActive()
    {
        gameObject.SetActive(false);
        prevPanel.SetActive(true);
        prevPanel = null;
    }    
}
