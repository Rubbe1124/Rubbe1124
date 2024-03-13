using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Dialogue_Adapt : MonoBehaviour
{
    private Dialogue_Data dialogue_data;

    [SerializeField] GameObject dialogue_Prefab;

    GameObject now_Dialogue_Prefab;

    public GameObject scrollbar;

    public GameObject gameManager;

    Game_Data_JSON game_Data_JSON;

    public bool dialogue_print = false; 
    public bool printing_With_Typing = false; 
    public bool make_dialogue_coroutine_is_running; 
    public bool make_space = false;

    public void Start()
    {
        game_Data_JSON = gameManager.GetComponent<Game_Data_JSON>();
    }

    Coroutine coroutine;

    public void Adapt_Dialogue_Contents(Dialogue_Data data)
    {
        dialogue_data = data;

        dialogue_print = false;
        printing_With_Typing = false;

        StartCoroutine(make_dialogue_prefab());
    }


    public int dialogue_Content_Count = 0;
    IEnumerator make_dialogue_prefab()
    {
        make_dialogue_coroutine_is_running = true;
        for (dialogue_Content_Count = 0; dialogue_Content_Count < dialogue_data.dialogue_Contents.Count; dialogue_Content_Count++)
        {
            dialogue_print = true;

            if (!Check_Condition(dialogue_data.dialogue_Contents[dialogue_Content_Count]))
            {
                if (dialogue_Content_Count == dialogue_data.dialogue_Contents.Count - 1)
                {
                    make_dialogue_coroutine_is_running = false;
                }
                continue;
            }

            GameObject newEnemy = Instantiate(dialogue_Prefab, transform);

            if (!make_space)
            {
                newEnemy.transform.Find("Space").gameObject.SetActive(false);
                make_space = true;
            }
            else
            {
                make_space = true;
            }

            now_Dialogue_Prefab = newEnemy;

            Get_Reward(dialogue_data.dialogue_Contents[dialogue_Content_Count]);

            coroutine = StartCoroutine(Fill_Dialogue_With_TypingEffect(newEnemy, dialogue_data.dialogue_Contents[dialogue_Content_Count]));

            if (dialogue_Content_Count == dialogue_data.dialogue_Contents.Count - 1)
            {
                make_dialogue_coroutine_is_running = false;
                if (dialogue_data.choose_Your_Choice.Count == 0)
                {

                }
                else
                {
                    Active_Choice(dialogue_data);
                }
            }
            if (dialogue_Content_Count == dialogue_data.dialogue_Contents.Count - 2 && !Check_Condition(dialogue_data.dialogue_Contents[dialogue_Content_Count + 1]))
            {
                make_dialogue_coroutine_is_running = false;
            }
            yield return new WaitWhile(() => dialogue_print == true);
        }

    }

    IEnumerator Fill_Dialogue_With_TypingEffect(GameObject dialogue_prefab, Dialogue_Contents d_c)
    {
        printing_With_Typing = true;

        Transform prefab_background = dialogue_prefab.transform.Find("Content_Image");
        dialogue_prefab.transform.Find("Text_Size").GetComponent<Text>().text = d_c.text;
        Text prefab_text = dialogue_prefab.transform.Find("Text_Size").Find("Text").GetComponent<Text>();

        if (d_c.imageSprite != null)
        {
            prefab_background.gameObject.SetActive(true);
            prefab_background.GetComponent<Image>().sprite = d_c.imageSprite;
        }

        yield return null;
        yield return null;
        LayoutRebuilder.MarkLayoutForRebuild(dialogue_prefab.GetComponent<RectTransform>());
        Canvas.ForceUpdateCanvases();
        scrollbar.GetComponent<Scrollbar>().value = 0;

        for (int i = 0; i <= d_c.text.Length; i++)
        {
            prefab_text.text = d_c.text.Substring(0, i);
            if (i == d_c.text.Length)
            {
                printing_With_Typing = false;
            }

            yield return new WaitForSeconds(0.10f);
        }
        scrollbar.GetComponent<Scrollbar>().value = 0;
    }

    public void Fill_Dialogue_Entire()
    {
        StopCoroutine(coroutine);
        now_Dialogue_Prefab.transform.Find("Text_Size").Find("Text").GetComponent<Text>().color = new Color(50 / 255f, 50 / 255f, 50 / 255f, 0);
        now_Dialogue_Prefab.transform.Find("Text_Size").GetComponent<Text>().color = new Color(50 / 255f, 50 / 255f, 50 / 255f, 1);
        scrollbar.GetComponent<Scrollbar>().value = 0;

        printing_With_Typing = false;
        dialogue_print = true;
    }

    public GameObject choose_Area;
    public bool uiClickable = false;
    public Transform button_type;

    public void Active_Choice(Dialogue_Data dialogue_Data)
    {
        uiClickable = true;
        int choice_num = dialogue_Data.choose_Your_Choice.Count;
        switch (choice_num) {
            case 1:
                button_type = choose_Area.transform.Find("Button_type_1");
                button_type.gameObject.SetActive(true);
                button_type.Find("Button_1").Find("Text").GetComponent<Text>().text = dialogue_Data.choose_Your_Choice[0].text;
                break;
            case 2:
                button_type = choose_Area.transform.Find("Button_type_2");
                button_type.gameObject.SetActive(true);
                button_type.Find("Button_1").Find("Text").GetComponent<Text>().text = dialogue_Data.choose_Your_Choice[0].text;
                button_type.Find("Button_2").Find("Text").GetComponent<Text>().text = dialogue_Data.choose_Your_Choice[1].text;
                break;
            case 3:
                button_type = choose_Area.transform.Find("Button_type_3");
                button_type.gameObject.SetActive(true);
                button_type.Find("Button_1").Find("Text").GetComponent<Text>().text = dialogue_Data.choose_Your_Choice[0].text;
                button_type.Find("Button_2").Find("Text").GetComponent<Text>().text = dialogue_Data.choose_Your_Choice[1].text;
                button_type.Find("Button_3").Find("Text").GetComponent<Text>().text = dialogue_Data.choose_Your_Choice[2].text;
                break;
        }
    }

    public void New_Dialogue()
    {
        //Debug.Log("���ο� ���̾�ε� ����");
        foreach (Transform child in transform)
        {
            Destroy(child.gameObject);
        }
        make_space = false;
        gameManager.GetComponent<Dialogue_Event_Maker>().Next_Dialogue();
    }

    public bool Check_Condition(Dialogue_Contents d_c)
    {

        if (d_c.condition == Condition.title)
        {
            if (game_Data_JSON.saveData.titles_Earned.ContainsKey(d_c.condition_name))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (d_c.condition == Condition.likability)
        {
            return true;
        }
        else if (d_c.condition == Condition.ability)
        {
            if(d_c.comparative_Operations== Comparative_Operations.none)
            {
                return true;
            }
            else if (d_c.comparative_Operations == Comparative_Operations.less)
            {
                if(game_Data_JSON.saveData.Get_Stat(d_c.condition_name)< d_c.condition_reference_value)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (d_c.comparative_Operations == Comparative_Operations.lessthan)
            {
                if (game_Data_JSON.saveData.Get_Stat(d_c.condition_name) <= d_c.condition_reference_value)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (d_c.comparative_Operations == Comparative_Operations.greater)
            {
                if (game_Data_JSON.saveData.Get_Stat(d_c.condition_name) > d_c.condition_reference_value)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (game_Data_JSON.saveData.Get_Stat(d_c.condition_name) >= d_c.condition_reference_value)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            return true;
        }
    }
    public void Get_Reward(Dialogue_Contents d_c)
    {
        if (d_c.reward == Reward.title)
        {
            game_Data_JSON.saveData.titles_Earned.Add(d_c.reward_detail, true);
        }
        else if (d_c.reward == Reward.ablilty)
        {
            game_Data_JSON.saveData.Add_Stat(d_c.reward_detail, d_c.weight);
        }
    }
}
