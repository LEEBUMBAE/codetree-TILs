#include <iostream>
#include<vector>
#include<queue>

using namespace std;

class Vertex {
public:
	int state; // 0이면 빈칸 1이면 basecamp
	int store; // 편의점
	Vertex() {
		this->state = 0;
		this->store = 0;
	}
	Vertex(int state, int store) {
		this->state = state;
		this->store = store;
	}
};
class Person {
public:
	int r, c;

	Person() {
		this->r = 0;
		this->c = 0;
	}
	Person(int r, int c) {
		this->r = r;
		this->c = c;
	}
};
class qV {
	int r, c, cnt;

};
int n, m, answer;
vector<vector<Vertex>> map;
vector<Person> personList;
int dr[4] = { -1, 0, 0, 1 }; // 상 좌 우 하
int dc[4] = { 0, -1, 1, 0 };

void move();
bool isGameover();
void block();
void goBasecamp();

void move() {
	// 격자에 있는 사람들 모두가 편의점 향해서 움직임
	for (int i = 1; i < personList.size(); i++) {
		int storeR = 0, storeC = 0;

		// 해당 편의점 찾기
		for (int r = 1; r <= n; r++) {
			for (int c = 1; c <= n; c++) {
				if (map[r][c].store == i) {
					storeR = r;
					storeC = c;
				}
				if (storeR > 0) break;
			}
			if (storeR > 0) break;
		}

		// 만약 이미 편의점에 도착한 상태면 그사람 넘어가
		if (map[storeR][storeC].state == -1) continue;

		// 최단거리로 상좌우하의 우선순위로 움직임 
		// bfs로 편의점에서 부터 각 좌표까지의 거리를 다 기록함
		vector<vector<int>> disMap(n + 1, vector<int>(n + 1, 10000));
		vector<vector<bool>> visited(n + 1, vector<bool>(n + 1, false));
		queue<pair<int, pair<int, int>>> q; // 거리, (좌표)
		
		disMap[storeR][storeC] = 0;
		q.push({ 0, {storeR, storeC} });
		visited[storeR][storeC] = true;

		while (!q.empty()) {
			int curDis = q.front().first;
			int curR = q.front().second.first;
			int curC = q.front().second.second;
			q.pop();

			for (int dir = 0; dir < 4; dir++) {
				int nDis = curDis + 1;
				int nr = curR + dr[dir];
				int nc = curC + dc[dir];
				// 격자를 벗어나거나 방문한 곳이거나 state가 -1인곳은 넘어가
				if (nr < 1 || nc < 1 || nr > n || nc > n || visited[nr][nc] || map[nr][nc].state == -1) continue;

				disMap[nr][nc] = nDis;
				q.push({ nDis,{nr, nc} });
				visited[nr][nc] = true;
			}
		}
		// 사람위치에서 상좌우하 순서로 확인해보고 min 갱신하면서 방향 기록
		int min = 10000;
		int nDir = -1;
		for (int dir = 0; dir < 4; dir++) {
			int nr = personList[i].r + dr[dir];
			int nc = personList[i].c + dc[dir];
			// 격자를 벗어난 곳이거나 state -1인곳은 넘어가
			if (nr < 1 || nc < 1 || nr > n || nc > n || map[nr][nc].state == -1) continue;
			// 더 작으면 min 갱신하면서 방향 기록
			if (min > disMap[nr][nc]) {
				min = disMap[nr][nc];
				nDir = dir;
			}
		}

		// 방향 결정됐으니 이동한다
		personList[i].r = personList[i].r + dr[nDir];
		personList[i].c = personList[i].c + dc[nDir];
	}
}
bool isGameover() {
	// 사람들이 모두 도착 한지 확인
	// 도착 했으면 true, 아니면 false를 return
	for (int i = 1; i < personList.size(); i++) {
		int pr = personList[i].r;
		int pc = personList[i].c;
		if (map[pr][pc].store != i) return false;
	}
	return true;
}
void block() {
	// 편의점에 도착한 사람있으면 그 자리 지나갈수 없게만들어
	// 
	// 편의점 도착한 사람 찾기
	for (int i = 1; i < personList.size(); i++) {
		int pr = personList[i].r;
		int pc = personList[i].c;
		if (map[pr][pc].store == i) {
			// 그 자리 못지나가는 state => -1 로 만들기
			map[pr][pc].state = -1;
		}
	}
}
void goBasecamp() {
	if (answer <= m) { // 4명이면 m
		// 편의점과 가장 가까이 있는 베이스 캠프를 찾음

		// 해당 편의점 찾기
		int storeR = 0, storeC = 0;
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				if (map[i][j].store == answer) {
					storeR = i;
					storeC = j;
				}
				if (storeR > 0) break;
			}
			if (storeR > 0) break;
		}

		// bfs로 편의점부터 베이스 캠프까지 거리 탐색
		// bfs로 편의점에서 부터 각 좌표까지의 거리를 다 기록함
		vector<vector<int>> disMap(n + 1, vector<int>(n + 1, 10000));
		vector<vector<bool>> visited(n + 1, vector<bool>(n + 1, false));
		queue<pair<int, pair<int, int>>> q; // 거리, (좌표)

		disMap[storeR][storeC] = 0;
		q.push({ 0, {storeR, storeC} });
		visited[storeR][storeC] = true;

		while (!q.empty()) {
			int curDis = q.front().first;
			int curR = q.front().second.first;
			int curC = q.front().second.second;
			q.pop();

			for (int dir = 0; dir < 4; dir++) {
				int nDis = curDis + 1;
				int nr = curR + dr[dir];
				int nc = curC + dc[dir];
				// 격자를 벗어나거나 방문한 곳이거나 state가 -1인곳은 넘어가
				if (nr < 1 || nc < 1 || nr > n || nc > n || visited[nr][nc] || map[nr][nc].state == -1) continue;

				disMap[nr][nc] = nDis;
				q.push({ nDis,{nr, nc} });
				visited[nr][nc] = true;
			}
		}

		// 베이스 캠프만 조사해서 제일 작은 거리가 몇인지 파악
		int min = 10000;
		int nr = 0, nc = 0;
		for (int i = 1; i <= n; i++) {
			for (int j = 1; j <= n; j++) {
				if (map[i][j].state == 1) { // 상태가 베이스 캠프면
					if (min > disMap[i][j]) { // min과 비교해서 더 작으면 갱신해라
						min = disMap[i][j];
						nr = i;
						nc = j;
					}
				}
			}
		}

		// 해당 베이스 캠프로 들어간다
		map[nr][nc].state = -1;
		personList.push_back(Person(nr, nc));
	}
}

int main() {
	cin >> n >> m;
	map = vector<vector<Vertex>>(n + 1, (vector<Vertex>(n + 1)));
	personList = vector<Person>(1); // 0번 인덱스 버리려고 하나는 미리 넣어둠
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> map[i][j].state;
		}
	}

	for (int i = 1; i <= m; i++) {
		int r, c;
		cin >> r >> c;
		map[r][c].store = i;
	}

	answer = 0; // 시간
	while (1) {
		answer++;
		move();
		block();
		goBasecamp();
		if (isGameover()) break;
	}
	cout << answer;
	return 0;
}