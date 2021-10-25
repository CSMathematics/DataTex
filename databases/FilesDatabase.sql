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
CREATE TABLE IF NOT EXISTS "Subject_Types" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL,
	"Description"	TEXT,
	PRIMARY KEY("Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "FileTypes" (
	"Id"	TEXT NOT NULL,
	"FileType"	TEXT NOT NULL,
	"FolderName"	TEXT NOT NULL,
	"Description"	TEXT,
	PRIMARY KEY("Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "Sections" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL UNIQUE,
	"Field"	TEXT NOT NULL,
	"Chapter"	TEXT NOT NULL,
	FOREIGN KEY("Field") REFERENCES "Fields"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("Chapter") REFERENCES "Chapters"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	PRIMARY KEY("Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "Sections_Exercises" (
	"Exercise_Id"	TEXT NOT NULL,
	"Exercise_Name"	TEXT NOT NULL,
	"Section_Id"	TEXT NOT NULL,
	FOREIGN KEY("Section_Id") REFERENCES "Sections"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	FOREIGN KEY("Exercise_Id") REFERENCES "Exercise_Types"("Id") ON UPDATE CASCADE
);end_of_query
CREATE TABLE IF NOT EXISTS "BackUp" (
	"Table_Id"	TEXT,
	"Id"	TEXT,
	"Name"	TEXT
);end_of_query
CREATE TABLE IF NOT EXISTS "Bibliography" (
	"Citation_Key"	TEXT,
	"Document_Type"	TEXT,
	"Title"	TEXT,
	"Author"	TEXT,
	"Editor"	TEXT,
	"Publisher"	TEXT,
	"Year"	INTEGER,
	"Month"	INTEGER,
	"ISBN"	INTEGER,
	"ISSN"	INTEGER,
	"Pages"	INTEGER,
	"Series"	TEXT,
	"Volume"	INTEGER,
	"Journal"	TEXT,
	"School/Institute"	TEXT,
	"Number/Issue"	INTEGER,
	"Address"	TEXT,
	"DOI"	INTEGER,
	"URL"	TEXT,
	"Edition"	TEXT,
	PRIMARY KEY("Citation_Key")
);end_of_query
CREATE TABLE IF NOT EXISTS "Chapters" (
	"Id"	TEXT NOT NULL,
	"Name"	TEXT NOT NULL UNIQUE,
	"Field"	TEXT NOT NULL,
	FOREIGN KEY("Field") REFERENCES "Fields"("Id") ON UPDATE CASCADE ON DELETE CASCADE,
	PRIMARY KEY("Id")
);end_of_query
CREATE TABLE IF NOT EXISTS "Database_Files" (
	"Id"	TEXT NOT NULL,
	"FileType"	TEXT,
	"Field"	TEXT,
	"Chapter"	TEXT,
	"Section"	TEXT NOT NULL,
	"ExerciseType"	TEXT,
	"Difficulty"	INTEGER,
	"Path"	TEXT NOT NULL,
	"Date"	TEXT,
	"Solved"	TEXT,
	"Bibliography"	TEXT,
	"FileContent"	TEXT,
	"Preamble"	TEXT,
	"BuildCommand"	TEXT,
	"FileDescription"	TEXT,
	FOREIGN KEY("Section") REFERENCES "Sections"("Id") ON UPDATE CASCADE,
	FOREIGN KEY("Field") REFERENCES "Fields"("Id") ON UPDATE CASCADE ON DELETE SET NULL,
	FOREIGN KEY("ExerciseType") REFERENCES "Exercise_Types"("Id") ON UPDATE CASCADE ON DELETE SET NULL,
	PRIMARY KEY("Id","Section"),
	FOREIGN KEY("FileType") REFERENCES "FileTypes"("Id") ON UPDATE CASCADE ON DELETE SET NULL,
	FOREIGN KEY("Chapter") REFERENCES "Chapters"("Id") ON UPDATE CASCADE ON DELETE SET NULL
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
