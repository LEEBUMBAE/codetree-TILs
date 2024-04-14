#include <iostream>
#include <vector>
#include <queue>

/*빠트렸던 부분은 이런식으로 기록해뒀음*/
using namespace std;

// 클래스 선언
class Tower {
public:
    int r, c, t, p, br, bc; // r:행, c:열, t:최근공격 p:공격력  br, bc: 직전위치 
    
    Tower() { // 기본 생성자
        this->r = -1;
        this->c = -1;
        this->t = 0;
        this->p = 5001;
        this->br = -1;
        this->bc = -1;
    }
    Tower(int r, int c, int t, int p, int br, int bc) { // 생성자
        this->r = r;
        this->c = c;
        this->t = t;
        this->p = p;
        this->br = br;
        this->bc = bc;
    }

};
 
// 변수 선언
int N, M, K; // 행 열 턴
vector<vector<Tower>> map; // 맵
vector<vector<bool>> route; // 공격자, 피격자, 공격 경로에 있던 놈들을 구분해서 4.포탑 정비에서 제외하려고
int dr[4] = { 0, 1, 0, -1 }; // 우 하 좌 상
int dc[4] = { 1, 0, -1, 0 };
int dr8[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int dc8[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

// 함수 선언

// 공격자 선정
Tower pickAttacker(int turn){
    Tower attacker; // 선정된 공격자

    for (int i = 0; i < N; i++){
        for (int j = 0; j < M; j++){
            if ((map[i][j].p != 0) && (attacker.p >= map[i][j].p)){ // 부서진 포탑이 아니고, 공격력 보다 낮거나 같으면
                if (attacker.p == map[i][j].p){ // 공격력이 같다면
                    if (attacker.t <= map[i][j].t){ // 최근공격이 더 최근이거나 같다면
                        if (attacker.t == map[i][j].t) { // 최근 공격 같다면
                            if ((attacker.r + attacker.c) <= (map[i][j].r + map[i][j].c)) { // r+c가 크거나 같으면
                                if ((attacker.r + attacker.c) == (map[i][j].r + map[i][j].c)) { // r+c가 같다면
                                    if (attacker.c < map[i][j].c) {
                                        attacker = map[i][j]; // c가 큰 포탑이 선정됨
                                    }
                                }else attacker = map[i][j]; // r+c가 큰 공격포탑이 선정됨   
                            }
                        }else attacker = map[i][j]; // 최근 공격포탑이 선정됨
                    }
                }else attacker = map[i][j]; // 공격력 낮은놈이 선정됨
            }
        }
    }
    map[attacker.r][attacker.c].t = turn; // 최근 공격턴 갱신 /* 빠트렸던 부분 */
    map[attacker.r][attacker.c].p += N + M; // 공격력 증가
    attacker.p = map[attacker.r][attacker.c].p; // 공격력 증가
    route[attacker.r][attacker.c] = true; // 공격자 포탑 정비에서 제외
    return attacker;
}

// 피격자 선정
Tower pickIsAttacked(Tower& attacker,int turn) {
    Tower isAttacked(-1,-1,0,-1,-1,-1); // 피격자

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if(!((attacker.r == i) && (attacker.c == j))) { // 공격자는 제외
                if((map[i][j].p != 0) && (isAttacked.p <= map[i][j].p)) { // 부서진 포탑이 아니고, 공격력 보다 크거나 같으면
                    if(isAttacked.p == map[i][j].p) { // 공격력이 같다면
                        if(isAttacked.t >= map[i][j].t) { // 최근공격이 더 오래되거나 같다면
                            if(isAttacked.t == map[i][j].t) { // 최근 공격 같다면
                                if((isAttacked.r + isAttacked.c) >= (map[i][j].r + map[i][j].c)) { // r+c가 작거나 같으면
                                    if((isAttacked.r + isAttacked.c) == (map[i][j].r + map[i][j].c)) { // r+c가 같다면
                                        if(isAttacked.c > map[i][j].c) { // c가 작다면
                                            isAttacked = map[i][j];
                                        }
                                    }else isAttacked = map[i][j]; // r+c가 작다면
                                }
                            }else isAttacked = map[i][j]; // 최근 공격 오래됐다면
                        }
                    }else isAttacked = map[i][j]; // 공격력 더 크다면
                }
            }
        }    
    }

    route[isAttacked.r][isAttacked.c] = true; // 피격자 4. 포탑정비에서 제외
    return isAttacked;
}

