import os
import sys
import subprocess
import shutil
from google.cloud import storage

# --- CONFIG ---
BUILD_KEY = os.environ.get("BUILD_KEY")
USER_ID = os.environ.get("USER_ID", "local-test-user")
BUCKET_NAME = os.environ.get("BUCKET_NAME")

def log(msg):
    print(f"🏭 [FACTORY]: {msg}", flush=True)

def run_command(cmd):
    result = subprocess.run(cmd, shell=True, text=True)
    if result.returncode != 0:
        log(f"❌ Command Failed: {cmd}")
        sys.exit(1)

def main():
    if not BUILD_KEY:
        log("❌ Error: BUILD_KEY is missing.")
        sys.exit(1)

    log(f"Starting Build for User: {USER_ID}")

    # 1. Inject Key (Create dummy model for the tool)
    os.makedirs("app/src/main/assets", exist_ok=True)
    with open("app/src/main/assets/dummy.tflite", "wb") as f:
        f.write(b"dummy content")

    log("⚙️  Running TensorSeal Tool...")
    run_command(f"python3 tools/tensorseal.py --input app/src/main/assets/dummy.tflite --output app/src/main/assets/dummy.lock --header app/src/main/cpp/SecretKey.h --key {BUILD_KEY}")

    # 2. Build Release AAR (Library)
    log("🐘 Running Gradle Build...")
    run_command("chmod +x gradlew")
    run_command("./gradlew :app:assembleRelease --no-daemon")

    aar_path = "app/build/outputs/aar/app-release.aar"
    if not os.path.exists(aar_path):
        log("❌ AAR not found after build! (Check if build.gradle is set to 'com.android.library')")
        sys.exit(1)

    # 3. Package Artifacts (The Fix)
    # We create a temporary "staging" folder inside the container
    staging_dir = "staging_temp"
    if os.path.exists(staging_dir):
        shutil.rmtree(staging_dir)
    os.makedirs(staging_dir)

    # Copy files to the staging folder
    shutil.copy(aar_path, f"{staging_dir}/tensorseal.aar")
    shutil.copy("tools/tensorseal.py", f"{staging_dir}/packer.py")

    # Output directory (This is the mounted volume, we DO NOT delete it)
    dist_dir = "dist"
    os.makedirs(dist_dir, exist_ok=True)

    # Create Zip
    # We zip the 'staging_dir' and place the result into 'dist_dir'
    zip_base_name = f"{dist_dir}/tensorseal-{USER_ID}"
    shutil.make_archive(zip_base_name, 'zip', staging_dir)

    zip_file = f"{zip_base_name}.zip"
    log(f"✅ Build Success! Artifact: {zip_file}")

    # 4. Upload to Firebase (Only if BUCKET_NAME is set)
    if BUCKET_NAME:
        log(f"☁️  Uploading to {BUCKET_NAME}...")
        try:
            client = storage.Client()
            bucket = client.bucket(BUCKET_NAME)
            # We want the file name in the bucket, not the path
            blob_name = f"builds/{USER_ID}/{os.path.basename(zip_file)}"
            blob = bucket.blob(blob_name)
            blob.upload_from_filename(zip_file)
            log("🚀 Upload Complete!")
        except Exception as e:
            log(f"❌ Upload Failed: {e}")
            sys.exit(1)
    else:
        log("⚠️  BUCKET_NAME not set. Skipping upload.")

if __name__ == "__main__":
    main()