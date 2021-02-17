#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#define EXIT_KEY 'q'    //프로그램 종료 명령키

int termy,termx;

int main()
{
    int ch;
    initscr();
    cbreak();   //no linebuffering
    noecho();   //no echo of inputs
    nodelay(stdscr,TRUE);
    keypad(stdscr,TRUE);    //enable keypad, arrows

    //read_tree();    //TODO: 현재 CWD를 기준으로 디렉토리/파일 정보 읽기
    while(1)    //탈출 조건-> q눌렀을때.
    {
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: 커서 다운
            //read_tree();  //옮긴 커서를 기준으로 디렉토리/파일 정보 읽기
            break;
            case KEY_UP:
            //TODO: 커서 업
            //read_tree();  //옮긴 커서를 기준으로 디렉토리/파일 정보 읽기
            break;
            case KEY_LEFT:
            //TODO: 커서 왼쪽(이전 위치)
            //read_tree();  //옮긴 커서를 기준으로 디렉토리/파일 정보 읽기
            break;
            case KEY_RIGHT:
            //TODO: 커서 오른쪽(디렉토리 들어가기/동작없음)
            
            break;
            case KEY_ENTER:
            //TODO: 정보보기?
            break;
        }
        if (ch == EXIT_KEY) //프로그램 종료
            break;
        clear();

        //print_tree(); //TODO: 화면 출력
        refresh();
        while((ch=getch())==ERR);   //키 입력 될때까지 대기
    }
    endwin();
    return 0;
}