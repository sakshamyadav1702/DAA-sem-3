#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* League limits */
#define NT 20
#define NV 10
#define NS 10
#define NM 190  /* 20 teams: 20 * 19 / 2 = 190 matches */

/* Button IDs are consecutive integers, starting at 1. */
enum { ADD_TEAM = 1, ADD_VENUE, GENERATE, RESET, SAMPLE, CLOSURE, VIEW, DEMO, LARGE_SAMPLE, CANCEL_TEAM, REMOVE_TEAM, VENUE_LIST };

#define LIGHT_BLUE RGB(231, 241, 250)
#define NAVY       RGB(25, 55, 85)

typedef struct {
    char name[50];
    double x, y;
} Place;

typedef struct {
    int a, b;       /* Team indices */
    int r, v, s;    /* Round, venue and slot indices */
} Match;

typedef struct {
    int id;
    double km;
} Travel;

typedef struct {
    Travel rank[NT];
    double teamKm[NT];
    double total;
    int scheduled;
} Report;

Place team[NT], venue[NV];
Match pairs[NM], base[NM], greedy[NM], final[NM];
Report baselineReport, greedyReport, finalReport;

int nt, nv, nm;       /* Number of teams, venues and matches */
int closed[NT][NV];   /* closed[round][venue] */
int exact[NT];        /* Did this round use backtracking? */
int ready;            /* Is there a current schedule to display? */

HWND nameBox[2], xBox[2], yBox[2], placeList[2];
HWND slotsBox, closureButton, roundBox, viewBox, headline, output;
int editingTeam = -1;  /* -1 means the team fields are for a new team. */
HWND teamButton, cancelButton, removeButton;
HFONT normalFont, headingFont, tableFont;
HBRUSH background;

/* 1. FIXTURES AND TRAVEL */

int rounds(void) {
    if (nt < 2) return 0;
    return nt - 1 + nt % 2;
}

/* One team travels from home to the ground and back. */
double trip(int t, int v) {
    double dx = team[t].x - venue[v].x;
    double dy = team[t].y - venue[v].y;
    return 2 * sqrt(dx * dx + dy * dy);
}

/* A match's travel is the sum of both teams' return trips. */
double cost(Match m, int v) {
    return trip(m.a, v) + trip(m.b, v);
}

/* Round-robin circle method */
void pairTeams(void) {
    int order[NT + 1];
    int n = nt + nt % 2;  /* Add a bye position for an odd league. */
    nm = 0;

    for (int i = 0; i < n; i++) order[i] = i;

    for (int r = 0; r < rounds(); r++) {
        for (int i = 0; i < n / 2; i++) {
            int a = order[i];
            int b = order[n - 1 - i];
            if (a != nt && b != nt)
                pairs[nm++] = (Match){a, b, r, -1, -1};
        }
        /* Keep the first team fixed and rotate the others. */
        int last = order[n - 1];
        for (int i = n - 1; i > 1; i--) order[i] = order[i - 1];
        order[1] = last;
    }
}

/* useGreedy = 0: take the first available ground and slot.
   useGreedy = 1: choose the available ground with the least travel. */
void assign(Match out[], int slots, int useGreedy) {
    int used[NS][NV] = {0};

    for (int i = 0; i < nm; i++) {
        Match m = pairs[i];
        double best = 0;
        if (i == 0 || m.r != pairs[i - 1].r)
            memset(used, 0, sizeof used);
        for (int s = 0; s < slots; s++) {
            for (int v = 0; v < nv; v++) {
                if (closed[m.r][v] || used[s][v]) continue;
                double km = cost(m, v);
                if (m.v < 0 || km < best) {
                    best = km;
                    m.v = v;
                    m.s = s;
                }
                if (!useGreedy) break;
            }
            if (!useGreedy && m.v >= 0) break;
        }
        if (m.v >= 0) used[m.s][m.v] = 1;
        out[i] = m;
    }
}

