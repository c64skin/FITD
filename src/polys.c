typedef char ColorP;
void hline(int x1, int x2, int y, ColorP c);


#include <stdio.h>
#include <assert.h>
#include <math.h>

#define SCREENHEIGHT 200
#define MAXPTS 10

#define putdot(x,y) if ((y >= 0) && (y < SCREENHEIGHT)) dots[y][counters[y]++] = x;

void swapFunc(int* a, int* b);
#define swap(a,b) (swapFunc(&a,&b))

void fillpoly(short int * datas, int n, ColorP c) {
    static int dots[SCREENHEIGHT][MAXPTS];
    static int counters[SCREENHEIGHT];
    short int x1, y1, x2, y2;
    int i, j, k, dir = 1;
    double step, curx;
    
    printf("fillpoly starting\n");
    
    assert(n >= 3);
    
    // Reinit array counters
    
    for (i = 0; i < SCREENHEIGHT; i++) {
	counters[i] = 0;
    }
    
    // Drawing lines
    
    x2 = datas[n * 2 - 2];
    y2 = datas[n * 2 - 1];
    
    for (k = 0; k < n; k++) {
	if (n == 4) {
	    switch (k) {
	    case 0: i = 0; break;
	    case 1: i = 1; break;
	    case 2: i = 2; break;
	    case 3: i = 3; break;
	    }
	} else {
	    i = k;
	}
	x1 = x2;
	y1 = y2;
	x2 = datas[i * 2];
	y2 = datas[i * 2 + 1];
	
	if (y1 == y2) {
	    // Not sure if this is right.
	    putdot(x1, y1);
	    putdot(x2, y2);
	    continue;
	}
	
	step = (double) (x2 - x1) / (y2 - y1);
	
	printf("x1: %i, y1 = %i, x2 = %i, y2 = %i, step: %f\n", x1, y1, x2, y2, step);
	
	curx = x1;
	
	if (y1 < y2) {
	    for (j = y1; j < y2; j++, curx += step) {
		printf("j = %i, curx = %f\n", j, curx);
		putdot(curx + 0.5, j);
	    }
	    if (dir == -1) {
		printf("Adding extra (%i, %i)\n", x1, y1);
		putdot(x1, y1);
	    }
	    dir = 1;
	} else {
	    for (j = y1; j > y2; j--, curx += step) {
		printf("j = %i, curx = %f\n", j, curx);
		putdot(curx + 0.5, j);
	    }
	    if (dir == 1) {
		printf("Adding extra (%i, %i)\n", x1, y1);
		putdot(x1, y1);
	    }
	    dir = -1;
	}
    }
    
    x1 = x2;
    y1 = y2;
    x2 = datas[0];
    y2 = datas[1];
    
    if (((y1 < y2) && (dir == -1)) || ((y1 > y2) && (dir == 1))) {
	printf("Adding extra (%i, %i)\n", x1, y1);
	putdot(x1, y1);
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
