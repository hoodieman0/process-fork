# ── Stage 1: builder ─────────────────────────────────────────────────────────
FROM debian:bookworm-slim AS builder

# Install build tools — compiler-agnostic; switch clang for gcc if preferred
RUN apt-get update && apt-get install -y --no-install-recommends \
        cmake \
        ninja-build \
        g++ \
        git \
        ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

# Copy source tree
COPY CMakeLists.txt .
COPY src/ src/
COPY tests/ tests/

# Configure (tests enabled) and build
RUN cmake -S . -B build \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_TESTING=ON \
    && cmake --build build --parallel

# ── Stage 2: test runner (optional target) ───────────────────────────────────
FROM builder AS tester
WORKDIR /workspace/build
RUN ctest --output-on-failure

# ── Stage 3: minimal runtime image ───────────────────────────────────────────
FROM debian:bookworm-slim AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
        libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /workspace/build/MyProject .

ENTRYPOINT ["./MyProject"]
