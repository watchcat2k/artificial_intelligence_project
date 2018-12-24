#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const double THE_TRUE_RESULT = 629;
const int MAX_CITY_NUM = 300;
const int GROUP_SIZE = 20;
const double MAX_FITING = 5000;
const int CROUSS_POINT_COUNT = 10;
const int TURN_P = 10;
const int CROSSOVER_P = 99;
const int INHERIT_TIME = 2000000;
//const int INHERIT_TIME = 20;
const double TEMPERATURE_DECREASE = 0.9999;
//double temperature = 0.6;

using namespace std;


int inputGraph(int graph[MAX_CITY_NUM][3]) {
	int len = 0;
	char c;
	int index = 0;
	cout << "请输入城市，以两个Ctrl + z结束输入" << endl;
	while ((c = getchar()) != EOF) {
		if (c != ' ' && c != EOF) {
			ungetc(c, stdin);
			cin >> graph[len][index];
			index++;
			if (index == 3) {
				len++;
				index = 0;
			}
		}
	}

	return len;
}

void copySolution(int solution_empty[MAX_CITY_NUM + 1], int solution_original[MAX_CITY_NUM + 1], int geneLen) {
	for (int i = 0; i < geneLen; i++) {
		solution_empty[i] = solution_original[i];
	}
}


bool isGeneExist(int solution[MAX_CITY_NUM + 1], int geneLen, int gene) {
	for (int i = 0; i < geneLen; i++) {
		if (solution[i] == gene) {
			return true;
		}
	}
	return false;
}

void initialSolutionGroup(int graph[MAX_CITY_NUM][3], int solutionGroup[GROUP_SIZE][MAX_CITY_NUM + 1], int geneLen) {
	int greedySolution[MAX_CITY_NUM + 1] = {0};
	greedySolution[0] = 1;
	greedySolution[geneLen - 1] = 1;
	for (int i = 1; i < geneLen - 1; i++) {
		int mindis = INT_MAX;
		int index = -1;
		for (int j = 2; j <= geneLen - 1; j++) {
			if (isGeneExist(greedySolution, geneLen, j)) continue;
			else {
				int temp = (graph[greedySolution[i] - 1][1] - graph[j - 1][1]) * (graph[greedySolution[i] - 1][1] - graph[j - 1][1]) + (graph[greedySolution[i] - 1][2] - graph[j - 1][2]) * (graph[greedySolution[i] - 1][2] - graph[j - 1][2]);
				if (temp <= mindis) {
					mindis = temp;
					index = j;
				}
			}
		}
		greedySolution[i] = index;
	}

	for (int i = 0; i < GROUP_SIZE; i++) {
		copySolution(solutionGroup[i], greedySolution, geneLen);
	}
	
	/*
	for (int j = 0; j < GROUP_SIZE; j++) {
		int i = 0;
		for (i = 0; i < geneLen - 1; i++) {
			solutionGroup[j][i] = i + 1;
		}
		solutionGroup[j][i] = 1;
	}

	for (int j = 0; j < GROUP_SIZE; j++) {
		for (int i = 1; i < geneLen - 1; i++) {
			int randnum1 = (rand() % (geneLen - 1 - 1)) + 1;
			int temp = solutionGroup[j][i];
			solutionGroup[j][i] = solutionGroup[j][randnum1];
			solutionGroup[j][randnum1] = temp;
		}
	}*/
}

double getAllDistance(int graph[MAX_CITY_NUM][3], int solution[MAX_CITY_NUM + 1], int geneLen) {
	//注意城市序号从1开始
	double result = 0;
	for (int i = 1; i < geneLen; i++) {
		double city1_x = graph[solution[i - 1] - 1][1];
		double city1_y = graph[solution[i - 1] - 1][2];
		double city2_x = graph[solution[i] - 1][1];
		double city2_y = graph[solution[i] - 1][2];
		double temp = (city1_x - city2_x) * (city1_x - city2_x) + (city1_y - city2_y) * (city1_y - city2_y);
		result += sqrt(temp);
	}
	return result;
}

//适应值是5000减去路径的总距离，因为适应值越大越好
double getFiting(int graph[MAX_CITY_NUM][3], int solution[MAX_CITY_NUM + 1], int geneLen) {
	double allDistance = getAllDistance(graph, solution, geneLen);
	//double result = 5000.0 - allDistance;
	double result = 1 / allDistance;
	return result;
}

int getSelectSolutionIndex(int border[GROUP_SIZE + 1], int randNum) {
	for (int i = 0; i < GROUP_SIZE; i++) {
		if (randNum >= border[i] && randNum <= border[i + 1]) {
			return i;
		}
	}
	return 0;
}