// 레이저 공격
bool lazerAttack(Tower& attacker, Tower& isAttacked) { 
    vector<vector<bool>> visited(N, vector<bool>(M, false)); // 방문여부 저장할 곳
    queue<Tower> q; // bfs할 q
    bool canLazerAttack = false; // 레이저 공격 가능여부

    visited[attacker.r][attacker.c] = true;
    q.push(Tower(map[attacker.r][attacker.c])); // 출발점

    while (!q.empty()) { // q에 남은 놈이 없을때 까지 반복
        Tower cur = q.front(); // q에서 한놈 꺼내
        q.pop(); // 지워줌

        if (cur.r == isAttacked.r && cur.c == isAttacked.c) { // 피격자에 도달하면
            canLazerAttack = true; // 레이저 어택 가능 상태
            break;
        }

        for (int i = 0; i < 4; i++) { // cur 기준 사방 탐색
            int nextR = (N + cur.r + dr[i]) % N;
            int nextC = (M + cur.c + dc[i]) % M;

            if (map[nextR][nextC].p != 0 && !visited[nextR][nextC]) { // 부서진곳 아니고, 방문 안 한곳이면
                visited[nextR][nextC] = true; // 방문
                map[nextR][nextC].br = cur.r; // 직전 위치 기록
                map[nextR][nextC].bc = cur.c; // 직전 위치 기록
                q.push(Tower(map[nextR][nextC])); 
            }
        }
    }

    if (canLazerAttack) { // 레이저 공격 가능하면 공격
        map[isAttacked.r][isAttacked.c].p -= map[attacker.r][attacker.c].p; // 피격자 공격
        int beforeR = map[isAttacked.r][isAttacked.c].br; // 직전 경로
        int beforeC = map[isAttacked.r][isAttacked.c].bc; // 직전 경로
        while (true) { // 경로를 역주행하면서 지나왔던길 포탑 공격
            if (beforeR == attacker.r && beforeC == attacker.c) break; // 직전 타워가 attacker면 탈출
            map[beforeR][beforeC].p -= map[attacker.r][attacker.c].p / 2; // 경로 공격
            route[beforeR][beforeC] = true; // 경로 4. 포탑 정비에서 제외
            int tempR = map[beforeR][beforeC].br; // 직전 경로
            int tempC = map[beforeR][beforeC].bc; // 직전 경로
            beforeR = tempR;
            beforeC = tempC;
        }
        return true;
    } else { return false; }
}

// 폭탄 공격
void bombAttack(Tower& attacker, Tower& isAttacked) { 
    map[isAttacked.r][isAttacked.c].p -= map[attacker.r][attacker.c].p; // 피격자는 공격자만큼 파워 감소
    for (int i = 0; i < 8; i++) { // 8방 공격
        int nextR = (N + isAttacked.r + dr8[i]) % N; /*% 해줘야했던거 빠트렸던 부분 */
        int nextC = (M + isAttacked.c + dc8[i]) % M; /*% 해줘야했던거 빠트렸던 부분 */

        if (attacker.r == nextR && attacker.c == nextC) continue; // 공격자는 공격 안 받음

        map[nextR][nextC].p -= map[attacker.r][attacker.c].p / 2; // 그 외는 공격 받음
        route[nextR][nextC] = true; // 4. 포탑정비에서 제외
    }
}

// 포탑 부서짐
void breakTower() { 
    for (int i = 0; i < N; i++) { // 공격력 0 이하된 포탑은 부서짐
        for (int j = 0; j < M; j++) {
            if (map[i][j].p <= 0) map[i][j].p = 0;
        }
    }
}

// 포탑 정비
int repairTower(int cnt) {
    for (int i = 0; i < N; i++) { // 포탑 정비
        for (int j = 0; j < M; j++) {
            if (map[i][j].p != 0 && route[i][j] == false) { 
                map[i][j].p += 1; 
            }
            if (map[i][j].p > 0) { // 안 부서졌으면 카운팅
                cnt++;
           }
        }
    }
    return cnt;
}

// 남은 놈중 가장 강한놈
int whoIsWinner() {
    int kingPower = 0;
    for (int i = 0; i < N; i++) { 
        for (int j = 0; j < M; j++) {
            if (map[i][j].p > kingPower) kingPower = map[i][j].p;
        }
    }
    return kingPower;
}

int main() {
    int ans = 0; // 정답
    cin >> N >> M >> K; // 행 열 턴 입력
    map = vector<vector<Tower>>(N, vector<Tower>(M)); // 맵의 크기가 정해지면 할당

    // 맵에 입력 받기
    for (int i = 0; i < N; i++) { 
        for (int j = 0; j < M; j++) {
            cin >> map[i][j].p; // 파워 입력
            map[i][j].r = i; // 행 위치 저장
            map[i][j].c = j; // 열 위치 저장

        }
    }

    //액션 수행 K번 턴 반복
    for (int i = 0; i < K; i++) {
        route = vector<vector<bool>>(N, vector<bool>(M, false)); // 포탑 정비 제외할놈 
        int cnt = 0; // 살아 있는 포탑수

        Tower attacker = pickAttacker(i + 1); // 1.공격자 선정 
        Tower isAttacked = pickIsAttacked(attacker, i + 1); // 2.피격자 선정
        if (!lazerAttack(attacker, isAttacked)) { // 2. 공격
            bombAttack(attacker, isAttacked);
        }
        breakTower(); // 3. 포탑 부서짐
        cnt = repairTower(cnt); // 4. 포탑 정비

        if (cnt == 1) { // 살아있는 포탑이 1개가 되면 그 즉시 중지
            ans = whoIsWinner();
            break;
        }
        if (i == K - 1) { // 마지막 턴엔 
            ans = whoIsWinner(); // 남아 있는 놈중 가장 강한놈 선정
        }
    }

    cout << ans;

    return 0;
}