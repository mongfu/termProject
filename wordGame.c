#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // usleep 함수 사용

#define MAX_WORD_LENGTH 20
#define MAX_NAME_LENGTH 30
#define MAX_USERS 100

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int score;
} User;

User users[MAX_USERS];
int user_count = 0;

void loadWords(char words[][MAX_WORD_LENGTH], int *word_count);
void loadScores();
void saveScores();
int findUserIndex(const char *name);
void playGame(const char words[][MAX_WORD_LENGTH], int word_count, const char *username, int difficulty);
void displayScores();
void sortScores();
void generateQuestion(char *question, const char *word, int difficulty);
void typingEffect(const char *text);
void printHeader(const char *title);
void printDivider();
void printWelcomeMessage();
void printGameRules();

int main()
{
    char words[50][MAX_WORD_LENGTH];
    int word_count = 0;
    char username[MAX_NAME_LENGTH];

    loadWords(words, &word_count);
    loadScores();

    printWelcomeMessage(); // 환영 메시지 출력
    printGameRules();      // 게임 룰 출력
    typingEffect("이름을 입력해주세요: ");
    scanf("%s", username);

    int user_index = findUserIndex(username);
    if (user_index == -1)
    {
        strcpy(users[user_count].name, username);
        users[user_count].score = 0;
        user_index = user_count++;
    }

    typingEffect("난이도를 선택하세요 ! (1: Easy, 2: Normal, 3: Hard): ");
    int difficulty;
    scanf("%d", &difficulty);

    playGame(words, word_count, username, difficulty);

    saveScores();
    displayScores();

    return 0;
}

void loadWords(char words[][MAX_WORD_LENGTH], int *word_count)
{
    FILE *file = fopen("words.txt", "r");
    if (!file)
    {
        printHeader("ERROR: words.txt 파일을 열 수 없습니다!");
        exit(1);
    }

    while (fscanf(file, "%s", words[*word_count]) != EOF)
    {
        (*word_count)++;
    }

    fclose(file);
}

void loadScores()
{
    FILE *file = fopen("wordScores.txt", "r");
    if (!file)
        return;

    while (fscanf(file, "%s %d", users[user_count].name, &users[user_count].score) != EOF)
    {
        user_count++;
    }

    fclose(file);
}

void saveScores()
{
    FILE *file = fopen("wordScores.txt", "w");
    if (!file)
    {
        printHeader("ERROR: 점수를 저장할 수 없습니다!");
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

void playGame(const char words[][MAX_WORD_LENGTH], int word_count, const char *username, int difficulty)
{
    int score = 0;
    int points_per_question = 0;
    int used_indices[word_count];
    int used_count = 0;

    if (difficulty == 1)
        points_per_question = 10;
    else if (difficulty == 2)
        points_per_question = 20;
    else if (difficulty == 3)
        points_per_question = 30;

    printHeader("게임 시작!");
    typingEffect("총 5문제입니다.\n");
    typingEffect("정답을 맞추고 1위를 쟁취하세요!\n");

    srand(time(NULL));

    for (int i = 0; i < 5; i++)
    {
        int random_index;
        do
        {
            random_index = rand() % word_count;

            int duplicate = 0;
            for (int j = 0; j < used_count; j++)
            {
                if (used_indices[j] == random_index)
                {
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate)
            {
                used_indices[used_count++] = random_index;
                break;
            }
        } while (1);

        printf("\n문제 %d:\n", i + 1);
        printf("\n");

        char question[MAX_WORD_LENGTH];
        generateQuestion(question, words[random_index], difficulty);

        char prompt[50];
        snprintf(prompt, sizeof(prompt), "%s", question);
        typingEffect(prompt);
        typingEffect("\n정답은? : ");
        char answer[MAX_WORD_LENGTH];
        scanf("%s", answer);
        printf("\n");

        if (strcmp(answer, words[random_index]) == 0)
        {
            typingEffect("wow! 정답입니다!\n");
            score += points_per_question;
        }
        else
        {
            char feedback[50];
            snprintf(feedback, sizeof(feedback), "ㅠㅠ틀렸습니다. 정답은 %s 입니다.\n", words[random_index]);
            typingEffect(feedback);
        }
        printDivider();
    }
    printf("\n");

    int user_index = findUserIndex(username);
    if (user_index != -1)
    {
        if (score > users[user_index].score)
        {
            users[user_index].score = score;
            typingEffect("최고 점수가 갱신되었습니다!\n");
        }
    }

    char final_score[50];
    snprintf(final_score, sizeof(final_score), "%s님의 최종 점수: %d\n", username, score);
    typingEffect(final_score);
    printDivider();
}

void displayScores()
{
    sortScores();
    printHeader("Scores");
    for (int i = 0; i < user_count; i++)
    {
        char score_entry[50];
        snprintf(score_entry, sizeof(score_entry), "%d위: %s  %d점", i + 1, users[i].name, users[i].score);
        typingEffect(score_entry);
        typingEffect("\n");
    }
    printDivider();
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

void generateQuestion(char *question, const char *word, int difficulty)
{
    int length = strlen(word);
    strcpy(question, word);

    if (difficulty == 1)
    {
        int hide_index = rand() % length;
        question[hide_index] = '_';
    }
    else if (difficulty == 2)
    {
        int hide_index1 = rand() % length;
        int hide_index2 = rand() % length;
        while (hide_index2 == hide_index1)
        {
            hide_index2 = rand() % length;
        }
        question[hide_index1] = '_';
        question[hide_index2] = '_';
    }
    else if (difficulty == 3)
    {
        for (int i = 0; i < length; i++)
        {
            if (rand() % 2 == 0)
            {
                question[i] = '_';
            }
        }
    }
}

void typingEffect(const char *text)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        printf("%c", text[i]);
        fflush(stdout);
        usleep(30000); // 0.03초 지연
    }
}

void printHeader(const char *title)
{
    int length = strlen(title);
    int totalWidth = 80;                              // 총 80칸 기준
    int padding = (totalWidth - length) / 2;          // 왼쪽 공백 계산
    int rightPadding = totalWidth - length - padding; // 오른쪽 공백 계산

    // 상단 테두리
    printf("\n╔");
    for (int i = 0; i < totalWidth; i++)
        printf("═");
    printf("╗\n");

    // 중앙 정렬된 타이틀 출력 (양쪽 패딩을 정확히 조정)
    printf("║%*s%s%*s║\n", padding, "", title, rightPadding, "");

    // 하단 테두리
    printf("╚");
    for (int i = 0; i < totalWidth; i++)
        printf("═");
    printf("╝\n");
}

void printDivider()
{
    printf("──────────────────────────────────────────\n");
}

void printWelcomeMessage()
{
    printHeader("단어 맞추기 게임에 오신 것을 환영합니다!");
}

void printGameRules()
{
    printHeader("게임 규칙");
    typingEffect("알파벳이 빠져있는 단어를 유추하고 정답을 맞추는 게임입니다\n");
    typingEffect("난이도는 Easy / Normal / Hard 중 선택할 수 있습니다\n");
    typingEffect("난이도가 쉬울수록 정답을 맞출 확률이 높아지지만...\n");
    typingEffect("그만큼 점수는 적게 얻습니다...\n");
    typingEffect("Easy mode: 한 문제 맞힐 때마다 +10점\n");
    typingEffect("Normal mode: 한 문제 맞힐 때마다 +20점\n");
    typingEffect("Hard mode: 한 문제 맞힐 때마다 +30점\n");
    typingEffect("용기 있는 도전으로 1위를 쟁취하세요!\n");
    printDivider();
}
