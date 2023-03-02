using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEditor;
using System.IO;

public class MainManagerUI : MonoBehaviour
{
    public static MainManagerUI instance;
    public string bestUser;
    public int bestScore;
    public string user;
    public Text bestScoreText;

    // UI components
    public InputField nameField;

    [System.Serializable]
    class BestInfor
    {
        public string user;
        public int score;
    }
    private void Awake()
    {
        if(instance != null)
        {
            Destroy(gameObject);
            return;
        }
        instance = this;
        LoadBestScore();
        DontDestroyOnLoad(gameObject);
    }

    public void SetUser()
    {
        user = nameField.text;
    }

    public void LoadMain()
    {
        if(user.Length == 0)
        {
            Debug.Log("Please type user name");
        }
        else
        {
            SceneManager.LoadScene(1);
        }
    }

    public void Exit()
    {
#if UNITY_EDITOR
        EditorApplication.ExitPlaymode();
#else
        Application.Quit();
#endif
    }

    public void LoadBestScore()
    {
        string path = Application.persistentDataPath + "/BestScore.json";
        Debug.Log(path);
        string json = File.ReadAllText(path);
        BestInfor data = JsonUtility.FromJson<BestInfor>(json);
        bestUser = data.user;
        bestScore = data.score;
        bestScoreText.text = "Best Score: " + bestUser + " - " + bestScore;
    }

    public void SaveBestScore()
    {
        BestInfor data = new BestInfor();
        data.user = MainManagerUI.instance.user;
        data.score = bestScore;
        string json = JsonUtility.ToJson(data);
        string path = Application.persistentDataPath + "/BestScore.json";
        File.WriteAllText(path, json);
    }
}
