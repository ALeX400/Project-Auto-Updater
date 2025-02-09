# AMXX Plugin Compiler

## 📌 Description
This project enables **automatic compilation of AMXX plugins** using different versions of the AMX Mod X compiler. 
It includes a **Python script** that extracts plugin information, compiles it, and updates an `Updates.json` file with details about versions and download links.

## 🚀 Features
- **Automatic compilation** of `.sma` plugins.
- **Support for multiple compiler versions**.
- **Automatic generation of download links**.
- **GitHub Actions** for automated compilation on every commit.

---

## 🛠️ Installation & Usage

### 1️⃣ **Local Setup**
You need **Python 3** and an AMX Mod X compiler.

```bash
# Clone the project
git clone https://github.com/ALeX400/Project-Auto-Updater.git
cd Project-Auto-Updater

# Install dependencies
pip install json5

# Run the compilation script
python compile_amxx.py
```

### 2️⃣ **Automate with GitHub Actions**
This project includes a **GitHub Actions workflow** that compiles plugins on every commit.
To manually trigger the workflow:

1. Go to the **Actions** tab in your GitHub repository.
2. Select the **Compile AMXX Plugins** workflow.
3. Click **Run Workflow**.

---

## 📥 Download AMX Mod X Compilers
To get the latest versions of the AMX Mod X compiler, download them here:

🔗 **[AMX Mod X Compiler](https://www.amxmodx.org)**

If you already have the compilers in the `Compiler/` folder, you do not need to download them again.

---