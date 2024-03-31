#include<iostream>
#include<queue>
#include<vector>

using namespace std;

class Knight {
public:
	int r, c, h, w, k, damage;
	bool attacked;
	Knight() :r(0), c(0), h(0), w(0), k(0), attacked(false), damage(0) {}
	Knight(int r, int c, int h, int w, int k, bool attacked, int damage) : r(r), c(c), h(h), w(w), k(k), attacked(attacked), damage(damage){}
};

int dR[4] = { -1, 0, 1, 0 };
int dC[4] = { 0, 1, 0, -1 };
int L, N, Q;
int map[41][41];
vector<Knight> knightList;

void move(int num, int dir) {
	queue<int> q;
	vector<bool> visited(N + 1, false);
	vector<vector<int>> knightMap(L + 1, vector<int>(L + 1, 0));
	for (int i = 1; i <= N; i++) {
		// 죽은 기사는 맵에서 제외
		if (knightList[i].k <= 0) continue;
		for (int r = knightList[i].r; r < knightList[i].r + knightList[i].h; r++) {
			for (int c = knightList[i].c; c < knightList[i].c + knightList[i].w; c++) {
				knightMap[r][c] = i;
			}
		}
	}
	
	q.push(num); // visited할 필요없음 어차피 첫명령자는 대미지 안입으니까 그리고 탐색과정중에 여길 다시 볼일이 없으니까

	// bfs처럼 q에 담으면서 while문 돌아
	while (!q.empty()) {
		int curNum = q.front();
		q.pop();
		int curR = knightList[curNum].r;
		int curC = knightList[curNum].c;
		int curH = knightList[curNum].h;
		int curW = knightList[curNum].w;

		int startR = curR, startC = curC, endR = curR + curH, endC = curC + curW;
		if (dir == 0) { // 상
			startR = curR - 1;
			endR = curR;
		}
		else if (dir == 1) { // 우
			startC = curC + curW;
			endC = curC + curW + 1;
		}
		else if (dir == 2) { // 하
			startR = curR + curH;
			endR = curR + curH + 1;
		}
		else if (dir == 3) { // 좌
			startC = curC - 1;
			endC = curC;
		}

		// 방향쪽으로 확인하면서 기사를 발견하면 담는다
		for (int nR = startR; nR < endR; nR++) {
			for (int nC = startC; nC < endC; nC++) {
				// 벽이면 리턴
				if (nR < 1 || nC < 1 || nR > L || nC > L || map[nR][nC] == 2) return;
				// 기사발견하면 기사 큐에 넣어
				// visited확인해서 이미 발견한기사면 안넣음
				int nKnightNum = knightMap[nR][nC];
				if (nKnightNum == 0) continue;
				if (visited[nKnightNum]) continue;
				q.push(nKnightNum);
				visited[nKnightNum] = true;
			}
		}
	}

	knightList[num].r += dR[dir];
	knightList[num].c += dC[dir];
	for (int i = 1; i <= N; i++) {
		if (!visited[i]) continue;
		knightList[i].attacked = true;
		knightList[i].r += dR[dir];
		knightList[i].c += dC[dir];

	}
}
void getDamage() {
	for (int i = 1; i <= N; i++) {
		if (!knightList[i].attacked) continue;
		int damage = 0;
		for (int r = knightList[i].r; r < knightList[i].r + knightList[i].h; r++) {
			for (int c = knightList[i].c; c < knightList[i].c + knightList[i].w; c++) {
				if (map[r][c] == 1) damage++;
			}
		}
		knightList[i].damage += damage;
		knightList[i].k -= damage;
		knightList[i].attacked = false;
	}
}

int main() {
	cin >> L >> N >> Q;
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> map[i][j];
		}
	}
	knightList = vector<Knight> (N + 1);
	for (int i = 1; i <= N; i++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		knightList[i] = Knight(r, c, h, w, k, false, 0);
	}

	for (int i = 1; i <= Q; i++) {
		int num, dir;
		cin >> num >> dir;

		move(num, dir); // 이동 과 attacked 표시
		getDamage(); // attacked 표시된 애들 대미지 입음
	}

	int sumDamage = 0;
	for (int i = 1; i <= N; i++) {
		if (knightList[i].k <= 0) continue;
		sumDamage += knightList[i].damage;
	}
	cout << sumDamage;
}