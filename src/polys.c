typedef char ColorP;
void hline(int x1, int x2, int y, ColorP c);


#include <stdio.h>
#include <assert.h>
#include <math.h>

#define SCREENHEIGHT 200
#define MAXPTS 10

void swapFunc(int* a, int* b);
#define swap(a,b) (swapFunc(&a,&b))

void fillpoly(short unsigned int * datas, int n, ColorP c) {
    static int dots[SCREENHEIGHT][MAXPTS];
    static int counters[SCREENHEIGHT];
    short unsigned int x1, y1, x2, y2;
    int i, j, k, dir = 0;
    double step, curx;
    
    assert(n >= 3);
    
    // Reinit array counters
    
    for (i = 0; i < SCREENHEIGHT; i++) {
	counters[i] = 0;
    }
    
    // Drawing lines
    
    x2 = datas[n * 2 - 2];
    y2 = datas[n * 2 - 1];
    
    for (i = 0; i < n; i++) {
	x1 = x2;
	y1 = y2;
	x2 = datas[i * 2];
	y2 = datas[i * 2 + 1];

	if (y1 == y2) {
	    dots[y1][counters[y1]++] = x1;
	    dots[y2][counters[y2]++] = x2;
	    continue;
	}
	
	step = (double) (x2 - x1) / (y2 - y1);
	
//	printf("x1: %i, y1 = %i, x2 = %i, y2 = %i, step: %f\n", x1, y1, x2, y2, step);
	
	curx = x1;
	
	if (y1 < y2) {
	    for (j = y1; j < y2; j++, curx += step) {
//		printf("j = %i, curx = %f\n", j, curx);
		dots[j][counters[j]++] = curx + 0.5;
	    }
	    if (dir == -1) {
//		printf("Adding extra (%i, %i)\n", x1, y1);
		dots[y1][counters[y1]++] = x1;
	    }
	    dir = 1;
	} else {
	    for (j = y1; j > y2; j--, curx += step) {
//		printf("j = %i, curx = %f\n", j, curx);
		dots[j][counters[j]++] = curx + 0.5;
	    }
	    if (dir == 1) {
//		printf("Adding extra (%i, %i)\n", x1, y1);
		dots[y1][counters[y1]++] = x1;
	    }
	    dir = -1;
	}
    }
    
    x1 = x2;
    y1 = y2;
    x2 = datas[0];
    y2 = datas[1];
    
    if (((y1 < y2) && (dir == -1)) || ((y1 > y2) && (dir == 1))) {
//	printf("Adding extra (%i, %i)\n", x1, y1);
	dots[y1][counters[y1]++] = x1;
    }
    
    // NOTE: all counters should be even now. If not, this is a bad (C) thing :-P
    
    // Sorting datas
    
    for (i = 0; i < SCREENHEIGHT; i++) {
        // Very bad sorting... but arrays are very small (0, 2 or 4), so it's no quite use...
	for (j = 0; j < (counters[i] - 1); j++) {
	    for (k = 0; k < (counters[i] - 1); k++) {
		if (dots[i][k] > dots[i][k + 1])
		    swap(dots[i][k], dots[i][k + 1]);
	    }
	}
    }
    
    // Drawing.
    
    for (i = 0; i < SCREENHEIGHT; i++) {
	if (counters[i]) {
    	    for (j = 0; j < counters[i]; j += 2) {
		hline(dots[i][j], dots[i][j + 1], i, c);
	    }
	}
    }
}