/* Two matches, one slot: build a choice recursively, then try its alternative.
   At depth 1 only the ground not used by the first match is available. */
void backtrack(int first, const int open[2], int choice[2], int depth, double *bestKm) {
    if (depth == 2) {
        double km = cost(pairs[first], choice[0]) + cost(pairs[first + 1], choice[1]);
        if (km < *bestKm) {
            *bestKm = km;
            final[first].v = choice[0];
            final[first + 1].v = choice[1];
        }
        return;
    }
    for (int j = 0; j < 2; j++) {
        if (depth == 1 && open[j] == choice[0]) continue;
        choice[depth] = open[j];
        backtrack(first, open, choice, depth + 1, bestKm);
        /* Next branch overwrites this choice; no booking persists. */
    }
}

void chooseFinal(int slots) {
    memcpy(final, greedy, nm * sizeof(Match));
    memset(exact, 0, sizeof exact);
    if (nt != 4 || slots != 1) return;

    for (int i = 0; i < nm; i += 2) {
        int r = pairs[i].r, open[NV], count = 0, choice[2];
        for (int v = 0; v < nv; v++)
            if (!closed[r][v]) open[count++] = v;
        if (count != 2) continue;
        double bestKm = cost(final[i], final[i].v) + cost(final[i + 1], final[i + 1].v);
        backtrack(i, open, choice, 0, &bestKm);
        exact[r] = 1;
    }
}

/* Stable merge sort: equal distances stay in team-entry order. */
void mergeSort(Travel list[], Travel temp[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(list, temp, left, mid);
    mergeSort(list, temp, mid + 1, right);
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        if (list[i].km <= list[j].km)
            temp[k++] = list[i++];
        else
            temp[k++] = list[j++];
    }

    while (i <= mid)   temp[k++] = list[i++];
    while (j <= right) temp[k++] = list[j++];

    for (k = left; k <= right; k++) list[k] = temp[k];
}

void makeReport(Match schedule[], Report *report) {
    Travel temp[NT];
    memset(report, 0, sizeof *report);

    for (int i = 0; i < nm; i++) {
        Match m = schedule[i];
        if (m.v < 0) continue;
        report->teamKm[m.a] += trip(m.a, m.v);
        report->teamKm[m.b] += trip(m.b, m.v);
        report->scheduled++;
    }
    for (int t = 0; t < nt; t++) {
        report->rank[t] = (Travel){t, report->teamKm[t]};
        report->total += report->teamKm[t];
    }
    mergeSort(report->rank, temp, 0, nt - 1);
}

/* Like printf, but writes one row in the results list. */
void line(const char *format, ...) {
    char text[256];
    va_list args;
    va_start(args, format);
    vsnprintf(text, sizeof text, format, args);
    va_end(args);
    SendMessageA(output, LB_ADDSTRING, 0, (LPARAM)text);
}

void clearOutput(void) {
    SendMessageA(output, LB_RESETCONTENT, 0, 0);
}

void invalidate(const char *message) {
    ready = 0;
    clearOutput();
    SetWindowTextA(headline, message);
}

void showClosures(void) {
    int found = 0;

    for (int r = 0; r < rounds(); r++) {
        for (int v = 0; v < nv; v++) {
            if (closed[r][v]) {
                line("Round %d: %s is closed.", r + 1, venue[v].name);
                found = 1;
            }
        }
    }

    if (!found) line("All venues are open.");
}

void summaryRow(const char *name, Report *report) {
    char played[24];
    snprintf(played, sizeof played, "%d/%d", report->scheduled, nm);
    line("%-18s %10s %14.1f", name, played, report->total);
}

