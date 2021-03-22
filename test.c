#include "test.h"   //헤더 파일 test.h 별도로 정의

int pop(){return top == 0 ? -1 : stack[--top];}
void push(int a){stack[top++] = a;}
int tops(){return top == 0 ? -1 : stack[top - 1];}

//메뉴 케이스 출력
void print_menu(int n) {
    int k;
    switch(n) {
        case 1:
            printw("|============== MENU ==============|\n\n");
            printw("\t1. See Detail\n\n");
            printw("\tX. Exit\n\n");
            printw("|==================================|\n\n");
            break;
        case 2:
            printw("|");
            for(k = 0; k < 36; k++)     //줄 출력
                printw("=");
            printw(" MENU ");
            for(k = 0; k < 36; k++)     //줄 출력
                printw("=");
            printw("|\n");
            printw("\t1. Go Back\t2. Sorting\t3. Searching\t 4. Editing\n");
            printw("|");
            for(k = 0; k < 78; k++)     //줄 출력
                printw("=");
            printw("|\n");
            break;
        case 3:
            printw("|==================================|\n\n");
            printw("\t1. Go Back\n\n");
            printw(" There's no file in this directory\n\n");
            printw("|==================================|\n\n");
            break;
        case 4:
            printw("\n|========= SORTING OPTION =========|\n\n");
            printw("\t1. File Name\n\n");
            printw("\t2. File Type\n\n");
            printw("\t3. Size\n\n");
            printw("\t4. Last Modification\n\n");
            printw("|==================================|\n\n");
            break;
        case 5:
            printw("\n|=========== PIN OPTION ===========|\n\n");
            printw("\t1. Use Pin\n\n");
            printw("\t2. Don't Use Pin\n\n");
            printw("|==================================|\n\n");
            break;
        case 6:
            printw("|");
            for(k = 0; k < 78; k++)     //줄 출력
                printw("=");
            printw("|\n");
            printw("\t1. Quit\t\tP. Use pin\n");
            printw("|");
            for(k = 0; k < 78; k++)     //줄 출력
                printw("=");
            printw("|\n");
            break;
        case 7:
            printw("\n|========= Editing Option =========|\n\n");
            printw("\t1. Copy\n\n");
            printw("\t2. Move\n\n");
            printw("\t3. Delete\n\n");
            printw("\t4. Rename\n\n");
            printw("|==================================|\n\n");
            break;
        case 8:
            printw("|==================================|\n\n");
            printw("\t1. Quit\n\n");
            printw("If you want to delete file, press 'd'\n\n");
            printw("|==================================|\n\n");
            break;
        case 9:
            printw("|==================================|\n\n");
            printw("\t1. Quit\n\n");
            printw("If you want to copy file, press 'c'\n\n");
            printw("|==================================|\n\n");
            break;
        case 10:
            printw("|==================================|\n\n");
            printw("\t1. Quit\n\n");
            printw("If you want to rename file, press 'r'\n\n");
            printw("|==================================|\n\n");
            break;
        case 11:
            printw("|==================================|\n\n");
            printw("\t1. Quit\n\n");
            printw("Select directory where to paste your file with press 'p'\n\n");
            printw("If you want to select home directory, press 'h'\n\n");
            printw("|==================================|\n\n");
            break;
        case 12:
            printw("|==================================|\n\n");
            printw("\t1. Quit\n\n");
            printw("If you want to move file, press 'm'\n\n");
            printw("|==================================|\n\n");
            break;
        case 13:
            printw("|==================================|\n\n");
            printw("\tX. Quit\n\n");
            printw("|==================================|\n\n");
            break;
        case 14:
            attron(COLOR_PAIR(2));
            printw(" PATH : %.*s\n",termx-1, tp);     //현재 경로 출력
            attroff(COLOR_PAIR(2));
            break;
    }   //케이스 번호별로 나누어서 메뉴를 출력. 각각의 경우마다 나누어 함수에서 필요로 하는 부분을 가져다가 사용
}
//precondition: xpath는 복사할 파일의 경로를 가지고 있어야 하고 strbuf는 그 파일이 복사될 경로를 가지고 있어야 한다.
//postcondition: xpath 경로의 파일을 strbuf 경로에 복사시킨다.
int copy(){
    FILE *fexist, *fnew;
    int a;
    if((fexist = fopen(xpath, "rb")) == NULL)          //복사하려고 하는 파일을 'rb'로 열고 실패한 경우
        return -1;
    if((fnew = fopen(strbuf, "wb")) == NULL){          //복사해 넣을 경로에 있는 파일을 만들어 'wb'로 열고 실패한 경우
        fclose(fexist);                                //fexist 파일 닫기
        return -1;
    }
    while(1){
        a = fgetc(fexist);                           //fexist파일에서 하나를 읽어서 a에 저장
        if(!feof(fexist))                            //fexist의 파일의 끝이 아닐 경우
            fputc(a, fnew);                          //fnew파일에 a를 출력
        else                                         //fexist의 파일의 끝에 왔을 경우
            break;
    }
    fclose(fexist);                                 
    fclose(fnew);
    return 0;
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: 화면에 메뉴와 detail 정보를 출력시켜주고 키보드를 통해 ()로 내가 복사하려고 하는 파일을 선택한다.
//               그리고 'c'을 누르면 move_file()을 호출해준 후 select_location()을 호출한 후 기능을 종료한다.
//               위 경우가 아니더라도 '1'을 누를 경우 move기능을 종료한다.
void copy_file_select(){
    int ch = 0;
    time_t lasttime;
    while(1){    //탈출 조건-> 1 눌렀을때.
        getmaxyx(curscr,termy,termx);
        lasttime=time(NULL);
        clear();        //화면 clear
        switch(ch){
            case KEY_DOWN:
            //TODO: 커서 다운
                move_key('D');
                break;
            case KEY_UP:
            //TODO: 커서 업
                move_key('U');
                break;
            case 'c':
                if(cur2 -> type == DT_DIR){             //선택한 파일이 디렉토리일 경우,
                    printw("You can't copy Directory"); //directory는 복사할 수 없습니다.
                    refresh();
                    while(getch() == ERR);              //아무 키나 입력할 때까지 대기
                    clear();    //화면 clear
                    break;
                }
                strcpy(File_n, cur2->a);             
                move_file();
                select_location(2);
                return;
                break;
        }
        print_menu(9);      //9번 메뉴 케이스 출력
        if (ch == '1')      //1키가 눌리면 종료
            break;
        cur = start -> back;
        if(start -> back -> back == NULL || cur2 -> op == 1)
            break;
        print_detail(); 
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
    clear();        //화면 clear
}
void rena(){        //이름 변경하는 함수
    chdir(tp);      //현재 주소 tp로 변경
    mvprintw(5,5,"-----------------------------");
    for(int i=6;i<9;i++)
        mvprintw(i,5,"|                           |");
    mvprintw(9,5,"-----------------------------");
    mvprintw(7,7,"new name: ");     //새로운 이름 입력받을 칸 출력
    nocbreak();
    echo();
    nodelay(stdscr,FALSE);      //mvscanf를 하기 위한 설정 변경
    strncpy(curname,cur2->a+1,strlen(cur2->a)-2);
    mvscanw(7,17,"%s",newname);     //새롭게 변경할 이름 입력받음
    int result=rename(curname,newname);     //현재 커서의 이름을 입력받은 이름으로 변경
    if(result==-1)      //rename함수가 -1을 리턴한다면
        mvprintw(8,7,"error");  //error 출력
    cbreak();
    noecho();
    nodelay(stdscr,TRUE);       //설정 원래대로 돌려놓음
    clear();        //화면 clear
}
void File_rename(){
    int ch;
    time_t lasttime;
    while(1){    //탈출 조건-> 1 눌렀을때.
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        clear();        //화면 clear
        switch(ch){
            case KEY_DOWN:
            //TODO: 커서 다운
                move_key('D');  //커서 밑으로 이동하는 함수
                break;
            case KEY_UP:
            //TODO: 커서 업
                move_key('U');  //커서 위로 이동하는 함수
                break;
            case 'r':
                print_menu(10);     //10번 메뉴 케이스 출력
                print_detail();     //디테일한 정보 출력 함수
                rena(); //이름 변경하는 함수
                sprintf(lastname,"(%s)",newname);   //lastname에다가 커서 포함하여 이름 넣어줌
                strcpy(cur2->a,lastname);   //
                break;
        }
        print_menu(10);     //10번 메뉴 케이스 출력
        if (ch == '1')      //1키가 눌리면 종료
            break;
        cur = start -> back;
        if(start -> back -> back == NULL || cur2 -> op == 1)    
            break;
        print_detail();     //파일 디테일한 정보 출력함수
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
    clear();        //화면 clear
}
//precondition: cur2가 이동하려는 디렉토리를 포인트하고 있어야 한다. tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다. 
//              strex는 이동시킬 파일의 이름을 가지고 있어야 한다. xpath는 이동시키고 싶은 파일의 경로를 가지고 있어야 한다.
//              move기능을 사용할 때는 con값이 1이어야 하고, copy기능을 사용하려면 con값이 2여야 한다.
//postcondition: 1) 이동시키려는 파일을 선택한 디렉토리에 이동시킨다. 열려있는 모든 디렉토리를 닫아준다.
//               2) 복사시키려는 파일을 선택한 디렉토리에 복사시킨다. 열려있는 모든 디렉토리를 닫아준다.
void location_p(int con){
    snprintf(strbuf, PATH_MAX*3, "%s/%s/%s", tp, cur2 -> a, strex);
    if(con == 1){
        rename(xpath, strbuf);
        if(temp != 1){
            cur = cur3;
            delete_l();
        }
    }
    else if(con == 2){
        copy();
    }
    while(cur2 -> front != start){
        if(cur2 -> front -> op == 1)
            close_dir();
        cur2 = cur2 -> front;
    }
    cur2 = start -> back;
}
//precondition: cur2가 이동하려는 디렉토리를 포인트하고 있어야 한다. wd는 기본 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//              strex는 이동시킬 파일의 이름을 가지고 있어야 한다. xpath는 이동시키고 싶은 파일의 경로를 가지고 있어야 한다.
//              move기능을 사용할 때는 con값이 1이어야 하고, copy기능을 사용하려면 con값이 2여야 한다.
//postcondition: 1) 이동시키려는 파일을 기본(가장 상위의) 디렉토리에 이동시킨다. 열려있는 모든 디렉토리를 닫아준다.
//               2) 복사시키려는 파일을 기본(가장 상위의) 디렉토리에 복사시킨다. 열려있는 모든 디렉토리를 닫아준다.
//               링크드 리스트에 이동/복사시킨 파일의 정보를 start -> back 에 추가해주고 cur2는 추가한 파일을 포인트하고 있다. 
//               tree view를 출력해주고 함수를 종료한다.
void location_home(int con){
    snprintf(strbuf, PATH_MAX*2, "%s/%s", wd, strex);
    if(con == 1)
        rename(xpath, strbuf);
    else if(con == 2)
        copy();
    char *str = strex;
    snprintf(strbuf, PATH_MAX*2, "%s/%s", wd, str);
    type_ex = move_type;
    cur = start;
    insert_l(str);
    while(cur2 -> front != start){
        if(cur2 -> front -> op == 1)
            close_dir();
        cur2 = cur2 -> front;
    }
    cur2 = start -> back;
    clear();        //화면 clear
    print_tree();
    refresh();
}
//precondition: cur2가 이동시키려는 파일을 포인트하고 있어 야한다. wd는 기본 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//              move기능을 사용할 때는 con값이 1이어야 하고, copy기능을 사용하려면 con값이 2여야 한다.
//postcondition: 메뉴를 출력해주고 main()과 비슷하게 tree view 형식으로 파일들을 보여주며 키보드를 통해 이동할 할 수 있다.
//               디렉토리를 선택한 후 'p'를 누르면 location_p()를 호출하여 그 디렉토리 안에 파일을 이동/복사시키고 기능을 종료한다.(디렉토리 이외의 파일을 선택한 후 'p'를 눌러도 아무 것도 일어나지 않는다.)
//               'h'를 누르면 지금 선택하고 있는 파일과 상관없이 location_home()을 호출하여 기본(가장 상위의) 디렉토리에 파일을 이동/복사시키고 기능을 종료한다.
//               위 경우가 아니더라도 '1'을 누를 경우 move기능을 종료한다.
void select_location(int con){
    int ch = 0;
    time_t lasttime;
    while(1){    //탈출 조건-> x눌렀을때.
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        clear();        //화면 clear
        //printw("%.*s\n",termx-1, tp);     //현재 경로가 어디인지 출력
        switch(ch){
            case KEY_DOWN:
            //TODO: 커서 다운
                move_key('D');
                break;
            case KEY_UP:
            //TODO: 커서 업
                move_key('U');
                break;
            case KEY_LEFT:
            //TODO: 커서 왼쪽(이전 위치)
                move_key('L');
                break;
            case KEY_RIGHT:
            //TODO: 커서 오른쪽(디렉토리 들어가기/동작없음)
                move_key('R');
                break;
            case 'p':
                if(cur2 -> type != DT_DIR)
                break;
                location_p(con);
                return;
            case 'h':
                location_home(con);
                return;
        }
        if (ch == '1') //프로그램 종료
            break;
        print_menu(11);     //11번 메뉴 케이스 출력
        snprintf(strbuf,PATH_MAX,"%s",wd);
		attron(COLOR_PAIR(2));      //색 변환 ON
		printw("   --------------------------------\n");
		printw("   Selected File : %s\n",File_n);		
		printw("   --------------------------------\n\n");
		attroff(COLOR_PAIR(2));     //색 변환 OFF
        printw("%.*s",termx-1,strbuf);
        cur = start -> back;
        if(start -> back -> back == NULL)
            break;
        print_tree(); // tree view 형식으로 출력
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
}
//precondition: cur2가 이동시키고 싶은 파일을 포인트하고 있어야 한다. tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: 이동시키려는 파일이 열린 디렉토리 안에 존재한다면 temp값을 1로 변경, 아니면 0으로 변경한다.
//               strex에 이동시킬 파일의 이름을 문자열로 저장하고, move_type에 이동시킬 파일의 type값을 저장한다.
//               xpath에 이동시킬 파일의 경로를 문자열로 저장한다.
void move_file(){
    snprintf(xpath, PATH_MAX*2, "%s/%s", tp, cur2 -> a);
    strcpy(strex, cur2 -> a);
    move_type = cur2 -> type;
    cur3 = cur2;
    while(cur3 -> front != start){
        if(cur3 -> op == 1){
            temp = 1;
            break;
        }
        cur3 = cur3 -> front;
        temp = 0;
    }
    cur3 = cur2;
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: 화면에 메뉴와 detail 정보를 출력시켜주고 키보드를 통해 ()로 내가 이동시키려고 하는 파일을 선택한다.
//               그리고 'm'을 누르면 move_file()을 호출해준 후 select_location()을 호출한 후 기능을 종료한다.
//               위 경우가 아니더라도 '1'을 누를 경우 move기능을 종료한다.
void move_file_select(){
    int ch;
    time_t lasttime;
    while(1){    //탈출 조건-> 1 눌렀을때.
        getmaxyx(curscr,termy,termx);
        lasttime=time(NULL);
        clear();        //화면 clear
        switch(ch){
            case KEY_DOWN:
            //TODO: 커서 다운
                move_key('D');
                break;
            case KEY_UP:
            //TODO: 커서 업
                move_key('U');
                break;
            case 'm':
                strcpy(File_n, cur2->a);
                move_file();
                select_location(1);
                return;
                break;
        }
        print_menu(12);     //12번 메뉴 케이스 출력
        if (ch == '1')      //1키를 눌러서 종료
            break;
        cur = start -> back;
        if(start -> back -> back == NULL || cur2 -> op == 1)
            break;
        print_detail();     //파일 상세 리스트 출력
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
    clear();        //화면 clear
}
//precondition: cur2가 삭제하고자 하는 파일을 포인트하고 있어야 한다. tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: 삭제하고자 하는 파일의 정보를 링크드리스테에서 삭제하고 실제로 파일을 제거한다. cur2는 삭제한 파일의 앞이나 뒤를 포인트한다.
void delete_file(){
    snprintf(strbuf, PATH_MAX*2, "%s/%s", tp, cur2 -> a);
    if(remove(strbuf) == 0){
        cur = cur2;
        if(cur2 -> back -> back != NULL && cur2 -> clos == 0)
            cur2 = cur2 -> back;
        else if(cur2 -> clos == 1){
            cur2 = cur2 -> front;
            cur2 -> clos = 1;
        }
        else
            cur2 = cur2 -> front;
        delete_l();
    }
    else{
        clear();        //화면 clear
        printw("| MENU | 1. Go Back\n\n");
        printw("ERROR\n");      //더 이상 삭제할 파일이 없을경우, ERROR 메세지 띄우고 1번을 눌러서 돌아가기
        refresh();
        while(getch() != '1');
        clear();        //화면 clear
    }
}
//precondition: cur2가 NULL값을 가지면 안 된다.
//postcondition: 메뉴창을 출력하고 키보드를 통해 ()로 내가 삭제하고자 하는 파일을 포인터한 후 제거한다. '1'을 누르면 delete기능을 종료한다.
//               디렉토리 안에 모든 파일을 제거하면 기능을 종료한다.
void delete_file_select(){
    int ch;
    time_t lasttime;
    while(1){    //탈출 조건-> 1 눌렀을때.
        getmaxyx(curscr,termy,termx);
        lasttime=time(NULL);
        clear();        //화면 clear
        switch(ch){
            case KEY_DOWN:
            //TODO: 커서 다운
                move_key('D');
                break;
            case KEY_UP:
            //TODO: 커서 업
                move_key('U');
                break;
            case 'd':
            //d키가 눌렸을때 delete_file함수 호출
                delete_file();
                break;
        }
        print_menu(8);      //8번 메뉴 케이스 출력
        if (ch == '1')
            break;      //1키를 누르면 종료
        cur = start -> back;
        if(start -> back -> back == NULL || cur2 -> op == 1)
            break;
        print_detail();
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
    clear();        //화면 clear
}
//precondition: cur2가 NULL값을 가지면 안 된다. name이 검색하고자 하는 키워드를 문자열로 저장하고 있어야 한다. 
//postcondition: 링크드리스트에서 키워드가 포함된 이름을 가진 구조체들의 search_list 값을 1로 변경한다.
//               만약 키워드가 포함된 단어가 하나도 없을 경우 temp값이 0, 하나라도 있을 경우 1로 변경한다.
void search(char name[100]){    //찾을 이름을 인자로 받는다
    temp = 0;   //찾았는지 여부를 저장
    char *str;
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;     //cur을 열린 디렉토리의 맨 처음까지 이동
    while(cur -> back != NULL && cur -> front -> clos != 1){        //열린 디렉토리의 맨 끝부분이 아니라면 시행
        str = cur -> a;     //cur위치의 이름을 포인트함
        while(*str){        //str안에 찾고자하는 이름이 포함되어 있는지 찾아봄
            if(strncmp(str, name, strlen(name)) == 0){      //만약 찾고자 하는 이름이 있다면   
                temp = 1;   //temp를 1로 바꿔준다
                cur2 = cur;     //현재 커서위치인 cur2를 cur로 바꿔준다
                cur -> search_list = 1;     
                break;
            }
            else{       //찾고자 하는 이름이 포함되어있지 않다면
                str++;  //문자열 한칸 뒤로 이동해서 다시 비교해봄
            }
        }
        cur = cur -> back;
    }
}
//precondition: cur2가 NULL값을 가지면 안 된다.
//postcondition: 박스 안에 출력된 찾은 파일들 중에서 키보드를 통해 ()로 내가 원하는 cur2가 포인트한 후 '1'을 누르면 종료된다.
void select_search_list(){
    int ch;
    time_t lasttime;
    cbreak();   
    noecho();
    nodelay(stdscr,TRUE);   //설정 변경
    while(1){    //탈출 조건-> 1 눌렀을때.
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        switch(ch){
            case KEY_DOWN:
            //TODO: 커서 다운
                if(cur2 -> back -> back != NULL && cur2 -> clos == 0){      
                    cur = cur2;
                    do{
                        cur = cur -> back;
                    } while (cur -> search_list != 1 && cur -> back != NULL && cur -> front -> clos == 0);
                    if(cur -> back != NULL && cur -> front -> clos == 0)
                        cur2 = cur;
                }
                break;
            case KEY_UP:
            //TODO: 커서 업
                if(cur2 -> front != start && cur2 -> front -> op == 0){     
                    cur = cur2;
                    do{
                        cur = cur -> front;
                    } while (cur -> search_list != 1 && cur != start && cur -> back -> op == 0);
                    if(cur != start && cur -> back -> op == 0)
                        cur2 = cur;
                }
                break;
        }
        if (ch == '1')      //1키를 눌렀을때 종료
            break;
        print_search_list();
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    clear();        //화면 clear
}
//precondition: cur2가 NULL값이면 안 된다.
//postcondition: 입력창을 출력해주고 키워드를 입력받는다.
//               search()함수를 실행시켜주고 찾은 파일이 있으면 select_search_list()함수를 실행하고, 파일이 없으면 name not found를 출력해준다.
//               cur2가 가리키는 파일에 ()를 붙여준다.
void get_name(){
    char name1[100];    //찾을 파일명 받는 문자열
    mvprintw(0,40,"------------------------------------------\n");
    for(int i=1;i<5;i++)
        mvprintw(i,39,"|                                          |");
    mvprintw(5,40,"------------------------------------------\n");      
    mvprintw(1,40,"File Name: ");           //이름받는 칸 출력
    nocbreak();
    echo();
    nodelay(stdscr,FALSE);          //mvscanw을 하기위해 설정 변경
    mvscanw(1,51,"%s",name1);       //찾을 파일명 입력받음
    search(name1);      //이름 찾는 함수
    if(temp == 1){      //찾고자 하는 이름이 포함되어 있다면
        select_search_list();   //포함한 이름을 모두 출력해준 후 선택하는 창 만들어줌
    }
    else{       //찾고자 하는 이름이 없다면
        mvprintw(3,40,"name not found : press enter to exit");      //찾을 수 없음을 출력해줌
        refresh();
        getch();
        nodelay(stdscr,TRUE);
        noecho();
        cbreak();   //변경한 설정을 원래대로 돌려놓음
        return;
    }
}
//precondition: cur2가 NULL값을 가지면 안 된다.
//postcondition: 화면에 검색된 이름들을 박스안에 차례대로 출력해준다.
void print_search_list(){
    int i = 3;
    cur = cur2;     //현재 커서 위치로 cur 이동
    while(cur -> front -> op != 1 && cur -> front != start)     //현재 위치한 디렉토리의 맨 처음 부분으로 갈 때 까지 cur 이동
        cur = cur -> front;     //cur 앞으로 이동
    while(cur -> back != NULL && cur -> front -> clos != 1){    //현재 위치한 디렉토리의 맨 끝부분이 될 때까지
        if(cur -> search_list == 1){                    //파일이 검색한 키워드와 일치하는 경우
            mvprintw(i,39,"|                                          |");
			if(cur==cur2){      //현재 커서 위치를 출력할 것임
                attron(COLOR_PAIR(1));        //색 변환 ON
                mvprintw(i, 40, "(%s)", cur -> a);     //괄호 포함해서 출력
                attroff(COLOR_PAIR(1));     //색 변환 OFF
			}
            else{   //현재 커서 위치가 아닌 파일명을 출력
                mvprintw(i, 40, "%s", cur -> a);    //그냥 파일 이름만 출력
            }
			i++;
        }
        cur = cur -> back;  //한칸 뒤로 이동
    }
    mvprintw(i,39,"|                                          |");
    mvprintw(i,39,"|           press button 1 to exit         |");
    mvprintw(i + 1,40,"------------------------------------------\n");
}
//precondition: cur2가 NULL값을 가지면 안 된다.
//postcondition: detail 정보를 옆에 출력해준 상태에서 searching 기능을 실행시켜 주고 기능이 끝나면 화면을 clear해 준다.
void use_search(){
    int ch;
    time_t lasttime;
    getmaxyx(curscr,termy,termx);     //가로세로 구하기
    lasttime=time(NULL);
    clear();        //화면 clear
    print_menu(2);      //2번 메뉴 케이스 출력
    print_menu(14);
    print_detail();                   //detail 정보 출력
    get_name();                       //이름 검색 실행
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)         //cur가 디렉토리 파일의 첫번째 파일을 포인트할 때까지 반복
        cur = cur -> front;
    while(cur -> back != NULL && cur -> front -> clos != 1){        //cur가 디렉토리 파일의 마지막 파일을 포인트할 때까지 반복
        cur -> search_list = 0;                                     //cur의 search_list값 0으로 초기화
        cur = cur -> back;
    }
    clear();        //화면 clear
}
//precondition: start가 NULL값을 가지면 안 된다. wd가 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: 링크드 리스트를 만들고 현재 디렉토리안의 파일들의 정보를 링크드 리스트에 삽입한다.
void save_tree(){
    start -> front = NULL;
    start -> back = NULL;
    TAIL = *start;      
    end = &TAIL;
    start -> back = end;        //링크드 리스트의 시작부분과 끝 부분을 연결
    end -> front = start;
    cur = start;
    DIR *dirp;      //디렉토리 포인터
    struct dirent *diritem;    //디렉토리 항목 포인터
    dirp = opendir(wd);                     //tp가 현재 경로를 가지도록 변경
    while((diritem=readdir(dirp)) != NULL){             //디렉토리 안의 파일들을 모두 읽을 때까지 반복
        if(strncmp(diritem -> d_name, ".", 1) != 0){        //.으로 시작하는 파일 제외
            type_ex = diritem -> d_type;                    
            char *str = diritem->d_name;
            snprintf(strbuf, PATH_MAX*2, "./%s", str);      //strbuf에 읽은 파일의 절대 경로를 저장
            insert_l(str);                      //파일의 정보를 링크드리스트에 삽입
        }
    }
}
//precondition: cur가 start, end를 포인터하고 있으면 안 된다.
//postcondition: 링크드리스트에서 cur가 가리키는 구조체를 삭제
void delete_l(){
    linked *p = cur;
    if(p == start)  return ;        //링크드리스트에 아무것도 존재하지 않는 경우
    p -> back -> front = p -> front;
    p -> front -> back = p -> back;
    cur = cur -> front;     //구조체 삭제 후 현재 위치 앞으로 이동
    free(p);
}
//precondition: tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다. ch는 디렉토리의 이름을 가지고 있어야 한다.
//postcondition: 디렉토리안에 존재하는 파일들의 크기의 합을 return한다.
int dir_size(char *ch){
    long int sum = 0;
    snprintf(strbuf, PATH_MAX*2, "%s/%s", tp, ch);
    DIR *dirp;                 //디렉토리 포인터
    struct dirent *diritem;    //디렉토리 항목 포인터
    dirp = opendir(strbuf);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){        //.으로 시작하는 파일 제외하고 링크드 리스트에 저장
            char *str = diritem->d_name;
            snprintf(name, PATH_MAX*2, "%s/%s",strbuf, str);
            stat(name, &st);
            sum += st.st_size;      //파일 각각의 사이즈를 구해서 sum에 더해줌
        }
    }
    return sum;     //최종적으로 구해진 sum의 값 return
}
//precondition: ch는 readdir로 읽은 파일의 이름으로 NULL이면 안 된다. strbuf는 파일의 경로를 저장하고 있어야 한다. type_ex는 파일의 type정보를 가지고 있어야 한다.
//postcondition: 동적으로 할당된 메모리에 파일 정보를 저장하여 링크드리스트에 삽입한다.
void insert_l(char *ch){
    linked *p = (linked *)malloc(sizeof(linked));
    strcpy(p -> a, ch);
    p -> type = type_ex;
    p -> op = 0;
    p -> clos = 0;
    p -> pin = 0;
    p  -> search_list = 0;      //구조체 인자들에 초기값 0으로 초기화
    stat(strbuf, &st);
    p -> list_time = st.st_mtim.tv_sec;
    sprintf(p -> list_change,"%s", ctime(&st.st_mtim.tv_sec));         //수정 시간을 요일/월/일/시간/년 형태로 바꾸어 문자열에 저장
    if(p -> type == DT_DIR)     //파일 타입이 디렉토리 일 경우, 
        p -> list_size = dir_size(ch);      //파일 사이즈 구조체 인자에 디렉토리 크기 배정
    else                        //파일 타입이 디렉토리가 아닐 경우,
        p -> list_size = st.st_size;        //일반 파일 크기 배정
    p -> back = cur -> back;
    p -> front = cur;
    cur -> back -> front = p;
    cur -> back = p;
    cur = cur -> back;
}
//precondition: cur와 least가 교환하고자 하는 구조체들을 각각 포인트하고 있어야한다.
//postcondition: cur가 가리키고 있는 구조체와 least가 가리키고 있는 구조체의 순서를 바꾼다.
//               cur3은 교환된 cur의 주소값을 가진고 cur는 교환된 least의 주소값을 가지게 한다.
void change_l(){
    linked *p;
    if(least -> clos == 1){       //least가 그 디렉토리 안의 마지막 파일일 경우
        least -> clos = 0;        //least의 clos값은 0으로
        cur -> clos = 1;          //cur의 clos값은 1로 바꾼다.
    }
    if(cur -> back == least){           //least가 cur바로 다음에 있는 경우
        cur -> front -> back = least;       //cur 앞에 있는 자료의 back이 least를 가리키도록 한다.
        least -> back -> front = cur;       //least 뒤에 있는 자료의 front가 cur를 가리키도록 한다.
        p = cur -> front;                   
        cur -> front = least;               
        least -> front = p;                 
        p = least -> back;
        least -> back = cur;
        cur -> back = p;
    }
    else{                               //least가 cur바로 다음에 오지 않는 경우
        cur -> front -> back = least;           //cur 앞에 있는 자료의 back이 least를 가리키도록 한다.
        cur -> back -> front = least;           //cur 뒤에 있는 자료의 front가 least를 가리키도록 한다.
        least -> front -> back = cur;           //least 앞에 있는 자료의 back이 cur를 가리키도록 한다.
        least -> back -> front = cur;           //least 뒤에 있는 자료의 front가 cur를 가리키도록 한다.
        p = cur -> front;
        cur -> front = least -> front;
        least -> front = p;
        p = cur -> back;
        cur -> back = least -> back;
        least -> back = p;
    }
    cur3 = cur;
    cur = least;
}
void Attrplus(){       //트리뷰에서 파일 이름 출력할 때 색 여부 판단하여 출력해주는 함수, 파일이름 앞에 +++붙임
	if(cur == cur2){    //출력하고자 하는 파일이 현재 커서라면
        attron(COLOR_PAIR(1));      //색 변환 ON
        printw("+++(%.*s)",termx-1,cur -> a);       //이름 앞에 +++ 포함하여 출력
        attroff(COLOR_PAIR(1));     //색변환 OFF
	}
    else{	//커서 위치가 아니라면
	    printw("+++%.*s",termx-1,cur -> a);     //색변환 없이 출력
	}
}
void Attrminus(){       //트리뷰에서 파일 이름 출력할 때 색 여부 판단하여 출력해주는 함수, 파일 이름 앞에 --- 붙임
    if(cur == cur2){    //출력하고자 하는 파일이 현재 커서라면
        attron(COLOR_PAIR(1));      //색 변환 ON
        printw("---(%.*s)",termx-1,cur -> a);   //이름 앞에 --- 포함하여 출력
        attroff(COLOR_PAIR(1));     //색 변환 OFF
    }
    else    //커서 위치가 아니라면
        printw("---%.*s",termx-1,cur -> a);     //색변환 없이 출력
}
void Attr(){        //트리뷰에서 파일 이름 출력할 때 색 여부 판단하여 출력해주는 함수
    if(cur == cur2){    //출력하고자 하는 파일이 현재 커서라면
        attron(COLOR_PAIR(1));     //색 변환 ON
        printw("(%.*s)",termx-1,cur -> a);      //이름만 출력
        attroff(COLOR_PAIR(1));     //색 변환 OFF
    }
    else   //커서 위치가 아니라면    
        printw("%.*s",termx-1,cur -> a);        //색변환 없이 출력
}
//precondition: cur은 start -> back의 주소값을 가져야 한다.
//postcondition: 링크드 리스트의 파일들을 tree view형식으로 출력한다.
void print_tree(){
    int i = 0;
    if(termy> 7)
        printw(" --- ");
    getyx(stdscr, y, x); //현재 커서 좌표 구하는 함수
    row = x - 3;
    push(row);      //row정보 stack에 저장
    termy -= 6;
    while(--termy > 0){
        if(cur -> op == 1){             //열린 디렉토리 맨 처음 부분일 때
            if(cur -> front -> op != 1 && cur -> front != start){
                mvprintw(column, row, "\\"); // 앞에 '\'출력하고 ---이름 출력
            	Attrminus();
            }
            else //디렉토리의 맨 처음 부분이 아니라면
				Attr();     //그냥 이름만 출력
            if(cur -> clos == 1)    //디렉토리의 맨 끝 부분이라면
                check = 1;
            cur = cur -> back;  //cur을 한칸 뒤로 이동 -> 다음 디렉토리로 이동
            termy += 7;
            print_tree();
        }
        else if(cur -> clos == 1){ //열린 디렉토리의 마지막 파일일 때
            if(i == 0){                 //열린 디렉토리 파일이 하나만 있을 때
                if (cur->type == DT_DIR)    //그 파일이 디렉토리라면
                	Attrplus();     //앞에 +++하고 이름 출력
				else if (cur->type == DT_REG)   //디렉토리가 아니라면
          	    	Attr(); //그냥 이름 출력
					column = y + 1;     //줄 한줄 바꿔줌
            }
            else{       //디렉토리의 맨 처음, 맨 마지막이 아닌 파일이라면
                mvprintw(column, row, "\\");    //                
                if (cur->type == DT_DIR)    //파일이 디렉토리라면
                	Attrplus();     //앞에 +++하고 이름 출력
				else if (cur->type == DT_REG)   //파일이 디렉토리가 아니라면
					Attrminus();    //---하고 파일이름 출력
                getyx(stdscr, y, x);
                column = y + 1;
            }
            pop();          //하위 디렉토리에서 상위 디렉토리의 row값으로 돌아가서 출력하기 위함
            if(check == 1){       //상위 디렉토리가 같은 경로의 파일 중 마지막 파일인지 확인
                pop();  //row 위치 전으로 돌림
                row = tops();   
                check = 0;
            }
            else
                row = tops();
            return ;
        }
        else if(i == 0){     //파일이 하나만 있을때
            if (cur->type == DT_DIR)    //디렉토리라면
            	Attrplus();     //+++하고 파일이름 출력
			else if (cur->type == DT_REG)   //디렉토리 아니라면
				Attr();     //그냥 이름 출력
            column = y + 1;     //줄 한줄 내림
        }
        else{
            mvprintw(column, row, "|");     //파일이름 맨 앞 '|' 출력            
            if (cur->type == DT_DIR)    //파일 타입이 디렉토리라면
				Attrplus();     //그냥 이름만 출력
            else if (cur->type == DT_REG)   //파일타입이 디렉토리가 아니라면
				Attrminus();    //앞에 --- 붙이고 출력
            getyx(stdscr, y, x);    //현재 출력하는 부분 위치 받아옴
            column = y + 1;     //줄 내림
        }
        if(cur -> back == &TAIL)       //링크드리스트의 맨 끝에 도달                                                                                                                                                                                                                                                                                                                     
            break;      //트리뷰 출력을 끝낸다
        cur = cur -> back;     
        i++;
    }
    if(termy > 0)
        mvprintw(column - 1, row, "\\");
}
//precondition: cur2는 디렉토리 파일을 포인트하고 있어야 하고, tp는 현재 디렉토리 경로를 문자열로 가지고 있어야 한다.
//postcondition: 디렉토리를 열어서 그 안에 있는 파일들의 정보를 링크드리스트에 저장한다.
//               열기로 한 디렉토리의 op값을 1로 변경, 열어놓은 디렉토리 안의 파일들 중 마지막 파일의 경우 clos값을 1로 저장한다.
//               만약 열기로 한 디렉토리에 파일이 하나도 없다면 링크드리스트에 Empty 라는 이름을 가진 파일을 추가해준다.
void open_dir(){
    int ch;
    cur = cur2;
    snprintf(dp, PATH_MAX*2, "%s/%s", tp, cur -> a);           //dp는 열려고 하는 디렉토리 경로, tp는 현재 디렉토리 경로
    cur -> op = 1;
    DIR *dirp;      //디렉토리 포인터
    struct dirent *diritem;    //디렉토리 항목 포인터
    dirp = opendir(dp);
    strcpy(tp, dp);                         //tp가 현재 경로를 가지도록 변경
    while((diritem=readdir(dirp)) != NULL){     //디렉토리 안의 파일들을 모두 읽을 때까지 반복
        if(strncmp(diritem -> d_name, ".", 1) != 0){        //.으로 시작하는 파일 제외하고 링크드 리스트에 저장
            type_ex = diritem -> d_type;
            char *str = diritem->d_name;
            snprintf(strbuf, PATH_MAX*2, "%s/%s",tp, str);   //strbuf에 읽은 파일의 절대 경로를 저장
            insert_l(str);                              //파일의 정보를 링크드리스트에 삽입
        }
    }
    cur -> clos = 1;                        //마지막 파일의 clos값을 1로 변경
    if(cur == cur2){                        //디렉토리 파일안에 아무 파일도 없을 때
        cur -> clos = 0;                    
        insert_l("Empty");                  //Empty라는 상태를 링크드리스트에 저장
        cur -> clos = 1;                    //Empty를 저장해 놓은 자료의 clos를 1로 변경
        empty = 1;
    }
}
//precondition: cur2는 열어놓은 디렉토리 안의 파일들 중 첫번쨰 파일을 포인트하고 있어야한다. 
//              dp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: 열어놓은 디렉토리의 파일들을 링크드리스에서 삭제하고 열었었던 디렉토리의 op값을 0으로 변경한다.
//               cur2는 닫은 디렉토리를 포인트하고 있고, 바뀐 경로를 tp와 dp에 문자열로 저장한다.
void close_dir(){
    cur = cur2 -> front;         //cur를 닫으려는 디렉토리를 가리키도록 한다.
    cur -> op = 0;               //cur->op값 0으로 초기화
    while(1){
        cur = cur -> back;       //다음 자료를 선택
        if(cur -> clos == 1){       //디렉토리 안의 마지막 파일일 경우
            delete_l();             //링크드리스트에서 자료를 삭제
            break;
        }
        else
            delete_l();             //링크드리스트에서 자료를 삭제
    }
    cur2 = cur;                 //현재 커서를 닫은 디렉토리를 가리키도록 한다.
    int k = strlen(dp) - strlen(cur -> a) - 1;          //k에 바뀐 경로의 문자 갯수를 저장
    sprintf(tp, "%.*s", k, dp);                          //tp에 바뀐 현재 경로를 저장한다.
}
//precondition: 1 <= ch <= 3 이어야하고 cur는 NULL이거나 정렬하고자 하는 파일들 중 마지막 파일을 포인트하고 있으면 안 된다. 
//              cur3은 cur -> back을 포인트하고 있어야한다.
//postcondition: ch값에 따라 파일이름, 크기, 수정날짜 중 하나를 기준으로 선택하여
//               cur가 포인트하고 있는 파일부터 현재 디렉토리의 마지막 파일까지 중 기준값이 가장 작은 값의 파일이 cur가 가리키는 위치에 들어간다.
void sort_type(char ch){
    if(ch == '1'){      //sort type인자가 1일 경우, 이름순으로 오름차순 정렬
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){    //cur3이 마지막 자료를 가리키지 않을 때까지 반복
            if(strcmp(least -> a, cur3 -> a) > 0)            //least가 남은 파일 중 이름이 가장 작은 자료를 가지도록 한다.
                least = cur3;                                
            cur3 = cur3 -> back;                            //cur3이 다음 자료를 가리키도록 한다.
        }
        if(strcmp(cur -> a, least -> a) != 0)              //바꾸려는 자료와 least가 가리키는 자료의 이름이 같지 않을 경우
            change_l();                                   //cur가 가리키는 자료와 least가 가리키는 자료를 변경한다.
    }
    else if(ch == '2'){     //sort type인자가 2일 경우, 파일 타입순으로 정렬(Directory인지 Regular file인지 구분)
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){      //cur3이 마지막 자료를 가리키지 않을 때까지 반복                           
            if(least -> type > cur3 -> type)                            //least가 타입이 디렉토리인 파일을 가리키도록 해라.
                least = cur3;
            cur3 = cur3 -> back;                        //cur3이 다음 자료를 가리키도록 한다.
        }
        if(cur -> type != least -> type)                //바꾸려는 자료와 least가 가리키는 자료의 타입이 같지 않을 경우
            change_l();                                 //cur가 가리키는 자료와 least가 가리키는 자료를 변경한다.
    }
    else if(ch == '3'){     //sort type인자가 3일 경우, 파일 크기순으로 오름차순 정렬
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){      //cur3이 마지막 자료를 가리키지 않을 때까지 반복 
            if(least -> list_size > cur3 -> list_size)              //least가 남은 파일 중 크기가 가장 작은 자료를 가지도록 한다.
                least = cur3;                    
            cur3 = cur3 -> back;                        //cur3이 다음 자료를 가리키도록 한다.
        }
        if(cur -> list_size != least -> list_size)      //바꾸려는 자료와 least가 가리키는 자료의 크기가 같지 않을 경우
            change_l();                                 //cur가 가리키는 자료와 least가 가리키는 자료를 변경한다.
    }
    else if(ch == '4'){     //sort type인자가 4일 경우, 파일 최종 수정시간순으로 오름차순 정렬
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){      //cur3이 마지막 자료를 가리키지 않을 때까지 반복 
            if(least -> list_time > cur3 -> list_time)              //least가 남은 파일 중 수정 시간이 가장 빠른 자료를 가지도록 한다.
                least = cur3;
            cur3 = cur3 -> back;                            //cur3이 다음 자료를 가리키도록 한다.
        }
        if(cur -> list_time != least -> list_time)      //바꾸려는 자료와 least가 가리키는 자료의 수정 시간이 같지 않을 경우
            change_l();                                     //cur가 가리키는 자료와 least가 가리키는 자료를 변경한다.
    }
}
//precondition: cur는 현재 디렉토리안의 첫번째 파일을 포인트하고 있어야 한다.
//postcondition: 현재 디렉토리안의 파일들 중 pin으로 고정된 파일들을 맨 앞으로 보내고 cur는 pin으로 고정되지 않은 첫번째 파일을 가리킨다.
void except_pin(){
    cur3 = cur;             //cur3은 디렉토리 안의 첫 번째 파일을 가리키도록 한다.
    while(cur -> back != NULL && cur -> front -> clos != 1){     //cur가 마지막 파일을 가리킬때까지 반복
        if(cur -> pin == 1){                //cur가 가리키는 파일이 pin기능이 사용된 경우
            cur -> pin = 0;
            least = cur3;                   //least가 디렉토리 안의 제일 처음 파일을 가리키도록 한다.
            change_l();                     //pin기능을 사용한 파일을 디렉토리 안에서 제일 처음 파일과 바꾼다.
            cur3 = cur3 -> back;            //cur3이 다음 파일을 가리킨다.
        }
        cur = cur -> back;                  //cur가 다음 파일을 가리킨다.
    }
    cur = cur3;             //cur가 pin기능으로 제외된 파일들 뒤의 자료를 가리키도록 한다.
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: 현재 디렉토리안의 파일을 ch값에 따라 파일이름, 크기, 수정날짜 중 하나를 선택하여 오름차순으로 정렬한다.
void sort(char ch){
    cur = cur2;         //cur가 현재 커서가 있는 자료를 가리키도록 한다.
    while(cur -> front -> op != 1 && cur -> front != start)     //cur가 디렉토리 안의 첫 번째 파일을 가리킬때까지 반복
        cur = cur -> front;                                     //cur가 앞의 자료를 가리킨다.
    except_pin();                       //pin기능을 사용한 파일들 정렬에서 제외
    while(cur -> back -> back != NULL && cur -> clos != 1){     //cur가 마지막 파일을 가리킬때까지 반복
        least = cur;
        cur3 = cur -> back;         //비교할 때 cur의 다음 자료부터 비교하도록 한다.
        sort_type(ch);      //sort_type함수 인자에 ch인자 넣어서 호출
        cur = cur -> back;          //cur가 다음 파일을 가리킨다.
    }
}
//precondition: tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: Sorting을 어떤 기준으로 할 것인지 메뉴를 화면에 출력해주고 ch값을 입력받고 정렬 기능을 실행한다.
void sorting_l(){
    char ch;
    time_t lasttime;
    print_menu(13);     //13번 메뉴 케이스 출력
    print_menu(14);
    print_menu(4);     //4번 메뉴 케이스 출력
    while(1){
        if(ch == '1'){
            sort(ch);
            break;      //sort함수 인자에 1넣어서 호출
        }
        else if(ch == '2'){
            sort(ch);
            break;      //sort함수 인자에 2넣어서 호출
        }
        else if(ch == '3'){
            sort(ch);
            break;      //sort함수 인자에 3넣어서 호출
        }
        else if(ch == '4'){
            sort(ch);
            break;      //sort함수 인자에 4넣어서 호출
        }
        else if(ch == 'x'){     //x키를 눌렀을때, 화면 clear후, 프로그램 종류
            clear();        //화면 clear
            break;
        }
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
    clear();        //화면 clear
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: Pin으로 고정된 파일들의 목록을 detail 옆에 출력해준다.
void print_pin(){
    int i = 3;
    mvprintw(0,86,"------------------------------------------\n");
    for(int i=1;i<3;i++){
        mvprintw(i,85,"|                                          |");
    }
    mvprintw(1,86,"PIN List: ");    
    cur = cur2;     //cur을 현재 커서 위치로 이동시킴
    while(cur -> front -> op != 1 && cur -> front != start)     //열린 디렉토리의 맨 처음이 될 때까지 cur을 앞으로 이동시킴
        cur = cur -> front;     //다음 cur로 이동
    while(cur -> back != NULL && cur -> front -> clos != 1){    //디렉토리의 맨 끝이 아니라면 
        if(cur -> pin == 1){
            mvprintw(i,85,"|                                          |");
            if(cur == cur2){    //cur의 위치가 현재 커서 위치라면
                snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);     
                mvprintw(i, 86, "%.*s",strlen(cur2 -> a) - 2, strbuf);
            }
            else
                mvprintw(i, 86, "%s", cur -> a);
            i++;
        }
        cur = cur -> back;
    }
    mvprintw(i,85,"|                                          |");
    mvprintw(i + 1,86,"------------------------------------------\n");
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: 현재 ()로 가리키고 있는 파일들을 p를 눌러서 Pin list에 추가하거나 뺄 수 있고 1누르면 Pin 기능을 종료한다.
void use_pin(){
    int ch = 0;
    time_t lasttime;
    while(1)    //탈출 조건-> 1 눌렀을때.
    {
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        clear();        //화면 clear
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: 커서 다운
                move_key('D');
                break;
            case KEY_UP:
            //TODO: 커서 업
                move_key('U');
                break;
            case 'p':           //현재 커서가 가리키는 파일에 pin기능을 사용
                if(cur2 -> pin == 1)       //선택한 파일이 이미 pin list로 선택한 경우
                    cur2 -> pin = 0;        //선택한 파일을 pin list에서 뺀다.
                else            //선택한 파일이 아직 pin list에 선택되지 않은 경우
                    cur2 -> pin = 1;        //선택한 파일을 pin list에 더한다.
                break;
        }
        print_menu(6);      //6번 메뉴 케이스 출력
        print_menu(14);
        if (ch == '1')     //1키를 눌렀을때, 프로그램 종류
            break;
        cur = start -> back;
        print_detail(); // 현재 디렉토리 안에 있는 파일만 자세하게 출력;
        print_pin();    //pin list를 출력한다.
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    clear();        //화면 clear
}
//precondition: tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: Pin기능을 사용할 것인지 메뉴로 출력하고 1을 누르면 Pin기능과 정렬 기능을 실행, 2를 누르면 정렬 기능만 실행한다.
void sorting_pin(){
    int ch = 0;
    time_t lasttime;
    print_menu(13);     //13번 메뉴 케이스 출력
    print_menu(14);
    print_menu(5);      //5번 메뉴 케이스 출력
    while(1){
        if(ch == '1'){
            clear();        //화면 clear
            use_pin();      //핀 기능 사용
            clear();        //화면 clear
            sorting_l();    //정렬 타입 선택해서 정렬
            break;
        }
        else if(ch == '2'){
            clear();        //화면 clear
            sorting_l();    //정렬 타입 선택해서 정렬
            break;
        }
        else if(ch == 'x'){     //x키를 눌렀을때, 화면 clear후, 프로그램 종류
            clear();        //화면 clear
            break;
        }
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);  //키 입력 될때 까지 대기
    }
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다.
//postcondition: 현재 디렉토리의 파일들의 이름, 크기, 수정날짜를 detail하게 화면에 출력한다.
void print_detail(){
    int q;
    cur = cur2;     //cur을 현재 위치로 옮겨준다
    while(cur -> front -> op != 1 && cur -> front != start)    //cur을 열린 디렉토리의 맨 처음으로 옮겨준다 
        cur = cur -> front;
    for(q = 0; q < 80; q++)     //줄 출력
        printw("-");
    printw("\n %-30s\tFile Type\tSize\tLast Modification\n", "File name");       //맨 위 파일 형태 출력
    for(q = 0; q < 80; q++)     //줄 출력
        printw("-");
    printw("\n");
    termy -= 6;       //줄바꿈
    while(--termy > 0){
        if(cur -> clos == 1 || cur -> back == &TAIL){       //열린 디렉토리의 맨 끝이 아닐때까지 탐색			
		    if(cur == cur2){   //출력 파일이 현재 위치라면 (현재위치==cur2) 
        		attron(COLOR_PAIR(1));      //색 변환
                snprintf(curstr, PATH_MAX*2,"(%s)", cur -> a);      ///curstr에 커서를 포함한 이름 넣어줌
        		printw(" %-30.*s\t%s\t\t%ld\t%s", termx-1, curstr, cur->type == 4 ? "Dir" : "Reg", cur -> list_size, cur ->list_change);     //파일 정보 색 변환해서 출력
       			//삼항 연산자를 이용하여 cur->type값이 4인 경우, Directory로 출력, 그렇지 않은 경우, Regular file로 출력
                attroff(COLOR_PAIR(1));     //색 변환 종료    
				break;
        	}
            else{      
            	printw(" %-30.*s\t%s\t\t%ld\t%s", termx-1, cur -> a, cur->type == 4 ? "Dir" : "Reg", cur -> list_size, cur ->list_change);       //파일 정보 색 변환 없이 출력
            	//삼항 연산자를 이용하여 cur->type값이 4인 경우, Directory로 출력, 그렇지 않은 경우, Regular file로 출력
                break;
			}
        }
        else{
            if(cur == cur2){
                attron(COLOR_PAIR(1));      //색 변환
                snprintf(curstr, PATH_MAX*2,"(%s)", cur -> a);      //curstr에 커서 포함 이름 넣어줌
                printw(" %-30.*s\t%s\t\t%ld\t%s", termx-1, curstr, cur->type == 4 ? "Dir" : "Reg", cur -> list_size, cur ->list_change);
                //삼항 연산자를 이용하여 cur->type값이 4인 경우, Directory로 출력, 그렇지 않은 경우, Regular file로 출력
                attroff(COLOR_PAIR(1));     //색 변환 종료
            }
            else{
                printw(" %-30.*s\t%s\t\t%ld\t%s", termx-1, cur -> a, cur->type == 4 ? "Dir" : "Reg", cur -> list_size, cur ->list_change);
       			//삼항 연산자를 이용하여 cur->type값이 4인 경우, Directory로 출력, 그렇지 않은 경우, Regular file로 출력
            }
        }
        cur = cur -> back;      //한칸 뒤로 이동
    }
    if(termy > 1)
        for(q = 0; q < 80; q++)
            printw("-");
}
//precondition: tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: 메뉴창을 출력해주고 숫자를 입력하여 복사, 이동, 삭제를 하는 함수를 호출한 후 기능이 끝이나면 화면을 clear하고 종료한다.
void use_edit(){
    char ch;
    time_t lasttime;
    print_menu(13);     //13번 메뉴 케이스 출력
    print_menu(14);
    print_menu(7);      //7번 메뉴 케이스 출력
    while(1){
        if(ch == '1'){      //파일 복사 기능
            copy_file_select();
            temp = 1;
            break;
        }
        else if(ch == '2'){      //파일 복사 기능
            move_file_select();
            temp = 1;
            break;
        }
        else if(ch == '3'){      //파일 삭제 기능
            cur = cur2;     //cur가 현재 커서가 가리키는 파일을 포인트하도록 한다.
            while(cur -> front -> op != 1 && cur -> front != start)     //cur가 디렉토리의 맨 처음 파일을 가리킬 때까지 반복
                cur = cur -> front;
            if(cur -> front -> clos == 1)           //현재 열어 놓은 디렉토리가 상위 디렉토리 파일 중 마지막 파일일 경우
                check = 1;
            delete_file_select();       //delete할 파일을 선택
            break;
        }
        else if(ch=='4'){
            File_rename();      //파일 이름 수정
            break;
		}
        else if(ch == 'x'){     //x키를 눌렀을때, 화면 clear후, 프로그램 종류
            clear();
            break;
        }
        refresh();
        while((ch=getch())==ERR);       //키를 입력받을때까지 대기
    }
    clear();        //화면 clear
}
//precondition: cur2가 NULL값을 가지거나 현재 디렉토리 밖의 파일을 포인트하면 안 된다. tp는 현재 디렉토리의 경로를 문자열로 가지고 있어야 한다.
//postcondition: 메뉴를 출력하고 ch값에 따라 2를 누르면 정렬 기능을 실행, 3을 누르면 검색기능을 실행하며 1을 누르면 detail 기능을 종료한다.
//               delete기능을 사용 후 디렉토리에 파일이 없으면 파일이 없다고 출력한 후 '1'누르면 tree view로 돌아간다. 그리고 dp, tp에 경로를 알맞게 저장한다.
void detail(){
    int ch;
    time_t lasttime;
    while(1)    //탈출 조건-> 뒤로가기 눌렀을때.
    {
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        clear();        //화면 clear
        switch(ch){
            case KEY_DOWN:
            //TODO: 커서 다운
                move_key('D');
                break;
            case KEY_UP:
            //TODO: 커서 업
                move_key('U');
                break;
            case '2':
                sorting_pin();        //정렬 기능
                break;
            case '3':
                use_search();           //검색 기능
                break;
            case '4':
                use_edit();             //편집 기능
                if(temp == 1){          
                    temp = 0;
                    return ;
                }
                break;
        }
        print_menu(2);      //2번 메뉴 케이스 출력
        print_menu(14);
        if (ch == '1')      //'1'누르면 tree view로 돌아감
            break;
        cur = start -> back;
        if(start -> back -> back == NULL || cur2 -> op == 1){       //delete 기능을 이용해서 디렉토리 안의 파일을 모두 제거했을 때
            cur2 -> op = 0;
            if(check != 1)              //현재 열어 놓은 디렉토리가 상위 디렉토리 파일 중 마지막 파일이 아닐 경우
                cur2 -> clos = 0;
            int k = strlen(dp) - strlen(cur2 -> a) + 1;     //k에 바뀐 경로의 문자 갯수를 저장
            sprintf(tp, "%.*s", k, dp);                 //tp에 바뀐 현재 경로를 저장한다.
            clear();        //화면 clear
            print_menu(3);      //3번 메뉴 케이스 출력
            refresh();
            while((ch=getch()) != '1');     //'1'누르면 tree view로 나가짐
            break;
        }
        print_detail();             // 현재 디렉토리 안에 있는 파일만 자세하게 출력;
        refresh();
        check = 0;
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    clear();        //화면 clear
}
//키보드 UP, DOWN, LEFT, RIGHT일 경우에 따라 현재 위치 변경
void move_key(char key){
    switch(key){
        case 'R':
            if(cur2 -> type == DT_DIR && cur2 -> op == 0){  //커서가 가리키는 파일이 디렉토리이고 열려있지 않은 경우
                open_dir();         //디렉토리 열기
                if(cur2 -> op == 1)
                    cur2 = cur2 -> back;    //연 디렉토리의 첫번째 항목으로 커서를 옮긴다
            }
            break;
        case 'L':
            if(cur2 -> front -> op != 0){
                empty = 0;
                close_dir();        //디렉토리 닫기
            }
            break;
        case 'U':
            if(cur2 -> front != start){
                if(cur2 -> front -> op == 0){
                    cur2 = cur2 -> front;       //커서 위로 이동
                }
            }
            break;
        case 'D':
            if(cur2 -> back -> back != NULL){
                if(cur2 -> clos == 0){
                    cur2 = cur2 -> back;        //커서 밑으로 이동
                }
            }
            break;
    }
}

int main(){
    int ch;
    time_t lasttime;
    initscr();
    cbreak();   //no linebuffering
    noecho();   //no echo of inputs
    nodelay(stdscr,TRUE);
	start_color();
    keypad(stdscr,TRUE);    //enable keypad, arrows
    getcwd(wd,PATH_MAX);    //현재 디렉토리 가져오기
    strcpy(tp, wd);         //wd는 최상위 디렉토리 경로로 바뀌지 않음
    save_tree(); //파일 정보 링크드리스트에 저장
    cur2 = end -> front;
	init_pair(1,COLOR_BLUE,COLOR_BLACK);
	init_pair(2,COLOR_CYAN,COLOR_BLACK);
    while(1){   //탈출 조건-> x눌렀을때.
        getmaxyx(curscr,termy,termx);     //가로세로 구하기
        lasttime=time(NULL);
        clear();        //화면 clear
        switch(ch){
            case KEY_DOWN:
            //TODO: 커서 다운
                move_key('D');
                break;
            case KEY_UP:
            //TODO: 커서 업
                move_key('U');
                break;
            case KEY_LEFT:
            //TODO: 커서 왼쪽(이전 위치)
                move_key('L');
                break;
            case KEY_RIGHT:
            //TODO: 커서 오른쪽(디렉토리 들어가기/동작없음)
                move_key('R');
                break;
            case '1':   //1번 누르면 detail 모드로 바뀜
                if(empty == 1){             //디렉토리 안에 파일이 아무것도 없을 때
                    clear();        //화면 clear
                    print_menu(3);      //3번 메뉴 케이스 출력
                    refresh();
                    while((ch=getch()) != '1');      //'1'누르면 tree view로 돌아감
                    clear();        //화면 clear
                    break;
                }
                detail();
                break;
        }
        if (ch == EXIT_KEY) //프로그램 종료
            break;
        print_menu(1);      //1번 메뉴 케이스 출력
        snprintf(strbuf,PATH_MAX,"%s",wd);
        if(termy-- > 8)
            printw("%.*s",termx-1,strbuf);           //현재 경로 출력
        cur = start -> back;
        if(start -> back -> back == NULL)           //링크드 리스트에 아무런 자료도 없을 때
            break;
        print_tree(); //tree view 형식으로 출력
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //키 입력 될때까지 대기
    }
    endwin();
    return 0;
}