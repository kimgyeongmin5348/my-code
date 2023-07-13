#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { //유닛 구조체
    int id;
    int x;
    int y;
    int hp;
    char name[20];
} Unit;

typedef struct unit_node {
    Unit* unit;
    struct unit_node* next;
} UnitNode;

typedef struct {
    UnitNode* head;
    UnitNode* tail;
} UnitList;

UnitList* UL_SL;

char mini_map[20][40];

void Display() {
    int i, j;
    for (j = 0; j < 20; j++) {
        for (i = 0; i < 40; i++) {
            printf("%c ", mini_map[j][i]);
        }
        printf("\n");
    }
}

void ShowUL_SL(UnitList* unitList) {
    UnitNode* current = unitList->head;
    while (current != NULL) {
        printf("ID: %d, Name: %s, X: %d, Y: %d, HP: %d\n", current->unit->id, current->unit->name, current->unit->x, current->unit->y, current->unit->hp);
        current = current->next;
    }
}

void SelectUL_SL(UnitList* unitList, int x1, int y1, int x2, int y2) {
    UnitNode* current = unitList->head;
    while (current != NULL) {
        if (current->unit->x >= x1 && current->unit->x <= x2 && current->unit->y >= y1 && current->unit->y <= y2) {
            printf("ID: %d, Name: %s, X: %d, Y: %d, HP: %d\n", current->unit->id, current->unit->name, current->unit->x, current->unit->y, current->unit->hp);
        }
        current = current->next;
    }
}

void ProduceUL_SL(UnitList* unitList) {
    Unit* newUnit = (Unit*)malloc(sizeof(Unit));
    int idInput;

    while (1) {
        printf("유닛 아이디 생성(21부터 시작) : ");
        scanf("%d", &idInput);
        // 입력받은 id가 이미 존재하는지 확인
        UnitNode* current = unitList->head;
        int duplicate = 0;
        while (current != NULL) {
            if (current->unit->id == idInput) {
                duplicate = 1;
                printf("중복된 아이디입니다. 다시 입력해주세요.\n");
                break;
            }
            current = current->next;
        }
        if (!duplicate) break;
    }

    newUnit->id = idInput;
    printf("유닛 이름을 생성해 주세요 : ");
    scanf("%s", newUnit->name);
    printf("유닛의 x 좌표를 입력해 주세요 : ");
    scanf("%d", &newUnit->x);
    printf("유닛의 y 좌표를 입력해 주세요 : ");
    scanf("%d", &newUnit->y);

    // 좌표가 유효한지 검사
    if (mini_map[newUnit->y][newUnit->x] != '-') {
        printf("해당 좌표에 이미 유닛이 존재합니다. 다른 위치를 선택해주세요.\n");
        free(newUnit);
        return;
    }


    printf("유닛의 HP를 입력해 주세요 : ");
    scanf("%d", &newUnit->hp);

    // 유닛 리스트에 새로운 유닛 추가.
    UnitNode* newNode = (UnitNode*)malloc(sizeof(UnitNode));
    newNode->unit = newUnit;
    newNode->next = NULL;
    if (unitList->head == NULL) {
        unitList->head = newNode;
        unitList->tail = newNode;
    }
    else {
        unitList->tail->next = newNode;
        unitList->tail = newNode;
    }

    // 미니맵 업데이트
    mini_map[newUnit->y][newUnit->x] = newUnit->name[0];
}

