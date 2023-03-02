using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public class MainManager : MonoBehaviour
{
    public static MainManager instance;
    public Color teamColor;
    [System.Serializable]
    class SaveData
    {
        public Color teamColor;
    }
    private void Awake()
    {
        // start of new code
        if (instance != null)
        {
            Destroy(gameObject);
            return;
        }
        // end of new code
        instance = this;
        DontDestroyOnLoad(gameObject);
        LoadColor();
    }

    public void SaveColor()
    {
        SaveData data = new SaveData();
        data.teamColor = teamColor;
        string json = JsonUtility.ToJson(data);
        File.WriteAllText(Application.persistentDataPath + "/savefile.json", json);
        Debug.Log("Save file: " + Application.persistentDataPath);
    }

    public void LoadColor()
    {
        string sPath = Application.persistentDataPath + "/savefile.json";
        if(File.Exists(sPath))
        {
            string json = File.ReadAllText(sPath);
            SaveData data = JsonUtility.FromJson<SaveData>(json);
            teamColor = data.teamColor;
        }
    }
}
