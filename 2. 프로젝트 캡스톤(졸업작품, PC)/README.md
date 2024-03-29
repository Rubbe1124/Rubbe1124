</br>

# 프로젝트 캡스톤 (졸업작품)

</br>

- 로스트아크의 보스 레이드 방식을 모티브로 네트워크 기능을 사용한 게임을 만드는 것을 기획하고 개발한 작품입니다.
    
- 팀에서 담당한 업무는 네트워크, UI, 캐릭터 기능 개발입니다.

</br>

## 네트워크
- Loading, Lobby, WaitingRoom, NetWork 폴더에 스크립트가 존재합니다.

</br>

### Loading
- 유저가 게임을 실행하면 Loading 씬이 나오며, 서버에 접속합니다.

![image](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/cb8590bd-f764-4404-a17b-07e4e21399af)


</br>

### Lobby
- 서버에 접속하게되면 Lobby 씬으로 전환되고, Lobby 씬에서는 현재 생성된 방 리스트와 그 방의 접속 인원이 화면에 표현됩니다.
- 방을 만들거나 이미 있는 방에 참여할 수 있는 기능이 포함됩니다.

![image](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/6c25724b-b3a7-48a6-8787-7b085a6363bf)

  
</br>

### WaitingRoom
방에 접속하면 유저는 캐릭터를 선택할 수 있습니다. 
관련 스크립트는 SetCharacterAndAlert, SelectCharacter, WaitingRoomInfo에 포함되어있습니다.
    
- WaitingRoomInfo 스크립트는 프로퍼티와 RPC를 이용해 다른 유저들과 소통하고 캐릭터의 중복 체크, 방장에게 게임 시작 권한 부여, 준비 상태 알림 등의 기능을 합니다.
    
- SetCharacterAndAlert와 SelectCharacter 스크립트는 캐릭터 선택 창에서 버튼 클릭 후, 중복 선택된 캐릭터가 아니라면 선택된 캐릭터를 고를 수 있다는 것을 가시적으로 유저에게 알려주기 위해 버튼의 색깔을 변경하는 스크립트입니다.
    
- 그 외의 방에서 나갈 때를 처리하는 스크립트, 레디 상태 알림, 게임 시작 등의 스크립트가 포함되어있습니다.

![image](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/1a82a8e0-364c-4e13-ae44-7c5bd36c3a34)

</br>

![image](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/bf029f94-88f1-4e51-b1c6-8f0ae1a82982)

</br>

## Character
인게임 내에서의 작업한 내용 중 비중이 큰 내용들입니다.
    
- CameraMove 스크립트는 보스 몬스터는 12시, 메인 캐릭터는 6시에 위치되도록 카메라의 위치를 조정하는 스크립트입니다.
    
- Character_Info 스크립트는 캐릭터의 스탯을 담아두는 스크립트입니다. 해당 스크립트에서는 데미지 계산 및 적용, 회복, 아머 시스템 등의 기능이 포함되어있습니다.
    
- Character_Move 스크립트는 캐릭터의 기본적인 이동과 애니메이션 동작을 담은 스크립트입니다. RPC를 통해 유저의 입력으로 인한 캐릭터의 변화를 다른 유저들의 화면에서도 나타날 수 있게끔 했습니다.
    
- Character_Skill 스크립트은 모든 플레이어블 캐릭터가 상속받는 부모 클래스입니다. 스킬의 발동시 캐릭터의 정면 보정, 스킬 파티클 수행, 이동 불가 상태 부여 등의 기능이 포함되어있습니다.
    
- 각 캐릭터는 해당 스크립트를 상속받아 위의 기능을 쓸 수 있고, 가상함수로 만든 함수의 내용을 오버라이딩해서 스킬을 실행하도록 만들었습니다.

![image](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/0279e0bb-9c37-42cc-9b61-3e05f84c3e0e)
    



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
