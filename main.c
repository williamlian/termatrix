#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define REFRESH_RATE 1

typedef struct col_info_t {
	int col;
	int max_row;
	int delay_ms;
	int length;
	int gap;

	int current_row;
	int step_count;
} col_info;

char random_char();
int random_delay();

int main() {
	int row,col,i;
	initscr();noecho();getmaxyx(stdscr,row,col);start_color();
	curs_set(0);
	srand((unsigned)time(NULL));
	nodelay(stdscr, true);

	init_color(100, 1000,1000,1000); //bright white
	init_color(101, 700,700,700); //greenish dim white
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);

	//clear screen
	bkgdset(COLOR_PAIR(1) | ' ');
	erase();


	col_info* columns = (col_info*)malloc(sizeof(col_info) * col);
	for(i = 0; i < col; i++) {
		col_info* c = columns + i;
		c->col = i;
		c->max_row = row;
		c->delay_ms = random_delay();
		c->length = (row/2) + (rand() % (int)(row / 2)); 
		c->gap = (row/2) + rand() % (int)(row / 2); 
		c->current_row = 0;
		c->step_count = 0;
	}

	//update screen every 1 ms
	int tick = 0;
	while(1) {

		for(i = 0; i < col; i++) {
			col_info* c = columns + i;
			if(tick % c->delay_ms == 0) {
				if(c->step_count < c->length  + c->gap) {
					if(c->step_count <= c->length) {
						//print char
						mvaddch(c->current_row,c->col,random_char());
						mvchgat(c->current_row,c->col,1,A_BOLD,1,NULL);
					}
					else {
						//print gap
						mvaddch(c->current_row,c->col,' ');
						c->step_count++;
					}
					//dim white previous char
					int prev1 = (c->current_row + c->max_row - 1) % c->max_row;
					mvchgat(prev1,c->col,1,A_NORMAL,2,NULL);
					//green out the one before previous
					int prev2 = (c->current_row + c->max_row - 2) % c->max_row;
					mvchgat(prev2,c->col,1,A_BOLD,3,NULL);
					//dim out the 10-th before previous
					int prev5 = (c->current_row + c->max_row - 10) % c->max_row;
					mvchgat(prev5,c->col,1,A_DIM,3,NULL);
					c->step_count++;
				}
				else {
					//restart
					c->step_count = 0;
				}
				c->current_row = (c->current_row + 1) % c->max_row;
			}
		}
		if(getch() != ERR) break;
		tick = (tick + REFRESH_RATE) % 1000; 
		usleep(REFRESH_RATE * 1000);
	}
	
	endwin();

	free(columns);
	return 0;
}


char random_char() {
	return (char)(rand() % 95 + 32);
}

// 10ms to 300ms
int random_delay() {
	return rand() % 310 + 10;
}
