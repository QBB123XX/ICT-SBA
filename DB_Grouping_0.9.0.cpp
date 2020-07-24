#include <bits/stdc++.h>
#include <fstream>
using namespace std;

class student {
	public:
	string name;
	string school;
	string sid;	
};
 
int op, stdcnt, target_idx, end_prog, chunk_size, chunk_num;
string inp;
student target;
vector <string> seed;
vector<student> std_list;
fstream stddata, results;

struct {
    bool operator () (student const &a, student const &b) const {
    return a.sid < b.sid;
    }
} sid_ascend;

struct {
    bool operator () (student const &a, student const &b){
    if (a.school == b.school) return a.name < b.name;
	else return a.school < b.school;
    }
} school_ascend;

struct {
    bool operator () (student const &a, student const &b){
    if ((a.name[0] == '*') != (b.name[0] == '*')) {
        return b.name[0] == '*';
    }
    if (a.school == b.school) return a.name < b.name;
    else return a.school < b.school;
    }
} school_seed_ascend;

int bin_search(vector<student> list, string id) { //searches for idx of student in list given student id
	//binary search
    int l = 0, r = stdcnt-1, mid;
	while (l <= r) {
		mid = (l+r)/2;
		if (id < list[mid].sid) r = mid - 1;
		else if (id > list[mid].sid) l = mid + 1;
		else return mid;
	}
    //sid not found
	return stdcnt;
}

void tidy () { //sort items in std_list
	sort(std_list.begin(), std_list.end(), sid_ascend);
	stdcnt = std_list.size();
}

void list_students () {
	stdcnt = std_list.size();
	cout << "Student number = " << stdcnt << '\n';
    const int col[2] = {30, 10};
    cout << left;
    cout << setw(col[0]) << "Name";
    cout << setw(col[0]) << "School";
    cout << setw(col[1]) << "Student ID";
    cout << '\n';
	for (int j = 0; j < stdcnt; j++) {
		target = std_list[j];
        cout << left;
		cout << setw(col[0]) << target.name;
		cout << setw(col[0]) << target.school;
		cout << setw(col[1]) << target.sid;
		cout << '\n';
	}
	cout << "Top Seeds: ";
	for (int i = 0; i < seed.size(); i++) {
		cout << seed[i] << ' ';
	}
	cout << '\n';
}

int search() { //executes when 'search' function is typed
	cout << "Enter student number\n";
	getline(cin, target.sid);
	target_idx = bin_search(std_list, target.sid);
	if (target_idx == stdcnt) {
		cout << "Student not found.\n";
	} else {
		target.name = std_list[target_idx].name;
		target.school = std_list[target_idx].school;
		cout << "Student found.\n";
		cout << "Name: " << target.name << "\n";
		cout << "School: " << target.school << "\n";
	}
    return target_idx;
}

void rm_seed(int idx) { //removes seed status of a student
    bool found;
    if (std_list[idx].name[0] == '*') {
        std_list[idx].name.erase(std_list[idx].name.begin());
	    for (int i = 0; i < seed.size(); i++) {
            //linear search for seed
		    if (seed[i] == target.sid) {
			    seed.erase(seed.begin()+i);
                found = true;
			    break; 
		    }
	    }
        cout << "Seed info of competitor removed.\n";
    } else cout << "The competitor was not a top seed.\n";
} 

void add_seed() { //executes when a new student is added or when a student info is updated
    cout << "Did the student finish in the top 4 last year? Enter 'Y' if true\n";
    string inp = "";
    char first_chr = ' ';
    //input validation
    while (first_chr != 'y' || first_chr != 'n') {
        getline (cin, inp);
        first_chr = tolower(inp[0]);
    }
	if (first_chr == 'y') {
        if (find(seed.begin(), seed.end(), target.sid) != seed.end()) {
            cout << "The competitor was already a seed competitor.\n";
            return;
        }
        //check if max seeds reached
		if (seed.size() < 4) {
            //add seed
			target.name = "*" + target.name;
			seed.push_back(target.sid);
		} else {
			cout << "Cannot add seed as maximum number of seeds has been reached.\nPlease edit details of other seed competitors.\n";
		}
	} else {
        //remove seed
        int idx = 0;
        while (idx < seed.size() && seed[idx] != target.sid) {
            idx++;
        }
        //if linear search successful erase the item
        if (idx < seed.size()) seed.erase(seed.begin() + idx);
    }
}

void add(){
    //input
	cout << "Enter name of student: \n";
	getline(cin, target.name);
	cout << "Enter school of student:\n";
	getline(cin, target.school);
	cout << "Enter student ID:\n";
	getline(cin, target.sid);
    int idx = bin_search(std_list, target.sid);
	if (idx != stdcnt) {
		cout << "This student ID has appeared in the database before. The information will not be added.\n";
		return;
	}
	add_seed();
    //input validation
	cout << "Is the student's data correct? (Enter 'Y' or 'N')\n";
	getline(cin, inp);
	if (inp == "Y" || inp == "y") {
		std_list.push_back(target);
		cout << "Student will be added at the end of the program.\n";
	} else {
		cout << "Action cancelled.\n";
	}
    //sort std_list to allow searching
	sort(std_list.begin(), std_list.end(), sid_ascend);
    stdcnt = std_list.size();
}

