</br>

## 서울 2033을 모티브로 한 개인 프로젝트입니다.

</br>

-매년 5가지의 이벤트가 발생하며 이벤트 메이커에서 직업과 연도에 따라 이벤트를 선별하며 발생시킵니다.
-사용된 주요 기술은 스크립터블 오브젝트에 따라 UI를 동적 생성하는 시스템을 만든 것입니다.
-스크립러블 오브젝트인 Dialogue_Data만을 채워 넣어 리스트에 추가만 해주면 자동으로 게임에 적용되는 시스템을 설계하고 구현했습니다.
</br>

### Dialogue_Adapt 스크립트
- 동적으로 UI를 생성하고 UI를 현재 진행되고 있는 이벤트의 내용을 채워 넣는 기능의 스크립트입니다.
</br>

#### Adapt_Dialogue_Contents 함수
- 외부에서 이 함수를 실행하면 인자로 받은 이벤트를 저장하고 UI를 동적으로 생성하는 make_dialogue_prefab을 실행하는 함수.
</br>

#### make_dialogue_prefab 
- 코루틴으로 실행되고, 이벤트에 적힌 텍스트만큼 반복해서 순차적으로 화면에 UI를 띄워줌.
- 이벤트에 보상이 포함되어있다면 적용하는 기능도 포함함.
- 유저의 선택을 필요로하는 이벤트라면 화면에 선택지 UI도 동적으로 생성함.
</br>

#### Fill_Dialogue_With_TypingEffect
- 텍스트가 한 글자씩 화면에 보이도록 해서 타이핑 하는 듯한 효과를 주는 함수
</br>

#### Fill_Dialogue_Entire
- 타이핑 효과가 나오는 중에 유저가 화면을 터치하면 타이핑 효과를 즉시 멈추고 모든 텍스트가 출력되도록 만드는 함수
</br>

#### Active_Choice
- 이벤트에 선택지가 포함된다면 선택지 UI를 화면에 띄우는 함수
</br>

#### New_Dialogue 함수
- 현재 이벤트가 끝났으니, 이벤트 메이커에게 다음 이벤트를 선택하라는 명령을 내리는 함수
</br>

#### Check_Condition 함수
- 이벤트가 진행될 때, 특수 조건을 만족한다면(직업이나 스탯) 추가 이벤트가 발생되도록 하는 함수.
</br>

#### Get_Reward 함수
- 이벤트에 포함된 보상을 적용하는 함수
- 적용된 보상은 JSON 파일로 저장되어, 게임을 다시 실행해도 보상 받은 걸 적용할 수 있게 해준다.
</br>

### Dialogue_Data 스크립트
- 이벤트를 나타내는 스크립터블 오브젝트입니다.
- 조건에 따라 이벤트를 스킵하거나 출력할 수 있고, 보상도 줄 수 있습니다.
- 콘텐츠는 리스트 형식으로 되어있어, 이벤트 하나에서 출력하는 텍스트를 문단을 나누어 화면에 출력하게 합니다.
- 선택지도 포함되며, 선택지를 누르면 해당 선택지가 가리키는 다음 이벤트를 발생시킵니다.
- 선택지에 이벤트가 포함되지않는다면 이벤트 메이커에게 이벤트 끝남을 알리고 다음 이벤트가 선택되도록 알려줍니다.

<!--
**Rubbe1124/Rubbe1124** is a ✨ _special_ ✨ repository because its `README.md` (this file) appears on your GitHub profile.

Here are some ideas to get you started:

- 🔭 I’m currently working on ...
- 🌱 I’m currently learning ...
- 👯 I’m looking to collaborate on ...
- 🤔 I’m looking for help with ...
- 💬 Ask me about ...
- 📫 How to reach me: ...
- 😄 Pronouns: ...
- ⚡ Fun fact: ...
-->
