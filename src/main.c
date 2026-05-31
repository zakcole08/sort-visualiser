#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 900
#define HEIGHT 600
#define MAX_COUNT 450

int count = 50; // Default
char *alg = "bubble";
int *numbers;

typedef struct {
	bool swappedThisPass;
	int i, j; // Swapped bars
	bool done;
} SortStatus;

SortStatus status;

void draw_bars() {
	int barWidth = WIDTH/count*0.89;
	for (int i = 0; i < count; i++) {
		Color colour = WHITE;
		int value = numbers[i];
		int barHeight = (value+1)*HEIGHT*0.75/count;
		int barY = HEIGHT*0.95-barHeight;
		int barX = WIDTH/count*i;
		if (status.swappedThisPass == true && (i == status.i || i == status.j))
		{
			colour = RED;
		}
		DrawRectangle(barX, barY, barWidth, barHeight, colour);
	}	
}

void swap(int i, int j) {
	int tmp = numbers[i];
	numbers[i] = numbers[j];
	numbers[j] = tmp;
}

void shuffle() {
	srand(time(NULL));
	// Fisher Yates Shuffle
	for (int i = count-1; i >= 0; i--) {
		int j = rand() % (i+1);
		swap(i, j);
	}
}

void shuffle_step(int step) {
	srand(time(NULL));
	// Fisher Yates Shuffle
	int i = step;
	int j = rand() % (i+1);
	swap(i, j);
	status.i = i;
	status.j = j;
}

bool is_sorted() {
	for (int i = 1; i < count; i++) {
		if (numbers[i] < numbers[i-1]) {
			return false;
		}
	}
	return true;
}	

int bogo_sort(int step) {
	int i = step;
	int sortedThisStep = 0;
	while (is_sorted() == false) {
		shuffle_step(step);
		sortedThisStep = 1;
		return sortedThisStep;
	}
	return sortedThisStep;	
}

int bubble_sort(int step) {
	int i = step;
	int sortedThisStep = 0;
	if (numbers[i] > numbers[i+1]) {
		swap(i, i+1);
		status.i = i;
		status.j = i+1;
		sortedThisStep = 1;
	}
	return sortedThisStep;
}

int sort_step(int step) {
	int sortedThisStep = 0;
	if (strcmp(alg, "bubble") == 0) {
		sortedThisStep = bubble_sort(step);
	}
	else if (strcmp(alg, "bogo") == 0) {
		sortedThisStep = bogo_sort(step);
	}
	else {
		printf("Specified alg doesn't exist. Defaulting to %s\n", alg);
	}
	return sortedThisStep;
}

void init_nums() {
	for (int i = 0; i < count; i++) {
		numbers[i] = i;
	}
	shuffle();	
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		count = atoi(argv[1]);
	}
	if (argc > 2) {
		alg = argv[2];
	}

	if (count > MAX_COUNT) {
		printf("ERROR: Count of %d is too large, cannot exceed %d\n", count, MAX_COUNT);
		return -1;
	}

	numbers = malloc(count * sizeof(int));

	init_nums();

	InitWindow(WIDTH, HEIGHT, "Sort Visualiser");
	SetTargetFPS(count*4 > 240 ? 240 : count*4);
	int step = 0;
	int sortedThisStep;
	while (!WindowShouldClose()) {
		BeginDrawing();
		if (status.done != 1) {
			ClearBackground(BLACK);
			sortedThisStep = sort_step(step);
			if (sortedThisStep == 1) {
				status.swappedThisPass = true;
			}
			draw_bars();
			step++;
			if (step >= count - 1) {
				if (status.swappedThisPass == 0) {
					// Finished sorting
					status.done = true;
				}
				status.swappedThisPass = 0;
				step = 0;
			}
		}
		EndDrawing();
	}

	free(numbers);
	CloseWindow();

	return 0;
}
