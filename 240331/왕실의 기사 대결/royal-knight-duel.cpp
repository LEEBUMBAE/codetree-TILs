#include<iostream>
#include<vector>
#include<queue>

using namespace std;

class Knight {
public:
	int r, c, h, w, k;
	bool attacked; // 초기명령받은기사or안 움직인기사 false, 밀쳐진기사 true
	int damageSum; // 누적받은데미지
	bool dead; // 죽었는지

	Knight() {
		this->r = 0;
		this->c = 0;
		this->h = 0;
		this->w = 0;
		this->k = 0;
		this->attacked = false;
		this->damageSum = 0;
		this->dead = false;
	}
	Knight(int r, int c, int h, int w, int k, bool attacked, int damageSum, bool dead) {
		this->r = r;
		this->c = c;
		this->h = h;
		this->w = w;
		this->k = k;
		this->attacked = attacked;
		this->damageSum = damageSum;
		this->dead = dead;
	}
};

int L, N, Q;
int map[41][41] = { 2 };
vector<Knight> knightList;
vector<vector<int>> knightMap;
vector<vector<int>> tempKnightMap;
int dR[4] = { -1, 0, 1, 0 };
int dC[4] = { 0, 1, 0, -1 };

bool canMove(int knightN, int knightD, int cnt);
pair<int, vector<int>> search(Knight& knight, int knightD);
void move(int knightN, int knightD);
void getDamage();

int main() {
	cin >> L >> N >> Q;
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> map[i][j];
		}
	}
	knightList = vector<Knight>(N + 1);
	knightMap = vector<vector<int>>(L + 1, vector<int>(L + 1));
	for (int i = 1; i <= N; i++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		// 기사배열
		knightList[i] = Knight(r, c, h, w, k, false, 0, false);
		// 기사맵
		for (int x = r; x < r + h; x++) {
			for (int y = c; y < c + w; y++) {
				knightMap[x][y] = i;
			}
		}
	}

	// 로직
	for (int q = 1; q <= Q; q++) {
		int knightN, knightD;
		cin >> knightN >> knightD;
		if (q == 90) {
			cout << "";
		}
		if (canMove(knightN, knightD, 0)) {
			move(knightN, knightD);
			getDamage();
		}
	}

	// 출력
	// 최후까지 '생존한 기사들이' 받은 대미지의 합을 출력 // 기사정보에 누적받은대미지 기록해야함
	int answer = 0;
	for (int i = 1; i <= N; i++) {
		if (knightList[i].dead) continue;
		answer += knightList[i].damageSum;
	}
	cout << answer;

	return 0;
}

// 1 기사이동
bool canMove(int knightN, int knightD, int cnt) {
	Knight& knight = knightList[knightN];
	// 기사가 죽은놈인지 판별(체스판 밖 기사면 안됨)
	if (knight.dead) return false;

	// search 실행
	pair<int, vector<int>> resultSearch = search(knight, knightD);
	// {1, 0}=벽 {2, 공격받은기사여부배열}=기사 {3, 0}=장애물or빈칸
	bool flag = true;
	if (resultSearch.first == 1) { // 1. 벽
		
		return flag;
	}
	else if (resultSearch.first == 2) { //2. 기사 
		
		for (int i = 1; i <= N; i++) {
			if (resultSearch.second[i] == 1) {
				if (!canMove(i, knightD, cnt + 1)) { //재귀
					flag = false;
				}
			}
		}
	}
	else if (resultSearch.first == 3) { //3. 장애물 or 빈칸
		flag true;
	}
	// 모든 재귀가 끝났을때
		// 영향 받는 기사중에 벽을 만난 기사가 하나라도 있다면
		if (cnt == 0 && !flag) {
			for (int i = 1; i <= N; i++) {
				knightList[i].attacked = false;
			}
		}
		return flag;
}

pair<int, vector<int>> search(Knight& knight, int knightD) {
	vector<int> isKnight(N + 1, 0);
	vector<int> wallOrEmpty;

	// 방향에 따른 검사 시작점과 끝점 설정
	int startR = knight.r, startC = knight.c, endR = knight.r + knight.h, endC = knight.c + knight.w;

	// 방향에 따라 검사 범위 조정
	if (knightD == 0) { // 상
		startR -= 1;
		endR = knight.r;
	}
	else if (knightD == 1) { // 우
		startC = knight.c + knight.w;
		endC += 1;
	}
	else if (knightD == 2) { // 하
		startR = knight.r + knight.h;
		endR += 1;
	}
	else if (knightD == 3) { // 좌
		startC -= 1;
		endC = knight.c;
	}

	// 지정된 방향에 대해 벽, 기사, 빈칸/함정 검사
	for (int r = startR; r < endR; ++r) {
		for (int c = startC; c < endC; ++c) {
			// 범위를 벗어나거나 벽이 있는 경우
			if (r < 1 || r > L || c < 1 || c > L || map[r][c] == 2) {
				return { 1, wallOrEmpty };
			}
			// 기사가 있는 경우
			// 일단 기록해둠
			if (knightMap[r][c] != 0) {
				isKnight[knightMap[r][c]] = 1;
				knightList[knightMap[r][c]].attacked = true;
			}
		}
	}

	// 기사가 발견된 기록이 있는 경우
	for (int i = 1; i <= N; ++i) {
		if (isKnight[i] == 1) {
			return { 2, isKnight };
		}
	}

	// 그 외의 경우, 장애물이나 빈칸으로 간주
	return { 3, wallOrEmpty };
}