void showSummary(void) {
    char text[200];
    int bestRounds = 0;

    for (int r = 0; r < rounds(); r++) bestRounds += exact[r];
    snprintf(text, sizeof text, "Final schedule: %d of %d matches scheduled",
             finalReport.scheduled, nm);
    SetWindowTextA(headline, text);
    line("Method                  Matches      Travel km");
    summaryRow("First available", &baselineReport);
    summaryRow("Greedy only", &greedyReport);
    summaryRow("Final schedule", &finalReport);
    line("");
    line("Rounds using Best: %d   |   Rounds using Greedy: %d", bestRounds, rounds() - bestRounds);

    if (finalReport.scheduled == nm) {
        double saved = greedyReport.total - finalReport.total;
        double low = finalReport.rank[0].km;
        double high = finalReport.rank[nt - 1].km;
        if (saved >= 0.05) {
            line("Final schedule saves %.1f km compared with greedy alone.", saved);
        } else {
            line("Final and greedy have the same total travel to 0.1 km.");
        }
        if (bestRounds == rounds())
            line("Every round was solved exactly for these fixed pairings.");
        else
            line("Some rounds use greedy; minimum league travel is not guaranteed.");
        line("");
        line("TRAVEL FAIRNESS - FINAL SCHEDULE");
        line("Lowest: %.1f km     Highest: %.1f km", low, high);
        line("Average: %.1f km    Travel gap: %.1f km", finalReport.total / nt, high - low);
    } else {
        line("Travel covers scheduled matches only.");
        line("Reopen a venue or add slots to get full-league fairness.");
    }
    line("");
    showClosures();
    line("Travel is a straight-line estimate, including the return home.");
}

void showFixtures(void) {
    int index = 0;
    SetWindowTextA(headline, "Final fixtures - venues actually assigned");

    for (int r = 0; r < rounds(); r++) {
        int paired[NT] = {0};
        double roundKm = 0;
        int missing = 0;
        line("ROUND %d  |  %s", r + 1, exact[r] ? "Best (backtracking)" : "Greedy");
        while (index < nm && final[index].r == r) {
            Match m = final[index++];
            paired[m.a] = paired[m.b] = 1;
            if (m.v < 0) {
                line("  UNSCHEDULED: %s vs %s", team[m.a].name, team[m.b].name);
                missing++;
            } else {
                double km = cost(m, m.v);
                roundKm += km;
                line("  Slot %d: %s vs %s", m.s + 1, team[m.a].name, team[m.b].name);
                line("          %s  |  %.1f km", venue[m.v].name, km);
            }
        }
        for (int t = 0; t < nt; t++) {
            if (!paired[t]) {
                line("  Bye: %s", team[t].name);
            }
        }
        line("  %s: %.1f km", missing ? "Scheduled matches only" : "Round total", roundKm);
        line("");
    }
}

void showTeamTravel(void) {
    SetWindowTextA(headline, "Team travel - ranked using the final fixtures");

    if (finalReport.scheduled != nm) {
        line("Complete the schedule to compare full-season team travel.");
        return;
    }
    int width = 10;
    for (int t = 0; t < nt; t++) {
        int length = (int)strlen(team[t].name);
        if (length > width) width = length;
    }
    line("Rank  %-*s %12s %12s", width, "Team", "Greedy km", "Final km");
    line("");

    for (int i = 0; i < nt; i++) {
        Travel t = finalReport.rank[i];
        line("%4d  %-*s %12.1f %12.1f", i + 1, width, team[t.id].name,
             greedyReport.teamKm[t.id], t.km);
    }
}

void displayView(void) {
    if (!ready) return;
    clearOutput();
    int view = (int)SendMessageA(viewBox, CB_GETCURSEL, 0, 0);

    if (view == 1)      showFixtures();
    else if (view == 2) showTeamTravel();
    else                showSummary();
    SendMessageA(output, LB_SETTOPINDEX, 0, 0);
}

/* 3. USER INPUT AND SAMPLE LEAGUES */

int readInt(HWND box, int low, int high) {
    char text[40], *end;
    GetWindowTextA(box, text, sizeof text);
    long value = strtol(text, &end, 10);

    if (end == text) return -1;
    while (isspace((unsigned char)*end)) end++;
    if (*end || value < low || value > high) return -1;
    return (int)value;
}

