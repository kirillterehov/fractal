#include <iostream>
#include <omp.h>
#include <iomanip>
#include <cmath>
# include <fstream>
#include <cstdlib>
# include <ctime>
# include <cstring>
# include "CImg.h"
#include <random>
#include <thread>

const int dlina = 500;
const int shirina = 500;
int mas[4][dlina][shirina];
int summamas[dlina][shirina];
float x[4][1000];
float y[4][1000];
using namespace std;
using namespace cimg_library;
int main() {
	int max, potok;
	int numberOfColorChannels = 3;
	unsigned char initialValue = 0;

	std::random_device rd;
	std::mt19937 gen[4];
	std::uniform_real_distribution<double>dis[4];


#pragma omp parallel
	{

		float a, b, c;
		c = (float)4 / dlina;
		int indeks, roundx, roundy, q;
		indeks = 0;
#pragma omp for
		for (int potok = 0; potok <= 3; potok++) {
			gen[potok] = mt19937(rd());
			dis[potok] = uniform_real_distribution<>(-2.0, 2.0);
			x[potok][0] = 0.0;
			y[potok][0] = 0.0;
			for (int i = 0; i < 250000; i++) {
				a = dis[potok](gen[potok]);
				b = dis[potok](gen[potok]);

				indeks = 0;
				for (int j = 0; j < 999; j++) {
					x[potok][j + 1] = pow(x[potok][j], 2) - pow(y[potok][j], 2) + a;
					y[potok][j + 1] = 2 * (x[potok][j] * y[potok][j]) + b;
					if (x[potok][j + 1] < -2 || x[potok][j + 1]>2 || y[potok][j + 1] > 2 || y[potok][j + 1] < -2) {
						indeks = j + 1;
						break;
					}
				}
				if (x[potok][indeks] < -2 || x[potok][indeks]>2 || y[potok][indeks] < -2 || y[potok][indeks]>2 && indeks >= 50) {
					for (int q = 1; q <= indeks; q++) {
						roundx = round(x[potok][q] / c) + (dlina / 2);
						roundy = round(y[potok][q] / c) + (dlina / 2);
						if (roundx <= dlina - 1 && roundx >= 0 && roundy <= shirina - 1 && roundy >= 0) {
							mas[potok][roundx][roundy]++;
						}
					}
				}
			}
		}
	}
	for (int potok = 0; potok <= 3; potok++) {
		for (int i = 0; i < dlina; i++) {
			for (int j = 0; j < shirina; j++) {
				summamas[i][j] = (mas[0][i][j] + mas[1][i][j] + mas[2][i][j] + mas[3][i][j]);
			}
		}
	}

	max = -3;
	for (int i = 0; i < dlina; i++) {
		for (int j = 0; j < shirina; j++) {
			if (max <= summamas[i][j]) {
				max = summamas[i][j];
			}
		}
	}

	float s, cvet;
	s = (255.0 / max);
	CImg <float> image(dlina, shirina, 1, 3);
	image.fill(0);
	float color[3];
	for (int i = 0; i < dlina; i++) {
		for (int j = 0; j < shirina; j++) {
			if (summamas[i][j] == max) {
				color[0] = { 255 };
				color[1] = { 255 };
				color[2] = { 255 };
			}
			else {
				cvet = ((int)summamas[i][j] * s);
				color[0] = { cvet };
				color[1] = { cvet };
				color[2] = { cvet };
			}
			image.draw_point(i, j, color);
		}
	}
	image.save("buddhabrott.bmp");
	cout << "runtime = " << clock() / 1000.0 << endl;
	return 0;
}
