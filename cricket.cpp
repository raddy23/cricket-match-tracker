//C++ Cricket Test  :just copy paste into c++ online compiler and run for output 

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// Base Class: Person
class Person {
protected:
    string name;
public:
    Person(const string &n) : name(n) {}
    string getName() const { return name; }
};

// Player Class
class Player : public Person {
private:
    string role;
    int runs;
    int balls;
    bool isOut;
    string outReason; // out status
public:
    Player(const string &n, const string &r)
        : Person(n), role(r), runs(0), balls(0), isOut(false), outReason("Not Out") {}

    void addRuns(int r, int b = 1) { runs += r; balls += b; }
    void setOut(const string &reason) { isOut = true; outReason = reason; }

    string getRole() const { return role; }
    int getRuns() const { return runs; }
    int getBalls() const { return balls; }
    bool getOutStatus() const { return isOut; }
    string getOutReason() const { return outReason; }
};

// Bowler Class
class Bowler {
private:
    string name;
    float overs;
    int runsConceded;
    int wickets;

public:
    Bowler(const string &n) : name(n), overs(0.0f), runsConceded(0), wickets(0) {}

    string getName() const { return name; }
    float getOvers() const { return overs; }
    int getRunsConceded() const { return runsConceded; }
    int getWickets() const { return wickets; }

    void setOvers(float o) { overs = o; }
    void setRuns(int r) { runsConceded = r; }
    void setWickets(int w) { wickets = w; }
};

// Umpire Class
class Umpire : public Person {
private:
    string category; //third umpire or ground umpire
    string country;

public:
    Umpire(const string &n, const string &c, const string& co) : Person(n), category(c), country(co) {}

    string getCategory() const { return category; }
    string getCountry() const { return country; }
};

// Team Class
class Team {
private:
    string teamName;
    vector<Player> players;
    vector<Bowler> bowlers;
    int totalRuns;
    int totalWickets;
    float overs;

public:
    Team(const string &n) : teamName(n), totalRuns(0), totalWickets(0), overs(0.0f) {}

    void addPlayer(const Player &p) { players.push_back(p); }
    void addBowler(const Bowler &b) { bowlers.push_back(b); }

    void updatePlayerScore(const string &playerName, int runs, int balls, bool isOut, const string &reason = "") {
        for (auto &p : players) {
            if (p.getName() == playerName) {
                p.addRuns(runs, balls);
                totalRuns += runs;
                if (isOut) {
                    p.setOut(reason.empty() ? "Out" : reason);
                    totalWickets++;
                }
                return;
            }
        }

        Player newP(playerName, "Batsman");
        newP.addRuns(runs, balls);
        if (isOut) { newP.setOut(reason.empty() ? "Out" : reason); totalWickets++; }
        players.push_back(newP);
        totalRuns += runs;
    }

    void updateBowler(const string &bowlerName, float o, int r, int w) {
        for (auto &b : bowlers) {
            if (b.getName() == bowlerName) {
                b.setOvers(o);
                b.setRuns(r);
                b.setWickets(w);
                return;
            }
        }
        Bowler nb(bowlerName);
        nb.setOvers(o);
        nb.setRuns(r);
        nb.setWickets(w);
        bowlers.push_back(nb);
    }
// wanted to have limited fix 5 overs
    void setOvers(float o) { overs = o; }

    void showScorecard() const {
        cout << "\n---- " << teamName << " Scorecard ----\n";
        cout << left << setw(22) << "Batter"
             << setw(10) << "Role"
             << setw(8) << "R"
             << setw(8) << "B"
             << setw(20) << "Status" << '\n';
        cout << "------------------------------------------------------------------\n";
        for (const auto &p : players) {
            cout << left << setw(22) << p.getName()
                 << setw(10) << p.getRole()
                 << setw(8) << p.getRuns()
                 << setw(8) << p.getBalls()
                 << setw(20) << (p.getOutStatus() ? p.getOutReason() : "Not Out")
                 << '\n';
        }
        cout << "------------------------------------------------------------------\n";
        cout << "Total: " << totalRuns << " - " << totalWickets
             << " (" << fixed << setprecision(1) << overs << " Overs)\n";

        if (!bowlers.empty()) {
            cout << "\nBowler summary:\n";
            cout << left << setw(18) << "Bowler"
                 << setw(8) << "O"
                 << setw(8) << "R"
                 << setw(8) << "W" << '\n';
            for (const auto &b : bowlers) {
                cout << left << setw(18) << b.getName()
                     << setw(8) << fixed << setprecision(1) << b.getOvers()
                     << setw(8) << b.getRunsConceded()
                     << setw(8) << b.getWickets()
                     << '\n';
            }
        }
    }
};

// Function to display match information
void showMatchInfo(const string &venue, const string &matchVs, const Umpire &umpire) {
    cout << "\n---- Match Information ----\n";
    cout << "Venue: " << venue << "\n";
    cout << "Match: " << matchVs << "\n";
    cout << "Umpire: " << umpire.getName() << " (" << umpire.getCategory() << ") - " << umpire.getCountry() << "\n";
    cout << "---------------------------\n";
}

// Main
int main() {
    // Team India
    Team india("India");
    india.addPlayer(Player("Abhishek Sharma", "Batsman"));
    india.addPlayer(Player("Shubman Gill", "Batsman"));
    india.addPlayer(Player("Suryakumar Yadav", "Batsman"));
    india.addBowler(Bowler("Malinga")); // to display under India summary

    // Team Sri Lanka
    Team sl("Sri Lanka");
    sl.addPlayer(Player("Kusal Mendis", "Batsman"));
    sl.addPlayer(Player("Pathum Nissanka", "Batsman"));
    sl.addBowler(Bowler("Bumrah"));

    // Umpire
    Umpire paulRiffel("Paul Riffel", "Field Umpire", "Aus");

    // Show match info BEFORE scorecards
    showMatchInfo("Wankhede Stadium, Mumbai", "India vs Sri Lanka", paulRiffel);

    // IND innings
    india.updatePlayerScore("Abhishek Sharma", 30, 16, true, "c Kusal b Malinga");
    india.updatePlayerScore("Shubman Gill", 20, 9, false);
    india.updatePlayerScore("Suryakumar Yadav", 7, 2, false);
    india.setOvers(5.0f);
    india.updateBowler("Malinga", 3.0f, 35, 1);  // Expected stats

    // SL innings
    sl.updatePlayerScore("Kusal Mendis", 18, 12, true, "c Gill b Bumrah");
    sl.updatePlayerScore("Pathum Nissanka", 22, 18, false);
    sl.setOvers(5.0f);
    sl.updateBowler("Bumrah", 3.0f, 28, 1);     // Expected stats

    // Show scorecards
    india.showScorecard();
    sl.showScorecard();

    return 0;
}
