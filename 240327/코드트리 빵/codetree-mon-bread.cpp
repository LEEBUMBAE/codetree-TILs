#include <iostream>
#include<vector>
#include<queue>
using namespace std;

class Person{
public:
	int r, c;
	bool isArrived;
	Person() {
		this->r = 0;
		this->c = 0;
		this->isArrived = false;
	}
	Person(int r, int c, bool isArrived) {
		this->r = r;
		this->c = c;
		this->isArrived = isArrived;
	}
};

int n, m, t = 0, cntArrivedPerson = 0;
vector<vector<int>> map;
vector<pair<int, int>> storePosList;
vector<Person> personList;


void move();
void arrive();
void intoBasecamp();
int bfs(int r1, int c1, int r2, int c2);

int main() {
	cin >> n >> m;
	map = vector<vector<int>>(n + 1, vector<int>(n + 1));
	storePosList = vector<pair<int, int>>(m + 1);
	personList = vector<Person>(m + 1);
	
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = 1; i <= m; i++) {
		int r, c;
		cin >> r >> c;
		storePosList[i] = { r, c };
	}

	t = 0;
	cntArrivedPerson = 0;
	while (1) {
		t++;
		move();
		arrive();
		intoBasecamp();
		if (cntArrivedPerson == m) {
			cout << t;
			return 0;
		}
	}
}

// 1번행동
// 격자에 있는 사람들 모두 가고싶은 편의점을 향해 최단거리로 1칸 이동
// 최단거리는 이동후 도달까지 거리가(지나갈수있는길만 지나갔을때) 최소가 되는 거리를 뜻함
// 상 좌 우 하 순서로 움직임
void move() {
	// 격자안에 있는 사람만 순회 // 격자에 t번째사람은 3번행동에서 들어옴
	for (int num = 1; num < m + 1; num++) {
		// 해당 번호사람이 움직일수있는지 확인
		if (num >= t) return;
		if (personList[num].isArrived) continue; 
		int curDis = bfs(personList[num].r, personList[num].c, storePosList[num].first, storePosList[num].second);
		// 해당번호 사람 기준 상 좌 우 하 순서로 좌표순회
		int dr[4] = { -1, 0, 0, 1 };
		int dc[4] = { 0, -1, 1, 0 };
		for (int d = 0; d < 4; d++) {
			int nextR = personList[num].r + dr[d];
			int nextC = personList[num].c + dc[d];
			if (nextR < 1 || nextC < 1 || nextR > n || nextC > n || map[nextR][nextC] < 0) continue;
			int nextDis = bfs(nextR, nextC, storePosList[num].first, storePosList[num].second);
			// nextDis - curDis == -1 이면 그쪽으로 움직여
			if (nextDis - curDis == -1) {
				personList[num].r = nextR;
				personList[num].c = nextC;
			}
		}
	}
}
// 2번행동 
// 편의점에 도착시 멈추게 되고, 이때부터 다른사람들은 그칸을 지나갈수없음 
// 주의! 격자에 있는 사람들이 모두 이동한뒤에 해당칸을 지나갈수없어지는것
void arrive(){
	for (int i = 1; i <= m; i++) {
		if (personList[i].isArrived) continue; // 이미 도착처리한사람이면 넘어가
		int pR = personList[i].r;
		int pC = personList[i].c;
		int sR = storePosList[i].first;
		int sC = storePosList[i].second;
		
		if (pR == sR && pC == sC) {
			map[pR][pC] = -1;
			personList[i].isArrived = true;
			cntArrivedPerson++;
		}
	}
}
// 3번행동
// 현재 t분이면 t번 사람이 베이스캠프에 들어갈거임 (t<=m)
// 자신이 가고 싶은 편의점과 가장 가까이에 있는 베이스캠프에 들어가는것
// 행이 작은 베이스캠프 , 열이 작은 베이스캠프 우선순위
// 사람들이 모두 이동한뒤 베이캠프는 폐쇄
void intoBasecamp(){
	if (t > m) return; // m명의 사람이 있기때문에 그 보다 큰 번호인 사람은 없다
	int minDis = 2100000000;
	int minR = 0, minC = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (map[i][j] != 1) continue;

			int dis = bfs(i, j, storePosList[t].first, storePosList[t].second);

			if (dis < minDis) {
				minDis = dis;
				minR = i;
				minC = j;
			}
		}
	}
	personList[t].r = minR;
	personList[t].c = minC;
	map[minR][minC] = -1; //폐쇄
}

// 최단거리를 구해 r1,c1 -> r2, c2
int bfs(int r1, int c1, int r2, int c2) {
	int dr[4] = { -1, 0, 0, 1 };
	int dc[4] = { 0, -1, 1, 0 };
	queue<pair<pair<int, int>, int>> q;
	vector<vector<bool>> visited(n + 1, vector<bool>(n + 1));

	q.push({ { r1, c1 }, 0 });
	visited[r1][c1] = true;

	while (!q.empty()) {
		int curR = q.front().first.first;
		int curC = q.front().first.second;
		int curCnt = q.front().second;
		q.pop();

		if (curR == r2 && curC == c2) {
			return curCnt;
		}

		for (int d = 0; d < 4; d++) {
			int nextR = curR + dr[d];
			int nextC = curC + dc[d];
			if (nextR < 1 || nextC < 1 || 
				nextR > n || nextC > n ||
				visited[nextR][nextC] || map[nextR][nextC] < 0) continue;

			q.push({ { nextR, nextC }, curCnt + 1 });
			visited[nextR][nextC] = true;
		}
	}
}