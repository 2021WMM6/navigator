#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define EXIT_KEY 'x'    //프로그램 종료 명령키

int termy,termx;
char wd[PATH_MAX];
char strbuf[PATH_MAX];
void read_tree()
{
    DIR *dirp;      //디렉토리 포인터
    struct dirent *diritem;    //디렉토리 항목 포인터

    dirp = opendir(wd);
    while((diritem=readdir(dirp)) != NULL)
    {
        sprintf(strbuf,"%ld       %s    %d",diritem->d_ino,diritem->d_name,diritem->d_type); //inode 번호, 이름, 종류
        printw("%.*s\n",termx-1,strbuf);
    }
}
int main()
{
    int ch;
    time_t lasttime;
    initscr();
    cbreak();   //no linebuffering
    noecho();   //no echo of inputs
    nodelay(stdscr,TRUE);
    keypad(stdscr,TRUE);    //enable keypad, arrows
    getcwd(wd,PATH_MAX);    //현재 디렉토리 가져오기
    //read_tree();    //TODO: 현재 CWD를 기준으로 디렉토리/파일 정보 읽기
    while(1)    //탈출 조건-> q눌렀을때.
    {
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        clear();
        sprintf(strbuf,"%s:",wd);
        printw("%.*s\n",termx-1,strbuf);
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: 커서 다운
            sprintf(strbuf,"down");
            //read_tree();  //옮긴 커서를 기준으로 디렉토리/파일 정보 읽기
            break;
            case KEY_UP:
            //TODO: 커서 업
            sprintf(strbuf,"up");
            //read_tree();  //옮긴 커서를 기준으로 디렉토리/파일 정보 읽기
            break;
            case KEY_LEFT:
            //TODO: 커서 왼쪽(이전 위치)
            sprintf(strbuf,"left");
            //read_tree();  //옮긴 커서를 기준으로 디렉토리/파일 정보 읽기
            break;
            case KEY_RIGHT:
            //TODO: 커서 오른쪽(디렉토리 들어가기/동작없음)
            sprintf(strbuf,"right");
            
            break;
            case KEY_ENTER:
            sprintf(strbuf,"Enter");
            //TODO: 정보보기?
            break;
        }
        if (ch == EXIT_KEY) //프로그램 종료
            break;
        //print_tree(); //TODO: 화면 출력
        printw("%.*s\n",termx-1,strbuf);
        read_tree();
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    endwin();
    return 0;
}