void update(int idx) {
    //inputs
	cout << "Enter correct name: \n";
	getline(cin, target.name);
	cout << "Enter correct school: \n";
	getline(cin,target.school);
	add_seed();
    //input validation
	cout << "Confirm changes?\n";
	getline(cin, inp);
    if (tolower(inp[0]) == 'y') {
        //modify element
	    target.sid = std_list[idx].sid;
	    std_list[idx] = target;
        //sort list after modification
	    sort(std_list.begin(), std_list.end(), sid_ascend);
        stdcnt = std_list.size();
	    cout << "Information will be updated when the program ends.\n";
    } else {
        cout << "Action cancelled.\n";
    }
}

void remove(int idx) {
    //confirm removal
	cout << "Remove student " << std_list[idx].name << "? (Enter 'Y' to confirm changes)" ;
	getline (cin, inp);
    //validation
	if (inp == "Y" || inp == "y") {
        rm_seed(idx);
		std_list.erase(std_list.begin()+idx);
		cout << "Competitor will be removed at the end of the program.";
	} else {
		cout << "Action cancelled.";
	}
	cout << "\n";
}

int setup() {
    results.open(".\\Data files\\Competitor Results.txt", fstream::in | fstream::out);
    if (results.is_open()) return 1; //stop the program when results file is generated
    results.close();
	stddata.open(".\\Data files\\Compdata.txt", fstream::in | fstream::out);
    //read stddata until end of file reached
	while (stddata.peek() != EOF) {
	    getline (stddata, target.name);
		getline (stddata, target.school);
		getline (stddata, target.sid);
		std_list.push_back(target);
        if (target.name[0] == '*') {
			seed.push_back(target.sid);
        }
	}
    //update stdcnt
	stdcnt = std_list.size();
    //finish setup
	cout << "Setup complete\n\n";
	stddata.close();
    return 0;
}

int commands() {
    string curr;
    unordered_map <string, int> cmds = { //stores available commands
        {"list", 1},
        {"search", 2},
        {"add", 3},
        {"update", 4},
        {"delete", 5},
        {"rmseed", 6},
        {"save", 0},
        {"create", -1}
    };
	cout << "Enter 'list' to list all competitors\n";
	cout << "Enter 'search' to search for competitor\n";
	cout << "Enter 'add' to add new competitor\n";
	cout << "Enter 'update' to update competitor information\n";
	cout << "Enter 'delete' to delete competitor\n";
    cout << "Enter 'rmseed' to remove seed status\n\n";
    cout << "Enter 'save' to save competitor info.\n";
    cout << "Enter 'create' to create tournament bracket.\n";
    getline(cin, curr);
    //command validation
	while(cmds.count(curr) == 0) {
 	    cin.clear();
 	    cin.ignore(999,'\n');
 	    cout<<"Invalid command.\n";
    }
    op = cmds[curr];
	switch (op) {
        case -1: {
            cout << "Note that this program cannot be used after the bracket is created.\n";
            cout << "Continue?\n";
            getline(cin, inp);
            break;
        }
        case 0: break;
		case 1: 
		{
			list_students();
			break;
		}
		case 2: 
		{
			search();
			break;
		}
		case 3:
		{
			add();
			break;
		}
		case 4:
		{
			int idx = search();
			if (idx != stdcnt) update(idx);
			break;
		}
		case 5:{
			int idx = search();
			if (idx != stdcnt) remove(idx);
			break;
		}
        case 6:{
            int idx = search();
            if (idx != stdcnt) rm_seed(idx);
            break;
        }
	}
    return op;
}

