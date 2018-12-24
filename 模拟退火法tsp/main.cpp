#include <iostream>
#include <cstdlib>
#include <ctime>

const int MAX_CITY_NUM = 300;
const int THE_TRUE_SOLUTION = 629;

using namespace std;

int inputGraph(int graph[MAX_CITY_NUM][3]) {
	cout << "���������:������CTRL + z����" << endl;
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

//�����󳤶���CITY_NUM + 1
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
	//ע�������Ŵ�1��ʼ
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
	//���ܳ��е�����,���CITY_NUM����ע���������Ǵ�1��ʼ��
	cityLen = inputGraph(graph);

	int solution[MAX_CITY_NUM + 1];
	int solution_len = cityLen + 1;
	//���ʼ��Ϊ�������е�˳��Ϊ1-2-3-4-...-1�����Ҫ�ص���ʼλ��
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
		//��timesPerTemperature����ѡ��õ�
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
		cout << "��ʱ�¶�Ϊ" << temperature << endl;
		double tempResult = getAllDistance(graph, solution, solution_len);
		cout << "��ʱ�����ľ���Ϊ" << tempResult << endl;
		cout << "���Ϊ���Ž��" << (tempResult - THE_TRUE_SOLUTION) / THE_TRUE_SOLUTION * 100 << "%" << endl;
		cout << "�õ���·��Ϊ:" << endl;
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