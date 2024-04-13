#include<iostream>
#include<vector>
#include<queue>

using namespace std;

class Knight {
public:
	int r, c, h, w, k, damage;
	Knight() : r(0), c(0), h(0), w(0), k(0), damage(0){}
	Knight(int r, int c, int h, int w, int k, int damage): r(r), c(c), h(h), w(w), k(k), damage(damage){}
};

int L, N, Q;
int dR[4] = { -1, 0, 1, 0 };
int dC[4] = { 0, 1, 0, -1 };
vector<vector<int>> map;
vector<vector<int>> knightMap;
vector<Knight> knightList;
vector<int> knightVisited;

void fillKnightMap(int n, int r, int c, int h, int w);
bool moveKnight(int knightN, int dir);
void doMove(int knightN, int dir);
void copyMap(vector<vector<int>>& map1, vector<vector<int>>& map2);
void getDamage();
void deadKnight();

int main() {
	// 테스트케이스를 한꺼번에 집어넣는다면 for문
	cin >> L >> N >> Q;
	map = vector<vector<int>>(L + 1, vector<int>(L + 1, 0));
	knightMap = vector<vector<int>>(L + 1, vector<int>(L + 1, 0));
	knightList = vector<Knight>(N + 1, Knight());
	
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = 1; i <= N; i++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		Knight tempKnight(r, c, h, w, k, 0);
		knightList[i] = tempKnight;
		fillKnightMap(i, r, c, h, w);
	}
	
	// 로직
	for (int q = 1; q <= Q; q++) {
		int knightN, dir;
		cin >> knightN >> dir;

		knightVisited = vector<int>(N + 1, false);
		if (!moveKnight(knightN, dir)) continue;
		getDamage();
		deadKnight();
	}

	int sumDamage = 0;
	for (int n = 1; n <= N; n++) {
		if (knightList[n].k <= 0) continue;
		sumDamage += knightList[n].damage;
	}
	cout << sumDamage;
}

void fillKnightMap(int n, int r, int c, int h, int w) {
	for (int i = r; i < r + h; i++) {
		for (int j = c; j < c + w; j++) {
			knightMap[i][j] = n;
		}
	}
}

bool moveKnight(int knightN, int dir) {
	// 이미 죽은 기사에 대한 명령
	if (knightList[knightN].k <= 0) return false;

	// 밀리는 기사들을 q에 보관하면서 knightVisited에 표시함
	queue<int> q;
	q.push(knightN);
	while (!q.empty()) {
		int curN = q.front();
		q.pop();

		int curR = knightList[curN].r;
		int curC = knightList[curN].c;
		int curH = knightList[curN].h;
		int curW = knightList[curN].w;

		// dir에 따라 스캔 구역을 바꿈
		int startR, endR, startC, endC;
		if (dir == 0) { // 상
			startR = curR - 1;
			endR = curR - 1;
			startC = curC;
			endC = curC + curW - 1;
		}
		else if (dir == 1) { // 우
			startR = curR;
			endR = curR + curH - 1;
			startC = curC + curW;
			endC = curC + curW;
		}
		else if (dir == 2) { // 하
			startR = curR + curH;
			endR = curR + curH;
			startC = curC;
			endC = curC + curW - 1;
		}
		else if (dir == 3) { // 좌
			startR = curR;
			endR = curR + curH - 1;
			startC = curC - 1;
			endC = curC - 1;
		}
		for (int nextR = startR; nextR <= endR; nextR++) {
			for (int nextC = startC; nextC <= endC; nextC++) {
				// 벽 만나면 이동 x
				if (nextR < 1 || nextC < 1 || nextR > L || nextC > L) return false;
				if (map[nextR][nextC] == 2) return false;
				// 기사 없는칸이면 q에 안넣음
				if (knightMap[nextR][nextC] == 0) continue;
				// 기사발견
				int nextN = knightMap[nextR][nextC];
				if (knightVisited[nextN]) continue;
				q.push(nextN);
				knightVisited[nextN] = true;
			}
		}
	}

	doMove(knightN, dir);
	return true;
}

void doMove(int knightN, int dir) {
	// 명령 받은기사와 밀린 기사들 움직여주는 작업

	// 기사맵에서 옮기기
	vector<vector<int>> tempKnightMap(L + 1, vector<int>(L + 1, 0));
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			int tempKnightN = knightMap[i][j];
			if (!knightVisited[tempKnightN] && knightN != tempKnightN) continue;
			tempKnightMap[i + dR[dir]][j + dC[dir]] = tempKnightN;
		}
	}
	copyMap(knightMap, tempKnightMap);
	
	// 기사배열에서 옮기기
	for (int i = 1; i <= N; i++) {
		if (!knightVisited[i] || knightN != i) continue;
		knightList[i].r += dR[dir];
		knightList[i].c += dC[dir];
	}
}

void copyMap(vector<vector<int>>& map1, vector<vector<int>>& map2) {
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			map1[i][j] = map2[i][j];
		}
	}
}

void getDamage() {
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			if (knightMap[i][j] == 0) continue;
			int curN = knightMap[i][j];
			if (!knightVisited[curN]) continue;
			if (map[i][j] != 1) continue;
			knightList[curN].k -= 1;
			knightList[curN].damage += 1;
		}
	}
}

void deadKnight() {
	for (int n = 1; n <= N; n++) {
		if (!knightVisited[n]) continue;
		if (knightList[n].k > 0) continue;
		int r = knightList[n].r;
		int c = knightList[n].c;
		int h = knightList[n].h;
		int w = knightList[n].w;

		for (int i = r; i < r + h; i++) {
			for (int j = c; j <= c + w; j++) {
				knightMap[i][j] = 0;
			}
		}
	}
	
}