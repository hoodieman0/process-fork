# Process Forking Learning

Me learning how to properly use unix processes.

---

## Architecture
* Setup
    * Create all processes from a single executable.
* Message Broker
    * Central place to process messages and pass them to the correct service.
* Logger
    * Handles log messages from across the system.
* Producers
    * Inputs data into the system.
    * Hands off data messages to the Message Broker service.
* Consumers
    * Processes data from producers.
    * TODO: send to database.
* Controller
    * User facing control software to manipulate the system.

---

## IPCs
* Message Broker
    * Named pipes for control line, producer/consumer registration, logger, producer data in line and control line, and consumer data out line and control line.
* Producer
    * Named pipes for control line, data out line, and logger.
* Consumer
    * Named pipes for control line, data in line, and logger.
* Logger
    * Named pipe for log message in.
* Controller
    * Named pipe for control message out.

### Producer/Consumer Registration
#### Request Register
* signature (4 bytes): miku
* type (1 byte): 1 for producer, 2 for consumer
* id len (1 byte):
* id (string, n bytes)
* null terminator (1 byte)

#### Confirm Register
* signature (4 bytes): miku
* ack (1 byte): \0x06
* uid (1 byte)
* null terminator (1 byte)

#### Deny Register
* signature (4 bytes): miku
* nack (1 byte): \0x25
* null terminator (1 byte)

### Data Messages
* signature (4 bytes): miku
* producer uid (1 byte)
* frame # (4 bytes, uint)
* msg len (4 bytes, uint): n
* msg (n bytes)
* null terminator (1 byte)

### Control Commands
* signature (4 bytes): miku
* cmd id (1 byte)
* cmd specific message
* null terminator (1 byte)

#### Command IDs
- 1: Start
- 2: Stop
- 3: Restart
- 4: Status
    - Add fd (4 bytes)
- 5: Dump
    - Add fd (4 bytes)

## Prerequisites

| Tool | Minimum version | Notes |
|------|-----------------|-------|
| CMake | 3.20 | Any C++17-capable compiler works (gcc, clang, MSVC) |
| C++ compiler | C++17 support | gcc 9+, clang 9+, MSVC 2019+ |
| Docker | 20.10 | Only needed for Docker builds |
| Docker Compose | v2 | Bundled with Docker Desktop |

---

## Local Builds

### 1. Configure

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

To enable tests during configuration:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
```

### 2. Build

```bash
cmake --build build --parallel
```

### 3. Run

```bash
./build/MyProject
```

---

## Running Tests Locally

Tests are built automatically when `BUILD_TESTING=ON` is passed to CMake.  
GoogleTest is fetched automatically at configure time via `FetchContent`.

```bash
# Configure with tests enabled (if not already done)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON

# Build
cmake --build build --parallel

# Run all tests
ctest --test-dir build --output-on-failure
```

---

## Docker Builds

The `Dockerfile` uses a **multi-stage build** with three named targets:

| Target | Purpose |
|--------|---------|
| `builder` | Compiles the project (tests included) |
| `tester` | Runs `ctest` — exits non-zero on any failure |
| `runtime` | Minimal image containing only the compiled binary |

### Build and run the application

```bash
docker compose up app
```

### Build and run tests only

```bash
docker compose up test
```

The `test` service exits with a non-zero code if any test fails, making it CI-friendly.

### Build a specific Docker stage manually

```bash
# Just compile
docker build --target builder -t myproject:builder .

# Run tests
docker build --target tester -t myproject:test .

# Production image
docker build --target runtime -t myproject:latest .
docker run --rm myproject:latest
```

---

## VS Code Integration

Open the repository folder in VS Code. When prompted, install the recommended extensions (`.vscode/extensions.json`).

### Build

Press <kbd>Ctrl+Shift+B</kbd> (or <kbd>Cmd+Shift+B</kbd> on macOS) — runs **CMake: Build (Debug)**.

### Run Tests

Open the Command Palette (<kbd>Ctrl+Shift+P</kbd>) → **Tasks: Run Test Task** → **CTest: Run Tests**.

### Debug

Press <kbd>F5</kbd>. Two configurations are provided in `.vscode/launch.json`:

- **Debug (gdb)** — for Linux / WSL
- **Debug (lldb)** — for macOS or when using the CodeLLDB extension

---

## Customising the Template

1. Rename the project in the top-level `CMakeLists.txt` (`project(MyProject …)`).
2. Replace `src/add.hpp` and update `src/main.cpp` with your own source files.
3. Add or replace tests in `tests/test_add.cpp`.
4. Update the binary name referenced in `.vscode/launch.json` and the `Dockerfile` `COPY` line if you rename the project.

---

## License

Replace this section with your chosen license.
