# Unreal5 : JH_Cropout
학습 목적으로 제작된 Unreal Engine 5의 대표적인 게임 [Cropout Sample](https://www.unrealengine.com/en-US/blog/cropout-casual-rts-game-sample-project) 샘플 클론 프로젝트입니다.
블루프린트로 제작 된 샘플을 C++로 만들었습니다.

<br>

- 엔진 : Unreal Engine 5.4
- IDE	: Rider
- 툴	: C++
- 인원 : 개인 개발
- UI	 : Common UI
- 언리얼 엔진 프레임워크 : Behavior Trees , EQS , Saving and Loading ,  Blueprint Interface, Geometry Script

<br>

## Cropout에서 배울 수 있는 것들

Cropout은 언리얼 엔진 5 기반의 캐주얼 톱다운 RTS 게임 샘플 프로젝트입니다. 다음과 같은 핵심 개념을 학습할 수 있습니다:

### 1. 크로스 플랫폼 최적화
- 모바일, 콘솔, PC를 아우르는 멀티 플랫폼 대응 구조
- 성능 차이에 따른 콘텐츠 품질 조절 및 스케일링

### 2. 블루프린트 기반 구조
- 대부분의 게임 로직이 블루프린트로 구현
- 비주얼 스크립팅 학습에 최적화된 예제

### 3. UE5 최신 기능 활용
- Common UI: 다양한 입력 디바이스를 지원하는 UI 프레임워크
- Enhanced Input: 입력 매핑 컨텍스트를 통해 유연한 입력 제어
- Geometry Script: 프로시저 방식의 섬(지형) 생성 예시

### 4. 게임플레이 시스템 구성
- Behavior Tree, EQS를 이용한 AI 시스템
- 저장 및 불러오기 시스템, Blueprint Interface 활용

### 5. 플랫폼별 패키징 최적화
- 모바일/콘솔/PC 환경별 리소스 품질 조정
- 텍스처, 메시, 파티클 등의 디테일 차등 적용

### 6. 플러그인 구조 활용
- 모듈 단위 설계로 유지보수와 재사용성이 높은 구조

<br>


## 영상

<br>

# Procedural Island Generator 
언리얼 엔진의 **Geometry Script 기능**을 활용하여 실시간으로 **섬 형태의 메시를 생성**하는 예제입니다.  
DynamicMesh 시스템을 활용해 **지형을 만들고, 솔리디파이(Solidify), 노멀 보정, 평탄화 및 UV 프로젝션**까지 처리합니다.


