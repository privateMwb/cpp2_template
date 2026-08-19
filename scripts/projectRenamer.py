from pathlib import Path


# ============================================================
# Project configuration
# ============================================================

lowcap = "name"
highcap = "Name"
RepoName = "REPO"


# ============================================================
# Configuration
# ============================================================

ROOT = Path(".")

# Absolute path of the currently running script.
# This prevents the script from modifying itself.
SCRIPT_PATH = Path(__file__).resolve()


# File extensions to process.
#
# ".cmake.in" is handled using endswith() because
# Path.suffix only returns ".in" for "PoolProConfig.cmake.in".
EXTENSIONS = {
    ".cpp",
    ".h",
    ".tpp",
    ".yml",
    ".yaml",
    ".json",
    ".txt",
    ".md",
    ".py",
    ".cmake",
    ".cmake.in",
}


# Files without a conventional extension to process.
FILENAMES = {
    "Doxyfile",
}


# ============================================================
# Text replacements
# ============================================================

REPLACEMENTS = {
    "name": lowcap,
    "Name": highcap,
    "REPO": RepoName,
}


# ============================================================
# Replace text inside files
# ============================================================

def replace_in_file(path: Path) -> None:
    try:
        text = path.read_text(encoding="utf-8")
    except (UnicodeDecodeError, OSError):
        return

    original = text

    for old, new in REPLACEMENTS.items():
        text = text.replace(old, new)

    if text != original:
        path.write_text(text, encoding="utf-8")
        print(f"Updated: {path}")


def replace_text() -> None:
    for path in ROOT.rglob("*"):

        if not path.is_file():
            continue

        # Never modify the currently running script itself.
        if path.resolve() == SCRIPT_PATH:
            continue

        # Process files explicitly listed by name.
        if path.name in FILENAMES:
            replace_in_file(path)
            continue

        # Process files with the configured extensions.
        path_string = str(path).lower()

        if not any(
            path_string.endswith(extension)
            for extension in EXTENSIONS
        ):
            continue

        replace_in_file(path)


# ============================================================
# Rename directories
# ============================================================

def rename_directories() -> None:
    for path in sorted(
        (p for p in ROOT.rglob("*") if p.is_dir()),
        key=lambda p: len(p.parts),
        reverse=True,
    ):
        if path.name == "name":
            new_path = path.with_name(lowcap)

            if new_path.exists():
                print(f"Skipped: {path} -> {new_path}")
                continue

            path.rename(new_path)
            print(f"Renamed: {path} -> {new_path}")


# ============================================================
# Rename Config.cmake.in
# ============================================================

def rename_config() -> None:
    for path in ROOT.rglob("NameConfig.cmake.in"):
        new_path = path.with_name(
            f"{highcap}Config.cmake.in"
        )

        if new_path.exists():
            print(f"Skipped: {path} -> {new_path}")
            continue

        path.rename(new_path)
        print(f"Renamed: {path} -> {new_path}")


# ============================================================
# Main
# ============================================================

def main() -> None:
    replace_text()
    rename_directories()
    rename_config()

    print("Done.")


if __name__ == "__main__":
    main()
