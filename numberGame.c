#define _CRT_SECURE_NO_WARNINGS
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

User users[MAX_USERS];
int user_count = 0;

void loadScores();
void saveScores();
int findUserIndex(const char *name);
void playGame(const char *username);
void displayScores();
void sortScores();

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main()
{
    srand((unsigned int)time(NULL));

    loadScores();

    char username[MAX_NAME_LENGTH];
    printf("사용자 이름을 입력하세요: ");
    scanf("%s", username);

    int user_index = findUserIndex(username);
    if (user_index == -1)
    {
        strcpy(users[user_count].name, username);
        users[user_count].score = 0;
        user_count++;
    }

    playGame(username);

    saveScores();
    displayScores();

    return 0;
}

void loadScores()
{
    FILE *file = fopen("numberScores.txt", "r");
    if (!file)
    {
        return;
    }

    while (fscanf(file, "%s %d", users[user_count].name, &users[user_count].score) != EOF)
    {
        user_count++;
    }

    fclose(file);
}

void saveScores()
{
    FILE *file = fopen("scores.txt", "w");
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

int findUserIndex(const char *name)
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

void playGame(const char *username)
{
    int difficulty, max_range, attempts_allowed, answer, attempts = 0;
    int user_index = findUserIndex(username);

    printf("난이도를 선택하세요:\n");
    printf("1: 1부터 99까지\n");
    printf("2: 1부터 999까지\n");
    printf("3: 1부터 9999까지\n");
    printf("난이도 선택 (1, 2, 3): ");
    while (scanf("%d", &difficulty) != 1 || difficulty < 1 || difficulty > 3)
    {
        printf("잘못된 입력입니다. 1, 2, 3 중 하나를 선택하세요: ");
        clear_input_buffer();
    }

    max_range = (difficulty == 1) ? 99 : (difficulty == 2) ? 999
                                                           : 9999;
    attempts_allowed = (difficulty == 1) ? 5 : (difficulty == 2) ? 10
                                                                 : 20;
    answer = rand() % max_range + 1;

    printf("숫자를 맞춰보세요! (1부터 %d까지)\n", max_range);

    while (attempts < attempts_allowed)
    {
        int guess;
        printf("%d번째 시도: ", attempts + 1);
        if (scanf("%d", &guess) != 1 || guess < 1 || guess > max_range)
        {
            printf("잘못된 입력입니다. 1부터 %d 사이의 숫자를 입력하세요.\n", max_range);
            clear_input_buffer();
            continue;
        }

        attempts++;
        if (guess == answer)
        {
            printf("정답입니다!\n");
            int score = (attempts_allowed - attempts + 1) * 10;
            if (score > users[user_index].score)
            {
                users[user_index].score = score;
                printf("축하합니다! 최고 점수가 갱신되었습니다: %d점\n", score);
            }
            else
            {
                printf("현재 최고 점수: %d점\n", users[user_index].score);
            }
            return;
        }
        else if (guess < answer)
        {
            printf("UP\n");
        }
        else
        {
            printf("DOWN\n");
        }
    }

    printf("기회를 모두 사용했습니다. 정답은 %d였습니다.\n", answer);
}

void displayScores()
{
    sortScores();
    printf("\n======== Scores ========\n");
    for (int i = 0; i < user_count; i++)
    {
        printf("%d위: %s - %d점\n", i + 1, users[i].name, users[i].score);
    }
    printf("=========================\n");
}

void sortScores()
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
