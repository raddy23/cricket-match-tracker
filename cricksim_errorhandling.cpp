// Cricket Match Simulation
// Player Hierarchy: Defines the base classes and derived classes for players in cricket match
// Cricket Match (Player, Batter, Bowler, WicketKeeper, AllRounder)
// Class Based approach and Polymorphic behaviour

/*---------------------------------------------------------------------------*/
#include <cassert>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// ------------------------
// Abstract Base: Player
// ------------------------
class Player {
public:
    explicit Player(const std::string& name) {
        if (name.empty()) throw std::invalid_argument("Player name cannot be empty");
        name_ = name;
    }
    virtual ~Player() = default;

    const std::string& Name() const noexcept { return name_; }

    virtual void PrintBattingLine(int idx = -1) const = 0;
    virtual void PrintBowlingLine() const = 0;

    virtual bool CanBat() const noexcept = 0;
    virtual bool CanBowl() const noexcept = 0;

    virtual int Runs() const noexcept { return 0; }
    virtual int BallsFaced() const noexcept { return 0; }
    virtual bool IsOut() const noexcept { return false; }
    virtual const std::string& Dismissal() const {
        static std::string s = "";
        return s;
    }

    virtual int BallsBowled() const noexcept { return 0; }
    virtual int RunsConceded() const noexcept { return 0; }
    virtual int Wickets() const noexcept { return 0; }

protected:
    std::string name_;
};

// ------------------------
// Batter
// ------------------------
class Batter : virtual public Player {
public:
    explicit Batter(const std::string& name)
        : Player(name), runs_(0), balls_(0), out_(false), dismissal_("Not Out") {}

    void SetBatting(int runs, int balls, bool out, const std::string& dismissal = "") {
        if (runs < 0 || balls < 0) throw std::invalid_argument("Runs and balls must be non-negative");
        runs_ = runs;
        balls_ = balls;
        out_ = out;
        dismissal_ = out ? (dismissal.empty() ? "Out" : dismissal) : "Not Out";
    }

    void PrintBattingLine(int idx = -1) const override {
        std::string label = (idx >= 0 ? std::to_string(idx) + " " : "") + name_;
        std::cout << std::left << std::setw(22) << label
                  << std::setw(6) << runs_
                  << std::setw(6) << balls_
                  << std::setw(24) << (out_ ? dismissal_ : "Not Out")
                  << '\n';
    }

    void PrintBowlingLine() const override {}

    bool CanBat() const noexcept override { return true; }
    bool CanBowl() const noexcept override { return false; }

    int Runs() const noexcept override { return runs_; }
    int BallsFaced() const noexcept override { return balls_; }
    bool IsOut() const noexcept override { return out_; }
    const std::string& Dismissal() const override { return dismissal_; }

protected:
    int runs_;
    int balls_;
    bool out_;
    std::string dismissal_;
};

// ------------------------
// Bowler
// ------------------------
class Bowler : virtual public Player {
public:
    explicit Bowler(const std::string& name)
        : Player(name), balls_bowled_(0), runs_conceded_(0), wickets_(0) {}

    void SetBowling(int balls, int runs, int wkts) {
        if (balls < 0 || runs < 0 || wkts < 0)
            throw std::invalid_argument("Bowling stats must be non-negative");
        balls_bowled_ = balls;
        runs_conceded_ = runs;
        wickets_ = wkts;
    }

    void PrintBowlingLine() const override {
        int overs = balls_bowled_ / 6;
        int rem = balls_bowled_ % 6;
        std::string oversStr = std::to_string(overs) + "." + std::to_string(rem);
        std::cout << std::left << std::setw(20) << name_
                  << std::setw(8) << oversStr
                  << std::setw(8) << runs_conceded_
                  << std::setw(6) << wickets_
                  << '\n';
    }

    void PrintBattingLine(int) const override {}

    bool CanBat() const noexcept override { return false; }
    bool CanBowl() const noexcept override { return true; }

    int BallsBowled() const noexcept override { return balls_bowled_; }
    int RunsConceded() const noexcept override { return runs_conceded_; }
    int Wickets() const noexcept override { return wickets_; }

protected:
    int balls_bowled_;
    int runs_conceded_;
    int wickets_;
};

// ------------------------
// WicketKeeper
// ------------------------
class WicketKeeper : public Batter {
public:
    explicit WicketKeeper(const std::string& name)
        : Player(name), Batter(name), catches_(0), stumpings_(0) {}

    void SetCatches(int c) { if (c < 0) throw std::invalid_argument("Catches must be non-negative"); catches_ = c; }
    void SetStumpings(int s) { if (s < 0) throw std::invalid_argument("Stumpings must be non-negative"); stumpings_ = s; }

    void PrintBattingLine(int idx = -1) const override {
        std::string label = (idx >= 0 ? std::to_string(idx) + " " : "") + name_ + " (WK)";
        std::cout << std::left << std::setw(22) << label
                  << std::setw(6) << runs_
                  << std::setw(6) << balls_
                  << std::setw(24) << (out_ ? dismissal_ : "Not Out")
                  << '\n';
    }

private:
    int catches_;
    int stumpings_;
};

