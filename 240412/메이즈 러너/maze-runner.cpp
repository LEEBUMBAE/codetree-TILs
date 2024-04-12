#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>

using namespace std;

// 참가자: -1 ~ -10, 빈칸: 0, 벽: 1~ 10 출구: 10 
int dR[4] = { -1, 1, 0, 0 }; // 상하좌우
int dC[4] = { 0, 0, -1, 1 };
vector<vector<int>> map;
int N, M, K;
int minSize, sumMoveDis;

void move();
void rotate();
bool checkGG();
pair<int, int> checkExit();
void copyMap(vector<vector<int>>& map1, vector<vector<int>>& map2);
void getMinSize();
int checkDis(int i, int j, int r, int c);
int getSize(int r, int c, int exitR, int exitC);
bool checkRec(int r, int c);

int main() {
	cin >> N >> M >> K;
	map = vector<vector<int>>(N + 1, vector<int>(N + 1, 0));

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = 1; i <= M; i++) {
		int r, c;
		cin >> r >> c;
		map[r][c]--;
	}
	int r, c;
	cin >> r >> c;
	map[r][c] = 10;


	sumMoveDis = 0;
	for (int k = 1; k <= K; k++) {
		move();
		if (checkGG()) {
			break;
		}
		minSize = 2100000000;
		rotate();
	}
	pair<int, int> exitPoint = checkExit();
	cout << sumMoveDis << "\n";
	cout << exitPoint.first << " " << exitPoint.second << "\n";
}

void move() {
	// 옮길땐 tempMap을 활용하여 옮긴다
	vector<vector<int>> tempMap(N + 1, vector<int>(N + 1, 0));
	copyMap(tempMap, map);

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			// 참가자가 있는칸이면 로직 시작
			if (map[i][j] > 0) continue;
			
			// 최단 거리를 잰다
			pair<int, int> exit = checkExit();
			int curDis = checkDis(i, j, exit.first, exit.second);

			// 상하좌우로 움직여보면서 최단거리를 재보고 그 최단거리가 현재보다 -1인곳이면 그곳으로 옮긴다.
			for (int d = 0; d < 4; d++) {
				int nextR = i + dR[d];
				int nextC = j + dC[d];

				if (nextR < 1 || nextC < 1 || nextR > N || nextC > N) continue;
				if (map[nextR][nextC] >= 1 && map[nextR][nextC] <= 9) continue;
				if (checkDis(nextR, nextC, exit.first, exit.second) != (curDis - 1)) continue;

				sumMoveDis -= map[i][j];
				tempMap[i][j] -= map[i][j]; //**주의** N명 이동하면 그맵에 +N해야됨(-(-N) = +N). -2가 다른쪽으로 가는거니까!
				if (map[nextR][nextC] == 10) { // 출구일때
					break;
				}
				else { // 출구가 아닌 빈칸이나 참가자일때
					tempMap[nextR][nextC] += map[i][j];
					break;
				}
			}
		}
	}

	// 옮길땐 tempMap을 활용하여 옮긴다
	copyMap(map, tempMap);
}

void copyMap(vector<vector<int>>& map1, vector<vector<int>>& map2) {
	for (int r = 1; r <= N; r++) {
		for (int c = 1; c <= N; c++) {
			map1[r][c] = map2[r][c];
		}
	}
}

int checkDis(int i, int j, int r, int c) {
	return abs(i - r) + abs(j - c);
}

void rotate() {
	// 사람과 출구 r, c 비교를 통해 더 '큰' 차이를 기록해놓고 그걸 size로 정함, 그후 제일 작은 minSize를 찾아내
	getMinSize();

	// 맵 순회하면서 좌상단 좌표기준 minSize 까지를 체킹하면서 출구와 사람이 있다면 그 사각형으로 선정한다.
	int recR = 0, recC = 0;
	bool flag = false;
	for (int r = 1; r <= N - minSize; r++) { // **주의** 사각형 체킹할때 N -minSize 안해줘서 범위밖까지 체킹하다 오류
		for (int c = 1; c <= N - minSize; c++) {
			if (checkRec(r, c)) {
				recR = r;
				recC = c;
				flag = true;
				break;
			}
		}
		if (flag) break;
	}

	// 선정된 사각형을 돌린다.
	vector<vector<int>> tempMap1(minSize + 2, vector<int>(minSize + 2));
	vector<vector<int>> tempMap2(minSize + 2, vector<int>(minSize + 2));

	for (int i = recR; i <= recR + minSize; i++) {
		for (int j = recC; j <= recC + minSize; j++) {
			tempMap1[i - recR + 1][j - recC + 1] = map[i][j];
		}
	}

	for (int i = 1; i <= minSize + 1; i++) {
		for (int j = 1; j <= minSize + 1; j++) {
			tempMap2[j][minSize + 1 - i + 1] = tempMap1[i][j];
		}
	}

	for (int i = recR; i <= recR + minSize; i++) {
		for (int j = recC; j <= recC + minSize; j++) {
			map[i][j] = tempMap2[i - recR + 1][j - recC + 1];
		}
	}

	// 회전된 벽은 내구도 깎인다.
	for (int i = recR; i <= recR + minSize; i++) {
		for (int j = recC; j <= recC + minSize; j++) {
			if (map[i][j] >= 1 && map[i][j] <= 9) map[i][j]--;
		}
	}
}

bool checkRec(int r, int c) {
	bool personFlag = false;
	bool exitFlag = false;

	for (int i = r; i <= r + minSize; i++) {
		for (int j = c; j <= c + minSize; j++) {
			if (map[i][j] < 0) { 
				personFlag = true;
			}
			if (map[i][j] == 10) {
				exitFlag = true; 
			}
		}
	}

	if (personFlag && exitFlag) return true; 
	else return false;	// **주의** return false가 아니라 그냥 false라 썼었음
}

void getMinSize() {
	pair<int, int> exit = checkExit();

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j] >= 0) continue; // 사람이 아니면 넘어가
			minSize = min(minSize, getSize(i, j, exit.first, exit.second));
		}
	}
}

int getSize(int r, int c, int exitR, int exitC) {
	int absR = abs(r - exitR);
	int absC = abs(c - exitC);
	return max(absR, absC);
}

bool checkGG() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j] < 0) return false; // 사람이면 false 
		}
	}
	return true;
}

pair<int, int> checkExit() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			// exit칸에서 로직발동
			if (map[i][j] != 10) continue;

			return { i, j }; // **의문** pair를 이렇게 넘겨도 되는건가
		}
	}
}