void DestroyUL_SL(UnitList* unitList, int x1, int y1, int x2, int y2) {
    // 입력된 x1, y1, x2, y2 중에서 가장 작은 좌표를 min_x, min_y에 저장
    // 가장 큰 좌표는 max_x, max_y에 저장
    int min_x = x1 < x2 ? x1 : x2;
    int max_x = x1 > x2 ? x1 : x2;
    int min_y = y1 < y2 ? y1 : y2;
    int max_y = y1 > y2 ? y1 : y2;

    // 리스트의 첫 번째 노드부터 마지막 노드까지 반복
    UnitNode* current = unitList->head;
    UnitNode* prev = NULL;
    while (current != NULL) {
        // 현재 노드의 좌표가 범위 내에 있는지 확인
        int x = current->unit->x;
        int y = current->unit->y;
        if (x >= min_x && x <= max_x && y >= min_y && y <= max_y) {
            // 미니맵에서 해당 유닛의 위치를 삭제('.'으로 교체)
            mini_map[y][x] = '.';

            // 현재 노드를 리스트에서 삭제
            if (prev == NULL) {  // 첫 번째 노드를 삭제할 경우
                unitList->head = current->next;
            }
            else if (current->next == NULL) {  // 마지막 노드를 삭제할 경우
                unitList->tail = prev;
                prev->next = NULL;
            }
            else {  // 중간 노드를 삭제할 경우
                prev->next = current->next;
            }

            // 삭제된 노드의 메모리를 해제
            free(current->unit);
            free(current);

            // 같은 좌표에 위치한 다른 유닛도 삭제할 수 있도록 current를 이전 노드로 바꾼다
            current = prev;
        }

        // 다음 노드로 이동합니다.
        prev = current;
        current = current->next;
    }

    // 삭제된 유닛들의 좌표 범위를 출력합니다.
    printf("============================= 유닛이 삭제 되었습니다. ===============================");
}