int getGeneDuplicatePosition(int solution[MAX_CITY_NUM + 1], int geneLen, int start, int end, int gene) {
	int position = -1;
	for (int i = 0; i < geneLen; i++) {
		if (solution[i] == gene) {
			if (!(i >= start && i <= end)) {
				position = i;
			}
		}
	}
	return position;
}


void crossOver(int solution1[MAX_CITY_NUM + 1], int solution2[MAX_CITY_NUM + 1], int geneLen) {
	int crossPoint[CROUSS_POINT_COUNT];
	for (int i = 0; i < CROUSS_POINT_COUNT; i++) {
		crossPoint[i] = (rand() % (geneLen - 2 - 1 + 1)) + 1;
	}
	sort(crossPoint, crossPoint + CROUSS_POINT_COUNT);
	
	int crossRandnum = (rand() % (100 - 0 + 1)) + 0;
	if (crossRandnum < 50) {
		crossPoint[0] = 1;
	}
	else {
		crossPoint[CROUSS_POINT_COUNT - 1] = geneLen - 2;
	}

	int tempSolution1[MAX_CITY_NUM + 1];
	int tempSolution2[MAX_CITY_NUM + 1];
	for (int i = 0; i < CROUSS_POINT_COUNT / 2; i++) {
		int tempSolutionLen = 0;
		for (int j = crossPoint[i * 2]; j <= crossPoint[i * 2 + 1]; j++) {
			tempSolution1[tempSolutionLen] = solution1[j];
			tempSolution2[tempSolutionLen] = solution2[j];
			solution1[j] = tempSolution2[tempSolutionLen];
			solution2[j] = tempSolution1[tempSolutionLen];
			tempSolutionLen++;
		}


		for (int j = 0; j < tempSolutionLen; j++) {
			int position = getGeneDuplicatePosition(solution1, geneLen, crossPoint[i * 2], crossPoint[i * 2 + 1], tempSolution2[j]);
			if (position != -1) {
				for (int k = 0; k < tempSolutionLen; k++) {
					if (!isGeneExist(solution1, geneLen, tempSolution1[k])) {
						solution1[position] = tempSolution1[k];
					}
				}
			}
		}

		for (int j = 0; j < tempSolutionLen; j++) {
			int position = getGeneDuplicatePosition(solution2, geneLen, crossPoint[i * 2], crossPoint[i * 2 + 1], tempSolution1[j]);
			if (position != -1) {
				for (int k = 0; k < tempSolutionLen; k++) {
					if (!isGeneExist(solution2, geneLen, tempSolution2[k])) {
						solution2[position] = tempSolution2[k];
					}
			
				}
			}
		}

	}



}

void turnGene(int solution[MAX_CITY_NUM + 1], int geneLen) {
	int randnum1 = (rand() % (geneLen - 2 - 1 + 1)) + 1;
	int randnum2 = (rand() % (geneLen - 2 - 1 + 1)) + 1;

	int temp = solution[randnum1];
	solution[randnum1] = solution[randnum2];
	solution[randnum2] = temp;
}

int getBestSolution(int graph[MAX_CITY_NUM][3], int solutionGroup[GROUP_SIZE][MAX_CITY_NUM + 1], int geneLen) {
	int index = 0;
	double minAllDistance = getAllDistance(graph, solutionGroup[0], geneLen);
	for (int i = 1; i < GROUP_SIZE; i++) {
		double tempAllDistance = getAllDistance(graph, solutionGroup[i], geneLen);
		if (tempAllDistance < minAllDistance) {
			minAllDistance = tempAllDistance;
			index = i;
		}
	}
	return index;
}

int getWorstSolution(int graph[MAX_CITY_NUM][3], int solutionGroup[GROUP_SIZE][MAX_CITY_NUM + 1], int geneLen) {
	int index = 0;
	double maxAllDistance = getAllDistance(graph, solutionGroup[0], geneLen);
	for (int i = 1; i < GROUP_SIZE; i++) {
		double tempAllDistance = getAllDistance(graph, solutionGroup[i], geneLen);
		if (tempAllDistance > maxAllDistance) {
			maxAllDistance = tempAllDistance;
			index = i;
		}
	}
	return index;
}

