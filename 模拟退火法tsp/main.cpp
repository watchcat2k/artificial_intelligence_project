#include <iostream>
#include <cstdlib>
#include <ctime>

const int MAX_CITY_NUM = 300;
const int THE_TRUE_SOLUTION = 629;

using namespace std;

int inputGraph(int graph[MAX_CITY_NUM][3]) {
	cout << "请输入城市:以两次CTRL + z结束" << endl;
	int len = 0;
	char c;
	int index = 0;

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

//解的最大长度是CITY_NUM + 1
void getNewSolution(int solution[MAX_CITY_NUM + 1], int newSolution[MAX_CITY_NUM + 1], int solution_len) {
	for (int i = 0; i < solution_len; i++) {
		newSolution[i] = solution[i];
	}
	int randnum1 = (rand() % (solution_len - 1 - 1)) + 1;
	int randnum2 = (rand() % (solution_len - 1 - 1)) + 1;
	int temp = newSolution[randnum1];
	newSolution[randnum1] = newSolution[randnum2];
	newSolution[randnum2] = temp;
			
}

void solutionGetNewValue(int solution[MAX_CITY_NUM + 1], int newSolution[MAX_CITY_NUM + 1], int solution_len) {
	for (int i = 0; i < solution_len; i++) {
		solution[i] = newSolution[i];
	}
}

double getAllDistance(int graph[MAX_CITY_NUM][3], int solution[MAX_CITY_NUM + 1], int solution_len) {
	//注意城市序号从1开始
	double result = 0;
	for (int i = 1; i < solution_len; i++) {
		int city1_x = graph[solution[i - 1] - 1][1];
		int city1_y = graph[solution[i - 1] - 1][2];
		int city2_x = graph[solution[i] - 1][1];
		int city2_y = graph[solution[i] - 1][2];
		double temp = (city1_x - city2_x) * (city1_x - city2_x) + (city1_y - city2_y) * (city1_y - city2_y);
		result += sqrt(temp);
	}
	return result;
}

int main() {
	int graph[MAX_CITY_NUM][3];
	int cityLen = 0;    
	//接受城市的输入,最多CITY_NUM个，注意城市序号是从1开始的
	cityLen = inputGraph(graph);

	int solution[MAX_CITY_NUM + 1];
	int solution_len = cityLen + 1;
	//令初始解为遍历城市的顺序为1-2-3-4-...-1，最后要回到初始位置
	for (int i = 0; i < cityLen; i++) {
		solution[i] = graph[i][0];
	}
	solution[cityLen] = graph[0][0];

	double temperature = 100.0;
	double temperature_decrease_num = 0.99999;
	int timesPerTemperature = 60;
	int count = 1200000;
	srand((unsigned)time(NULL));
	while (count--) {
		//在timesPerTemperature个中选最好的
		int newSolution[MAX_CITY_NUM + 1];
		int tempSolution[MAX_CITY_NUM + 1];
		getNewSolution(solution, newSolution, solution_len);
		int allDistance_new = getAllDistance(graph, newSolution, solution_len);
		for (int i = 0; i < timesPerTemperature; i++) {
			getNewSolution(solution, tempSolution, solution_len);
			int allDistance_temp = getAllDistance(graph, tempSolution, solution_len);
			if (allDistance_temp < allDistance_new) {
				solutionGetNewValue(newSolution, tempSolution, solution_len);
				allDistance_new = getAllDistance(graph, newSolution, solution_len);
			}
		}
		int allDistance = getAllDistance(graph, solution, solution_len);
		if (allDistance_new < allDistance) {
			solutionGetNewValue(solution, newSolution, solution_len);
		}
		else {
			double randnum = (rand() % (100 - 0 + 1)) + 0;
			double p = 100 * exp((allDistance - allDistance_new) / temperature);
			if (randnum < p) {
				solutionGetNewValue(solution, newSolution, solution_len);
			}
		}
		cout << "此时温度为" << temperature << endl;
		double tempResult = getAllDistance(graph, solution, solution_len);
		cout << "此时经过的距离为" << tempResult << endl;
		cout << "误差为最优解的" << (tempResult - THE_TRUE_SOLUTION) / THE_TRUE_SOLUTION * 100 << "%" << endl;
		cout << "得到的路径为:" << endl;
		for (int i = 0; i < solution_len; i++) {
			cout << solution[i] << " ";
		}
		cout << endl << endl;

		temperature = temperature * temperature_decrease_num;
	
	}
    int newSolution[MAX_CITY_NUM + 1];
	getNewSolution(solution, newSolution, solution_len);

	for (int i = 0; i < solution_len; i++) {
		cout << solution[i] << " ";
	}
	cout << endl;
	cout << getAllDistance(graph, solution, solution_len) << endl;
	

	return 0;
}