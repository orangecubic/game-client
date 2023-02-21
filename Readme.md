# 게임 클라이언트

[axmol 엔진(cocos2dx fork)](https://github.com/axmolengine/axmol)을 사용했으며 모바일 대응 x </br>
axmol 링크에 설명되어 있는대로 OS에 맞게 프로젝트 생성 후 빌드

localhost:9988 이 기본 연결 주소로 설정돼 있기 때문에 서버가 로컬에서 실행되고 있어야함 (AppDelegate.cpp에서 주소 변경 가능)

</br>

조작법

방향키 </br>
점프: Space </br>
발사: A </br>

</br>

Client Prediction, Smoothing, Ping Interpolation등의 클라이언트 사용성을 위한 작업을 하지 않았기 떄문에 조작감은 좋지 못함