int readCoord(HWND box, double *value) {
    char text[40], *end;
    GetWindowTextA(box, text, sizeof text);
    double number = strtod(text, &end);

    if (end == text) return 0;
    while (isspace((unsigned char)*end)) end++;
    if (*end || !isfinite(number) || fabs(number) > 10000) return 0;
    *value = number;
    return 1;
}

void showPlace(int group, Place p, int id) {
    char text[120];
    snprintf(text, sizeof text, "%d. %s  (%.1f, %.1f)",
             id, p.name, p.x, p.y);
    SendMessageA(placeList[group], LB_ADDSTRING, 0, (LPARAM)text);
}

void clearInputs(int group) {
    SetWindowTextA(nameBox[group], "");
    SetWindowTextA(xBox[group], "");
    SetWindowTextA(yBox[group], "");
}

/* The selected row is the venue. Availability still belongs to one round. */
void refreshVenues(void) {
    if (!closureButton) return;
    int r = readInt(roundBox, 1, rounds());
    int v = (int)SendMessageA(placeList[1], LB_GETCURSEL, 0, 0);
    int valid = r > 0 && v >= 0 && v < nv;
    SetWindowTextA(closureButton, valid && closed[r-1][v] ? "Reopen" : "Close");
    EnableWindow(closureButton, valid);
    InvalidateRect(placeList[1], NULL, TRUE);
}

/* Use -1 to leave edit mode without modifying a team. */
void selectTeam(int id) {
    char number[40];
    editingTeam = id;
    SetWindowTextA(teamButton, id < 0 ? "Add team" : "Save team");
    EnableWindow(cancelButton, id >= 0);
    EnableWindow(removeButton, id >= 0);
    if (id < 0) {
        clearInputs(0);
        SendMessageA(placeList[0], LB_SETCURSEL, -1, 0);
        return;
    }
    SetWindowTextA(nameBox[0], team[id].name);
    snprintf(number, sizeof number, "%.17g", team[id].x);
    SetWindowTextA(xBox[0], number);
    snprintf(number, sizeof number, "%.17g", team[id].y);
    SetWindowTextA(yBox[0], number);
}

void removeTeam(HWND window) {
    if (editingTeam < 0 || editingTeam >= nt) return;
    char question[100];
    snprintf(question, sizeof question, "Remove %s from this league?", team[editingTeam].name);
    if (MessageBoxA(window, question, "Remove team", MB_YESNO | MB_DEFBUTTON2) != IDYES) return;

    for (int t = editingTeam; t + 1 < nt; t++) team[t] = team[t + 1];
    nt--;
    nm = 0;
    selectTeam(-1);
    SendMessageA(placeList[0], LB_RESETCONTENT, 0, 0);
    for (int t = 0; t < nt; t++) showPlace(0, team[t], t + 1);
    for (int r = rounds(); r < NT; r++) memset(closed[r], 0, sizeof closed[r]);
    if (readInt(roundBox, 1, rounds()) < 0) SetWindowTextA(roundBox, "1");
    refreshVenues();
    invalidate("Team removed. Click Generate to rebuild the fixtures.");
}

