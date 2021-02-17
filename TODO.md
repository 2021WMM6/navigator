# Navigator
## 기본 기능
- 실행 시 현재 작업 디렉토리(cwd)를 기준으로 tree view 생성
    - 디렉토리의 정보를 읽어 필요한 정보를 구조체로 저장
        - unistd.h의 표준 라이브러리 함수 사용 필요
            - lstat
            - readdir
            - opendir
    - 구조체를 바탕으로 tree view 형식으로 출력
    - 예시
        ```
        home --- 1st directory  --- file01
              |                  \- subdirectory --- file11
              |- 2nd directory  --- file21
              |- 3rd directrory
              \- 4th directrory --- file41
                                 |- file42
                                 \- file43
        ```
- 화살표, 기능키를 사용해 조작 가능
    - ncurses 라이브러리 사용 필요
    - 화살표를 사용해 스크롤, 엔터키를 이용해 내부 디렉토리 탐색
        - 예시 (현재 커서를 () 로 표시한다 가정)
        ```
        (home) --- 1st directory  --- file01
              |                  \- subdirectory --- file11
              |- 2nd directory  --- file21
              |- 3rd directrory
              \- 4th directrory --- file41
                                 |- file42
                                 \- file43
        ```
        오른쪽 화살표 입력
        ```
        home --- (1st directory)  --- file01
              |                  \- subdirectory --- file11
              |- 2nd directory  --- file21
              |- 3rd directrory
              \- 4th directrory --- file41
                                 |- file42
                                 \- file43
        ```
        아래쪽 화살표 입력
        ```
        home --- 1st directory  --- file01
              |                  \- subdirectory --- file11
              |- (2nd directory)  --- file21
              |- 3rd directrory
              \- 4th directrory --- file41
                                 |- file42
                                 \- file43
        ```

## 추가 기능 1: 목록
- 표시되고 있는 파일/디렉토리 목록을 지정한 기준으로 정렬
    - 편집 시간/크기/이름/파일 유형/…
- 정렬과 관계 없이 일부 파일을 위에 고정할 수 있는 pin 기능
- pin된 파일간 순서를 변경할 수 있는 커스텀 기능

## 추가 기능 2 : 검색
- 파일명 검색: 기본적으로 파일명 일치
- 추가 조건: 파일명이 비슷한 파일 표시
    - 주어진 이름이 포함되어 있음 (AAA -> bAAA, AAAccc, …)
    - 글자가 얼마 차이나지 않음(편집거리) (AAA-> AaA, AA, AAC,…)
- 조건 검색: 확장자, 편집날짜, 등

## 추가 기능 3 : 편집
- 복사
- 붙여넣기
- 이동
- 삭제
- 링크 생성
    - 심볼릭 링크
    - 하드 링크

## 추가기능 4 : 부가기능
- 파일 미리보기
    - txt 인코딩으로 표시
    - HEX로 표시
    - 재생…?
- 표시 모드 변경
    - 파일 크기, 파일 상태 등 추가 정보 표시 모드
- 용량 관리
    - 사용된 지 오래된 파일을 강조 표시