void inheritOperation(int graph[MAX_CITY_NUM][3], int solutionGroup[GROUP_SIZE][MAX_CITY_NUM + 1], int geneLen) {
	int tempSolutionGroup[GROUP_SIZE][MAX_CITY_NUM + 1];
	int tempSolutionGroupLen = 0;
	double allGroupFiting = 0;
	double solutionFiting[GROUP_SIZE];
	int border[GROUP_SIZE + 1] = { 0 };

	for (int i = 0; i < GROUP_SIZE; i++) {
		solutionFiting[i] = getFiting(graph, solutionGroup[i], geneLen);
		allGroupFiting += solutionFiting[i];
		//border[i + 1] = (int)allGroupFiting;
	}
	for (int i = 0; i < GROUP_SIZE; i++) {
		border[i + 1] = (solutionFiting[i] / allGroupFiting) * 100;
	}
	
	for (int i = 0; i < GROUP_SIZE / 2; i++) {
		int randnum1 = (rand() % (border[GROUP_SIZE] - 0 + 1)) + 0;
		int randnum2 = (rand() % (border[GROUP_SIZE] - 0 + 1)) + 0;
		int index1 = getSelectSolutionIndex(border, randnum1);
		int index2 = getSelectSolutionIndex(border, randnum2);
	
		int solution_index1[MAX_CITY_NUM + 1];
		int solution_index2[MAX_CITY_NUM + 1];
		copySolution(solution_index1, solutionGroup[index1], geneLen);
		copySolution(solution_index2, solutionGroup[index2], geneLen);

		int crossoverNum1 = (rand() % (100 - 1 + 1)) + 1;
		if (crossoverNum1 <= CROSSOVER_P) {
			crossOver(solution_index1, solution_index2, geneLen);
		}

		int turnNum1 = (rand() % (100 - 1 + 1)) + 1;
		int turnNum2 = (rand() % (100 - 1 + 1)) + 1;
		if (turnNum1 <= TURN_P) {
			turnGene(solution_index1, geneLen);
		}
		if (turnNum2 <= TURN_P) {
			turnGene(solution_index2, geneLen);
		}

		

		copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index1, geneLen);
		tempSolutionGroupLen++;
		copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index2, geneLen);
		tempSolutionGroupLen++;

		/*double allDistance1 = getAllDistance(graph, solutionGroup[index1], geneLen);
		double allDistance2 = getAllDistance(graph, solutionGroup[index2], geneLen);
		double allDistance3 = getAllDistance(graph, solution_index1, geneLen);
		double allDistance4 = getAllDistance(graph, solution_index2, geneLen);
		double Distance[4] = { allDistance1 , allDistance2, allDistance3, allDistance4 };
		sort(Distance, Distance + 4);

		double pickrandnum = (rand() % (100 - 0 + 1)) + 0;
		double pickp = 100 * exp((-1) / temperature);
		if (pickrandnum < pickp) {
			if (Distance[2] == allDistance1) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index1], geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[2] == allDistance2) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index2], geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[2] == allDistance3) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index1, geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[2] == allDistance4) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index2, geneLen);
				tempSolutionGroupLen++;
			}

			if (Distance[3] == allDistance1) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index1], geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[3] == allDistance2) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index2], geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[3] == allDistance3) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index1, geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[3] == allDistance4) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index2, geneLen);
				tempSolutionGroupLen++;
			}
		}
		else {
			if (Distance[0] == allDistance1) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index1], geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[0] == allDistance2) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index2], geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[0] == allDistance3) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index1, geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[0] == allDistance4) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index2, geneLen);
				tempSolutionGroupLen++;
			}

			if (Distance[1] == allDistance1) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index1], geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[1] == allDistance2) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index2], geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[1] == allDistance3) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index1, geneLen);
				tempSolutionGroupLen++;
			}
			else if (Distance[1] == allDistance4) {
				copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index2, geneLen);
				tempSolutionGroupLen++;
			}
		}*/

		

		
		/*double fiting1 = getFiting(graph, solutionGroup[index1], geneLen);
		double fiting2 = getFiting(graph, solutionGroup[index2], geneLen);
		double fiting3 = getFiting(graph, solution_index1, geneLen);
		double fiting4 = getFiting(graph, solution_index2, geneLen);
		double allPickFiting = fiting1 + fiting2 + fiting3 + fiting4;
		int allFitingBorder1 = fiting1 / allPickFiting * 100;
		int allFitingBorder2 = fiting2 / allPickFiting * 100;
		int allFitingBorder3 = fiting3 / allPickFiting * 100;
		int allFitingBorder4 = fiting4 / allPickFiting * 100;

		int allFiting[5] = { 0, allFitingBorder1, allFitingBorder1 + allFitingBorder2, allFitingBorder1 + allFitingBorder2 + allFitingBorder3, allFitingBorder1 + allFitingBorder2 + allFitingBorder3 + allFitingBorder4 };
		int tempRandnum1 = (rand() % (allFiting[4] - 0 + 1)) + 0;
		int tempRandnum2 = (rand() % (allFiting[4] - 0 + 1)) + 0;

		if (tempRandnum1 >= allFiting[0] && tempRandnum1 <= allFiting[1]) {
			copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index1], geneLen);
			tempSolutionGroupLen++;
		}
		else if (tempRandnum1 >= allFiting[1] && tempRandnum1 <= allFiting[2]) {
			copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index2], geneLen);
			tempSolutionGroupLen++;
		}
		else if (tempRandnum1 >= allFiting[2] && tempRandnum1 <= allFiting[3]) {
			copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index1, geneLen);
			tempSolutionGroupLen++;
		}
		else if (tempRandnum1 >= allFiting[3] && tempRandnum1 <= allFiting[4]) {
			copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index2, geneLen);
			tempSolutionGroupLen++;
		}

		if (tempRandnum2 >= allFiting[0] && tempRandnum2 <= allFiting[1]) {
			copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index1], geneLen);
			tempSolutionGroupLen++;
		}
		else if (tempRandnum2 >= allFiting[1] && tempRandnum2 <= allFiting[2]) {
			copySolution(tempSolutionGroup[tempSolutionGroupLen], solutionGroup[index2], geneLen);
			tempSolutionGroupLen++;
		}
		else if (tempRandnum2 >= allFiting[2] && tempRandnum2 <= allFiting[3]) {
			copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index1, geneLen);
			tempSolutionGroupLen++;
		}
		else if (tempRandnum2 >= allFiting[3] && tempRandnum2 <= allFiting[4]) {
			copySolution(tempSolutionGroup[tempSolutionGroupLen], solution_index2, geneLen);
			tempSolutionGroupLen++;
		}*/


	}

	

	int fatherBestIndex = getBestSolution(graph, solutionGroup, geneLen);
	int sonBestIndex = getBestSolution(graph, tempSolutionGroup, geneLen);
	double fatherBestFiting = getFiting(graph, solutionGroup[fatherBestIndex], geneLen);
	double sonBestFiting = getFiting(graph, tempSolutionGroup[sonBestIndex], geneLen);
	if (fatherBestFiting > sonBestFiting) {
		int sonWorstIndex = getWorstSolution(graph, tempSolutionGroup, geneLen);

		int count = 5000;
		int solution_empty[MAX_CITY_NUM + 1];
		copySolution(solution_empty, solutionGroup[fatherBestIndex], geneLen);
		while (count--) {
			turnGene(solution_empty, geneLen);
			if (getFiting(graph, solution_empty, geneLen) > getFiting(graph, solutionGroup[fatherBestIndex], geneLen)) {
				copySolution(solutionGroup[fatherBestIndex], solution_empty, geneLen);
				break;
			}
		}
		copySolution(tempSolutionGroup[sonWorstIndex], solutionGroup[fatherBestIndex], geneLen);
	}
	
	
	for (int i = 0; i < GROUP_SIZE; i++) {
		copySolution(solutionGroup[i], tempSolutionGroup[i], geneLen);
	}

	temperature = temperature * TEMPERATURE_DECREASE;
}

int main() {
	int graph[MAX_CITY_NUM][3];
	int cityLen = 0;
	//接受城市的输入,最多CITY_NUM个，注意城市序号是从1开始的
	cityLen = inputGraph(graph);

	int solutionGroup[GROUP_SIZE][MAX_CITY_NUM + 1];
	int geneLen = cityLen + 1;

	srand((unsigned)time(NULL));

	initialSolutionGroup(graph, solutionGroup, geneLen);

	for (int i = 0; i < INHERIT_TIME; i++) {
		inheritOperation(graph, solutionGroup, geneLen);
		int index = getBestSolution(graph, solutionGroup, geneLen);
		double finalResult = getAllDistance(graph, solutionGroup[index], geneLen);
		cout << "经过的总路径长度为：" << finalResult << endl;
		cout << "与最优解的误差为" << (finalResult - THE_TRUE_RESULT) / THE_TRUE_RESULT * 100 << "%" << endl;
		cout << "经过的路径为:" << endl;
		for (int i = 0; i < geneLen; i++) {
			cout << solutionGroup[index][i] << " ";
		}
		cout << endl << endl;
		//cout << "温度为：" << temperature << endl;
	}
	

	return 0;
}