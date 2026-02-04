# Use CirrusLabs Android SDK 34
FROM --platform=linux/amd64 ghcr.io/cirruslabs/android-sdk:34
# Switch to root
USER root

# 1. Install System Tools & Java 17
# We install it, but we WON'T hardcode the ENV variable yet
RUN apt-get update && apt-get install -y \
    openjdk-17-jdk-headless \
    python3 \
    python3-pip \
    zip \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# 2. Automatically set JAVA_HOME
# This command asks the system "Where is java?" and sets the variable
ENV JAVA_HOME="/usr/lib/jvm/java-17-openjdk-amd64"

# SAFETY NET: If the path above is wrong, we symlink the default java to it
# This ensures ./gradlew finds it regardless of the folder name
RUN if [ ! -d "$JAVA_HOME" ]; then \
      ln -s /usr/lib/jvm/java-17-openjdk-* $JAVA_HOME || true; \
    fi

# 3. Setup Workspace
WORKDIR /app

# 4. Install Python Libraries
RUN pip3 install --break-system-packages --ignore-installed \
    google-cloud-storage \
    pycryptodome \
    protobuf

# 5. Copy Project Files
COPY gradlew .
COPY gradle gradle
COPY build.gradle .
COPY settings.gradle .
COPY gradle.properties .
COPY app app
COPY tools tools

# 6. Copy Worker Script
COPY factory/cloud_worker.py /app/cloud_worker.py

# 7. Entrypoint
ENTRYPOINT ["python3", "/app/cloud_worker.py"]