#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define WIDTH 900
#define HEIGHT 600
#define MAX_COUNT 450

int count = 50; // Default
int *numbers;

void draw_bars() {
	int barWidth = WIDTH/count*0.89;
	for (int i = 0; i < count; i++) {
		int value = numbers[i];
		int barHeight = value*HEIGHT*0.75/count;
		int barY = HEIGHT*0.95-barHeight;
		int barX = WIDTH/count*i;
		DrawRectangle(barX, barY, barWidth, barHeight, WHITE);
	}	
}

void swap(int i, int j) {
	int tmp = numbers[i];
	numbers[i] = numbers[j];
	numbers[j] = tmp;
}

int sort_step(int step) {
	int i = step;
	int sorted = 0;
	if (numbers[i] > numbers[i+1]) {
		swap(i, i+1);
		sorted = 1;
	}
	return sorted;
}

void init_nums() {
	srand(time(NULL));
	for (int i = 0; i < count; i++) {
		numbers[i] = i;
	}

	// Fisher Yates Shuffle
	for (int i = count-1; i >= 0; i--) {
		int j = rand() % (i+1);

		swap(i, j);
	}
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		count = atoi(argv[1]);
	}

	if (count > MAX_COUNT) {
		printf("ERROR: Count of %d is too large, cannot exceed %d\n", count, MAX_COUNT);
		return -1;
	}

	numbers = malloc(count * sizeof(int));

	init_nums();

	InitWindow(WIDTH, HEIGHT, "Sort Visualiser");
	SetTargetFPS(count*10);
	int step = 0;
	int sortedThisStep;
	int sortedThisPass;
	int finished = 0;
	while (!WindowShouldClose()) {
		BeginDrawing();
		if (finished != 1) {
			ClearBackground(BLACK);
			sortedThisStep = sort_step(step);
			if (sortedThisStep == 1) {
				sortedThisPass = 1;
			}
			draw_bars();
			step++;
			if (step >= count - 1) {
				if (sortedThisPass == 0) {
					// Finished sorted
					int finished = 1;
				}
				sortedThisPass = 0;
				step = 0;
			}
		}
		EndDrawing();
	}

	free(numbers);
	CloseWindow();

	return 0;
}
