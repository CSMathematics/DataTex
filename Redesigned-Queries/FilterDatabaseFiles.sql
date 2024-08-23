SELECT "df"."Id" ,"ft"."FileType" ,"f"."Name" , replace(group_concat(DISTINCT c.Name),',','-'), 
replace(group_concat(DISTINCT s.Name),',','-') ,replace(group_concat(DISTINCT et.Name),',','-') ,"Difficulty","Path","Date","Solved","Bibliography",
"FileContent","Preamble","BuildCommand","FileDescription","MultiSection","Class","Year","Semester","df"."FileType","ft"."Solvable"
FROM "Database_Files" "df" 
JOIN "FileTypes" "ft" ON "ft"."Id" = "df"."FileType" 
JOIN "Fields" "f" ON "f"."Id" = "df"."Field" 
JOIN "Chapters_per_File" "cpf" ON "cpf"."File_Id"="df"."Id"
JOIN "Chapters" "c" ON "c"."Id" = "cpf"."Chapter_Id" 
JOIN "Sections_per_File" "spf" ON "spf"."File_Id" = "df"."Id" 
JOIN "Sections" "s"  ON "s"."Id" = "spf"."Section_Id"
LEFT JOIN "Sections_Exercises" "se" ON "se"."Section_Id" = "s"."Id"
LEFT JOIN "ExerciseTypes_per_File" "epf" ON "epf"."File_Id" = "df"."Id"
LEFT JOIN "Exercise_Types" "et" ON "et"."Id" = "epf"."ExerciseType_Id"

WHERE "df"."Id" LIKE "%%"  
AND "ft"."FileType" LIKE "%%"  
AND "f"."Name" LIKE "%%"  
AND "c"."Name" LIKE "%%"
AND "s"."Name" LIKE "%%"  
AND "et"."Name" LIKE "%%"
AND "df"."Difficulty" LIKE "%%"
AND "df"."Date" LIKE "%%"
AND "df"."Solved" LIKE "%%"
AND "df"."Preamble" LIKE "%%" 
AND "df"."BuildCommand" LIKE "%%"
AND ("df"."FileDescription" LIKE "%%" OR "df"."FileDescription" ISNULL)
-- AND ("df"."Class" LIKE "%%" OR "df"."Class" ISNULL) 
-- AND ("df"."Year" LIKE "%%" OR "df"."Year" ISNULL) 
-- AND ("df"."Semester" LIKE "%%" OR "df"."Semester" ISNULL)
GROUP BY df.Id 
ORDER BY df.rowid;