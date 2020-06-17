#include <bits/stdc++.h>
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using namespace std;

class student {
    public:
    string name;
    string school;
    string sid;
    vector <string> rivals;
};

class match {
    public:
    student* s1;
    student* s2;
    int winner; // 0 = none, 1 = s1, 2 = s2
};


fstream std_data_file, results_file, rounds_file;
vector <student> std_list;
vector <match> bracket;
int std_cnt;
string op;

student make_bye() {
    student bye;
    bye.name = "(bye)";
    bye.school = "bye";
    bye.sid = "bye";
    return bye;
}

student bye = make_bye(); 

void clear_screen() {
    const int times = 1;
    for (int i = 0; i < times; i++) {
        cout << '\n';
    }
} 

int bin_search(vector<student> list, string id) {
	int l = 0, r = list.size()-1, mid;
	while (l <= r) {
		mid = (l+r)/2;
		if (id < list[mid].sid) r = mid - 1;
		else if (id > list[mid].sid) l = mid + 1;
		else return mid;
	}
	return -1;
}

int next_power(int num) {
    num--;
    num |= (num >> 1);
    num |= (num >> 2);
    num |= (num >> 4);
    num |= (num >> 8);
    num |= (num >> 16);
    return ((num ^ (num >> 1)) << 1);
}

void modify_rivals(student* winner, student* loser) { //change status of rival in student (win/lose)
    winner -> rivals.back() = "*" + loser -> sid;
    loser -> rivals.back() = "-" + winner -> sid;
}

void save_bracket(int round_std_cnt) { //saves current results to results file
    results_file.open(".\\Data files\\Competitor Results.txt", fstream::out);
    results_file << round_std_cnt << '\n';
    for (match m: bracket) { // save each match
        student* std1 = m.s1;
        student* std2 = m.s2;
        results_file << std1 -> sid << '\n';
        results_file << std1 -> rivals.size() << '\n';
        for (auto x: std1 -> rivals) {
            results_file << x << ' ';
        }
        results_file << '\n';
        results_file << std2 -> sid << '\n';
        results_file << std2 -> rivals.size() << '\n';
        for (auto x: std2 -> rivals) {
            results_file << x << ' ';
        }
        results_file << '\n';
        results_file << m.winner << "\n\n";
    }
    results_file.close();
    if (std_cnt > 1) {
        string rounds_file_name = "";
        if (std_cnt == 2) rounds_file_name = "Finals";
        else if (std_cnt == 4) rounds_file_name = "Semifinals";
        else rounds_file_name = "Round of " + to_string(round_std_cnt);
        rounds_file_name = ".\\Data files\\" + rounds_file_name + ".txt";
        rounds_file.open(rounds_file_name, fstream::out);
        rounds_file << round_std_cnt << '\n';
        for (int i = 0; i < round_std_cnt/2; i++) { // save each match
            match m = bracket[i];
            student* std1 = m.s1;
            student* std2 = m.s2;
            rounds_file << std1 -> sid << '\n';
            rounds_file << std2 -> sid << '\n';
            rounds_file << m.winner << "\n\n";
        }
        rounds_file.close();
    }
}

int setup() { //prepares std_list & bracket for file input
    //opens competitor datafile and bracket file
    std_data_file.open(".\\Data files\\Compdata.txt", fstream::in);
    results_file.open(".\\Data files\\Competitor Results.txt", fstream::in | fstream::out);
    //test for whether files are empty
    if (std_data_file.peek() == EOF) return 1;
    if (results_file.peek() == EOF) return 2;
    cout << "Files found.\n";
    //read from student data file
    student temp_std; //used for temporary storage in setup
    while (std_data_file.peek() != EOF) {
	    getline (std_data_file, temp_std.name);
		getline (std_data_file, temp_std.school);
		getline (std_data_file, temp_std.sid);
		std_list.push_back(temp_std);
	}
    results_file >> std_cnt;
    //recreate competition bracket
    while (results_file.peek() != EOF) {
        //initialise
        match temp_match;
        int rival_num;
        int idx;
        string temp_sid;
        for (int i = 0; i < 2; i++) {
            //read ID
            results_file >> temp_sid;
            //find position of student in std_list
            idx = bin_search(std_list, temp_sid);
            //check if Results is inconsistent with Compdata
            if (idx == -1 && temp_sid != "bye") {
                return 10;
            }
            //read temp_std rivals
            results_file >> rival_num;
            for (int i = 0; i < rival_num; i++) {
                results_file >> temp_sid;
                if (idx != -1) std_list[idx].rivals.push_back(temp_sid);
            }
            if (i == 0) {
                //add 1st student to match
                if (idx != -1) temp_match.s1 = &std_list[idx];
                else temp_match.s1 = &bye;
            } else {
                //add 2nd student to match
                if (idx != -1) temp_match.s2 = &std_list[idx];
                else temp_match.s2 = &bye;
            }
        }
        //read match result
        results_file >> temp_match.winner;
        results_file.get();
        //insert match into bracket
        bracket.push_back(temp_match);
        results_file.get();
    }
    std_data_file.close();
    results_file.close();
    save_bracket(std_cnt);
    return 0;
}

