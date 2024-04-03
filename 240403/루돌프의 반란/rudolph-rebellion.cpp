#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include<cmath>

using namespace std;

int N, M, P;
int dolfR, dolfC, dolfDir, dolfPower, santaPower;
vector<int> santaR;
vector<int> santaC;
vector<int> santaDir;
vector<int> santaScore;
vector<int> santaStun;
vector<bool> santaDie;
int map[51][51]; // 산타기록 맵
int dR[8] = { -1, 0, 1, 0, -1, 1, 1, - 1 };
int dC[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };


void moveDolf();
void moveSanta();
int calDis(int r, int c);
void crush(int santaN, int dir, int power);
void chainMove(int nextSantaN, int dir);

// 매턴 루돌프 움직이고 산타 1~P(격자밖, 기절한 산타는 제외)번까지 움직임
// 두점사이 거리는 차의 제곱 + 차의 제곱으로 계산함

// 루돌프 이동
// 게임에서 탈락안한 산타 중 가장 가까운 산타 선택
// 기절산타일 수 도있음 주의
// 동일거리시 R좌표, C좌표가 '큰' 산타를 향해 돌진 
// 대각선 포함한 인접 8방으로 1칸(이땐 대각선도 편의상 그냥 한칸이라고 생각) 돌진가능
void moveDolf() {
    int targetR = 0, targetC = 0;
    bool targetOn = false;
    int minCurDis = 2100000000;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (map[i][j] == 0) continue;
            int santaN = map[i][j];

            if (santaDie[santaN]) continue;

            // 루돌프와 해당산타의 거리를 재는 calDis
            int curDis = calDis(i, j);
            // 최소거리와 비교 
            // 최소거리가 새로 갱신되면 그 산타로 지정할거임 targetR,C 에 저장한다
            // 타겟이 지정되면 targetOn = true
            // R좌표크고, 똑같으면 C좌표 가장큰 
            if (curDis <= minCurDis) {
                minCurDis = curDis;
                targetR = i;
                targetC = j;
                targetOn = true;
            }
        }
    }
    if (targetOn) {
        int minNextDis = 2100000000;
        int minDir = -1;

        for (int d = 0; d < 8; d++) {
            int nextR = dolfR + dR[d];
            int nextC = dolfC + dC[d];
            if (nextR < 1 || nextC < 1 || nextR > N || nextC > N) continue;

            int NextDis = (targetR - nextR) * (targetR - nextR) + (targetC - nextC) * (targetC - nextC);
            
            if (NextDis < minNextDis) {
                minNextDis = NextDis;
                minDir = d;
            }
        }

        // 정해진 최소거리 방향대로 루돌프 이동, 거기에 산타있으면 충돌진행
        dolfR += dR[minDir];
        dolfC += dC[minDir];
        dolfDir = minDir;

        // 해당위치에 산타있으면
        if (0 < map[dolfR][dolfC]) {
            int santaN = map[dolfR][dolfC];
            crush(santaN, dolfDir, dolfPower);
        }
    }
}

void moveSanta() {
    for (int i = 1; i <= P; i++) {
        if (santaStun[i] > 0 || santaDie[i]) continue; // 기절산타, 죽은산타는 move불가
        // 루돌프와 해당산타의 거리를 재는 calDis
        // 거리를 재서 기록해놓음
        int minDis = calDis(santaR[i], santaC[i]);
        int minDir = -1;
        for (int d = 0; d < 4; d++) {
            int nextR = santaR[i] + dR[d];
            int nextC = santaC[i] + dC[d];

            if (nextR < 1 || nextC < 1 || nextR > N || nextC > N) continue; // 격자밖
            if (map[nextR][nextC] > 0) continue; // 다른산타
            
            int NextDis = (dolfR - nextR) * (dolfR - nextR) + (dolfC - nextC) * (dolfC - nextC);
            if (minDis > NextDis) {
                minDis = NextDis;
                minDir = d;
            }
        }
        
        if (minDir == -1) continue;// 움직일곳 없으면 넘어가
         // 정해진 최소거리 방향대로 산타 이동
        map[santaR[i]][santaC[i]] = 0;
        santaR[i] += dR[minDir];
        santaC[i] += dC[minDir];
        santaDir[i] = minDir;
        map[santaR[i]][santaC[i]] = i;
        // 해당위치에 루돌프 있으면 충돌진행
        if (santaR[i] == dolfR && santaC[i] == dolfC) {
            int santaN = map[santaR[i]][santaC[i]];
            crush(santaN, (santaDir[i] + 2) % 4, santaPower);
        }
    }
}

