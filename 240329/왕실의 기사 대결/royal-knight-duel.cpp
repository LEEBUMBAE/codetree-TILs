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

		if (canMove(knightN, knightD, 0)) {
			move(knightN, knightD);
			getDamage();
		}
	}

	// 출력
	// 최후까지 '생존한 기사들이' 받은 대미지의 합을 출력 // 기사정보에 누적받은대미지 기록해야함
	int answer = 0;
	for (int i = 1; i <= N; i++) {
		if (knightList[i].dead == true) continue;
		answer += knightList[i].damageSum;
	}
	cout << answer;

	return 0;
}

// 1 기사이동
bool canMove(int knightN, int knightD, int cnt) {
	Knight &knight = knightList[knightN];
	// 기사가 죽은놈인지 판별(체스판 밖 기사면 안됨)
	if (knight.dead) return false;

	// 이동방향 쪽 변기준 +1칸 search()
	pair<int, vector<int>> resultSearch = search(knight, knightD);
	// search의 결과 {1, 0}=벽 {2, 기사번호}=기사 {3, 0}=장애물or빈칸
	if (resultSearch.first == 1) { // 1. 벽
		return false;
	}
	else if (resultSearch.first == 2) { //2. 기사 => 재귀 실행
		bool flag = true;
		for (int i = 1; i <= N; i++) {
			if (resultSearch.second[i] == 1) {
				if (!canMove(i, knightD, cnt + 1)) {
					flag = false;
				}
			}
		}
		// 모든 조사가 끝났을때
		if (cnt == 0 && !flag) {
			for (int i = 1; i <= N; i++) {
				knightList[i].attacked = false;
			}
		}
		return flag;
	}
	else if (resultSearch.first == 3) { //3. 장애물 or 빈칸
		return true;
	}
}

pair<int, vector<int>> search(Knight& knight, int knightD) {
	// search의 결과 {1, 0}=벽 {2, 기사번호}=기사 {3, 0}=장애물or빈칸
	vector<int> isKnight(N + 1, 0);
	vector<int> wallOrEmpty;
	if (knightD == 0) { // 상
		for (int c = knight.c; c < knight.c + knight.w; c++) {
			int up = knight.r - 1;

			if (up < 1 || map[up][c] == 2) { // 벽이있으면 {1, wall 리턴}
				return { 1, wallOrEmpty };
			}
			if (map[up][c] != 2) { // 벽이없으면 
				if (knightMap[up][c] != 0) { // 기사있으면
					isKnight[knightMap[up][c]] = 1;
					knightList[knightMap[up][c]].attacked = true;
				}
			}
		}
		// 끝까지 장애물 or 빈칸만 있으면(마지막에 isKnight.size() == 0 면) {3, wallOrEmpty} return
		for (int i = 1; i <= N; i++) {
			if (isKnight[i] == 1) {
				return { 2, isKnight };
			}
		}
		return { 3, wallOrEmpty };
	}
	else if (knightD == 1) { // 우 
		for (int r = knight.r; r < knight.r + knight.h; r++) {
			int right = knight.c + knight.w;

			if (right > L || map[r][right] == 2) { // 벽이있으면 {1, wall 리턴}
				return { 1, wallOrEmpty };
			}
			if (map[r][right] != 2) { // 벽이없으면 
				if (knightMap[r][right] != 0) { // 기사있으면
					isKnight[knightMap[r][right]] = 1;
					knightList[knightMap[r][right]].attacked = true;
				}
			}
		}
		// map에서 끝까지 장애물 or 빈칸만 있으면(마지막에 isKnight.size() == 0 면) {3, wallOrEmpty} return
		for (int i = 1; i <= N; i++) {
			if (isKnight[i] == 1) {
				return { 2, isKnight };
			}
		}
		return { 3, wallOrEmpty };
	}
	else if (knightD == 2) { // 하
		for (int c = knight.c; c < knight.c + knight.w; c++) {
			int down = knight.r + knight.h;

			if (down > L || map[down][c] == 2) { // 벽이있으면 {1, wall 리턴}
				return { 1, wallOrEmpty };
			}
			if (map[down][c] != 2) { // 벽이없으면 
				if (knightMap[down][c] != 0) { // 기사있으면
					isKnight[knightMap[down][c]] = 1;
					knightList[knightMap[down][c]].attacked = true;
				}
			}
		}
		// map에서 끝까지 장애물 or 빈칸만 있으면(마지막에 isKnight.size() == 0 면) {3, wallOrEmpty} return
		for (int i = 1; i <= N; i++) {
			if (isKnight[i] == 1) {
				return { 2, isKnight };
			}
		}
		return { 3, wallOrEmpty };
	}
	else { // 좌
		for (int r = knight.r; r < knight.r + knight.h; r++) {
			int left = knight.c - 1;

			if (left < 1 || map[r][left] == 2) { // 벽이있으면 {1, wall 리턴}
				return { 1, wallOrEmpty };
			}
			if (map[r][left] != 2) { // 벽이없으면 
				if (knightMap[r][left] != 0) { // 기사있으면
					isKnight[knightMap[r][left]] = 1;
					knightList[knightMap[r][left]].attacked = true;
				}
			}
		}
		for (int i = 1; i <= N; i++) {
			if (isKnight[i] == 1) {
				return { 2, isKnight };
			}
		}
		return { 3, wallOrEmpty };
	}
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
		
		// 한개씩 이동시켜서 tempMap에 저장
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
		Knight &curKnight = knightList[i];
		
		if (curKnight.attacked) {
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

			if (curKnight.k <= 0) {
				for (int i = 1; i <= L; i++) {
					for (int j = 1; j <= L; j++) {
						visited[i][j] = false;
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
}