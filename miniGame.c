#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_WORD_LENGTH 20
#define MAX_NAME_LENGTH 30
#define MAX_USERS 100

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int score;
} User;

void saveScoresToFile(const char *filename);
void loadScoresFromFile(const char *filename);
void printWelcomeMessage();
void printGameRules();
void playGame();

User users[MAX_USERS];
int user_count = 0;

void playRockPaperScissors();
void playNumberGuessGame();
void playWordGuessGame();
void loadWords(char words[][MAX_WORD_LENGTH], int *word_count);
void loadScores();
void saveScores();
int findUserIndex(const char *name);
void generateQuestion(char *question, const char *word, int difficulty);
void typingEffect(const char *text);
void printHeader(const char *title);
void printDivider();
void displayScores();
void sortScores();

int main()
{
    int choice;

    printf("\n======== 미니게임 선택 ========\n");
    printf("1. 가위바위보 게임\n");
    printf("2. 숫자 맞추기 게임\n");
    printf("3. 단어 맞추기 게임\n");
    printf("선택하세요: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        playRockPaperScissors();
        break;
    case 2:
        playNumberGuessGame();
        break;
    case 3:
        playWordGuessGame();
        break;
    default:
        printf("잘못된 입력입니다. 1-3 사이의 숫자를 입력하세요.\n");
        break;
    }

    printf("게임을 종료합니다. 플레이 해주셔서 감사합니다!\n");
    return 0;
}

void playRockPaperScissors()
{
    const char *filename = "rspScores.txt";
    loadScoresFromFile(filename);

    char username[MAX_NAME_LENGTH];
    printf("\n[가위바위보 게임]\n");
    printf("이름을 입력해주세요: ");
    scanf("%s", username);

    int user_index = findUserIndex(username);
    if (user_index == -1)
    {
        strcpy(users[user_count].name, username);
        users[user_count].score = 0;
        user_index = user_count++;
    }

    const char *choices[] = {"가위", "바위", "보"};
    int userScore = 0, computerScore = 0, rounds = 0;
    char playAgain;
    srand(time(NULL));

    do
    {
        int userChoice, computerChoice;
        printf("가위(1), 바위(2), 보(3) 중 하나를 선택하세요: ");
        scanf("%d", &userChoice);
        computerChoice = (rand() % 3) + 1;

        printf("사용자 선택: %s\n", choices[userChoice - 1]);
        printf("컴퓨터 선택: %s\n", choices[computerChoice - 1]);

        if (userChoice == computerChoice)
        {
            printf("결과: 무승부!\n");
        }
        else if ((userChoice == 1 && computerChoice == 3) ||
                 (userChoice == 2 && computerChoice == 1) ||
                 (userChoice == 3 && computerChoice == 2))
        {
            printf("결과: 승리!\n");
            userScore++;
        }
        else
        {
            printf("결과: 패배!\n");
            computerScore++;
        }

        rounds++;
        printf("현재 점수 - 사용자: %d, 컴퓨터: %d (총 %d 라운드)\n", userScore, computerScore, rounds);

        printf("게임을 계속하시겠습니까? (y/n): ");
        scanf(" %c", &playAgain);

    } while (playAgain == 'y' || playAgain == 'Y');

    printf("최종 점수 - 사용자: %d, 컴퓨터: %d\n", userScore, computerScore);

    if (userScore > users[user_index].score)
    {
        users[user_index].score = userScore;
        printf("최고 점수가 갱신되었습니다!\n");
    }

    saveScoresToFile(filename);
    displayScores();
}

void playNumberGuessGame()
{
    const char *filename = "numberScores.txt";
    loadScoresFromFile(filename);

    char username[MAX_NAME_LENGTH];
    printf("\n[숫자 맞추기 게임]\n");
    printf("이름을 입력해주세요: ");
    scanf("%s", username);

    int user_index = findUserIndex(username);
    if (user_index == -1)
    {
        strcpy(users[user_count].name, username);
        users[user_count].score = 0;
        user_index = user_count++;
    }

    int difficulty, attempts_allowed, answer, attempts = 0;
    srand(time(NULL));

    printf("난이도를 선택하세요 (1: 1~99, 2: 1~999, 3: 1~9999): ");
    scanf("%d", &difficulty);

    int max_range = (difficulty == 1) ? 99 : (difficulty == 2) ? 999
                                                               : 9999;
    attempts_allowed = (difficulty == 1) ? 5 : (difficulty == 2) ? 10
                                                                 : 20;
    answer = rand() % max_range + 1;

    printf("숫자를 맞춰보세요! (1부터 %d까지)\n", max_range);

    int score = 0;

    while (attempts < attempts_allowed)
    {
        int guess;
        printf("%d번째 시도: ", attempts + 1);
        scanf("%d", &guess);
        attempts++;

        if (guess == answer)
        {
            printf("정답입니다!\n");
            score = attempts_allowed - attempts + 1;
            break;
        }
        else if (guess < answer)
        {
            printf("up\n");
        }
        else
        {
            printf("down\n");
        }
    }

    if (score == 0)
    {
        printf("기회를 모두 사용했습니다. 정답은 %d였습니다.\n", answer);
    }

    if (score > users[user_index].score)
    {
        users[user_index].score = score;
        printf("최고 점수가 갱신되었습니다!\n");
    }

    saveScoresToFile(filename);
    displayScores();
}

void playWordGuessGame()
{
    char words[50][MAX_WORD_LENGTH];
    int word_count = 0;
    char username[MAX_NAME_LENGTH];

    loadWords(words, &word_count);
    loadScores();

    printWelcomeMessage();
    printGameRules();
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
        usleep(30000);
    }
}

void printHeader(const char *title)
{
    int length = strlen(title);
    int totalWidth = 80;
    int padding = (totalWidth - length) / 2;
    int rightPadding = totalWidth - length - padding;

    printf("\n╔");
    for (int i = 0; i < totalWidth; i++)
        printf("═");
    printf("╗\n");

    printf("║%*s%s%*s║\n", padding, "", title, rightPadding, "");

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

void printGameRules(void)
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

void saveScoresToFile(const char *filename)
{
    FILE *file = fopen(filename, "w");
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

void loadScoresFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return;

    user_count = 0;
    while (fscanf(file, "%s %d", users[user_count].name, &users[user_count].score) != EOF)
    {
        user_count++;
    }

    fclose(file);
}
