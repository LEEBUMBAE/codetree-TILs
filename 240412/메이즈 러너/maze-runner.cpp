#include <iostream>
#include<vector>

using namespace std;

// 클래스 선언
class Vertex {
public:
	int state;
	int player;
	Vertex() {
		this->state = 0;
		this->player = 0;
	}
	Vertex(int state, int player) {
		this->state = state;
		this->player = player;
	}
};

// 변수 선언
int N, M, K, answer; // 맵크기, 참가자 수, 게임시간, 참가자들 이동거리
vector<vector<Vertex>> map; // 맵
pair<int, int> exitPoint; // 출구좌표
int dr[4] = { -1, 1, 0, 0 };
int dc[4] = { 0, 0, -1, 1 };

// 함수 선언
void move();
bool isGameover();
void subrotate(int r, int c, int minL);
void rotate();
void getExitPoint();

// 함수 정의
void move() {
	vector<vector<Vertex>> newMap(N + 1, vector<Vertex>(N + 1));
	
	// newMap에 맵복사, state는 복사해 둬도 됨. 바뀌는게 없거든
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
				newMap[i][j].state = map[i][j].state;
		}
	}

	// newMap에 이동시킴
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j].player > 0) {
				int curDis = abs(i - exitPoint.first) + abs(j - exitPoint.second); // 최단거리

				// 사방 탐색 하면서 출구까지 거리확인
				bool moveFlag = false;
				for (int dir = 0; dir < 4; dir++) {
					int nr = i + dr[dir]; // 상하먼저
					int nc = j + dc[dir];

					if (nr < 1 || nc < 1 || nr > N || nc > N) continue; // 격자 벗어나면 넘어감

					int nDis = abs(nr - exitPoint.first) + abs(nc - exitPoint.second);
					// 최단거리가 더 가까워진다면 
					if (nDis < curDis) {
						if (map[nr][nc].state == 0) { // 다음 좌표가 빈칸일때 
							newMap[nr][nc].player += map[i][j].player; // newMap에 player 누적
							answer += map[i][j].player; //참가자 이동거리 누적
							map[i][j].player = 0; // 기존 맵에서 player 제거
							moveFlag = true; // 움직였다고 표시
							break;
						}
						else if (map[nr][nc].state == 10) { // 다음 좌표가 출구면
							answer += map[i][j].player; // 참가자 이동거리 누적
							map[i][j].player = 0; // 기존 맵에서 palyer 제거
							moveFlag = true; // 움직였다고 표시
							break;
						}
					}
				}

				if (!moveFlag) { // 안 움직였으면 플레이어 움직이지 않은 위치에 복사(누적해야함)
					newMap[i][j].player += map[i][j].player;
				}
			}
		}
	}

	// map에다가 newMap 복사
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			map[i][j]= newMap[i][j];
		}
	}
}

bool isGameover(){ // 참가자가 미로를 다 탈출했나 ?
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j].player > 0) return false;
		}
	}
	return true;
}

void rotate() {
	// 가장 작은 정사각형의 크기를 먼저 구하고 -> 완탐으로 출구좌표에서 참가자의 거리 조사(그 중 max) minL보다 작으면 갱신 하는 식으로
	int minL = 10000;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j].player > 0) {
				int curL = max(abs(i - exitPoint.first), abs(j - exitPoint.second));

				if (curL < minL) minL = curL;
			}
		}
	}

	// 탐색하면서 정사각형을 선택, 좌상단 좌표를 찾아야함
	minL++; // 기존 minL에 1을 더해야 정사각형의 길이
	pair<int, int> rec = { 0, 0 };
	for (int i = 1; i <= N - (minL - 1) ; i++) {
		for (int j = 1; j <= N - (minL - 1); j++) {

			// 좌상단 i,j ~ i + minL - 1, j + minL - 1까지 확인해봐야함, 그 안에 출구랑 참가자 둘다 존재하는지
			bool playerFlag = false;
			bool exitFlag = false;
			for (int r = i; r <= i + (minL - 1); r++) {
				for (int c = j; c <= j + (minL - 1); c++) {
					if (map[r][c].player > 0) playerFlag = true;
					if (map[r][c].state == 10) exitFlag = true;
					if (playerFlag && exitFlag) rec = { i, j }; // 둘다 존재하면 rec 좌표에 저장
				}
			}
			if (rec.first > 0) break;
		}
		if (rec.first > 0) break;
	}

	// 선택된 정사각형을 90도로 회전 및, 벽 내구도 감소
	subrotate(rec.first, rec.second, minL);

}

void subrotate(int r, int c, int minL) { // 선택된 정사각형을 90도로 회전 및, 벽 내구도 감소
	vector<vector<Vertex>> newMap1(minL + 1, vector<Vertex> (minL + 1));
	vector<vector<Vertex>> newMap2(minL + 1, vector<Vertex> (minL + 1));

	// 원래 사각형에서 newMap1으로 복사
	for (int i = r; i <= r + (minL - 1); i++) {
		for (int j = c; j <= c + (minL - 1); j++) {
			newMap1[i - r + 1][j - c + 1] = map[i][j];
		}
	}
	// newMap1에서 newMap2로 회전 (r, c) -> (c, n- r + 1)
	for (int i = 1; i <= minL; i++) {
		for (int j = 1; j <= minL; j++) {
			if (newMap1[i][j].state > 0 && newMap1[i][j].state < 10) newMap1[i][j].state--; // 벽이면 내구도 감소
			newMap2[j][minL - i + 1] = newMap1[i][j];
		}
	}

	// newMap2에서 원래 사각형으로 복사
	for (int i = r; i <= r + (minL - 1); i++) {
		for (int j = c; j <= c + (minL - 1); j++) {
			map[i][j] = newMap2[i - r + 1][j - c + 1];
		}
	}
}

void getExitPoint() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (map[i][j].state == 10) exitPoint = {i, j};
		}
	}
}



// 메인 함수
int main() {
	cin >> N >> M >> K;
	map = vector<vector<Vertex>>(N + 1, vector<Vertex>(N + 1));
	// 맵 좌표
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> map[i][j].state; // 벽과 빈칸 기록
		}
	}
	// 참가자 좌표
	for (int i = 0; i < M; i++) {
		int pr, pc;
		cin >> pr >> pc;
		map[pr][pc].player++; // 참가자 여러명이 한 곳 에 있을 수 있음
	}
	// 출구 좌표
	int er, ec;
	cin >> er >> ec;
	exitPoint = { er, ec }; // 출구 좌표 등록
	map[er][ec].state = 10; // 맵에도 표시

	answer = 0;
	for (int i = 1; i <= K; i++) {
		move();// 이동
		if (isGameover()) break;// 참가자 미로 다 탈출 했으면 종료 -> 이동과정에서 exit 이 바뀔일은 없으니까 바로 종료해도됨
		rotate();// 회전
		getExitPoint();// 출구 좌표 기록
	}
	cout << answer << endl;
	cout << exitPoint.first << " " << exitPoint.second << endl;
}