int main() {
    int input;
    UL_SL = (UnitList*)malloc(sizeof(UnitList));
    UL_SL->head = NULL;
    UL_SL->tail = NULL;
    memset(mini_map, '.', sizeof(mini_map));

   
    Unit* unit1 = (Unit*)malloc(sizeof(Unit));
    unit1->id = 1;
    unit1->x = 0;
    unit1->y = 0;
    unit1->hp = 25 ;
    strcpy(unit1->name, "M");
    UnitNode* node1 = (UnitNode*)malloc(sizeof(UnitNode));
    node1->unit = unit1;
    node1->next = NULL;

    Unit* unit2 = (Unit*)malloc(sizeof(Unit));
    unit2->id = 2;
    unit2->x = 0;
    unit2->y = 1;
    unit2->hp = 20;
    strcpy(unit2->name, "M");
    UnitNode* node2 = (UnitNode*)malloc(sizeof(UnitNode));
    node2->unit = unit2;
    node2->next = NULL;

    Unit* unit3 = (Unit*)malloc(sizeof(Unit));
    unit3->id = 3;
    unit3->x = 0;
    unit3->y = 2;
    unit3->hp = 30;
    strcpy(unit3->name, "M");
    UnitNode* node3 = (UnitNode*)malloc(sizeof(UnitNode));
    node3->unit = unit3;
    node3->next = NULL;

    Unit* unit4 = (Unit*)malloc(sizeof(Unit));
    unit4->id = 4;
    unit4->x = 1;
    unit4->y = 2;
    unit4->hp = 80;
    strcpy(unit4->name, "V");
    UnitNode* node4 = (UnitNode*)malloc(sizeof(UnitNode));
    node4->unit = unit4;
    node4->next = NULL;

    Unit* unit5 = (Unit*)malloc(sizeof(Unit));
    unit5->id = 5;
    unit5->x = 2;
    unit5->y = 2;
    unit5->hp = 75;
    strcpy(unit5->name, "V");
    UnitNode* node5 = (UnitNode*)malloc(sizeof(UnitNode));
    node5->unit = unit5;
    node5->next = NULL;

    Unit* unit6 = (Unit*)malloc(sizeof(Unit));
    unit6->id = 6;
    unit6->x = 0;
    unit6->y = 3;
    unit6->hp = 120;
    strcpy(unit6->name, "T");
    UnitNode* node6 = (UnitNode*)malloc(sizeof(UnitNode));
    node6->unit = unit6;
    node6->next = NULL;

    Unit* unit7 = (Unit*)malloc(sizeof(Unit));
    unit7->id = 7;
    unit7->x = 1;
    unit7->y = 3;
    unit7->hp = 15;
    strcpy(unit7->name, "M");
    UnitNode* node7 = (UnitNode*)malloc(sizeof(UnitNode));
    node7->unit = unit7;
    node7->next = NULL;

    Unit* unit8 = (Unit*)malloc(sizeof(Unit));
    unit8->id = 8;
    unit8->x = 0;
    unit8->y = 4;
    unit8->hp = 50;
    strcpy(unit8->name, "T");
    UnitNode* node8 = (UnitNode*)malloc(sizeof(UnitNode));
    node8->unit = unit8;
    node8->next = NULL;

    Unit* unit9 = (Unit*)malloc(sizeof(Unit));
    unit9->id = 9;
    unit9->x = 1;
    unit9->y = 4;
    unit9->hp = 30;
    strcpy(unit9->name, "M");
    UnitNode* node9 = (UnitNode*)malloc(sizeof(UnitNode));
    node9->unit = unit9;
    node9->next = NULL;

    Unit* unit10 = (Unit*)malloc(sizeof(Unit));
    unit10->id = 10;
    unit10->x = 0;
    unit10->y = 5;
    unit10->hp = 27;
    strcpy(unit10->name, "M");
    UnitNode* node10 = (UnitNode*)malloc(sizeof(UnitNode));
    node10->unit = unit10;
    node10->next = NULL;

    Unit* unit11 = (Unit*)malloc(sizeof(Unit));
    unit11->id = 11;
    unit11->x = 39;
    unit11->y = 19;
    unit11->hp = 37;
    strcpy(unit11->name, "H");
    UnitNode* node11 = (UnitNode*)malloc(sizeof(UnitNode));
    node11->unit = unit11;
    node11->next = NULL;

    Unit* unit12 = (Unit*)malloc(sizeof(Unit));
    unit12->id = 12;
    unit12->x = 39;
    unit12->y = 18;
    unit12->hp = 47;
    strcpy(unit12->name, "H");
    UnitNode* node12 = (UnitNode*)malloc(sizeof(UnitNode));
    node12->unit = unit12;
    node12->next = NULL;

    Unit* unit13 = (Unit*)malloc(sizeof(Unit));
    unit13->id = 13;
    unit13->x = 39;
    unit13->y = 17;
    unit13->hp = 35;
    strcpy(unit13->name, "Q");
    UnitNode* node13 = (UnitNode*)malloc(sizeof(UnitNode));
    node13->unit = unit13;
    node13->next = NULL;

    Unit* unit14 = (Unit*)malloc(sizeof(Unit));
    unit14->id = 14;
    unit14->x = 38;
    unit14->y = 17;
    unit14->hp = 70;
    strcpy(unit14->name, "Q");
    UnitNode* node14 = (UnitNode*)malloc(sizeof(UnitNode));
    node14->unit = unit14;
    node14->next = NULL;

    Unit* unit15 = (Unit*)malloc(sizeof(Unit));
    unit15->id = 15;
    unit15->x = 37;
    unit15->y = 17;
    unit15->hp = 30;
    strcpy(unit15->name, "H");
    UnitNode* node15 = (UnitNode*)malloc(sizeof(UnitNode));
    node15->unit = unit15;
    node15->next = NULL;

    Unit* unit16 = (Unit*)malloc(sizeof(Unit));
    unit16->id = 16;
    unit16->x = 39;
    unit16->y = 16;
    unit16->hp = 15;
    strcpy(unit16->name, "D");
    UnitNode* node16 = (UnitNode*)malloc(sizeof(UnitNode));
    node16->unit = unit16;
    node16->next = NULL;

    Unit* unit17 = (Unit*)malloc(sizeof(Unit));
    unit17->id = 17;
    unit17->x = 38;
    unit17->y = 16;
    unit17->hp = 20;
    strcpy(unit17->name, "H");
    UnitNode* node17 = (UnitNode*)malloc(sizeof(UnitNode));
    node17->unit = unit17;
    node17->next = NULL;

    Unit* unit18 = (Unit*)malloc(sizeof(Unit));
    unit18->id = 18;
    unit18->x = 39;
    unit18->y = 15;
    unit18->hp = 40;
    strcpy(unit18->name, "D");
    UnitNode* node18 = (UnitNode*)malloc(sizeof(UnitNode));
    node18->unit = unit18;
    node18->next = NULL;

    Unit* unit19 = (Unit*)malloc(sizeof(Unit));
    unit19->id = 19;
    unit19->x = 38;
    unit19->y = 15;
    unit19->hp = 45;
    strcpy(unit19->name, "H");
    UnitNode* node19 = (UnitNode*)malloc(sizeof(UnitNode));
    node19->unit = unit19;
    node19->next = NULL;

    Unit* unit20 = (Unit*)malloc(sizeof(Unit));
    unit20->id = 20;
    unit20->x = 39;
    unit20->y = 14;
    unit20->hp = 50;
    strcpy(unit20->name, "H");
    UnitNode* node20 = (UnitNode*)malloc(sizeof(UnitNode));
    node20->unit = unit20;
    node20->next = NULL;


    UL_SL->head = node1;
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
    node4->next = node5;
    node5->next = node6;
    node6->next = node7;
    node7->next = node8;
    node8->next = node9;
    node9->next = node10;
    node10->next = node11;
    node11->next = node12;
    node12->next = node13;
    node13->next = node14;
    node14->next = node15;
    node15->next = node16;
    node16->next = node17;
    node17->next = node18;
    node18->next = node19;
    node19->next = node20;
    UL_SL->tail = node20;


    mini_map[unit1->y][unit1->x] = unit1->name[0];
    mini_map[unit2->y][unit2->x] = unit2->name[0];
    mini_map[unit3->y][unit3->x] = unit3->name[0];
    mini_map[unit4->y][unit4->x] = unit4->name[0];
    mini_map[unit5->y][unit5->x] = unit5->name[0];
    mini_map[unit6->y][unit6->x] = unit6->name[0];
    mini_map[unit7->y][unit7->x] = unit7->name[0];
    mini_map[unit8->y][unit8->x] = unit8->name[0];
    mini_map[unit9->y][unit9->x] = unit9->name[0];
    mini_map[unit10->y][unit10->x] = unit10->name[0];
    mini_map[unit11->y][unit11->x] = unit11->name[0];
    mini_map[unit12->y][unit12->x] = unit12->name[0];
    mini_map[unit13->y][unit13->x] = unit13->name[0];
    mini_map[unit14->y][unit14->x] = unit14->name[0];
    mini_map[unit15->y][unit15->x] = unit15->name[0];
    mini_map[unit16->y][unit16->x] = unit16->name[0];
    mini_map[unit17->y][unit17->x] = unit17->name[0];
    mini_map[unit18->y][unit18->x] = unit18->name[0];
    mini_map[unit19->y][unit19->x] = unit19->name[0];
    mini_map[unit20->y][unit20->x] = unit20->name[0];


    while (1) {
        printf("========= 선택 사항 =========\n");
        printf("1. 모든 유닛을 순서대로 출력\n");
        printf("2. 주어진 좌표안에 있는 유닛 출력\n");
        printf("3. 새로운 유닛 삽입\n");
        printf("4. 미니맵 출력\n");
        printf("5. 지정 범위 유닛 삭제\n");
        printf("6. 종료\n");
        printf("번호를 눌러주세요: ");
        scanf("%d", &input);
        switch (input) {
        case 1:
            printf("\n모든 유닛 :\n");
            ShowUL_SL(UL_SL);
            printf("\n");
            break;
        case 2: {
            int x1, y1, x2, y2;
            printf("\n범위를 지정할 x,y 좌표 두곳을 입력해 주세요 : ");
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            printf("\n");
            SelectUL_SL(UL_SL, x1, y1, x2, y2);
            printf("\n");
            break;
        }
        case 3:
            printf("\n== 새로운 유닛 생성 ==\n");
            ProduceUL_SL(UL_SL);
            break;
        case 4:
            printf("\n=========== 미니맵 ============\n");
            Display();
            printf("\n");
            break;
        case 5: {
            int x1, y1, x2, y2;
            printf("\n범위를 지정할 x,y,좌표 두곳을 입력해 주세요 : ");
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
            printf("\n");
            DestroyUL_SL(UL_SL, x1, y2, x2, y2);
            printf("\n");
            break;
            }
        case 6:
            printf("\n프로그램 종료.\n");
            exit(0);
        default:
            printf("\n잘못된 값 입니다. 다시 눌러주세요. \n\n");
        }
    }

    return 0;
}