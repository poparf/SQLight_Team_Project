# SQLite Clone C++

This project was done as an assignment for OOP course lead by Boja Catalin at CSIE.

# Command processor

The program recognizes commands using the **regex** library from C++. For some commands, it shows the exact place where a syntax error was made.

Avaliable commands:

 - CREATE TABLE
 - CREATE INDEX
 - SELECT
 - IMPORT
 - INSERT
 - DISPLAY TABLE
 - DELETE FROM
 - DROP INDEX
 - DROP TABLE

Also you can generate XML or CSV reports about a table using: 

 - /toggle XML
 - /toggle CSV
 
 ## Files
 All data is saved as a string and for each column we register the actual data type: text, integer or float. The tables are saved in a binary format on disk. There is no B-Tree. (yet)

## How to start the project

TODO:

## Presentation

TODO:
