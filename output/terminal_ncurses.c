#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

int init_terminal_ncurses() {
    initscr();
    noecho();
    use_default_colors();
    curs_set(0);
    timeout(0);
    return 0;
}


void get_terminal_dim_ncurses(int *w, int *h) {
    getmaxyx(stdscr,*h,*w);
    clear(); //clearing in case of resieze
}

int draw_terminal_ncurses(int virt, int h,  int w, 
                          int bars, int bw, int bs, 
                          int rest, int f[], int flastd[]) {

    if (virt != 0 && (LINES != h || COLS != w)) return -1;
    
    //const wchar_t* bh[] = {L"\u2581", L"\u2582", L"\u2583", L"\u2584", 
    //                      L"\u2585", L"\u2586", L"\u2587", L"\u2588"};

    int i, n;
    int msgLen = 23;
    const wchar_t* msg[] = {L"ｱ", L"ｷ", L"ｿ", L"ﾁ", L"ｺ",
                            L"ｽ", L"ｾ", L"ｵ", L"ｲ", L"ｻ",
                            L"ｴ", L"ﾀ", L"ｻ", L"ﾁ",
                            L"ｵ", L"ｶ", L"ｵ", L"ｾ", L"ｺ", L"ｴ",
                            L"ｵ", L"ﾀ", L"ｿ"};
    
    //do non-virtual things
    //i.e. not in a terminal application, but a "true" terminal
    if(virt == 0){
        for (i = 0; i <  bars; i++) {
            if(f[i] > flastd[i]){//higher then last one
                for (n = flastd[i]; n < f[i]; n++){ 
                    mvprintw(n, i*bw + i*bs + rest, "%d",8);
                    mvprintw(h - n - 1, i*bw + i*bs + rest, "%d",8);
                }
            }else if(f[i] <= flastd[i]){//lower then last one
                for (n = f[i]; n < flastd[i] + 1; n++){
                    mvaddstr(n, i*bw + i*bs + rest, " ");
                    mvaddstr(h - n - 1, i*bw + i*bs + rest, " ");
                }
            }
            flastd[i] = f[i]; //memmory for falloff func
        }
    }else{//Virtual terminal things are here i.e. gnome-terminal, xterm etc.
        for (i = 0; i <  bars; i++) {
            if(f[i] > flastd[i]){//higher then last one
                for (n = flastd[i]; n < f[i]; n++){ 
                    mvaddwstr(n, i*bw + i*bs + rest, msg[((i+n) % msgLen)]);
                    mvaddwstr(h - n - 1, i*bw + i*bs + rest, msg[((i+n) % msgLen)]);
                }
            }else if(f[i] < flastd[i]){//lower then last one
                for (n = f[i]; n < flastd[i]; n++){
                    mvaddstr(n, i*bw + i*bs + rest, " ");
                    mvaddstr(h - n - 1, i*bw + i*bs + rest, " ");
                }
            }
            flastd[i] = f[i]; //memmory for falloff func
        }
    }

    refresh();
    return 0;
}
// general: cleanup
void cleanup_terminal_ncurses(void){
    echo();
    system("setfont  >/dev/null 2>&1");
    system("setfont /usr/share/consolefonts/Lat2-Fixed16.psf.gz  >/dev/null 2>&1");
    system("setterm -blank 10");
    endwin();
    system("clear");
}

