#include "test.h"

int pop(){return top == 0 ? -1 : stack[--top];}
void push(int a){stack[top++] = a;}
int tops(){return top == 0 ? -1 : stack[top - 1];}

void search(char name[100]){
    temp = 0;
    char *str;
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back -> back != NULL && cur -> clos != 1){
        str = cur -> a;
        while(*str){
            if(strncmp(str, name, strlen(name)) == 0){
                temp = 1;
                cur2 = cur;
                cur -> search_list = 1;
                break;
            }
            else{
                str++;
            }
        }
        cur = cur -> back;
    }
}
void select_search_list(){
    int ch;
    time_t lasttime;
    cbreak();   
    noecho();
    nodelay(stdscr,TRUE);
    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
    while(1)    //Ż�� ����-> 1 ��������.
    {
        getmaxyx(curscr,termy,termx);     //���μ��� ���ϱ�
        lasttime=time(NULL);
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: Ŀ�� �ٿ�
            if(cur2 -> back -> back != NULL && cur2 -> clos == 0){
                snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                cur = cur2;
                do{
                    cur = cur -> back;
                } while (cur -> search_list != 1 && cur -> back != NULL && cur -> front -> clos == 0);
                if(cur -> back != NULL && cur -> front -> clos == 0)
                    cur2 = cur;
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
            case KEY_UP:
            //TODO: Ŀ�� ��
            if(cur2 -> front != start && cur2 -> front -> op == 0){
                snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                cur = cur2;
                do{
                    cur = cur -> front;
                } while (cur -> search_list != 1 && cur != start && cur -> back -> op == 0);
                if(cur != start && cur -> back -> op == 0)
                    cur2 = cur;
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
        }
        if (ch == '1')
            break;
        print_search_list();
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //Ű �Է� �ɶ����� ���
    }
    clear();
}
void get_name(){
    char name1[100];
    mvprintw(0,86,"------------------------------------------\n");
    for(int i=1;i<5;i++){
        mvprintw(i,85,"|                                          |");
    }
    mvprintw(5,86,"------------------------------------------\n");
    mvprintw(1,86,"File Name: ");
    nocbreak();
    echo();
    nodelay(stdscr,FALSE);
    mvscanw(1,97,"%s",name1);
    search(name1);
    if(temp == 1){
        select_search_list();
    }
    else{
        mvprintw(3,86,"name not found");
        refresh();
        getch();
        nodelay(stdscr,TRUE);
        noecho();
        cbreak();
        snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
        strcpy(cur2 -> a, strbuf);
        return;
    }
}
void print_search_list(){
    int i = 3;
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back != NULL && cur -> front -> clos != 1){
        if(cur -> search_list == 1){
            mvprintw(i,85,"|                                          |");
            mvprintw(i, 86, "%s", cur -> a);
            i++;
        }
        cur = cur -> back;
    }
    mvprintw(i,85,"|                                          |");
    mvprintw(i + 1,86,"------------------------------------------\n");
}
void use_search(){
    int ch;
    time_t lasttime;
    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
    getmaxyx(curscr,termy,termx);     //���μ��� ���ϱ�
    lasttime=time(NULL);
    clear();
    printw("| MENU | 1. Go Back\t2. Sorting\t3. Searching\n\n");
    printw("%.*s\n",termx-1, tp);     //���� ��ΰ� ������� ���
    print_detail();
    get_name();
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back -> back != NULL && cur -> clos != 1){
        cur -> search_list = 0;
        cur = cur -> back;
    }
    clear();
}
//precondition: start�� NULL���� ������ �� �ȴ�. wd�� ���� ���丮�� ��θ� ���ڿ��� ������ �־�� �Ѵ�.
//postcondition: ��ũ�� ����Ʈ�� ����� ���� ���丮���� ���ϵ��� ������ ��ũ�� ����Ʈ�� �����Ѵ�.
void save_tree(){
    start -> front = NULL;
    start -> back = NULL;
    TAIL = *start;
    end = &TAIL;
    start -> back = end;
    end -> front = start;
    cur = start;
    DIR *dirp;      //���丮 ������
    struct dirent *diritem;    //���丮 �׸� ������

    dirp = opendir(wd);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){
            temp = diritem -> d_type;
            char *str = diritem->d_name;
            snprintf(strbuf, PATH_MAX*2, "./%s", str);
            insert_l(str);
        }
    }
}
//precondition: cur�� start, end�� �������ϰ� ������ �� �ȴ�.
//postcondition: ��ũ�帮��Ʈ���� cur�� ����Ű�� ����ü�� ����
void delete_l(){
    linked *p = cur;
    if(p == start)  return ;
    p -> back -> front = p -> front;
    p -> front -> back = p -> back;
    cur = cur -> front;
    free(p);
}
//precondition: ch�� readdir�� ���� ������ �̸����� NULL�̸� �� �ȴ�.
//postcondition: �������� �Ҵ�� �޸𸮿� ���� ������ �����Ͽ� ��ũ�帮��Ʈ�� �����Ѵ�.
void insert_l(char *ch){
    linked *p = (linked *)malloc(sizeof(linked));
    strcpy(p -> a, ch);
    p -> type = temp;
    p -> op = 0;
    p -> clos = 0;
    p -> pin = 0;
    p  -> search_list = 0;
    stat(strbuf, &st);
    p -> list_size = st.st_size;
    p -> list_time = st.st_mtim.tv_sec;
    sprintf(p -> list_change,"%s", ctime(&st.st_mtim.tv_sec));         //���� �ð��� ����/��/��/�ð�/�� ���·� �ٲپ� ���ڿ��� ����
    p -> back = cur -> back;
    p -> front = cur;
    cur -> back -> front = p;
    cur -> back = p;
    cur = cur -> back;
}
//precondition: cur�� least�� ��ȯ�ϰ��� �ϴ� ����ü���� ���� ����Ʈ�ϰ� �־���Ѵ�.
//postcondition: cur�� ����Ű�� �ִ� ����ü�� least�� ����Ű�� �ִ� ����ü�� ������ �ٲ۴�.
//               cur3�� ��ȯ�� cur�� �ּҰ��� ������ cur�� ��ȯ�� least�� �ּҰ��� ������ �Ѵ�.
void change_l(){
    linked *p;
    if(least -> clos == 1){
        least -> clos = 0;
        cur -> clos = 1;
    }
    if(cur -> back == least){           //least�� cur�ٷ� ������ �ִ� ���
        cur -> front -> back = least;
        least -> back -> front = cur;
        p = cur -> front;
        cur -> front = least;
        least -> front = p;
        p = least -> back;
        least -> back = cur;
        cur -> back = p;
    }
    else{
        cur -> front -> back = least;
        cur -> back -> front = least;
        least -> front -> back = cur;
        least -> back -> front = cur;
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
//precondition: cur�� start -> back�� �ּҰ��� ������ �Ѵ�.
//postcondition: ��ũ�� ����Ʈ�� ���ϵ��� tree view�������� ����Ѵ�.
void print_tree(){
    int i = 0;
    printw(" --- ");
    getyx(stdscr, y, x); //���� Ŀ�� ��ǥ ���ϴ� �Լ�
    row = x - 3;
    push(row);      //row���� stack�� ����
    while(--termy > 0){
        if(cur -> op == 1){             //���� ���丮�� ���� ��
            if(cur -> front -> op != 1){
                mvprintw(column, row, "\\");
                
                if (cur->type == DT_DIR)
                    printw("+%.*s",termx-1,cur -> a);
                else if (cur->type == DT_REG)
                    printw("-%.*s",termx-1,cur -> a);
            }
            else
                printw("%.*s",termx-1,cur -> a);
            if(cur -> clos == 1)
                check = 1;
            cur = cur -> back;
            print_tree();
        }
        else if(cur -> clos == 1){ //���� ���丮�� ������ ������ ��
            if(i == 0){                 //���� ���丮 ������ �ϳ��� ���� ��
                printw("%.*s",termx-1,cur -> a);
                column = y + 1;
            }
            else{
                mvprintw(column, row, "\\");
                
                if (cur->type == DT_DIR)
                    printw("+%.*s",termx-1,cur -> a);
                else if (cur->type == DT_REG)
                    printw("-%.*s",termx-1,cur -> a);

                getyx(stdscr, y, x);
                column = y + 1;
            }
            pop();          //���� ���丮���� ���� ���丮�� row������ ���ư��� ����ϱ� ����
            if(check == 1){       //���� ���丮�� ���� ����� ���� �� ������ �������� Ȯ��
                pop();
                row = tops();
                check = 0;
            }
            else
                row = tops();
            return ;
        }
        else if(i == 0){
            printw("%.*s",termx-1,cur -> a);
            column = y + 1;
        }
        else{
            mvprintw(column, row, "|");
            
            if (cur->type == DT_DIR)
                printw("+%.*s",termx-1,cur -> a);
            else if (cur->type == DT_REG)
                printw("-%.*s",termx-1,cur -> a);

            getyx(stdscr, y, x);
            column = y + 1;
        }
        if(cur -> back == &TAIL)                                                                                                                                                                                                                                                                                                                          
            break;
        cur = cur -> back;
        i++;
    }
    if(termy > 0)
        mvprintw(column - 1, row, "\\");
}
//precondition: cur2�� ���丮 ������ ����Ʈ�ϰ� �־�� �ϰ�, tp�� ���� ���丮 ��θ� ���ڿ��� ������ �־�� �Ѵ�.
//postcondition: ���丮�� ��� �� �ȿ� �ִ� ���ϵ��� ������ ��ũ�帮��Ʈ�� �����Ѵ�.
//               ����� �� ���丮�� op���� 1�� ����, ������� ���丮 ���� ���ϵ� �� ������ ������ ��� clos���� 1�� �����Ѵ�.
void open_dir(){
    cur = cur2;
    snprintf(dp, PATH_MAX*2, "%s/%s", tp, cur -> a);           //dp�� ������ �ϴ� ���丮 ���, tp�� ���� ���丮 ���
    cur -> op = 1;
    DIR *dirp;      //���丮 ������
    struct dirent *diritem;    //���丮 �׸� ������
    dirp = opendir(dp);
    strcpy(tp, dp);
    while((diritem=readdir(dirp)) != NULL){
        if(strncmp(diritem -> d_name, ".", 1) != 0){        //.���� �����ϴ� ���� �����ϰ� ��ũ�� ����Ʈ�� ����
            temp = diritem -> d_type;
            char *str = diritem->d_name;
            snprintf(strbuf, PATH_MAX*2, "%s/%s",tp, str);
            insert_l(str);
        }
    }
    cur -> clos = 1;
    if(cur == cur2){
        cur -> op = 0;
        cur -> clos = 0;
        int k = strlen(dp) - strlen(cur -> a) - 1;  
        sprintf(tp, "%.*s", k, dp);                               
        strcpy(dp, tp);
    }
}
//precondition:  cur2�� ������� ���丮 ���� ���ϵ� �� ù���� ������ ����Ʈ�ϰ� �־���Ѵ�. 
//               dp�� ���� ���丮�� ��θ� ���ڿ��� ������ �־�� �Ѵ�.
//postcondition: ������� ���丮�� ���ϵ��� ��ũ�帮������ �����ϰ� �������� ���丮�� op���� 0���� �����Ѵ�.
//               cur2�� ���� ���丮�� ����Ʈ�ϰ� �ְ�, �ٲ� ��θ� tp�� dp�� ���ڿ��� �����Ѵ�.
void close_dir(){
    cur = cur2 -> front;
    cur -> op = 0;
    while(1){
        cur = cur -> back;
        if(cur -> clos == 1){
            delete_l();     
            break;
        }
        else
            delete_l();
    }
    cur2 = cur;
    int k = strlen(dp) - strlen(cur -> a) - 1;
    sprintf(tp, "%.*s", k, dp);                                
    strcpy(dp, tp);
}
//precondition: 1 <= ch <= 3 �̾���ϰ� cur�� NULL�̰ų� �����ϰ��� �ϴ� ���ϵ� �� ������ ������ ����Ʈ�ϰ� ������ �� �ȴ�. 
//              cur3�� cur -> back�� ����Ʈ�ϰ� �־���Ѵ�.
//postcondition: ch���� ���� �����̸�, ũ��, ������¥ �� �ϳ��� �������� �����Ͽ�
//               cur�� ����Ʈ�ϰ� �ִ� ���Ϻ��� ���� ���丮�� ������ ���ϱ��� �� ���ذ��� ���� ���� ���� ������ cur�� ����Ű�� ��ġ�� ����.
void sort_type(char ch){
    if(ch == '1'){
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){
            if(strcmp(least -> a, cur3 -> a) > 0)
                least = cur3;
            cur3 = cur3 -> back;
        }
        if(strcmp(cur -> a, least -> a) != 0)
            change_l();
    }
    else if(ch == '2'){
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){
            if(least -> list_size > cur3 -> list_size)
                least = cur3;
            cur3 = cur3 -> back;
        }
        if(cur -> list_size != least -> list_size)
            change_l();
    }
    else if(ch == '3'){
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){
            if(least -> list_time > cur3 -> list_time)
                least = cur3;
            cur3 = cur3 -> back;
        }
        if(cur -> list_time != least -> list_time)
            change_l();
    }
    else if(ch == '4'){
        while(cur3 -> back != NULL && cur3 -> front -> clos != 1){
            if(least -> type > cur3 -> type)
                least = cur3;
            cur3 = cur3 -> back;
        }
        if(cur -> type != least -> type)
            change_l();
    }
}
//precondition: cur�� ���� ���丮���� ù��° ������ ����Ʈ�ϰ� �־�� �Ѵ�.
//postcondition: ���� ���丮���� ���ϵ� �� pin���� ������ ���ϵ��� �� ������ ������ cur�� pin���� �������� ���� ù��° ������ ����Ų��.
void except_pin(){
    cur3 = cur;
    while(cur -> back != NULL && cur -> front -> clos != 1){
        if(cur -> pin == 1){
            cur -> pin = 0;
            least = cur3;
            change_l();
            cur3 = cur3 -> back;
        }
        cur = cur -> back;
    }
    cur = cur3;
}
//precondition: cur2�� NULL���� �����ų� ���� ���丮 ���� ������ ����Ʈ�ϸ� �� �ȴ�.
//postcondition: ���� ���丮���� ������ ch���� ���� �����̸�, ũ��, ������¥ �� �ϳ��� �����Ͽ� ������������ �����Ѵ�.
void sort(char ch){
    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    except_pin();
    while(cur -> back -> back != NULL && cur -> clos != 1){
        least = cur;
        cur3 = cur -> back;
        sort_type(ch);
        cur = cur -> back;
    }
    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
}
//precondition: tp�� ���� ���丮�� ��θ� ���ڿ��� ������ �־�� �Ѵ�.
//postcondition: Sorting�� � �������� �� ������ �޴��� ȭ�鿡 ������ְ� ch���� �Է¹ް� ���� ����� �����Ѵ�.
void sorting_l(){
    char ch;
    time_t lasttime;
    printw("| MENU | 1. Go Back\t2. Sorting\n\n");
    printw("%.*s\n",termx-1, tp);
    printw("\n1. File Name\n2. Size\n3. Last Modification\n4. File Type\n");
    while(1){
        if(ch == '1'){
            sort(ch);
            break;
        }
        else if(ch == '2'){
            sort(ch);
            break;
        }
        else if(ch == '3'){
            sort(ch);
            break;
        }
        else if(ch == '4'){
            sort(ch);
            break;
        }
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
    clear();
}
//precondition: cur2�� NULL���� �����ų� ���� ���丮 ���� ������ ����Ʈ�ϸ� �� �ȴ�.
//postcondition: Pin���� ������ ���ϵ��� ����� detail ���� ������ش�.
void print_pin(){
    int i = 3;
    mvprintw(0,86,"------------------------------------------\n");
    for(int i=1;i<3;i++){
        mvprintw(i,85,"|                                          |");
    }
    mvprintw(1,86,"PIN List: ");
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    while(cur -> back != NULL && cur -> front -> clos != 1){
        if(cur -> pin == 1){
            mvprintw(i,85,"|                                          |");
            if(cur == cur2){
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
//precondition: cur2�� NULL���� �����ų� ���� ���丮 ���� ������ ����Ʈ�ϸ� �� �ȴ�.
//postcondition: ���� ()�� ����Ű�� �ִ� ���ϵ��� p�� ������ Pin list�� �߰��ϰų� �� �� �ְ� 1������ Pin ����� �����Ѵ�.
void use_pin(){
    int ch;
    time_t lasttime;
    while(1)    //Ż�� ����-> 1 ��������.
    {
        getmaxyx(curscr,termy,termx);     //���μ��� ���ϱ�
        lasttime=time(NULL);
        clear();
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: Ŀ�� �ٿ�
            if(cur2 -> back -> back != NULL){
                if(cur2 -> clos == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> back;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case KEY_UP:
            //TODO: Ŀ�� ��
            if(cur2 -> front != start){
                if(cur2 -> front -> op == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> front;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case 'p':
            if(cur2 -> pin == 1)
                cur2 -> pin = 0;
            else
                cur2 -> pin = 1;
            break;
        }
        printw("| MENU | 1. Quit\n\n");
        printw("|| If you want to use pin, press 'p' ||\n");
        if (ch == '1')
            break;
        cur = start -> back;
        print_detail(); // ���� ���丮 �ȿ� �ִ� ���ϸ� �ڼ��ϰ� ���;
        print_pin();
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //Ű �Է� �ɶ����� ���
    }
    clear();
}
//precondition: tp�� ���� ���丮�� ��θ� ���ڿ��� ������ �־�� �Ѵ�.
//postcondition: Pin����� ����� ������ �޴��� ����ϰ� 1�� ������ Pin��ɰ� ���� ����� ����, 2�� ������ ���� ��ɸ� �����Ѵ�.
void sorting_pin(){
    char ch;
    time_t lasttime;
    printw("| MENU | 1. Go Back\t2. Sorting\n\n");
    printw("%.*s\n",termx-1, tp);
    printw("\n1. Use pin\n2. Don't use pin\n");
    while(1){
        if(ch == '1'){
            clear();
            use_pin();
            clear();
            sorting_l();
            break;
        }
        else if(ch == '2'){
            clear();
            sorting_l();
            break;
        }
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);
    }
}
//precondition: cur2�� NULL���� �����ų� ���� ���丮 ���� ������ ����Ʈ�ϸ� �� �ȴ�.
//postcondition: ���� ���丮�� ���ϵ��� �̸�, ũ��, ������¥�� detail�ϰ� ȭ�鿡 ����Ѵ�.
void print_detail(){
    cur = cur2;
    while(cur -> front -> op != 1 && cur -> front != start)
        cur = cur -> front;
    printw("\n%-30s\tFile Type\tSize\tLast Modification\n\n", "File name");
    while(--termy > 0){
        if(cur -> clos == 1 || cur -> back == &TAIL){
            printw("%-30.*s\t%s\t\t%ld\t%s", termx-1, cur -> a, cur->type == 4 ? "Dir" : "Reg", cur -> list_size, cur ->list_change);
            break;
        }
        else{
            printw("%-30.*s\t%s\t\t%ld\t%s", termx-1, cur -> a, cur->type == 4 ? "Dir" : "Reg", cur -> list_size, cur ->list_change);
        }
        cur = cur -> back;
    }
}
//precondition: cur2�� NULL���� �����ų� ���� ���丮 ���� ������ ����Ʈ�ϸ� �� �ȴ�. tp�� ���� ���丮�� ��θ� ���ڿ��� ������ �־�� �Ѵ�.
//postcondition: �޴��� ����ϰ� ch���� ���� 2�� ������ ���� ����� ����, 3�� ������ �˻������ �����ϸ� 1�� ������ detail ����� �����Ѵ�.
void detail(){
    int ch;
    time_t lasttime;
    while(1)    //Ż�� ����-> �ڷΰ��� ��������.
    {
        getmaxyx(curscr,termy,termx);     //���μ��� ���ϱ�
        lasttime=time(NULL);
        clear();
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: Ŀ�� �ٿ�
            if(cur2 -> back -> back != NULL){
                if(cur2 -> clos == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> back;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case KEY_UP:
            //TODO: Ŀ�� ��
            if(cur2 -> front != start){
                if(cur2 -> front -> op == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> front;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case '2':
            sorting_pin();        //���� ���
            break;
            case '3':
			use_search();
            break;
        }
        printw("| MENU | 1. Go Back\t2. Sorting\t3. Searching\n\n");
        printw("%.*s\n",termx-1, tp);     //���� ��ΰ� ������� ���
        if (ch == '1') //left������ tree view�� ���ư�
            break;
        cur = start -> back;
        print_detail(); // ���� ���丮 �ȿ� �ִ� ���ϸ� �ڼ��ϰ� ���;
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //Ű �Է� �ɶ����� ���
    }
    clear();
}

int main(){
    int ch;
    time_t lasttime;
    initscr();
    cbreak();   //no linebuffering
    noecho();   //no echo of inputs
    nodelay(stdscr,TRUE);
    keypad(stdscr,TRUE);    //enable keypad, arrows
    getcwd(wd,PATH_MAX);    //���� ���丮 ��������
    save_tree(); //���� ���� ��ũ�帮��Ʈ�� ����
    cur2 = end -> front;
    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
    strcpy(cur2 -> a, strbuf);
    strcpy(tp, wd);         //wd�� �ֻ��� ���丮 ��η� �ٲ��� ����
    while(1)    //Ż�� ����-> x��������.
    {
        getmaxyx(curscr,termy,termx);     //���μ��� ���ϱ�
        lasttime=time(NULL);
        clear();
        //printw("%.*s\n",termx-1, tp);     //���� ��ΰ� ������� ���
        switch(ch)
        {
            case KEY_DOWN:
            //TODO: Ŀ�� �ٿ�
            if(cur2 -> back -> back != NULL){
                if(cur2 -> clos == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> back;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case KEY_UP:
            //TODO: Ŀ�� ��
            if(cur2 -> front != start){
                if(cur2 -> front -> op == 0){
                    snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                    strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                    cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                    cur2 = cur2 -> front;
                    snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                    strcpy(cur2 -> a, strbuf);
                }
            }
            break;
            case KEY_LEFT:
            //TODO: Ŀ�� ����(���� ��ġ)
            if(cur2 -> front -> op != 0){
                close_dir();        //���丮 �ݱ�
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
            case KEY_RIGHT:
            //TODO: Ŀ�� ������(���丮 ����/���۾���)
            if(cur2 -> type == DT_DIR && cur2 -> op == 0){
                snprintf(strbuf,PATH_MAX, "%s", cur2 -> a + 1);
                strncpy(cur2 -> a, strbuf, strlen(cur2 -> a) - 2);
                cur2 -> a[strlen(cur2 -> a) - 2] = '\0';
                open_dir();         //���丮 ����
                if(cur2 -> op == 1)
                    cur2 = cur2 -> back;
                snprintf(strbuf,PATH_MAX*2,"(%s)", cur2 -> a);
                strcpy(cur2 -> a, strbuf);
            }
            break;
            case '1':   //1�� ������ detail ���� �ٲ�
            detail();
            break;
        }
        if (ch == EXIT_KEY) //���α׷� ����
            break;
        printw("| MENU | 1. See Detail\t\tx. Exit\n\n");
        snprintf(strbuf,PATH_MAX,"%s",wd);
        printw("%.*s",termx-1,strbuf);
        cur = start -> back;
        print_tree(); // tree view �������� ���
        refresh();
        while((ch=getch())==ERR && time(NULL)-lasttime<3);   //Ű �Է� �ɶ����� ���
    }
    endwin();
    return 0;
}