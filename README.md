Program Manual

Setup:
1. Open a new folder.
2. Download the .exe files "DB_Grouping.exe" & "Results.exe" and place them in the new folder.
3. Open a new folder named "Data files".
4. Execute "DB_Grouping.exe" to start entering data.

"DB_Grouping.exe" stores all student information in a text file "Compdata.txt" in the folder "Data files".
"Compdata.txt" acts as a database when used by DB_Grouping.exe.

Commands:

list: 
List all student names, schools and corresponding student IDs.
A '*' at the beginning of a name indicates a seed player.

search:
Find information of a student based on his/her student ID.

add:
Add a new student into the database.

update:
Change the information of a student given his/her corresponding student ID.

delete:
Removes a student from the database.

rmseed:
Turns a seed player into a non-seed player given his/her student ID.

save:
Saves edited student information into "Compdata.txt".
Note that all changes made in DB_Grouping.exe will not be saved until the "save" command is executed.
Once you save current information, you must restart the program to continue inputting new information.

create:
Creates a tournament bracket based on the current student data.
After a bracket is created, you need to confirm whether the generated bracket should be used.
If so, a new text file "Competitor Results.txt" will be created to store competition data.
After a bracket is generated, "DB_Grouping.exe" CANNOT be used to edit student information.