void show_bracket(vector<match> bracket, int std_cnt) { //shows results of a round
    clear_screen();
    switch (std_cnt) {
        case 2:
            cout << "Finals\n";
            break;
        case 4:
            cout << "Semifinals\n";
            break;
        default:
            cout << "Round of " << std_cnt << '\n';
            break;
        }   
    //output matches
    const int col[3] = {7, 10, 30};
    cout << left;
    cout << setw(col[0]) << "Match";
    cout << setw(col[0]) << "Winner";
    cout << setw(col[2]) << "Player 1" << setw(col[2]) << "School" << setw(col[0]) << "Seed";
    cout << setw(col[2]) << "Player 2" << setw(col[2]) << "School" << setw(col[0]) << "Seed";
    cout << '\n';
    for (int i = 0; i < std_cnt/2; i++) {
        match curr_match = bracket[i];
        cout << setw(col[0]) << i+1;
        cout << setw(col[0]) << curr_match.winner;
        cout << setw(col[2]) << curr_match.s1 -> name << setw(col[2]) << curr_match.s1 -> school << setw(col[0]) << (curr_match.s1 -> name[0] == '*');
        cout << setw(col[2]) << curr_match.s2 -> name << setw(col[2]) << curr_match.s2 -> school << setw(col[0]) << (curr_match.s2 -> name[0] == '*');
        cout << '\n';
    }
    cout << '\n';
}

void update() {
    bool valid = false;
    int match = -1, winner = -1;
    string op;
    cout << "Which match?\n";
    while (!valid) {
        try {
            getline(cin, op);
            match = stoi(op);
            valid = match > 0 && match <= std_cnt/2;
            if (!valid) throw "Not in range";
        } catch (...) {
            cout << "Invalid input. Please try again." << endl;
        }
    }
    cout << "Winner?\n";
    valid = false;
    while (!valid) {
        try {
            getline(cin, op);
            winner = stoi(op);
            valid = winner >= 0 && winner <= 2;
            if (!valid) throw "Not in range";
        } catch (...) {
            cout << "Invalid input. Please try again." << endl;
        }
    }
    bracket[match-1].winner = winner;
    if (winner == 1) {
        modify_rivals(bracket[match-1].s1, bracket[match-1].s2);
    } else {
        modify_rivals(bracket[match-1].s2, bracket[match-1].s1);
    }
    save_bracket(std_cnt);
}

bool filled(vector<match> matches) {
    for (int i = 0; i < std_cnt/2; i++) {
        if (matches[i].winner == 0) return false;
    }
    return true;
}

void make_next_bracket(vector<match> &bracket, int curr_std_cnt) {
    int n = curr_std_cnt/2;
    vector<match> advanced{}, dropped{}; //advanced = won in last round, dropped = lost in last round
    for (int i = 0; i < n; i += 2) {
        match winners, losers; //winners: winners of the two matches, losers: losers of the two matches
        winners.s1 = bracket[i].s1; 
        losers.s1 = bracket[i].s2;
        if (bracket[i].winner == 2) {
            swap(winners.s1, losers.s1);
        } 
        modify_rivals(winners.s1, losers.s1);
        winners.s2 = bracket[i+1].s1;
        losers.s2 = bracket[i+1].s2;
        if (bracket[i+1].winner == 2) {
            swap(winners.s2, losers.s2);
        }
        modify_rivals(winners.s2, losers.s2);
        winners.winner = 0;
        losers.winner = 0;
        winners.s1 -> rivals.push_back(winners.s2 -> sid);
        winners.s2 -> rivals.push_back(winners.s1 -> sid);
        advanced.push_back(winners);
        dropped.push_back(losers);
    }
    advanced.insert(advanced.end(), dropped.begin(), dropped.end()); //combine two lists
    std_cnt /= 2;
    bracket = advanced;
    save_bracket(std_cnt);
}

void show_winner() {
    cout << "Tournament ended.\n";
    cout << "Winner: \n";
    match final = bracket[0];
    student* winner;
    if (final.winner == 1) {
        winner = bracket[0].s1;
    } else {
        winner = bracket[0].s2;
    }
    cout << "Name: " << winner -> name << '\n';
    cout << "School: " << winner -> school << '\n';
    cout << "Student ID: " << winner -> sid << '\n';
    std_cnt = 1;
    return;
}

void advance(int x) {
    if (!filled(bracket)) {
        cout << "Bracket not filled.\nPress Enter to continue editing the bracket.\n";
        cin.get();
        return;
    }
    if (x == 2) {
        std_cnt = 1;
        save_bracket(std_cnt);
        return;
    }
    cout << "Press Enter to advance to next round.\n";
    cin.get();
    make_next_bracket(bracket, std_cnt);
    save_bracket(std_cnt);
}


void list_students () {
    int stdcnt = std_list.size();
    cout << "Student number = " << stdcnt << '\n';
    const int col[3] = {30, 30, 10};
    cout << left;
    cout << setw(col[0]) << "Name" << ' ';
    cout << setw(col[1]) << "School" << ' ';
    cout << setw(col[2]) << "Student ID" << ' ';
    cout << '\n';
    for (student target: std_list) {
        cout << setw(col[0]) << target.name << ' ';
        cout << setw(col[1]) << target.school << ' ';
        cout << setw(col[2]) << target.sid << ' ';
        cout << '\n';
    }
}

