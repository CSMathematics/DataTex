# DataTex

DataTex is a tool for creating and managing databases of LaTeX files.

Create and organize a LaTeX file database to store your exercises, proofs, definitions of any other type of file you need. You can easily use your files to create documents through DataTex or any other external TeX editor. Additionally you can use DataTex as a bibliographic manager where you can organise and manage your bibliographic references as well as connect your files and document with the bibliography database.
There are two types of databases that can be created with DataTex.

- Files database
- Documents database
- Bibliography database

---

## LaTeX files database

By "LaTeX file" we mean a single .tex file that can be included in a LaTeX document. This file could be a

- Definition
- Theorem along with it's proof
- Figure
- Table
- Exercise
- Solution of an exercise

or any other type of file that can be included in a project. These files can be stored in sqlite databases by the field, chapter, section that each file belongs. Here are some basic stuff that you can do with DataTex

1. Create a new file
2. Add existing files to a database
3. Edit the metadata of a file
4. Remove a file from a database
5. Create one or several solutions for a file that needs to have one, such as an exercise, problem etc.
6. Clone a file from one database to onother.

Each file can be compiled using one of the basic TeX engines such as PdfLaTeX, XeLaTeX, LaTeX, PythonTeX and have a Pdf view.

The preambles needed to build our files can be created and stored inside the settings database. That way the files can contain only the necessary content, making our database small in size.

![DataTex1](https://user-images.githubusercontent.com/53839524/143144805-9a2de304-2d48-4ab3-a5e1-cd4c2deb3dfd.png)

---

## Document database

When we want to create a new article, book, report or any other kind of document, this project can be stored in a Document Database. In each of these documents we can add or remove files chosen from the LaTeX databases we mentioned above. Some of the basic features are the following:

1. Create a new document.
2. Add files from multiple file databases, to a new or existing document.
3. Remove a document from a database.
4. Add existing documents to the database.
5. Edit metadata.
6. If a project contains exercises, we can create a pdf containing all the solutions.
7. Update content of a document when a file changes.
8. Clone documents to another database.

![DataTexng1](https://user-images.githubusercontent.com/53839524/143144957-1059c126-0adf-41fa-8ec5-22f296b1b219.png)

## Bibliography database

Use DataTex as a bibliography manager. Store bibliographic references in a seperate database which can be used to add citations to the files and documents stored in DataTex. Some basic functions:

- Manually add and import references.
- Export your references as a BibTeX file.
- Organise and manage your bibliography.
- Connect your files and documents with the bibliography database.

![bib](https://user-images.githubusercontent.com/53839524/177306838-d6a0251a-5ab0-4649-a30a-628f719518c6.png)

## Build

Open a terminal in the DataTex folder and run the following commands

```
qmake
make
```
