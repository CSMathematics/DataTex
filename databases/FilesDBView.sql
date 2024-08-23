DROP VIEW "FilesDBView";
CREATE VIEW "FilesDBView" AS SELECT df.Id ,ft.Name ,f.Name , replace(group_concat(DISTINCT c.Name),',','|') ,
replace(group_concat(DISTINCT s.Name),',','|') ,replace(group_concat(DISTINCT et.Name),',','|') ,
"Difficulty","Path","Date","Solved_Prooved","Bibliography","FileContent","Preamble","BuildCommand","FileDescription","<additional_columns>",
df.FileType,ft.Solvable,replace(group_concat(DISTINCT t.Tag_Id),',','|')
FROM Database_Files df JOIN FileTypes ft ON ft.Id = df.FileType JOIN Fields f ON f.Id = df.Field
JOIN Chapters_per_File cpf ON cpf.File_Id=df.Id
JOIN Chapters c ON c.Id = cpf.Chapter_Id
JOIN Sections_per_File spf ON spf.File_Id = df.Id
JOIN Sections s ON s.Id = spf.Section_Id
LEFT JOIN Sections_Exercises se ON se.Section_Id = s.Id
LEFT JOIN ExerciseTypes_per_File epf ON epf.File_Id = df.Id
LEFT JOIN Exercise_Types et ON et.Id = epf.ExerciseType_Id
LEFT JOIN Tags_per_File t ON t.File_Id = df.Id
GROUP BY df.Id ORDER BY df.rowid
