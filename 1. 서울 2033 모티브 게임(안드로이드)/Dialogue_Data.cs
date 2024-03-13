using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;

public enum Comparative_Operations
{
    none,
    less,
    lessthan,
    greater,
    greaterthan
}

public enum Condition
{
    none,
    title,
    likability,
    ability
}

public enum Reward
{
    none,
    title,
    ablilty
}

[CreateAssetMenu]
public class Dialogue_Data : ScriptableObject
{
    public Sprite imageSprite;

    public List<Dialogue_Contents> dialogue_Contents;

    public List<Choose_Your_Choice> choose_Your_Choice;

    public Dialogue_Data insert_Dialogue_In_Queue;
}

[Serializable]
public class Dialogue_Contents
{
    public Condition condition;
    public string condition_name;
    public Comparative_Operations comparative_Operations;
    public int condition_reference_value;

    public Sprite imageSprite;
    [TextArea(1, 100)]
    public string text;

    public Reward reward;
    public string reward_detail;
    public int weight;
}

[Serializable]
public class Choose_Your_Choice
{
    public Condition condition;
    public string condition_name;
    public Comparative_Operations comparative_Operations;
    public string text;
    public Dialogue_Data next_Dialogue;
}