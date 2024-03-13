# 생존 게임 (가제 : aftermass)
담당 업무
1) 건축 시스템 제작
2) 전투 시스템 제작

## 건축 시스템
### BuildingModeCharacter

</br>

![image](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/44d4d901-902f-42b6-8e60-1c02737fa6de)

</br>

![image](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/6f56417f-c985-498b-a889-272dad2a6712)

</br>

건축에 관한 모든 기능을 넣은 클래스입니다.

틱 함수를 중심으로 기능을 살펴보는게 좋습니다.

</br>


틱 함수의 기능은 다음과 같습니다.

1. 월드에 배치되지 않은 임시 건물을 마우스 위치로 이동시킵니다. (SetTransformBasedOnTarget함수와 HoldingObjectOnAir 함수와 관련있습니다.)
2. 임시 건물이 배치 가능한지 확인합니다. 배치 불가능 판정이 나오면 마테리얼을 붉은색으로 표시해 사용자에게 시각적으로 정보를 제공합니다. (IsValidToBuild함수, ChangeMaterial함수와 관련 있습니다.)
3. 마우스 클릭 시 손에 들고 있는 건물을 그 위치 그대로 배치합니다. (BuildOneObjectOnMousePosition함수와 관련있습니다.)
4. 건물의 층을 결정합니다. (SetFloorNumBasedOnTarget함수와 관련있습니다.)
    
</br>


코드에서 HoldingObject와 TargetObject 변수가 자주 쓰이는데 각각 임시 건물과 라인 트레이스로 검출된 배치된 건물을 의미합니다.

</br>


#### SetTransformBasedOnTarget함수
지정된 위치로 임시 건물의 위치를 변경하는 함수 입니다.

유저 편의성 측면에서 중요한 기능인데, 예를 들어 심즈4에서 벽을 세운다고 생각해봅시다. 

벽을 그냥 세워도 좋지만, 대부분의 경우 바닥의 끝부분에 딱 맞춰 지어지기를 원하실겁니다.

그걸 해결하는게 이 함수입니다.

</br>

![image](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/13c70ee6-d3df-49e9-9b86-de0da447f5bd)

</br>

건물 오브젝트에 씬 컴포넌트로 좌표를 지정해 놓고 특정 조합(EX:바닥과 벽)을 맞추면 지정된 좌표를 알려주는 기능을 넣어놨습니다.

이 기능을 담당하는건 BuildingBaseObject의 FindNearestTransform 함수입니다. 

이는 가상함수로 상속받은 모든 오브젝트에 달려있고, 각자 원하는대로 좌표를 출력하는 함수로 오버라이딩 하면 됩니다.

</br>


예를 들어 같이 첨부한 BuildingWooodFoundation을 살펴보겠습니다.

해당 클래스 역시 BuildingBaseObject를 상속받아 FindNearestTransform함수가 있고, 벽,바닥,계단의 경우 특정 좌표를 리턴하는 함수로 오버라이딩 되어 있습니다.

</br>


BuildingModeCharacter클래스의 SetTransformBasedOnTarget함수 내부에서 임시 건물의 FindNearestTransform함수를 호출해 건물의 위치를 지정된 위치로 변경합니다.
    

</br>


## 전투 시스템
### CombatActorComponent

</br>


![image](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/cd960344-4c24-4e59-91f1-d5dca98efeb5)

</br>


건축 시스템을 캐릭터 클래스를 상속 받아 만들었기에, 이식성이 부족하다는 느낌을 받았습니다.

그래서 전투 시스템을 제작할 때는 그 부분을 보완하기 위해 액터 컴포넌트를 상속 받아 제작하기로 결정했습니다.

또한 성능적인 측면을 고려해 틱 함수를 사용하지 않는 것을 목표로 했고, 쓰기 간편해야 한다는 방향을 잡고 개발했습니다.

</br>

해당 컴포넌트는 체력, 스테미나 계산 외에도 몽타주 재생, 딜레이 계산을 자동으로 수행해줍니다.

</br>

ComboAttack 함수만 설명을 넣자면, 이 함수는 애님 노티파이로 다음 연속기를 시작할 지점을 설정해줘야합니다.

기존 공격이 행해지는 도중 그러니깐 애님 몽타주가 재생되고 있을 때, 공격 입력이 추가로 들어오면 saveAttack 값이 true가 되면서 ComboAttack 함수의 실행 조건을 만족합니다.

지정된 연속기의 개수를 넘어가지 않는다면 그 다음에 해당되는 공격 몽타주를 재생하는 방식으로 애님 몽타주를 연속 재생하는 함수입니다.

</br>

![콤보 공격 영상](https://github.com/Rubbe1124/Rubbe1124/assets/61380448/cd784da1-b617-4ea0-b34c-80998b3f487d)



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