void finish() {
    //saves data
	stdcnt = std_list.size();
	stddata.open(".\\Data files\\Compdata.txt", fstream::out | fstream::trunc);
	for (int i = 0; i < stdcnt; i++) {
		stddata << std_list[i].name << '\n';
		stddata << std_list[i].school << '\n';
		stddata << std_list[i].sid << '\n';
	}
	stddata.close();
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

student make_bye() {
    student bye;
    bye.name = "(bye)";
    bye.school = "bye";
    bye.sid = "bye";
    return bye;
}

void group_randomize(int l, int r) { //randomizes std_list from l to r (inclusive)
    int swap_idx;
    srand(time(NULL));
    auto start = std_list.begin();
    for (int i = r; i > l; i--) {
        swap_idx = l + rand() % (i-l+1); //choose a student to swap with
        iter_swap(start + i, start + swap_idx);  //swap students
    }
    return;
}

void show_bracket() {
    cout << '\n';
    //round name
    switch (stdcnt) {
        case 2:
            cout << "Finals\n";
            break;
        case 4:
            cout << "Semifinals\n";
            break;
        default:
            cout << "Round of " << stdcnt << '\n';
            break;
    }
    //formatting + column names
    const int col1 = 10, col2 = 30, col3 = 5;
    cout << left;
    cout << setw(col1) << "Player 1" << setw(col2) << "School" << setw(col3) << "Seed";
    cout << setw(col1) << "Player 2" << setw(col2) << "School" << setw(col3) << "Seed";
    cout << "Winner" << '\n';
    //table contents
    for (int i = 0; i < stdcnt; i += 2) {
        student stu_1 = std_list[i], 
        stu_2 = std_list[i+1]; 
        cout << left;
        cout << setw(col1) << stu_1.sid << setw(col2) << stu_1.school << setw(col3) << (stu_1.name[0] == '*');
        cout << setw(col1) << stu_2.sid << setw(col2) << stu_2.school << setw(col3) << (stu_2.name[0] == '*'); 
        if (stu_1.school == "bye")  cout << 2;
        else if (stu_2.school == "bye")  cout << 1;
        else cout << 0;
        cout << '\n';
    }
}

void make_bracket(int l, int r) {
    //safe return
    if (l == r) return;
    //check if all people have distinct schools 
    bool distinct = true;
    unordered_set<string> schools;
    for (int i = l; i <= r; i++) {
        if (schools.count(std_list[i].school) > 0) {
            distinct = false;
            break;
        } else {
            schools.insert(std_list[i].school);
            if (std_list[i].name[0] == '*') {
                if (schools.count("Seed") == 0) {
                    schools.insert("Seed");
                } else {
                    distinct = false;
                    break;
                }
            }
        }
    }
    //debugging code (remove later)
    cout << l << ' ' << r << '\n';
    cout << distinct << '\n';
    for (string x: schools) {
        cout << x << ' ';
    }
    //end debugging code
    cout << '\n';
    if (distinct) {
        //randomise groups
        group_randomize(l, r);
    } else {
        //split the range into 2 parts and call the function recursively
        vector <student> a, b;
        for (int i = l; i <= r; i += 2) {
            a.push_back(std_list[i]);
            if (i+1 <= r) b.push_back(std_list[i+1]);
        }
        a.insert(a.end(), b.begin(), b.end());
        for (int i = l; i <= r; i++) {
            std_list[i] = a[i-l];
        }
        int mid = (l+r)/2;
        make_bracket(l, mid);
        make_bracket(mid+1, r);
    }
}

void save_bracket() { //create results file
    results.open(".\\Data files\\Competitor Results.txt", fstream::out);
    results << stdcnt << '\n';
    for (int i = 0; i < stdcnt; i += 2) {
        student std1 = std_list[i], std2 = std_list[i+1];
        results << std1.sid << '\n';
        results << 1 << '\n';
        results << std2.sid << '\n';
        results << std2.sid << '\n';
        results << 1 << '\n';
        results << std1.sid << '\n';
        results << 0 << "\n\n";
    }
    results.close();
}

void create_tournament_bracket() {
    cout << '\n';
    //insert 'bye' until stdcnt is a power of 2
    stdcnt = next_power(stdcnt);
    const student bye = make_bye();
    while (std_list.size() < stdcnt) {
        std_list.push_back(bye);
    }
    do {
        //sort by school and seed
        sort (std_list.begin(), std_list.end(), school_seed_ascend);
        //randomise students in each school
        int l = 0, r = 0;
        while (r+1 < stdcnt && std_list[r+1].sid != "bye") {
            if (std_list[r].school != std_list[r+1].school) {
                group_randomize(l, r);
                l = r+1;
            }
            r++;
        } 
        //randomise students in last school
        group_randomize(l, r);
        make_bracket(0, stdcnt-1);
        cout << '\n';
        show_bracket();
        cout << "Confirm? (Type 'y' or 'yes' to confirm, type 'n' or 'no' to re-generate)\n";
        inp = "";
        while (inp != "y" && inp != "yes" && inp != "n" && inp != "no"){
            getline(cin, inp);
            int idx = 0;
            while (inp[idx]) {
                inp[idx] = tolower(inp[idx]);
                idx++;
            }
        }
    } while (inp != "y" && inp != "yes");
    save_bracket();
    cout << "Bracket saved.\n";
}

int main() {
    srand(time(NULL));
	op = setup();
    switch (op) {
        case 1:
            cout << "Tournament bracket has already been generated.\n";
            cout << "To reconstruct the tournament bracket, please delete 'Competitor Results.txt'.\n";
            getline(cin, inp);
            return 1;
    }
	do {
        cout << '\n';
		end_prog = commands();
	} while (end_prog > 0);
    cout << "Finishing program...\n";
	finish();
    if (end_prog == -1) create_tournament_bracket();
    cout << "All actions processed. You may close this program.\n";
    cin >> inp;
    return 0;
}