student query_std(string sid) {
    student temp_std;
    int idx = bin_search(std_list, sid);
    if (idx == -1) return bye;
    temp_std = std_list[idx];
    return temp_std;
}

void query() {
    cout << "Query by student/round?\n";
    string op;
    do {
        getline(cin, op);
    } while (op != "student" && op != "round");
    if (op == "student") {
        list_students();
        cout << "\nEnter student number\n";
        string id;
        getline(cin, id);
        student temp_std = query_std(id);
        if (temp_std.sid == "bye") {
            cout << "Student not found.\n";
        } else {
            int temp_std_cnt = next_power(std_list.size());
            cout << "Name: " << temp_std.name << '\n';
            cout << "School: " << temp_std.school << '\n';
            cout << "Student ID: " << temp_std.sid << '\n';
            cout << "Number of matches played: " << temp_std.rivals.size() << '\n';
            for (int i = 0; i < temp_std.rivals.size(); i++) {
                string rival = temp_std.rivals[i]; //sid
                string prefix = "", suffix = "";
                char state = rival[0];
                if (state == '*') {
                    prefix = "Defeated ";
                    rival.erase(rival.begin());
                } else if (state == '-') {
                    prefix = "Lost to ";
                    rival.erase(rival.begin());
                } else {
                    prefix = "Playing ";
                }
                if (temp_std_cnt == 2) {
                    suffix = " in Finals";
                } else if (temp_std_cnt == 4) {
                    suffix = " in Semifinals";
                } else {
                    suffix = " in Round of " + to_string(temp_std_cnt);
                }
                int rival_idx = bin_search(std_list, rival);
                rival = std_list[rival_idx].name; //Name
                cout << prefix << rival << suffix << '\n';
                temp_std_cnt /= 2;
            }
            cout << '\n';
        }
        cout << '\n';
    } else {
        fstream curr_round_file;
        string op, temp_file_name;
        int curr_std_cnt = 0;
        bool ok = false;
        cout << "Which round?\n";
        while (!ok) {
            try {
                getline(cin, op);
                if (op != "Finals" && op != "Semifinals") {
                    curr_std_cnt = stoi(op);
                    if (curr_std_cnt != next_power(curr_std_cnt) && curr_std_cnt <= 4) throw curr_std_cnt;
                }
                ok = true;
            }
            catch (...) {
                cout << "Invalid input. Please try again.\n";
            }
        }
        if (op == "Finals" || op == "Semifinals") {
            //special case
            temp_file_name = op;
            if (op == "Finals") curr_std_cnt = 2;
            else curr_std_cnt = 4;
        } else {
            //general case
            temp_file_name = "Round of " + to_string(curr_std_cnt);
        }
        curr_round_file.open(".\\Data files\\" + temp_file_name +".txt");
        if (curr_round_file.is_open()) {
            vector <match> curr_bracket;
            match curr_match = {};
            curr_round_file >> curr_std_cnt;
            for (int i = 0; i < curr_std_cnt/2; i++) {
                string s1, s2;
                student* ref_s1;
                student* ref_s2;
                int winner, temp_idx;
                curr_round_file >> s1;
                curr_round_file >> s2;
                curr_round_file >> winner;
                curr_round_file.get();
                curr_round_file.get();
                temp_idx = bin_search(std_list, s1);
                ref_s1 = &(std_list[temp_idx]);
                temp_idx = bin_search(std_list, s2);
                ref_s2 = &(std_list[temp_idx]);
                curr_match.s1 = ref_s1;
                curr_match.s2 = ref_s2;
                curr_match.winner = winner;
                curr_bracket.push_back(curr_match);
            }
            curr_round_file.close();
            show_bracket(curr_bracket, curr_std_cnt);
        } else {
            cout << "File not found.\n";
        }
    }
    getline(cin, op);
    return;
}

int main() {
    int setup_ok = setup();
    switch (setup_ok) {
        case 1: 
            cout << "No competitor data found.\n";
            cin.get();
            return 1;
        case 2:
            cout << "Tournament bracket not generated.\n";
            cin.get();
            return 2;
        case 10:
            cout << "Files not in sync. Please delete 'Competitor Results.txt' and generate a bracket again.";
            cin.get();
            return 10;
    }
    op = "";
    do {
        if (std_cnt > 1) {
            show_bracket(bracket, std_cnt);
            cout << "Write 'update' to change results of this round\n";
            cout << "Write 'advance' to create the bracket for next round\n";
            cout << "Write 'query' to search for results\n";
            getline(cin, op);
            if (op == "update") {
                update();
            } else if (op == "advance") {
                advance(std_cnt);
            } else if (op == "query") {
                query();
            }
        } else {
            show_winner(); 
            cout << '\n';
            cout << "Write 'query' to search for results\n";
            getline(cin, op);
            if (op == "query") {
                query();
            }
        }
    } while (op != "end");
    return 0;
}