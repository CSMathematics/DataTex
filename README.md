# DataTex
DataTex is a tool for creating and managing databases of LaTeX files.
There are two types of databases that can be created with DataTex. 
  - Files database 
  - Documents database
  ---
## LaTeX files database
By "LaTeX file" we mean a single .tex file that can be included in a LaTeX document. This file could be a
  - Definition
  - Theorem along with it's proof
  - Figure
  - Table
  - Exercise
  - Solution of an exercise

or any other type of file that can be included in a project. These files can be stored in sqlite databases by the field, chapter, section that each file belongs. Some of the main features are

  1. Create a new file
  1. Add existing files to a database
  1. Edit the metadata of a file
  1. Remove a file from a database
  1. Create one or several solutions for a file that needs to have one, such as an exercise, problem etc.

Each file can be compiled using one of the basic TeX engines such as PdfLaTeX, XeLaTeX, LaTeX, PythonTeX and have a Pdf view.

The preambles needed to build our files can be created and stored inside the settings database. That way the files can contain only the necessary content, making our database small in size.

![DataTex1](https://user-images.githubusercontent.com/53839524/143144805-9a2de304-2d48-4ab3-a5e1-cd4c2deb3dfd.png)

---
## Document database
When we want to create a new article, book, report or any other kind of document, this project can be stored in a Document Database. In each of these documents we can add or remove files chosen from the LaTeX databases we mentioned above. Some of the basic features are the following

  1. Create a new document
  1. Add files from several LaTeX database, to a new or existing document
  1. Remove a document from a database
  1. Add existing documents to the database
  1. Edit metadata
  1. If a project contains exercises, we can create a pdf containing all the solutions.
  1. Update content of a document if a file changes.

![DataTexng1](https://user-images.githubusercontent.com/53839524/143144957-1059c126-0adf-41fa-8ec5-22f296b1b219.png)

## Build

Open a terminal in the DataTex folder and run the following commands

  ```
  qmake
  make
  ```
