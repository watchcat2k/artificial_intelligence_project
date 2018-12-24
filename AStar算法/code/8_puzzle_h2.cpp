#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

int id = 0;

struct node {
	vector<vector<int>> puzzle;
	int depth;
	int id;
	int preId;
	int f2;

	node(vector<vector<int>> puzzle, int depth = 0) {
		this->puzzle = puzzle;
		this->depth = depth;
		this->preId = -1;
	}
};

vector<node> open;
vector<node> close;


/**
* 计算3*3矩阵中放错位置的数字的个数h1
*
*  @param  3*3的矩阵
*  @return 数字放错位置的个数
**/
int h1(node n) {
	int count = 0;
	for (int j = 0; j < 3; j++) {
		if (n.puzzle[0][j] != j + 1)
			count++;
	}

	if (n.puzzle[1][0] != 8)
		count++;

	if (n.puzzle[1][2] != 4)
		count++;

	for (int j = 0; j < 3; j++) {
		if (n.puzzle[2][j] != 7 - j)
			count++;
	}

	return count;
}


/**
*  @param  3*3的矩阵
*  @return 所有数字当前位置通过最短路径到正确位置的步数和
**/
int h2(node n) {
	int count = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			switch (n.puzzle[i][j]) {
			case 0:
				break;
			case 1:
				count += abs(i - 0) + abs(j - 0);
				break;
			case 2:
				count += abs(i - 0) + abs(j - 1);
				break;
			case 3:
				count += abs(i - 0) + abs(j - 2);
				break;
			case 4:
				count += abs(i - 1) + abs(j - 2);
				break;
			case 5:
				count += abs(i - 2) + abs(j - 2);
				break;
			case 6:
				count += abs(i - 2) + abs(j - 1);
				break;
			case 7:
				count += abs(i - 2) + abs(j - 0);
				break;
			case 8:
				count += abs(i - 1) + abs(j - 0);
				break;
			}
		}
	}
	return count;
}

/**
* 计算3*3矩阵的f2 = h2 + 搜索树深度n.d
*
*  @param  3*3的矩阵
*  @return f2值
**/
int f2(node n) {
	return n.depth + h2(n);
}

/**
*  判断空白节点能否向指定方向移动
*
*  @param1  移动前3*3的矩阵
*  @param2  在i上移动的大小
*  @param3  在y上移动的大小
*  @return  空白节点能否移动
**/
bool canMove(node n, int iChange, int jChange) {
	int iOrigin;
	int jOrigin;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (n.puzzle[i][j] == 0) {
				iOrigin = i;
				jOrigin = j;
				break;
			}
		}
	}

	if (iChange == -1) {
		if (iOrigin == 0)
			return false;
		return true;
	}
	else if (iChange == 1) {
		if (iOrigin == 2)
			return false;
		return true;
	}
	else if (jChange == -1) {
		if (jOrigin == 0)
			return false;
		return true;
	}
	else {
		if (jOrigin == 2)
			return false;
		return true;
	}
}


/**
*  移动空白节点
*
*  @param1  移动前3*3的矩阵
*  @param2  在i上移动的大小
*  @param3  在y上移动的大小
*  @return  移动后3*3的矩阵
**/
node moveSpace(node n, int iChange, int jChange) {
	int iOrigin;
	int jOrigin;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (n.puzzle[i][j] == 0) {
				iOrigin = i;
				jOrigin = j;
				break;
			}
		}
	}

	vector<vector<int>> result = n.puzzle;
	if (iChange == -1) {        //  上
		swap(result[iOrigin][jOrigin], result[iOrigin + iChange][jOrigin + jChange]);
	}
	else if (iChange == 1) {    //  下
		swap(result[iOrigin][jOrigin], result[iOrigin + iChange][jOrigin + jChange]);
	}
	else if (jChange == -1) {   //  左
		swap(result[iOrigin][jOrigin], result[iOrigin + iChange][jOrigin + jChange]);
	}
	else {                      //  右
		swap(result[iOrigin][jOrigin], result[iOrigin + iChange][jOrigin + jChange]);
	}
	node afterMove(result, n.depth + 1);
	afterMove.preId = n.id;
	afterMove.id = id;
	id++;
	return afterMove;
}


/**
*  判断节点是否在close表中
*
*  @param1  节点
*  @return  在则返回true, 不在返回false
**/
bool isFind(node n) {
	for (int i = 0; i < close.size(); i++) {
		if (close[i].depth == n.depth) {
			bool hasFind = true;
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					if (close[i].puzzle[j][k] != n.puzzle[j][k]) {
						hasFind = false;
						break;
					}
				}
				if (!hasFind)
					break;
			}
			if (hasFind)
				return true;
		}
	}
	return false;
}


/**
*  打印每一步Open节点数、总扩展的结点数、评估函值最小的结点
**/
void dynamicPrint(node n) {
	cout << "OPEN表节点数: " << open.size() << endl;
	cout << "总扩展节点数: " << open.size() + close.size() << endl;
	cout << "评估函值最小的结点: " << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << n.puzzle[i][j] << " ";
		}
		cout << endl;
	}
	cout << "+----------------------------------+" << endl;
	cout << endl;
}


/**
*  打印结果路径
**/
void resultPrint(node n) {
	cout << endl;
	cout << "最佳路径的逆序输出如下" << endl;
	cout << "+----------------------------------+" << endl;

	while (n.preId != -1) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				cout << n.puzzle[i][j] << " ";
			}
			if (i == 0)
				cout << "\t评估涵值为" << n.depth;
			cout << endl;
		}
		cout << endl;

		for (int k = 0; k < close.size(); k++) {
			if (close[k].id == n.preId) {
				n = close[k];
				break;
			}
		}
	}
}


/**
*  启发式搜索AStar
*
*  @param  3*3的矩阵
**/
void AStar(node n) {
	open.push_back(n);

	while (!open.empty()) {

		int minF = 100;
		int minIndex = -1;
		for (int i = 0; i < open.size(); i++) {
			open[i].f2 = f2(open[i]);
			if (f2(open[i]) <= minF) {
				minF = f2(open[i]);
				minIndex = i;
			}
		}
		node temp = open[minIndex];
		open.erase(open.begin() + minIndex);
		close.push_back(temp);

		if (h1(temp) == 0) {
			cout << "找到结果, 成功退出" << endl;
			resultPrint(temp);
			return;
		}

		if (canMove(temp, -1, 0)) {
			node moveUp = moveSpace(temp, -1, 0);
			if (!isFind(moveUp))
				open.push_back(moveUp);
		}
		if (canMove(temp, 1, 0)) {
			node moveDown = moveSpace(temp, 1, 0);
			if (!isFind(moveDown))
				open.push_back(moveDown);
		}
		if (canMove(temp, 0, -1)) {
			node moveLeft = moveSpace(temp, 0, -1);
			if (!isFind(moveLeft))
				open.push_back(moveLeft);
		}
		if (canMove(temp, 0, 1)) {
			node moveRight = moveSpace(temp, 0, 1);
			if (!isFind(moveRight))
				open.push_back(moveRight);
		}

		dynamicPrint(temp);
	}
	cout << "找不到结果" << endl;
}

int main() {
	vector<vector<int>> v;
	vector<int> v1 = { 2, 8, 3 };
	vector<int> v2 = { 1, 6, 4 };
	vector<int> v3 = { 7, 0, 5 };
	v.push_back(v1);
	v.push_back(v2);
	v.push_back(v3);
	node n(v);
	AStar(n);

	return 0;
}