// ------------------------
// AllRounder
// ------------------------
class AllRounder : public Batter, public Bowler {
public:
    explicit AllRounder(const std::string& name)
        : Player(name), Batter(name), Bowler(name) {}

    void PrintBattingLine(int idx = -1) const override { Batter::PrintBattingLine(idx); }
    void PrintBowlingLine() const override { Bowler::PrintBowlingLine(); }

    bool CanBat() const noexcept override { return true; }
    bool CanBowl() const noexcept override { return true; }
};

// ------------------------
// Team
// ------------------------
class Team {
public:
    explicit Team(std::string name, int maxOvers = 5)
        : name_(std::move(name)), maxOvers_(maxOvers), maxBalls_(maxOvers * 6) {
        if (maxOvers <= 0) throw std::invalid_argument("Max overs must be positive");
    }

    Batter* AddBatter(const std::string& name) {
        auto up = std::make_unique<Batter>(name);
        Batter* p = up.get();
        players_.push_back(std::move(up));
        return p;
    }

    Bowler* AddBowler(const std::string& name) {
        auto up = std::make_unique<Bowler>(name);
        Bowler* p = up.get();
        players_.push_back(std::move(up));
        return p;
    }

    WicketKeeper* AddWicketKeeper(const std::string& name) {
        auto up = std::make_unique<WicketKeeper>(name);
        WicketKeeper* p = up.get();
        players_.push_back(std::move(up));
        return p;
    }

    AllRounder* AddAllRounder(const std::string& name) {
        auto up = std::make_unique<AllRounder>(name);
        AllRounder* p = up.get();
        players_.push_back(std::move(up));
        return p;
    }

    void PrintBattingCard(const std::vector<Player*>& order) const {
        std::cout << "---- " << name_ << " Batting ----\n";
        std::cout << std::left << std::setw(22) << "Batter"
                  << std::setw(6) << "R"
                  << std::setw(6) << "B"
                  << std::setw(24) << "Status" << '\n';
        std::cout << std::string(66, '-') << '\n';

        int idx = 1;
        for (auto* p : order) {
            p->PrintBattingLine(idx++);
        }

        std::cout << std::string(66, '-') << '\n';
        int runs = 0, wkts = 0, balls = 0;
        for (const auto& u : players_) {
            runs += u->Runs();
            balls += u->BallsFaced();
            if (u->IsOut()) ++wkts;
        }

        int overs = balls / 6;
        int rem = balls % 6;
        if (rem == 0)
            std::cout << "Total: " << runs << " - " << wkts << " (" << overs << " Overs)\n\n";
        else
            std::cout << "Total: " << runs << " - " << wkts << " (" << overs << "." << rem << " Overs)\n\n";
    }

    void PrintBowlingCard() const {
        std::cout << "---- " << name_ << " Bowling ----\n";
        std::cout << std::left << std::setw(20) << "Bowler"
                  << std::setw(8) << "O"
                  << std::setw(8) << "R"
                  << std::setw(6) << "W" << '\n';
        std::cout << std::string(66, '-') << '\n';

        for (const auto& u : players_) {
            if (u->CanBowl() && u->BallsBowled() > 0) {
                u->PrintBowlingLine();
            }
        }
        std::cout << '\n';
    }

    int TotalRuns() const {
        int s = 0;
        for (auto& u : players_) s += u->Runs();
        return s;
    }

    int TotalWickets() const {
        int c = 0;
        for (auto& u : players_) if (u->IsOut()) ++c;
        return c;
    }

    int TotalBallsFaced() const {
        int s = 0;
        for (auto& u : players_) s += u->BallsFaced();
        return s;
    }

private:
    std::string name_;
    int maxOvers_;
    int maxBalls_;
    std::vector<std::unique_ptr<Player>> players_;
};

// ------------------------
// Match Helper
// ------------------------
std::string DetermineWinner(const Team& first, const Team& second,
                            const std::string& firstName, const std::string& secondName) {
    int runsFirst = first.TotalRuns();
    int runsSecond = second.TotalRuns();
    int wktsSecond = second.TotalWickets();

    if (runsFirst > runsSecond) {
        int margin = runsFirst - runsSecond;
        return firstName + " win by " + std::to_string(margin) + " runs";
    } else if (runsSecond > runsFirst) {
        int wktsRemaining = 10 - wktsSecond;
        return secondName + " win by " + std::to_string(wktsRemaining) + " wkts";
    } else {
        return "Match tied";
    }
}

// ------------------------
// Improved Test Framework
// ------------------------
void RunTest(const std::string& name, const std::string& got, const std::string& expected) {
    if (got == expected)
        std::cout << "[PASS] " << name << " -> " << got << "\n";
    else
        std::cout << "[FAIL] " << name << " (got: " << got << ", expected: " << expected << ")\n";
}