/* group 0 = teams, group 1 = venues */
void addPlace(HWND window, int group) {
    Place p;
    Place *places = group ? venue : team;
    int *count = group ? &nv : &nt;
    int limit = group ? NV : NT;

    int editing = group == 0 && editingTeam >= 0;
    if (!editing && *count >= limit) {
        MessageBoxA(window, "Maximum number reached.", "Cannot add", MB_OK);
        return;
    }
    GetWindowTextA(nameBox[group], p.name, sizeof p.name);
    int start = 0, end = (int)strlen(p.name);
    while (start < end && isspace((unsigned char)p.name[start])) start++;
    while (end > start && isspace((unsigned char)p.name[end - 1])) end--;
    memmove(p.name, p.name + start, end - start);
    p.name[end - start] = '\0';

    if (!p.name[0]) {
        MessageBoxA(window, "Enter a name first.", "Missing name", MB_OK);
        return;
    }

    if (!readCoord(xBox[group], &p.x) || !readCoord(yBox[group], &p.y)) {
        MessageBoxA(window, "Enter X and Y between -10000 and 10000.",
                    "Invalid coordinates", MB_OK);
        return;
    }

    for (int i = 0; i < *count; i++) {
        if (editing && i == editingTeam) continue;
        if (lstrcmpiA(p.name, places[i].name) == 0) {
            MessageBoxA(window, "This name already exists.", "Duplicate", MB_OK);
            return;
        }
    }
    if (editing) {
        team[editingTeam] = p;  /* Keep the team ID and existing closures. */
        SendMessageA(placeList[0], LB_RESETCONTENT, 0, 0);
        for (int i = 0; i < nt; i++) showPlace(0, team[i], i + 1);
        selectTeam(-1);
        invalidate("Team updated. Click Generate.");
        return;
    }
    places[*count] = p;
    (*count)++;
    showPlace(group, p, *count);
    clearInputs(group);
    memset(closed, 0, sizeof closed);
    refreshVenues();
    invalidate("League updated. Click Generate.");
    line("Changing the league clears previous venue closures.");
    SetFocus(nameBox[group]);
}

void generate(HWND window) {
    if (nt < 2 || nv < 1) {
        MessageBoxA(window, "Add at least two teams and one venue.",
                    "League incomplete", MB_OK);
        return;
    }
    int slots = readInt(slotsBox, 1, NS);

    if (slots < 0) {
        MessageBoxA(window, "Slots per round must be 1 to 10.",
                    "Invalid slots", MB_OK);
        return;
    }
    pairTeams();
    assign(base, slots, 0);
    assign(greedy, slots, 1);
    chooseFinal(slots);
    makeReport(base, &baselineReport);
    makeReport(greedy, &greedyReport);
    makeReport(final, &finalReport);
    ready = 1;
    SendMessageA(viewBox, CB_SETCURSEL, 0, 0);
    displayView();
}

void toggleClosure(void) {
    int v = (int)SendMessageA(placeList[1], LB_GETCURSEL, 0, 0);
    int r = readInt(roundBox, 1, rounds());
    if (v < 0 || v >= nv || r < 1) return;
    closed[r-1][v] = !closed[r-1][v];
    refreshVenues();
    invalidate("Venue availability changed. Click Generate.");
    showClosures();
}

void resetData(void) {
    selectTeam(-1);
    ready = nt = nv = nm = 0;
    memset(closed, 0, sizeof closed);

    for (int g = 0; g < 2; g++) {
        clearInputs(g);
        SendMessageA(placeList[g], LB_RESETCONTENT, 0, 0);
    }
    SetWindowTextA(slotsBox, "1");
    SetWindowTextA(roundBox, "1");
    SendMessageA(viewBox, CB_SETCURSEL, 0, 0);
    refreshVenues();
    invalidate("Add your league or load a sample.");
}

typedef struct {
    int teams, venues;
    Place home[NT], ground[NV];
} SampleLeague;

static const SampleLeague samples[] = {
    /* Normal league */
    {4, 2,
        {{"Falcons", 0, 0}, {"Tigers", 6, 0}, {"Eagles", 0, 8},
         {"Lions", 6, 8}},
        {{"Central Ground", 3, 4}, {"East Ground", 6, 4}}},
    /* Travel demo */
    {4, 2,
        {{"Falcons", 4, 0}, {"Tigers", 0, 0}, {"Eagles", 1, 0},
         {"Lions", 4, 0}},
        {{"West Ground", 0, 0}, {"East Ground", 10, 0}}},
    /* 20-team sample */
    {20, 10,
        {{"Falcons", 1, 2}, {"Tigers", 9, 1}, {"Eagles", 18, 3},
         {"Lions", 27, 2}, {"Panthers", 35, 4}, {"Wolves", 42, 1},
         {"Rovers", 3, 12}, {"Rangers", 11, 14}, {"United", 20, 11},
         {"Athletic", 29, 13}, {"City", 37, 15}, {"Wanderers", 44, 12},
         {"Strikers", 2, 24}, {"Spartans", 12, 26}, {"Knights", 19, 23},
         {"Warriors", 28, 25}, {"Titans", 36, 27}, {"Royals", 45, 24},
         {"Harriers", 14, 33}, {"Comets", 32, 35}},
        {{"West Park", 4, 5}, {"Old Mill Ground", 15, 4}, {"East Field", 32, 5},
         {"River Stadium", 42, 8}, {"Central Ground", 22, 15}, {"Station Park", 8, 17},
         {"Valley Ground", 34, 20}, {"Hilltop Arena", 5, 27}, {"North Park", 17, 30},
         {"Summit Ground", 34, 31}}},
};

