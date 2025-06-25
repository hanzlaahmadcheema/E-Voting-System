# 🗳️ E-Voting System (C++ | CLI | JSON)

A modular, command-line electronic voting system built with **C++**, **FTXUI**, and **JSON**. Supports voter registration, secure vote casting, and real-time result computation with persistent storage. Developed as a PBL project for Data Structures & Algorithms.

---

## 📌 Features

- 🔐 CNIC-based voter authentication
- 🧾 Candidate & party management
- 🏛️ City, constituency & polling station setup
- 🗳️ Secure, one-vote-per-voter casting
- 📊 Real-time vote counting & winner calculation
- ⚙️ Admin dashboard (login protected)
- 💾 Persistent JSON storage with auto-increment IDs
- 🖥️ Polished CLI interface via FTXUI

---

## 🧱 Technologies Used

| Component      | Tech/Library         |
|----------------|---------------------|
| Language       | C++ (C++20)         |
| UI             | CLI (FTXUI)         |
| Data Storage   | JSON (nlohmann/json)|
| Build System   | CMake               |
| Architecture   | Modular, OOP        |

---

## 📂 Project Structure

```
E-Voting-System/
├── data/           # JSON data files
├── src/
│   ├── models/     # Voter, Candidate, Party, etc.
│   ├── core/       # AdminCLI, UserCLI, VotingSystem
│   ├── cli/        # FTXUI UI components
│   └── main.cpp    # Entry point
├── include/        # Header files
├── CMakeLists.txt
└── README.md
```

---

## 📈 Data Structures Used

- `vector` – dynamic entity storage
- `binary search` – fast ID lookup (e.g., election existence)
- `unordered_map` – vote tallying
- `sorting algorithm` – candidate ranking
- `stack` (planned) – CLI menu navigation
- `queue` (planned) – voter queue management

---

## 📦 JSON-Based File System

**Why JSON?**  
Lightweight, portable, and easy to manage for modular systems.  
Each module persists data as a `.json` file with a defined schema.

```json
// Example: voters.json
[
    {
        "VoterID": 1,
        "VoterName": "John Doe",
        "VoterCNIC": "1234567890123",
        "VoterAge": 30,
        "VoterGender": "Male",
        "VoterAddress": "123 Main St",
        "PollingStationID": 101,
        "ConstituencyID": 1
    }
]
```

**Other files:**  
- elections.json  
- candidates.json  
- cities.json  
- polling_stations.json  
- results.json  
- counters.json (auto-increment IDs)

---

## 🧩 System Modules

- Voter & candidate registration
- Party creation
- City → Constituency → Polling station setup
- Election management
- Vote casting (validation, file write)
- Result tallying (by candidate, constituency)
- Admin login system (JSON-based)
- CLI interface (menus, tables, forms, notifications)

---

## 👥 Team Contributions

**[Hanzla Ahmad](https://github.com/hanzlaahmadcheema)** – Logic & Validation  
- Voting process, result calculation,   ID validation  
- Exception handling, system consistency, entity linkages  
- Example: `castVote()`, `declareWinner()`, `isValidCNIC()`

**[Bilawal Hussain](https://github.com/bilawal-11)** – JSON & Persistence Layer  
- File I/O, `toJSON()` & `fromJSON()` for all modules  
- Handles `loadXYZ()`, `saveXYZ()` for each entity  
- Example: `loadVoters()`, `saveCandidates()`, `counters.json`

**[Meeral Nafees](https://github.com/Me-raaal)** – CLI Interface Developer  
- `manageXYZ()` functions for admin/user interface  
- Built `ShowMenu()`, `ShowForm()`, `ShowTable()` using FTXUI  
- CLI polish, layout consistency, input validation

---

## 🚀 Setup Instructions

### 🛠 Prerequisites

- C++20 compiler (g++ or MSVC)
- CMake
- FTXUI
- nlohmann/json

### ⚙️ Build & Run

```sh
# Clone the repo
git clone https://github.com/hanzlaahmadcheema/E-Voting-System.git
cd E-Voting-System

# Clone FTXUI (required for UI)
git clone https://github.com/ArthurSonzogni/FTXUI.git

# Create build directory
mkdir build && cd build

# Run CMake
cmake ..
cmake --build .

# Run the executable
./bin/Debug/E-Voting-System
```

---

## 🎯 Future Enhancements

- Role-based admins (polling station, constituency, election level)
- GUI version (Qt or Dear ImGui)
- Biometric login (fingerprint/face) – experimental
- Cloud JSON sync (Firebase)

---

## 📄📚 License
MIT License.
Use freely for academic, personal, and demo purposes.

---

## 🧠 Learning Outcomes

- CLI Design with FTXUI
- Object-Oriented Programming
- File Persistence with JSON
- Modular System Architecture
- Real-world DSA application

---

## 👋 Author & Credits
**Hanzla Ahmad (Cheemæ)**  

[🔗 LinkedIn](https://www.linkedin.com/in/hanzlaahmadcheema)  
[📁 GitHub](https://github.com/hanzlaahmadcheema)  

**Bilawal Hussain**  

[🔗 LinkedIn](https://www.linkedin.com/in/bilawal-hussain-196825345)  
[📁 GitHub](https://github.com/bilawal-11)  

**Meeral Nafees**  

[🔗 LinkedIn](https://www.linkedin.com/in/meeral-nafees-a62965310)  
[📁 GitHub](https://github.com/Me-raaal)  

---

Project built with ❤️ for the Data Structures & Algorithms Project-Based Learning (PBL) course at Riphah International University (RSCI), Lahore, Pakistan.  
We hope this project inspires others to explore modular software design, real-world data structures, and modern C++ development!