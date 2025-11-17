#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "leaderboard.h"
#include "utils.h"

#define LEADER_FILE "data/leaderboard.txt"

typedef struct {
    char name[100];
    int score;
    char date[50];
} Entry;

static Entry entries[200];
static int entry_count = 0;

void load_leaderboard() {
    entry_count = 0;

    FILE* f = fopen(LEADER_FILE, "r");
    if (!f) return;

    while (!feof(f)) {
        Entry e;
        if (!fgets(e.name, sizeof(e.name), f)) break;
        trim_newline(e.name);

        char scorebuf[50];
        if (!fgets(scorebuf, sizeof(scorebuf), f)) break;
        e.score = atoi(scorebuf);

        if (!fgets(e.date, sizeof(e.date), f)) break;
        trim_newline(e.date);

        entries[entry_count++] = e;
    }

    fclose(f);
}

static void save_internal() {
    FILE* f = fopen(LEADER_FILE, "w");
    if (!f) return;

    for (int i = 0; i < entry_count; i++) {
        fprintf(f, "%s\n%d\n%s\n", entries[i].name, entries[i].score, entries[i].date);
    }

    fclose(f);
}

void save_leaderboard() {
    save_internal();
}

void add_score(const char* name, int score) {
    Entry e;
    strcpy(e.name, name);
    e.score = score;

    time_t t = time(NULL);
    strcpy(e.date, ctime(&t));
    trim_newline(e.date);

    entries[entry_count++] = e;

    // sort by score descending
    for (int i = 0; i < entry_count - 1; i++) {
        for (int j = i + 1; j < entry_count; j++) {
            if (entries[j].score > entries[i].score) {
                Entry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }

    if (entry_count > 100) entry_count = 100;

    save_internal();
}

void view_leaderboard() {
    printf("\n=== Leaderboard ===\n");

    if (entry_count == 0) {
        printf("No entries yet.\n");
        press_enter();
        return;
    }

    for (int i = 0; i < entry_count; i++) {
        printf("%d. %s | %d pts | %s\n",
            i + 1,
            entries[i].name,
            entries[i].score,
            entries[i].date
        );
    }

    press_enter();
}
