#include <iostream>
#include <queue>
#include <vector>
#include<algorithm>

using namespace std;

int R, C, K; // 행, 열, 골렘의 개수를 의미합니다
vector<vector<int>> shipMap;
vector<vector<int>> exitMap;
vector<int> exitList;
vector<pair<int, int>> shipList;
int dR[4] = { -1, 0, 1, 0 };
int dC[4] = { 0, 1, 0, -1 };

//함수 
void down(int c, int dir);
bool canDown(int r, int c);
bool canLeft(int r, int c);
bool canRight(int r, int c);
int doSearching();

int main() {
    cin >> R >> C >> K;
    shipMap = vector<vector<int>>(R + 3, vector<int> (C, 0));
    exitList.push_back(-1);
    shipList.push_back({ -1, -1 });

    int sum = 0;
    for (int id = 1; id <= K; id++) { // 골렘 번호 id
        int c, d;
        cin >> c >> d; // 골렘의 출발 c좌표, 방향 d를 입력받습니다
        c--;
        down(c, d);
        sum += doSearching();
    }
    cout << sum << "\n";
}

// 1. 남쪽으로 한 칸 내려갑니다.
// 2. (1)의 방법으로 이동할 수 없으면 서쪽 방향으로 회전하면서 내려갑니다.
// 3. (1)과 (2)의 방법으로 이동할 수 없으면 동쪽 방향으로 회전하면서 내려갑니다.
void down(int c, int dir) {
    int curR = 1;
    int curC = c;
    int curDir = dir;
    exitList.push_back(curDir);
    shipList.push_back({ curR, curC });
    int curN = shipList.size() - 1;

    while (1) {
        int cnt = 0;
        bool done = false;
        curR = shipList[curN].first;
        curC = shipList[curN].second;
        curDir = exitList[curN];

        if (!done && canDown(curR, curC)) {
            int nextR = curR + dR[2];
            int nextC = curC + dC[2];
            shipList[curN] = { nextR, nextC };
            done = true;
        }
        if (!done && canLeft(curR, curC)) {
            int nextR = curR + dR[3] + dR[2];
            int nextC = curC + dC[3] + dC[2];
            shipList[curN] = { nextR, nextC };
            exitList[curN] = (curDir - 1 + 4) % 4;
            done = true;
        }
        if (!done && canRight(curR, curC)) {
            int nextR = curR + dR[1] + dR[2];
            int nextC = curC + dC[1] + dC[2];
            shipList[curN] = { nextR, nextC };
            exitList[curN] = (curDir + 1 + 4) % 4;
            done = true;
        }

        if (!done) {
            // 멈췄으면 검사해서 삐져나왔으면 맵 초기화 시켜버림
            bool resetFlag = false;
            for (int d = 0; d < 5; d++) {
                int nextR = shipList[curN].first;
                int nextC = shipList[curN].second;
                if (d < 4) {
                    nextR = shipList[curN].first + dR[d];
                    nextC = shipList[curN].second + dC[d];
                }

                if (nextR < 3 || nextC < 0 || nextR > R + 2 || nextC > C - 1) {
                    resetFlag = true;
                    break;
                }
            }

            if (resetFlag) {
                // 맵 초기화
                shipMap = vector<vector<int>>(R + 3, vector<int>(C, 0));
                exitList.clear();
                shipList.clear();
                exitList.push_back(-1);
                shipList.push_back({ -1, -1 });
            }
            // while문 탈출
            break;
        }
    }

    // 맵에 기록
    for (int i = 1; i <= shipList.size() - 1; i++) {
        int curR = shipList[i].first;
        int curC = shipList[i].second;

        for (int d = 0; d < 5; d++) {
            int nextR = curR;
            int nextC = curC;
            if (d < 4) {
                nextR = curR + dR[d];
                nextC = curC + dC[d];
            }

            shipMap[nextR][nextC] = i;
        }
    }
}
bool canDown(int r, int c) {
    int curR = r + dR[2];
    int curC = c + dC[2];

    for (int d = 0; d < 5; d++) {
        int nextR, nextC;
        if (d < 4) {
            nextR = curR + dR[d];
            nextC = curC + dC[d];
        }
        else {
            nextR = curR;
            nextC = curC;
        }

        if (nextR < 0 || nextC < 0 || nextR > R + 2 || nextC > C - 1) {
            return false;
        }

        if (shipMap[nextR][nextC] != 0) {
            return false;
        }
    }
    return true;
}
bool canLeft(int r, int c) {
    int curR = r + dR[3];
    int curC = c + dC[3];

    for (int d = 0; d < 5; d++) {
        int nextR = curR;
        int nextC = curC;
        if (d < 4) {
            nextR = curR + dR[d];
            nextC = curC + dC[d];
        }

        if (nextR < 0 || nextC < 0 || nextR > R + 2 || nextC > C - 1) {
            return false;
        }
        if (shipMap[nextR][nextC] != 0) {
            return false;
        }
    }

    curR = r + dR[3] + dR[2];
    curC = c + dC[3] + dC[2];

    for (int d = 0; d < 5; d++) {
        int nextR = curR;
        int nextC = curC;
        if (d < 4) {
            nextR = curR + dR[d];
            nextC = curC + dC[d];
        }

        if (nextR < 0 || nextC < 0 || nextR > R + 2 || nextC > C - 1) {
            return false;
        }
        if (shipMap[nextR][nextC] != 0) {
            return false;
        }
    }
    return true;
}
bool canRight(int r, int c) {
    int curR = r + dR[1];
    int curC = c + dC[1];

    for (int d = 0; d < 5; d++) {
        int nextR = curR;
        int nextC = curC;
        if (d < 4) {
            nextR = curR + dR[d];
            nextC = curC + dC[d];
        }

        if (nextR < 0 || nextC < 0 || nextR > R + 2 || nextC > C - 1) {
            return false;
        }
        if (shipMap[nextR][nextC] != 0) {
            return false;
        }
    }

    curR = r + dR[1] + dR[2];
    curC = c + dC[1] + dC[2];

    for (int d = 0; d < 5; d++) {
        int nextR = curR;
        int nextC = curC;
        if (d < 4) {
            nextR = curR + dR[d];
            nextC = curC + dC[d];
        }

        if (nextR < 0 || nextC < 0 || nextR > R + 2 || nextC > C - 1) {
            return false;
        }
        if (shipMap[nextR][nextC] != 0) {
            return false;
        }
    }
    return true;
}