void move(int knightN, int knightD) {
	vector<vector<int>> tempMap(L + 1, vector<int>(L + 1));
	vector<vector<bool>> visited(L + 1, vector<bool>(L + 1));
	queue<pair<int, int>> q;

	q.push({ knightList[knightN].r , knightList[knightN].c });
	visited[knightList[knightN].r][knightList[knightN].c] = true;

	while (!q.empty()) {
		int curR = q.front().first;
		int curC = q.front().second;
		q.pop();

		// 한개씩 tempMap에 이동시켜서 저장
		tempMap[curR + dR[knightD]][curC + dC[knightD]] = knightMap[curR][curC];

		for (int d = 0; d < 4; d++) {
			int nextR = curR + dR[d];
			int nextC = curC + dC[d];
			if (nextR < 1 || nextC < 1 ||
				nextR > L || nextC > L ||
				visited[nextR][nextC] ||
				!(knightMap[nextR][nextC] == knightN || knightList[knightMap[nextR][nextC]].attacked)) continue;
			q.push({ nextR, nextC });
			visited[nextR][nextC] = true;
		}
	}

	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			// 이동하는 덩어리에 속하지않고(방문 하지않았고), 기사가있는 위치는 기존기사맵을 따른다 
			if (!visited[i][j] && knightMap[i][j] != 0) {
				tempMap[i][j] = knightMap[i][j];
			}
		}
	}
	// 맵 복사
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			knightMap[i][j] = tempMap[i][j];
		}
	}

	// 기사리스트에도 r,c 정보 최신화
	knightList[knightN].r += dR[knightD];
	knightList[knightN].c += dC[knightD];
	for (int i = 1; i <= N; i++) {
		if (knightList[i].attacked) {
			knightList[i].r += dR[knightD];
			knightList[i].c += dC[knightD];
		}
	}
}

// 2 대결 대미지
void getDamage() {
	for (int i = 1; i <= N; i++) {
		Knight& curKnight = knightList[i];
		//밀린 기사만 대미지 
		if (!curKnight.attacked) continue;

		vector<vector<bool>> visited(L + 1, vector<bool>(L + 1));
		queue<pair<int, int>> q;
		int trapNum = 0; // 장애물수

		q.push({ curKnight.r, curKnight.c });
		visited[curKnight.r][curKnight.c] = true;

		while (!q.empty()) {
			int curR = q.front().first;
			int curC = q.front().second;
			q.pop();

			if (map[curR][curC] == 1) trapNum++;

			for (int d = 0; d < 4; d++) {
				int nextR = curR + dR[d];
				int nextC = curC + dC[d];
				if (nextR < 1 || nextC < 1 ||
					nextR > L || nextC > L || visited[nextR][nextC] || knightMap[nextR][nextC] != i) continue;
				q.push({ nextR, nextC });
				visited[nextR][nextC] = true;
			}
		}

		curKnight.k -= trapNum;
		curKnight.damageSum += trapNum;
		curKnight.attacked = false;

		// 죽은기사 기사맵에서 제거
		if (curKnight.k <= 0) {
			for (int x = 1; x <= L; x++) {
				for (int y = 1; y <= L; y++) {
					visited[x][y] = false;
				}
			}

			q.push({ curKnight.r, curKnight.c });
			visited[curKnight.r][curKnight.c] = true;

			while (!q.empty()) {
				int curR = q.front().first;
				int curC = q.front().second;
				q.pop();

				knightMap[curR][curC] = 0;

				for (int d = 0; d < 4; d++) {
					int nextR = curR + dR[d];
					int nextC = curC + dC[d];
					if (nextR < 1 || nextC < 1 ||
						nextR > L || nextC > L || visited[nextR][nextC] || knightMap[nextR][nextC] != i) continue;
					q.push({ nextR, nextC });
					visited[nextR][nextC] = true;
				}
			}
			curKnight.dead = true;
		}
	}
}