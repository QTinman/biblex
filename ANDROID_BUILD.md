# Building Bible Lexicon for Android

This document describes how to build and deploy the Bible Lexicon application for Android devices.

## Prerequisites

1. **Qt for Android**
   - **Qt 6.x** (Qt 6.2 or later recommended) - Fully supported
   - **Qt 5.x** (Qt 5.12 or later) - Also supported
   - Download from: https://www.qt.io/download
   - Install Qt for Android component

   **Note**: The code is compatible with both Qt 5 and Qt 6. Qt 6 users don't need the deprecated `androidextras` module.

2. **Android SDK**
   - **API Level 28 (Android 9.0) minimum** - Required for Qt 6.7+
   - API Level 33 (Android 13) target
   - Install via Android Studio or Qt Creator

   **Note**: Qt 6.7+ requires Android 9.0 (API 28) minimum. If using Qt 6.2-6.6, you can lower minSdkVersion to 23.

3. **Android NDK**
   - NDK r21 or later
   - Install via Android Studio or Qt Creator

4. **Java Development Kit (JDK)**
   - JDK 8 or later

## Setup Instructions

### 1. Configure Qt Creator for Android

1. Open Qt Creator
2. Go to **Tools → Options → Devices → Android**
3. Set the following paths:
   - JDK location
   - Android SDK location
   - Android NDK location
4. Click "Apply" and ensure all required components are detected

### 2. Open the Project

1. Open `biblex.pro` in Qt Creator
2. Select an Android kit when prompted (e.g., "Android Qt 5.15.2 Clang arm64-v8a")
3. Configure the project for Android build

### 3. Build the Application

1. Select the Android kit from the kit selector
2. Choose **Build → Build Project "biblex"**
3. Wait for the build to complete

### 4. Deploy to Device/Emulator

1. Connect an Android device via USB with USB debugging enabled
   - OR start an Android emulator
2. Click the **Run** button (green play icon) in Qt Creator
3. The app will be installed and launched automatically

## Android-Specific Features

### File Storage

On Android, the application uses the following storage locations:

- **Settings**: `QStandardPaths::AppDataLocation/settings.txt`
- **Lexicon Files**: `Documents/BibleLexicon/greek/` and `Documents/BibleLexicon/hebrew/`
- **Saved Outputs**: `Documents/BibleLexicon/`
- **Screenshots**: `Pictures/BibleLexicon/`

### Setting Up Lexicon Files

After installing the app:

1. Download the Greek and Hebrew lexicon files (see main README.md)
2. Connect your Android device to a computer
3. Copy lexicon folders to:
   ```
   /storage/emulated/0/Documents/BibleLexicon/greek/
   /storage/emulated/0/Documents/BibleLexicon/hebrew/
   ```
4. Alternatively, use the app's menu to specify custom locations

### Permissions

The app requires the following permissions:
- **READ_EXTERNAL_STORAGE**: To read lexicon files
- **WRITE_EXTERNAL_STORAGE**: To save outputs and screenshots
- **INTERNET**: To open BibleHub links

These permissions are requested at runtime when the app first launches.

### Android-Specific Functionality

#### Printing Replacement
- Desktop "Print" feature → Android "Share" feature
- Exports to HTML and opens Android share dialog
- Can share via email, messaging apps, cloud storage, etc.

#### Screenshot Feature
- Saves screenshots to Pictures/BibleLexicon/
- Option to share immediately after capture

#### File Management
- All save operations include a share option
- Files are saved to appropriate Android directories
- Uses FileProvider for secure file sharing

## UI Optimizations for Mobile

The Android build includes:
- Larger touch-friendly input fields (50dp minimum height)
- Increased font sizes for better readability
- Fullscreen mode by default
- Responsive layouts for various screen sizes
- Kinetic scrolling support

## Qt 5 vs Qt 6 Compatibility

The project automatically detects your Qt version and uses the appropriate APIs:

### Qt 6 Changes
- **QtAndroidExtras**: Removed in Qt 6, functionality integrated into QtCore
- **QAndroidJniObject**: Replaced with `QJniObject`
- **QtAndroid namespace**: Replaced with direct JNI calls using `QJniObject`
- No need to add `androidextras` to the .pro file

### Qt 5 Support
- Uses `QtAndroid` and `QAndroidJniObject`
- Requires `androidextras` module (automatically added by .pro file)

The code uses preprocessor directives (`#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)`) to provide compatibility with both versions.

## Troubleshooting

### Build Errors

**"Unknown module(s) in QT: androidextras" (Qt 6)**
- This is normal and expected. The .pro file only adds androidextras for Qt 5
- If you're still seeing this error, make sure you have the latest code
- Qt 6 doesn't need this module

**"Android SDK not found"**
- Ensure Android SDK is properly installed
- Check paths in Qt Creator settings

**"NDK not found"**
- Install Android NDK via SDK Manager
- Update NDK path in Qt Creator

**Missing dependencies**
- Run Qt Maintenance Tool
- Install "Qt for Android" component

### Runtime Issues

**"Permission denied" errors**
- Grant storage permissions in Android Settings → Apps → Bible Lexicon
- Or uninstall and reinstall to trigger permission requests

**Lexicon files not found**
- Verify files are in `/Documents/BibleLexicon/greek/` and `/hebrew/`
- Use Menu → Select Lexicon to specify custom paths
- Check file permissions (files should be readable)

**App crashes on startup**
- Check logcat output: `adb logcat | grep biblex`
- Ensure minimum Android version is 6.0 (API 23)

## Creating Release APK

1. In Qt Creator, select **Build → Build Android APK**
2. Choose "Release" configuration
3. Sign the APK with your keystore:
   - If you don't have a keystore, create one in Android settings
4. The APK will be created in the build directory
5. Transfer to device or publish to Google Play Store

## Performance Tips

- Lexicon files can be large; ensure sufficient storage space
- For better performance, place lexicon files in internal storage
- Consider compressing large lexicon datasets
- Use night mode to reduce battery consumption

## Support

For issues specific to the Android build:
1. Check this documentation
2. Review Qt for Android documentation
3. Check Android logcat for error messages
4. Report issues on the project repository

## Building from Command Line

```bash
# Set environment variables
export ANDROID_SDK_ROOT=/path/to/android/sdk
export ANDROID_NDK_ROOT=/path/to/android/ndk
export JAVA_HOME=/path/to/jdk

# Configure
qmake biblex.pro -spec android-clang CONFIG+=release

# Build
make

# Create APK
androiddeployqt --input android-biblex-deployment-settings.json --output android-build --android-platform android-33 --gradle
```

## Minimum Requirements

- **Android Version**: 9.0 (Pie, API 28) or higher (required by Qt 6.7+)
- **Storage**: 100MB+ (depends on lexicon file sizes)
- **RAM**: 1GB minimum, 2GB recommended
- **Screen**: 360x600dp minimum resolution

**Note**: If using Qt 6.2-6.6, minimum Android version can be lowered to 6.0 (API 23).

## Recommended Device Specifications

- **Android Version**: 9.0 (Pie) or higher
- **Storage**: 500MB+ free space
- **RAM**: 2GB+
- **Screen**: 5" or larger for comfortable viewing
