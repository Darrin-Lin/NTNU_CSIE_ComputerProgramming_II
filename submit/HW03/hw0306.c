#include <curses.h> /* 引進 curses.h, 並自動引進 stdio.h */

#define StartX 1 /* 決定游標初始位置 */
#define StartY 1

void initial();

main()
{
    int x = StartX; /* 宣告 x,y 並設定其初值 */
    int y = StartY;
    int ch; /* 宣告 ch 為整數,配合 getch() 使用 */

    initial(); /* 呼叫 initial(), 啟動 curses 模式, */
               /* 並完成其它設定 */

    // box(stdscr, '|', '-'); /* 畫方框 */
    init_pair(1, COLOR_BLUE, COLOR_GREEN);
    wattron(stdscr, COLOR_PAIR(1));
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    attron(A_REVERSE); /* 開啟反白模式 */
    wattroff(stdscr, COLOR_PAIR(1));
    
    mvaddstr(0, 20, "Curses Program"); /* 在 (20,0) 處輸出反白字元 */

    attroff(A_REVERSE); /* 關閉反白模式 */

    move(x, y); /* 將游標移至初始位置 */

    do
    {                 /* 以無限迴圈不斷等待輸入 */
        ch = getch(); /* 等待自鍵盤輸入字元 */
        switch (ch)
        { /* 判斷輸入字元為何 */

        case KEY_UP:
            --y; /* 判斷是否"↑"鍵被按下 */
            break;
        case KEY_DOWN:
            ++y; /* 判斷是否"↓"鍵被按下 */
            break;
        case KEY_RIGHT:
            ++x; /* 判斷是否"→"鍵被按下 */
            break;
        case KEY_LEFT:
            --x; /* 判斷是否"←"鍵被按下 */
            break;
        case '\r': /* 判斷是否 ENTER 鍵被按下 */
            ++y;
            x = 0;
            break;
        case '\t': /* 判斷是否 TAB 鍵被按下 */
            x += 7;
            break;
        case 127:                 /* 判斷是否 BACKSPACE 鍵被按下 */
            mvaddch(y, --x, ' '); /* delete 一個字元 */
            break;

        case 27:
            endwin(); /* 判斷是否[ESC]鍵被按下 */
            exit(1);  /* 結束 curses 模式 */
                      /* 結束此程式 */

        default:
            addch(ch); /* 如果不是特殊字元, 將此字元印出 */
            x++;
            break;
        }
        move(y, x); /* 移動游標至現在位置 */
    } while (1);
}

void initial() /* 自定開啟 curses 函式 */
{
    initscr();
    cbreak();
    nonl();
    noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    start_color();
    refresh();
}