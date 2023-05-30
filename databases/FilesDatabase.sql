CREATE TABLE IF NOT EXISTS "Fields" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL UNIQUE,
	"Description"	TEXT,
	PRIMARY KEY("Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "Exercise_Types" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL,
	PRIMARY KEY("Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "BackUp" (
	"Table_Id"	TEXT,
	"Id"	TEXT,
	"Name"	TEXT
);end_of_query
CREATE TABLE IF NOT EXISTS "Chapters" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL UNIQUE,
	"Field"	TEXT NOT NULL,
	FOREIGN KEY("Field") REFERENCES "Fields"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	PRIMARY KEY("Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "FileTypes" (
	"Id"	TEXT NOT NULL,
	"FileType"	TEXT NOT NULL,
	"FolderName"	TEXT NOT NULL,
	"Solvable"	INTEGER,
	"BelongsTo"	TEXT,
	PRIMARY KEY("Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "Solutions_per_File" (
	"Solution_Id"	TEXT,
	"Solution_Path"	TEXT,
	"File_Id"	TEXT,
	FOREIGN KEY("Solution_Id") REFERENCES "Database_Files"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("File_Id") REFERENCES "Database_Files"("Id") ON UPDATE CASCADE ON DELETE CASCADE
);end_of_query
CREATE TABLE IF NOT EXISTS "CustomTags" (
	"Tag"	TEXT,
	PRIMARY KEY("Tag")
);end_of_query
CREATE TABLE IF NOT EXISTS "Tags_per_File" (
	"Tag_Id"	TEXT,
	"File_Id"	TEXT,
	FOREIGN KEY("File_Id") REFERENCES "Database_Files"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("Tag_Id") REFERENCES "CustomTags"("Tag") ON UPDATE CASCADE ON DELETE CASCADE
);end_of_query
CREATE TABLE IF NOT EXISTS "Sections" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL UNIQUE,
	"Chapter"	TEXT NOT NULL,
	FOREIGN KEY("Chapter") REFERENCES "Chapters"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	PRIMARY KEY("Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "Chapters_per_File" (
	"File_Id"	TEXT,
	"Chapter_Id"	TEXT,
	FOREIGN KEY("File_Id") REFERENCES "Database_Files"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("Chapter_Id") REFERENCES "Chapters"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	UNIQUE("File_Id","Chapter_Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "ExerciseTypes_per_File" (
	"File_Id"	TEXT,
	"ExerciseType_Id"	TEXT,
	UNIQUE("File_Id","ExerciseType_Id"),
	FOREIGN KEY("ExerciseType_Id") REFERENCES "Exercise_Types"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("File_Id") REFERENCES "Database_Files"("Id") ON UPDATE CASCADE ON DELETE CASCADE
);end_of_query
CREATE TABLE IF NOT EXISTS "Sections_per_File" (
	"File_Id"	TEXT,
	"Section_Id"	TEXT,
	UNIQUE("File_Id","Section_Id"),
	FOREIGN KEY("File_Id") REFERENCES "Database_Files"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("Section_Id") REFERENCES "Sections"("Id") ON UPDATE CASCADE ON DELETE CASCADE
);end_of_query
CREATE TABLE IF NOT EXISTS "Sections_Exercises" (
	"Exercise_Id"	TEXT NOT NULL,
	"Section_Id"	TEXT NOT NULL,
	FOREIGN KEY("Section_Id") REFERENCES "Sections"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("Exercise_Id") REFERENCES "Exercise_Types"("Id") ON UPDATE CASCADE,
	UNIQUE("Exercise_Id","Section_Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "Database_Files" (
	"Id"	TEXT NOT NULL,
	"FileType"	TEXT,
	"Field"	INTEGER,
	"Difficulty"	INTEGER,
	"Path"	TEXT NOT NULL,
	"Date"	TEXT,
	"Solved_Prooved"	TEXT,
	"Bibliography"	TEXT,
	"FileContent"	TEXT,
	"Preamble"	TEXT,
	"BuildCommand"	TEXT,
	"FileDescription"	TEXT,
	"Class"	TEXT,
	"Year"	INTEGER,
	"Semester"	INTEGER,
	PRIMARY KEY("Id"),
	FOREIGN KEY("FileType") REFERENCES "FileTypes"("Id") ON UPDATE CASCADE,
	FOREIGN KEY("Field") REFERENCES "Fields"("Id") ON UPDATE CASCADE ON DELETE SET NULL
);end_of_query
CREATE TABLE IF NOT EXISTS "Bib_Entries_per_File" (
	"Bib_Id"	TEXT,
	"File_Id"	TEXT,
	FOREIGN KEY("File_Id") REFERENCES "Database_Files"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	UNIQUE("Bib_Id","File_Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "Edit_History" (
	"File_Id"	TEXT,
	"Date_Time"	TEXT,
	"Modification"	TEXT,
	"FileContent"	TEXT,
	"Metadata"	TEXT,
	FOREIGN KEY("File_Id") REFERENCES "Database_Files"("Id") ON UPDATE CASCADE ON DELETE CASCADE
);end_of_query
CREATE TRIGGER Delete_Exercise_Type
AFTER DELETE
ON Sections_Exercises
BEGIN
DELETE FROM Exercise_Types
WHERE Id <> '-' AND Id NOT IN (SELECT DISTINCT Exercise_Id FROM Sections_Exercises);
END;end_of_query
CREATE TRIGGER Delete_Exercise_Type_on_Update
AFTER UPDATE
ON Sections_Exercises
BEGIN
DELETE FROM Exercise_Types
WHERE Id <> '-' AND Id NOT IN (SELECT DISTINCT Exercise_Id FROM Sections_Exercises);
END;
