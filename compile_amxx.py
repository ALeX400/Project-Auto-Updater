import os
import json5
import re
import subprocess
import logging
import platform
from datetime import datetime

logging.basicConfig(
    format="%(asctime)s [%(levelname)s] %(message)s",
    datefmt="%H:%M:%S",
    level=logging.INFO
)

def extract_plugin_info(sma_path):
    """Extract plugin info from .sma file"""
    with open(sma_path, "r", encoding="utf-8") as f:
        content = f.read()

    plugin_info = {"PluginName": "Unknown", "Version": "Unknown", "Author": "Unknown"}

    match = re.search(r'register_plugin\(\s*"([^"]+)",\s*"([^"]+)",\s*"([^"]+)"\s*\)', content)
    if match:
        plugin_info["PluginName"], plugin_info["Version"], plugin_info["Author"] = match.groups()
        return plugin_info

    match = re.search(r'register_plugin\(\s*(\w+),\s*(\w+),\s*(\w+)\s*\)', content)
    if match:
        var_names = match.groups()
    else:
        var_names = ("PluginName", "PluginVersion", "PluginAuthor")

    var_values = {}
    for var in var_names:
        pattern = rf'(?:new|static|const|static\s+new|static\s+const)?\s*{var}\s*\[\]\s*=\s*"([^"]+)"'
        match = re.search(pattern, content)
        if match:
            var_values[var] = match.group(1)

    for var in var_names:
        pattern = rf'#define\s+{var}\s+"([^"]+)"'
        match = re.search(pattern, content)
        if match:
            var_values[var] = match.group(1)

    array_match = re.search(r'new\s+(?:const|static)?(?:\s*\w+\s+)?PluginInfo\s*\[\]\[\]\s*=\s*\{(.*?)\};', content, re.DOTALL)
    if array_match:
        values = re.findall(r'"(.*?)"', array_match.group(1))
        if len(values) >= 3:
            var_values[var_names[0]], var_values[var_names[1]], var_values[var_names[2]] = values[:3]

    enum_match = re.search(r'new\s+\w+\[\w+\]\s*=\s*\{(.*?)\};', content, re.DOTALL)
    if enum_match:
        values = re.findall(r'"(.*?)"', enum_match.group(1))
        if len(values) >= 3:
            var_values[var_names[0]], var_values[var_names[1]], var_values[var_names[2]] = values[:3]

    for i, key in enumerate(["PluginName", "Version", "Author"]):
        if var_names[i] in var_values:
            plugin_info[key] = var_values[var_names[i]]

    return plugin_info

def compile_plugin(source_folder, compiler_folder, compiled_folder, plugin_info, project_name):
    """Compile the plugin for each version in Build.json"""
    build_json_path = os.path.join(source_folder, "Build.json")
    
    if not os.path.exists(build_json_path):
        logging.warning(f"Build.json missing in {source_folder}. Skipping.")
        return [], []
    
    with open(build_json_path, "r", encoding="utf-8") as f:
        build_config = json5.load(f)
    
    versions = build_config.get("BuildVersions", [])
    use_default_include = build_config.get("DefaultIncludeFolder", True)
    use_custom_include = build_config.get("CustomIncludeFolder", True)

    sma_file = plugin_info.get("Source", "")
    if not sma_file:
        logging.warning(f"No source file defined in {source_folder}. Skipping.")
        return [], []
    
    sma_path = os.path.join(source_folder, sma_file)
    compiled_project_path = os.path.join(compiled_folder, project_name)
    os.makedirs(compiled_project_path, exist_ok=True)

    successful_versions = []
    failed_versions = []

    for version in versions:
        compiler_path = os.path.join(compiler_folder, version)
        include_paths = []
        
        if use_default_include:
            include_paths.append(os.path.join(compiler_path, "include"))
        if use_custom_include:
            include_paths.append(os.path.abspath("Custom_Includes"))
        
        version_path = os.path.join(compiled_project_path, version)
        os.makedirs(version_path, exist_ok=True)
        
        amxx_output = os.path.join(version_path, sma_file.replace(".sma", ".amxx"))
        compiler_exe = os.path.join(compiler_path, "amxxpc")
        
        cmd = [compiler_exe, sma_path, "-o" + amxx_output] + [f"-i{path}" for path in include_paths]
        
        result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        if result.returncode == 0:
            successful_versions.append(version)
        else:
            failed_versions.append(version)

    return successful_versions, failed_versions

def update_json(updates_file, project_name, compiled_folder, repo_url, plugin_info):
    """Update the Updates.json file with the new plugin version"""
    if os.path.exists(updates_file):
        with open(updates_file, "r", encoding="utf-8") as f:
            updates = json5.load(f)
    else:
        updates = {}

    build_num = int(updates.get(project_name, {}).get("Build", 0)) + 1
    updates[project_name] = {
        "PluginName": plugin_info["PluginName"],
        "Version": plugin_info["Version"],
        "Author": plugin_info["Author"],
        "Build": str(build_num),
        "Source": plugin_info.get("Source", "Unknown"),
        "Date": datetime.today().strftime('%d.%m.%Y'),
        "Download": {}
    }

    compiled_versions = os.listdir(os.path.join(compiled_folder, project_name))

    for version in compiled_versions:
        amxx_file = f"{project_name}/{version}/{plugin_info['Source'].replace('.sma', '.amxx')}"
        updates[project_name]["Download"][version] = f"{repo_url}/Compiled/{amxx_file}"
    
    json_str = json5.dumps(updates, indent=4, quote_keys=True)
    json_str = re.sub(r",(\s*})", r"\1", json_str)  # Elimină virgulele în exces

    with open(updates_file, "w", encoding="utf-8") as f:
        f.write(json_str)

if __name__ == "__main__":
    SOURCE_DIR = "Source"
    COMPILER_DIR = "Compiler"
    COMPILED_DIR = "Compiled"
    UPDATES_FILE = "Updates.json"
    REPO_URL = "https://github.com/ALeX400/Project-Auto-Updater"
    
    GITHUB_RAW_URL = REPO_URL.replace("github.com", "raw.githubusercontent.com") + "/refs/heads/main"

    logging.info("Starting compilation process...")
    
    for project in os.listdir(SOURCE_DIR):
        source_path = os.path.join(SOURCE_DIR, project)
        if os.path.isdir(source_path):
            source_files = [f for f in os.listdir(source_path) if f.endswith(".sma")]
            if not source_files:
                logging.warning(f"No .sma file found in {source_path}. Skipping.")
                continue
            
            sma_file = source_files[0]
            plugin_info = extract_plugin_info(os.path.join(source_path, sma_file))
            plugin_info["Source"] = sma_file

            successful_versions, failed_versions = compile_plugin(source_path, COMPILER_DIR, COMPILED_DIR, plugin_info, project)

            if successful_versions:
                logging.info(f"Compiled '{project}' successfully for versions: {', '.join(successful_versions)}.")
            if failed_versions:
                logging.error(f"Failed to compile '{project}' for versions: {', '.join(failed_versions)}.")

            update_json(UPDATES_FILE, project, COMPILED_DIR, GITHUB_RAW_URL, plugin_info)

    logging.info("Compilation and update process completed.")