int calDis(int r, int c) {
    return (dolfR - r) * (dolfR - r) + (dolfC - c) * (dolfC - c);
}

void crush(int santaN, int dir, int power) {
    // 충돌한 산타 점수 얻음
    santaScore[santaN] += power;
    // 맵에서 충돌한곳 산타 지움
    map[santaR[santaN]][santaC[santaN]] = 0;

    int nextR = santaR[santaN] + power * dR[dir];
    int nextC = santaC[santaN] + power * dC[dir];
    if (nextR < 1 || nextC < 1 || nextR > N || nextC > N) { //격자 밖이면 return
        santaR[santaN] = -1;
        santaC[santaN] = -1;
        santaDie[santaN] = true;
        return;
    }
    if (map[nextR][nextC] > 0) { // 다른 산타가 있는곳이면
        int nextSantaN = map[nextR][nextC];
        chainMove(nextSantaN, dir);
    }
    santaR[santaN] = nextR;
    santaC[santaN] = nextC;
    map[nextR][nextC] = santaN;
    // 기절
    // k번째 턴에 '루돌프' 충돌한 산타는 기절 k +1 턴까지는 기절해있고 k+2 턴부터 활동가능
    // 기절산타는 못움직이고, 루돌프는 기절산타한테 돌진가능
    santaStun[santaN] = 2;
    return;
}
// 상호작용 이동
void chainMove(int nextSantaN, int dir) {
    // 기절산타 못 움직이는지 제시문에 안써져있음
    int nextR = santaR[nextSantaN] + dR[dir];
    int nextC = santaC[nextSantaN] + dC[dir];

    if (nextR < 1 || nextC < 1 || nextR > N || nextC > N) { //격자 밖이면
        santaR[nextSantaN] = -1;
        santaC[nextSantaN] = -1;
        santaDie[nextSantaN] = true;
        return;
    }
    if (map[nextR][nextC] > 0) { // 다른 산타가 있는곳이면
        int nextSantaN = map[nextR][nextC];
        chainMove(nextSantaN, dir);
    }
    santaR[nextSantaN] = nextR;
    santaC[nextSantaN] = nextC;
    map[nextR][nextC] = nextSantaN;
    return;
}
// 각 산타가 얻은 최종점수 출력
int main() {
    cin >> N >> M >> P >> dolfPower >> santaPower;
    santaR = vector<int> (P + 1, 0);
    santaC = vector<int>(P + 1, 0);
    santaDir = vector<int>(P + 1, 0);
    santaScore = vector<int>(P + 1, 0);
    santaStun = vector<int>(P + 1, 0);
    santaDie = vector<bool>(P + 1, false);

    cin >> dolfR >> dolfC;

    for (int i = 1; i <= P; i++) {
        // 산타번호는 순서대로 주어지지않음
        int santaN;
        cin >> santaN ;
        cin >> santaR[santaN] >> santaC[santaN];
        map[santaR[santaN]][santaC[santaN]] = santaN;
    }

    for (int t = 0; t < M; t++) {
        // M번 턴 걸쳐서 진행
        // 루돌프 -> 산타 순서로 진행

        moveDolf();
        moveSanta();
        // 산타 다 탈락했으면 게임종료
        // 탈락하지 않은산타는 점수 +1
        bool gameOver = true;
        for (int i = 1; i <= P; i++) {
            if (!santaDie[i]) {
                if (santaStun[i] > 0) santaStun[i]--;
                gameOver = false;
                santaScore[i]++;
            }
        }
        if (gameOver) break;
    }

    for (int i = 1; i <= P; i++) {
        cout << santaScore[i] << " ";
    }
    return 0;
}