// 정령이 움직일 수 있는 모든 범위를 확인하고 도달할 수 있는 최하단 행을 반환합니다
int doSearching() {
    if (shipList.size() - 1 == 0) return 0;

    // exit 맵 채우기
    vector<vector<int>> exitMap(R + 3, vector<int>(C, false));
    for (int i = 1; i <= shipList.size() - 1; i++) {
        int curR = shipList[i].first;
        int curC = shipList[i].second;
        int curDir = exitList[i];

        exitMap[curR + dR[curDir]][curC + dC[curDir]] = true;
    }

    // bfs
    int curN = shipList.size() - 1;
    int curR = shipList[curN].first;
    int curC = shipList[curN].second;
    queue<pair<int, int> > q;
    vector<vector<int>> visited(R + 3, vector<int>(C, false));
    q.push(make_pair(curR, curC));
    visited[curR][curC] = true;

    int maxR = curR;
    while (!q.empty()) {
        curR = q.front().first;
        curC = q.front().second;
        curN = shipMap[curR][curC];
        q.pop();

        for (int d = 0; d < 4; d++) {
            int nextR = curR + dR[d];
            int nextC = curC + dC[d];

            if (nextR < 3 || nextC < 0 || nextR > R + 2 || nextC > C - 1) continue;
            if (visited[nextR][nextC]) continue;
            if (shipMap[nextR][nextC] == 0) continue;
            if (shipMap[nextR][nextC] == curN) {
                q.push({ nextR, nextC });
                visited[nextR][nextC] = true;
                maxR = max(nextR, maxR);
            }
            else {
                if (exitMap[curR][curC]) {
                    q.push({ nextR, nextC });
                    visited[nextR][nextC] = true;
                    maxR = max(nextR, maxR);
                }
            }
        }
    }


    return (maxR - 2);
}