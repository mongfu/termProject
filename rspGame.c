#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LENGTH 30
#define MAX_USERS 100

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int score;
} User;

// 가위바위보 전역 변수
User rspUsers[MAX_USERS];
int rspUserCount = 0;
const char *rspFilename = "rspScores.txt";

// 공통 함수 선언
void saveScoresToFile(const char *filename, User *users, int user_count);
void loadScoresFromFile(const char *filename, User *users, int *user_count);
int findUserIndex(const char *name, User *users, int user_count);
void displayScores(User *users, int user_count);
void sortScores(User *users, int user_count);

// 가위바위보 전용 함수
void playRockPaperScissors();

int main()
{
    srand(time(NULL)); // 난수 생성 초기화

    // 가위바위보 게임 실행
    playRockPaperScissors();

    return 0;
}

void playRockPaperScissors()
{
    char username[MAX_NAME_LENGTH];
    char playAgain;

    // 점수 로드
    loadScoresFromFile(rspFilename, rspUsers, &rspUserCount);

    // 사용자 이름 입력
    printf("\n[가위바위보 게임]\n");
    printf("사용자 이름을 입력하세요: ");
    scanf("%s", username);

    // 사용자 확인 및 초기화
    int user_index = findUserIndex(username, rspUsers, rspUserCount);
    if (user_index == -1)
    {
        strcpy(rspUsers[rspUserCount].name, username);
        rspUsers[rspUserCount].score = 0;
        user_index = rspUserCount++;
    }

    do
    {
        int userChoice, computerChoice;
        printf("가위(1), 바위(2), 보(3) 중 하나를 선택하세요: ");
        scanf("%d", &userChoice);

        computerChoice = rand() % 3 + 1;
        printf("사용자 선택: %s\n", (userChoice == 1 ? "가위" : (userChoice == 2 ? "바위" : "보")));
        printf("컴퓨터 선택: %s\n", (computerChoice == 1 ? "가위" : (computerChoice == 2 ? "바위" : "보")));

        // 승패 판정
        if (userChoice == computerChoice)
        {
            printf("결과: 무승부!\n");
        }
        else if ((userChoice == 1 && computerChoice == 3) ||
                 (userChoice == 2 && computerChoice == 1) ||
                 (userChoice == 3 && computerChoice == 2))
        {
            printf("결과: 승리!\n");
            rspUsers[user_index].score++;
        }
        else
        {
            printf("결과: 패배!\n");
        }

        printf("현재 점수 - 사용자: %d점\n", rspUsers[user_index].score);

        // 게임 반복 여부
        printf("게임을 계속하시겠습니까? (y/n): ");
        scanf(" %c", &playAgain);

    } while (playAgain == 'y' || playAgain == 'Y');

    // 점수 저장
    saveScoresToFile(rspFilename, rspUsers, rspUserCount);

    // 점수 출력
    displayScores(rspUsers, rspUserCount);
}

void saveScoresToFile(const char *filename, User *users, int user_count)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("점수를 저장하는 데 실패했습니다.\n");
        return;
    }

    for (int i = 0; i < user_count; i++)
    {
        fprintf(file, "%s %d\n", users[i].name, users[i].score);
    }

    fclose(file);
}

void loadScoresFromFile(const char *filename, User *users, int *user_count)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return;

    *user_count = 0;
    while (fscanf(file, "%s %d", users[*user_count].name, &users[*user_count].score) != EOF)
    {
        (*user_count)++;
    }

    fclose(file);
}

int findUserIndex(const char *name, User *users, int user_count)
{
    for (int i = 0; i < user_count; i++)
    {
        if (strcmp(users[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
}

void displayScores(User *users, int user_count)
{
    sortScores(users, user_count);
    printf("\n==== 모든 유저의 점수 ====\n");
    for (int i = 0; i < user_count; i++)
    {
        printf("%d위: %s - %d점\n", i + 1, users[i].name, users[i].score);
    }
    printf("=========================\n");
}

void sortScores(User *users, int user_count)
{
    for (int i = 0; i < user_count - 1; i++)
    {
        for (int j = i + 1; j < user_count; j++)
        {
            if (users[i].score < users[j].score)
            {
                User temp = users[i];
                users[i] = users[j];
                users[j] = temp;
            }
        }
    }
}