void loadSample(HWND window, int sampleID) {
    if ((nt || nv) &&
        MessageBoxA(window, "Replace the current league?", "Load sample",
                    MB_YESNO) != IDYES)
        return;
    resetData();
    int index = 0;
    if (sampleID == DEMO) index = 1;
    if (sampleID == LARGE_SAMPLE) index = 2;
    nt = samples[index].teams;
    nv = samples[index].venues;
    memcpy(team, samples[index].home, nt * sizeof(Place));
    memcpy(venue, samples[index].ground, nv * sizeof(Place));
    for (int i = 0; i < nt; i++) showPlace(0, team[i], i + 1);
    for (int i = 0; i < nv; i++) showPlace(1, venue[i], i + 1);

    refreshVenues();
    if (sampleID == DEMO) {
        generate(window);
        SendMessageA(viewBox, CB_SETCURSEL, 1, 0);
        displayView();
    } else if (sampleID == LARGE_SAMPLE) {
        invalidate("20 teams and 10 venues ready. Click Generate.");
    } else {
        invalidate("Sample ready. Click Generate.");
    }
}

/* 4. WINDOWS INTERFACE */

HWND control(HWND parent, const char *type, const char *text,
             DWORD style, int x, int y, int w, int h, int id) {
    HWND item = CreateWindowA(
        type, text, WS_CHILD | WS_VISIBLE | style,
        x, y, w, h, parent, (HMENU)(INT_PTR)id,
        GetModuleHandle(NULL), NULL
    );
    SendMessageA(item, WM_SETFONT, (WPARAM)normalFont, TRUE);
    return item;
}

HWND label(HWND parent, const char *text, int x, int y, int w) {
    return control(parent, "STATIC", text, 0, x, y, w, 23, 0);
}

HWND edit(HWND parent, const char *text, int x, int y, int w) {
    HWND box = control(parent, "EDIT", text, WS_BORDER | ES_AUTOHSCROLL,
                       x, y, w, 25, 0);
    SendMessageA(box, EM_SETLIMITTEXT, 32, 0);
    return box;
}

void button(HWND window, const char *text, int x, int y, int w, int id) {
    control(window, "BUTTON", text, 0, x, y, w, 30, id);
}

void createPanel(HWND window, int group, int x) {
    HWND title = label(window, group ? "VENUES" : "TEAM HOME LOCATIONS",
                       x, 12, group ? 320 : 200);
    SendMessageA(title, WM_SETFONT, (WPARAM)headingFont, TRUE);
    nameBox[group] = edit(window, "", x, 40, 205);
    SendMessageA(nameBox[group], EM_SETLIMITTEXT, 49, 0);
    HWND action = control(window, "BUTTON", group ? "Add venue" : "Add team",
            0, x + 215, 39, 95, 27, group ? ADD_VENUE : ADD_TEAM);
    if (group == 0) {
        teamButton = action;
        removeButton = control(window, "BUTTON", "Remove team", 0, x+205, 8, 105, 26, REMOVE_TEAM);
        EnableWindow(removeButton, FALSE);
        cancelButton = control(window, "BUTTON", "Cancel", 0, x + 273, 72, 60, 27, CANCEL_TEAM);
        EnableWindow(cancelButton, FALSE);
    }
    label(window, "X:", x, 77, 20);
    xBox[group] = edit(window, "", x + 24, 73, 100);
    label(window, "Y:", x + 145, 77, 20);
    yBox[group] = edit(window, "", x + 169, 73, 100);
    placeList[group] = control(window, "LISTBOX", "",
                               WS_BORDER | WS_VSCROLL | LBS_NOTIFY |
                               (group ? LBS_OWNERDRAWFIXED | LBS_HASSTRINGS : 0),
                               x, 106, 310, 83, group ? VENUE_LIST : 0);
}

LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        createPanel(window, 0, 20);
        createPanel(window, 1, 360);
        label(window, "Coordinates are in km on one map. Click a team to edit its name or location.",
              20, 193, 660);
        label(window, "Slots per round:", 20, 229, 112);
        slotsBox = edit(window, "1", 135, 225, 43);
        button(window, "Generate", 192, 223, 100, GENERATE);
        button(window, "Load sample", 302, 223, 110, SAMPLE);
        button(window, "Travel demo", 422, 223, 110, DEMO);
        button(window, "20-team sample", 542, 223, 125, LARGE_SAMPLE);
        button(window, "Reset", 677, 223, 78, RESET);
        label(window, "Round:", 20, 271, 58);
        roundBox = edit(window, "1", 82, 267, 42);
        closureButton = control(window, "BUTTON", "Close", 0, 140, 264, 105, 31, CLOSURE);
        refreshVenues();
        label(window, "Select a venue above. Grey venues are closed for this round.", 20, 303, 680);
        label(window, "Show:", 20, 339, 48);
        viewBox = control(window, "COMBOBOX", "",
                          CBS_DROPDOWNLIST | WS_VSCROLL,
                          72, 334, 170, 130, VIEW);
        SendMessageA(viewBox, CB_ADDSTRING, 0, (LPARAM)"Summary");
        SendMessageA(viewBox, CB_ADDSTRING, 0, (LPARAM)"Fixtures");
        SendMessageA(viewBox, CB_ADDSTRING, 0, (LPARAM)"Team travel");
        SendMessageA(viewBox, CB_SETCURSEL, 0, 0);
        headline = label(window, "Add your league or choose a sample.",
                         20, 373, 710);
        SendMessageA(headline, WM_SETFONT, (WPARAM)headingFont, TRUE);
        output = control(window, "LISTBOX", "",
                         WS_BORDER | WS_VSCROLL | WS_HSCROLL | LBS_NOINTEGRALHEIGHT,
                         20, 405, 710, 260, 0);
        SendMessageA(output, WM_SETFONT, (WPARAM)tableFont, TRUE);
        SendMessageA(output, LB_SETHORIZONTALEXTENT, 1500, 0);
        return 0;
    case WM_SIZE:
        if (output && wp != SIZE_MINIMIZED) {
            int w = LOWORD(lp) - 40;
            int h = HIWORD(lp) - 425;
            if (w > 0 && h > 0) {
                MoveWindow(headline, 20, 373, w, 25, TRUE);
                MoveWindow(output, 20, 405, w, h, TRUE);
            }
        }
        return 0;
    case WM_MEASUREITEM:
        if (wp == VENUE_LIST) {
            ((MEASUREITEMSTRUCT *)lp)->itemHeight = 21;
            return TRUE;
        }
        break;
    case WM_DRAWITEM:
        if (wp == VENUE_LIST) {
            DRAWITEMSTRUCT *row = (DRAWITEMSTRUCT *)lp;
            int selected = row->itemState & ODS_SELECTED;
            RECT area = row->rcItem;
            SetDCBrushColor(row->hDC, selected ? RGB(223,236,250) : RGB(255,255,255));
            FillRect(row->hDC, &area, (HBRUSH)GetStockObject(DC_BRUSH));
            if (row->itemID < (UINT)nv) {
                int v = (int)row->itemID, r = readInt(roundBox, 1, rounds());
                int shut = r > 0 && closed[r-1][v];
                char text[128];
                snprintf(text, sizeof text, "%s%s  (%.1f, %.1f)",
                         shut ? "[Closed] " : "", venue[v].name, venue[v].x, venue[v].y);
                SetBkMode(row->hDC, TRANSPARENT);
                SetTextColor(row->hDC, shut ? RGB(108,112,118) : NAVY);
                area.left += 4;
                DrawTextA(row->hDC, text, -1, &area,
                          DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
            }
            if (row->itemState & ODS_FOCUS) DrawFocusRect(row->hDC, &row->rcItem);
            return TRUE;
        }
        break;
    case WM_CTLCOLORSTATIC:
        SetTextColor((HDC)wp, NAVY);
        SetBkColor((HDC)wp, LIGHT_BLUE);
        return (LRESULT)background;
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
        SetTextColor((HDC)wp, RGB(30, 45, 60));
        SetBkColor((HDC)wp, RGB(255, 255, 255));
        return (LRESULT)GetStockObject(WHITE_BRUSH);
    case WM_COMMAND:
        if ((HWND)lp == placeList[0] && HIWORD(wp) == LBN_SELCHANGE) {
            int id = (int)SendMessageA(placeList[0], LB_GETCURSEL, 0, 0);
            if (id >= 0 && id < nt) selectTeam(id);
            return 0;
        }
        if (((HWND)lp == placeList[1] && HIWORD(wp) == LBN_SELCHANGE) ||
            ((HWND)lp == roundBox && HIWORD(wp) == EN_CHANGE)) {
            refreshVenues();
            return 0;
        }
        if (LOWORD(wp) == VIEW && HIWORD(wp) == CBN_SELCHANGE) {
            displayView();
            return 0;
        }
        if (ready && (HWND)lp == slotsBox && HIWORD(wp) == EN_CHANGE) {
            invalidate("Slot limit changed. Click Generate.");
            return 0;
        }
        if (HIWORD(wp) == BN_CLICKED) {
            switch (LOWORD(wp)) {
            case REMOVE_TEAM: removeTeam(window); break;
            case CANCEL_TEAM: selectTeam(-1); break;
            case ADD_TEAM:  addPlace(window, 0); break;
            case ADD_VENUE: addPlace(window, 1); break;
            case GENERATE:  generate(window); break;
            case CLOSURE:   toggleClosure(); break;
            case RESET:     resetData(); break;
            case SAMPLE:
            case DEMO:
            case LARGE_SAMPLE:
                loadSample(window, LOWORD(wp));
                break;
            }
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(window, msg, wp, lp);
}

HFONT makeFont(const char *name, int height, int weight) {
    return CreateFontA(-height, 0, 0, 0, weight, FALSE, FALSE, FALSE,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                       CLEARTYPE_QUALITY, DEFAULT_PITCH, name);
}

int main(void) {
    HINSTANCE instance = GetModuleHandle(NULL);
    background = CreateSolidBrush(LIGHT_BLUE);
    normalFont = makeFont("Segoe UI", 16, FW_NORMAL);
    headingFont = makeFont("Segoe UI", 18, FW_SEMIBOLD);
    tableFont = makeFont("Consolas", 16, FW_NORMAL);
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = "LeagueScheduler";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = background;

    if (!RegisterClassA(&wc)) return 1;
    HWND window = CreateWindowA(
        wc.lpszClassName, "Sports League Scheduler",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
        WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 790, 745,
        NULL, NULL, instance, NULL
    );

    if (!window) return 1;
    ShowWindow(window, SW_SHOW);
    MSG msg;
    int status;

    while ((status = GetMessageA(&msg, NULL, 0, 0)) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    UnregisterClassA(wc.lpszClassName, instance);
    DeleteObject(normalFont);
    DeleteObject(headingFont);
    DeleteObject(tableFont);
    return status == -1 ? 1 : 0;
}
