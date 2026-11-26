Sports League Fixture and Venue Scheduling Engine
Saksham Yadav · SAP ID 590024978 · Design and Analysis of Algorithms, UPES

An offline Windows application written in C. Enter teams, grounds and available time slots to see who plays whom, where each match takes place and how far each team travels.
Build and run offline
1. Use Windows with MinGW GCC installed and available in your terminal. Run gcc --version to check. The interface uses the Windows API; Qt and internet access are not required.
2. In VS Code, open the project folder containing src. The source file should be at src/main.c.
3. Open the integrated terminal in that folder and run:
gcc -std=c11 -Wall -Wextra src/main.c -o scheduler.exe -lgdi32 -luser32 -lm
.\scheduler.exe
The first command builds the executable; the second opens the scheduler. If you see an “undefined reference to GetStockObject” error, use the full build command above, including -lgdi32.
Try the scheduler
1. Click Load sample, leave Slots per round at 1 and click Generate. The four teams should play six matches across three rounds.
2. Select Central Ground, enter 1 in Round and click Close. Its name turns grey. Click Generate, then choose Fixtures. Only East Ground can host a match in Round 1, so one match is unscheduled. Central Ground remains available in later rounds.
3. Increase Slots per round to 2 and generate again. East Ground can now host both Round 1 matches at different times. To undo the closure, select Central Ground in Round 1 and click Reopen.
4. Choose Summary to compare the scheduling strategies or Team travel to view the distance ranking. For a larger example, click 20-team sample and generate: 20 teams produce 190 pairings.
To use your own league, click Reset, then enter a name and X/Y coordinates for each team and venue. Use kilometres on the same flat map. You need 2–20 teams, 1–10 venues and 1–10 slots per round. Click an existing team to edit or remove it. After changing the inputs, click Generate again.
How the results are calculated
- Round robin: fixes one team in place and rotates the others to pair every team with every other team once. An odd number of teams gives one team a bye per round.
- First available: takes an open venue with a free slot. This is the baseline for comparison.
- Greedy: picks the available venue with the lowest travel for the current match.
- Backtracking: tries alternative venue assignments on eligible small rounds. The Fixtures view identifies rounds that used backtracking or greedy.
- Merge sort: orders teams by total travel. The travel gap is the highest team total minus the lowest.
Travel is an estimate: each team goes from home to the venue and back after every match. Distances are straight lines, not road routes or driving times. An unscheduled match adds no travel, so check how many matches were scheduled before comparing kilometres.