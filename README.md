# Library Management System

An engineering submission for an internship project showcasing a console-based Library Management System developed in C++. The system models structured inventory tracking constraints using Object-Oriented Programming (OOP) paradigms and binary streams to govern physical library transactions natively.

## 🚀 Key Features
* **Inventory Entry Mapping:** Allows direct cataloging of new books alongside independent library member profiles.
* **Smart Content Discovery:** Search for volumes dynamically via string filtering matching against indexed book titles or author records.
* **Transactional Tracking Engine:** Fully functional book issue and automatic return loops that actively check and modify dynamic availability flags.
* **Persistent Binary Storage:** Saves database metrics inside structured data configurations (`books.dat` and `members.dat`), preserving logs between runs.

## 🛠️ Technical Concepts Used
* **Multi-Class Infrastructure:** Employs explicit Entity definitions handling segmented state values independently.
* **Sub-String Query Processing:** Includes dynamic array searches to verify custom filter conditions.
* **File Stream Pointer Shifting:** Executes granular read/write data blocks through positional file seek functions (`seekp`).

## 📂 Architecture Map
* `main.cpp` - Holds primary implementation routines, query handling, and file parsing branches.
* `books.dat` - Automatically managed binary ledger for keeping tracks of active inventory states.
* `members.dat` - Digital profile record storage holding validation parameters for checked members.