// ------------------------
// Main
// ------------------------
int main() {
    try {
        // -------------------- Scenario 1: India vs SL --------------------
        Team india("India XI");
        Team sl("Sri Lanka XI");

        WicketKeeper* kishan = india.AddWicketKeeper("Ishan Kishan");
        kishan->SetBatting(15, 8, true, "b Malinga");

        AllRounder* hardik = india.AddAllRounder("Hardik Pandya");
        hardik->SetBatting(22, 8, false);

        Batter* sky = india.AddBatter("Suryakumar Yadav");
        sky->SetBatting(7, 2, false);

        Bowler* malinga = india.AddBowler("Malinga");
        malinga->SetBowling(12, 25, 1);

        Batter* mendis = sl.AddBatter("Kusal Mendis");
        mendis->SetBatting(18, 10, true, "c Gill b Bumrah");

        Batter* nissanka = sl.AddBatter("Pathum Nissanka");
        nissanka->SetBatting(22, 8, false);

        Bowler* bumrah = sl.AddBowler("Bumrah");
        bumrah->SetBowling(12, 28, 1);

        // Print scorecards
        std::cout << "---- Match Information ----\n";
        std::cout << "Venue : Wankhede Stadium, Mumbai\n";
        std::cout << "Match : India vs Sri Lanka\n";
        std::cout << "Umpire: Paul Riffel (Field Umpire) - Aus\n";
        std::cout << "---------------------------\n\n";

        india.PrintBattingCard({kishan, hardik, sky});
        india.PrintBowlingCard();

        sl.PrintBattingCard({mendis, nissanka});
        sl.PrintBowlingCard();

        std::string result1 = DetermineWinner(india, sl, "India", "Sri Lanka");
        std::cout << "Result: " << result1 << "\n\n";

        // Tests
        RunTest("India vs SL winner", result1, "India win by 4 runs");
        RunTest("India runs", std::to_string(india.TotalRuns()), "44");
        RunTest("India balls", std::to_string(india.TotalBallsFaced()), "18");
        RunTest("SL runs", std::to_string(sl.TotalRuns()), "40");
        RunTest("SL balls", std::to_string(sl.TotalBallsFaced()), "18");

        // -------------------- Scenario 2: Chase successful --------------------
        Team teamA("Team A");
        Team teamB("Team B");

        Batter* a1 = teamA.AddBatter("A1");
        a1->SetBatting(30, 15, true);

        Batter* a2 = teamA.AddBatter("A2");
        a2->SetBatting(20, 10, false);

        Batter* b1 = teamB.AddBatter("B1");
        b1->SetBatting(40, 20, false);

        std::string result2 = DetermineWinner(teamA, teamB, "Team A", "Team B");
        RunTest("Chase successful", result2, "Team B win by 10 wkts");

        // -------------------- Scenario 3: Tie --------------------
        Team teamC("Team C");
        Team teamD("Team D");

        Batter* c1 = teamC.AddBatter("C1");
        c1->SetBatting(25, 12, false);

        Batter* d1 = teamD.AddBatter("D1");
        d1->SetBatting(25, 12, false);

        std::string result3 = DetermineWinner(teamC, teamD, "Team C", "Team D");
        RunTest("Match tie", result3, "Match tied");

        // -------------------- Scenario 4: Error Handling --------------------
        try {
            Batter* bad = teamD.AddBatter("");
            bad->SetBatting(-5, 3, false);
        } catch (const std::invalid_argument& ex) {
            std::cout << "[PASS] Error handling check -> " << ex.what() << "\n";
        }

    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
/*--------------OUTPUT---------------------*/
/*---- Match Information ----
Venue : Wankhede Stadium, Mumbai
Match : India vs Sri Lanka
Umpire: Paul Riffel (Field Umpire) - Aus
---------------------------

---- India XI Batting ----
Batter                R     B     Status                  
------------------------------------------------------------------
1 Ishan Kishan (WK)   15    8     b Malinga               
2 Hardik Pandya       22    8     Not Out                 
3 Suryakumar Yadav    7     2     Not Out                 
------------------------------------------------------------------
Total: 44 - 1 (3 Overs)

---- India XI Bowling ----
Bowler              O       R       W     
------------------------------------------------------------------
Malinga             2.0     25      1     

---- Sri Lanka XI Batting ----
Batter                R     B     Status                  
------------------------------------------------------------------
1 Kusal Mendis        18    10    c Gill b Bumrah         
2 Pathum Nissanka     22    8     Not Out                 
------------------------------------------------------------------
Total: 40 - 1 (3 Overs)

---- Sri Lanka XI Bowling ----
Bowler              O       R       W     
------------------------------------------------------------------
Bumrah              2.0     28      1     

Result: India win by 4 runs

[PASS] India vs SL winner -> India win by 4 runs
[PASS] India runs -> 44
[PASS] India balls -> 18
[PASS] SL runs -> 40
[PASS] SL balls -> 18
[FAIL] Chase successful (got: Team A win by 10 runs, expected: Team B win by 10 wkts)
[PASS] Match tie -> Match tied
[PASS] Error handling check -> Player name cannot be empty


=== Code Execution Successful ===
*/
