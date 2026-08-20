#!/usr/bin/env python3

import argparse
import hashlib
import json
import pathlib
import re
import sys

parser = argparse.ArgumentParser()

parser.add_argument("--package", required=True)
parser.add_argument("--repo", required=True)
parser.add_argument("--version", required=True)
parser.add_argument("--commit", required=True)
parser.add_argument("--archive", required=True)
parser.add_argument("--root-dir", required=True)
parser.add_argument("--conan-dir", required=True)
parser.add_argument("--vcpkg-dir", required=True)

args = parser.parse_args()

package = args.package
repo = args.repo
version = args.version
commit = args.commit

archive = pathlib.Path(args.archive)

if not archive.exists():
    sys.exit(f"Archive not found: {archive}")

data = archive.read_bytes()

# Only vcpkg needs a hash (git itself is the integrity check for the
# Conan git-clone flow; conandata.yml pins a commit, not a tarball).
sha512 = hashlib.sha512(data).hexdigest()

print(f"Version : {version}")
print(f"Commit  : {commit}")
print(f"SHA512  : {sha512}")

# ---------------------------------------------------------------------
# Root CMakeLists.txt
# ---------------------------------------------------------------------

root_cmakelists = pathlib.Path(args.root_dir) / "CMakeLists.txt"

text = root_cmakelists.read_text(encoding="utf-8")


def _bump_project_version(match: re.Match) -> str:
    # Substitute only within the matched project(...) call, so this
    # can't touch an unrelated VERSION elsewhere in the file (e.g.
    # cmake_minimum_required(VERSION ...)).
    return re.sub(
        r"VERSION\s+[0-9A-Za-z.\-_]+",
        f"VERSION {version}",
        match.group(0),
        count=1,
    )


new_text, count = re.subn(
    r"project\s*\([^)]*\)",
    _bump_project_version,
    text,
    count=1,
    flags=re.DOTALL,
)

if count == 0:
    sys.exit(f"No project() call found in {root_cmakelists}")

root_cmakelists.write_text(new_text, encoding="utf-8")

print("✓ Updated CMakeLists.txt")

# ---------------------------------------------------------------------
# Conan
# ---------------------------------------------------------------------

recipe_dir = pathlib.Path(args.conan_dir)

# conanfile.py

conanfile = recipe_dir / "conanfile.py"

text = conanfile.read_text(encoding="utf-8")

text = re.sub(
    r'version\s*=\s*"[^"]+"',
    f'version = "{version}"',
    text,
)

conanfile.write_text(text, encoding="utf-8")

print("✓ Updated conanfile.py")

# conandata.yml

conandata = recipe_dir / "conandata.yml"

# Matches the git-clone flow in conanfile.py's source() (Git(self).run(...)
# against sources['url'] / sources['commit']) -- not a tarball download, so
# no hash is stored here. The commit is the integrity check.
conandata.write_text(
f'''sources:
  "{version}":
    url: "https://github.com/{repo}.git"
    commit: "{commit}"
''',
encoding="utf-8"
)

print("✓ Updated conandata.yml")

# ---------------------------------------------------------------------
# vcpkg
# ---------------------------------------------------------------------

port_dir = pathlib.Path(args.vcpkg_dir)

# portfile.cmake

portfile = port_dir / "portfile.cmake"

text = portfile.read_text(encoding="utf-8")

# Only touches the first REF -- the top-level vcpkg_from_github() block
# for the library itself. SUBMODULE_SPECS entries are plain "name|ref|sha"
# strings (no literal "REF" keyword), and the per-submodule
# vcpkg_from_github() call inside the foreach loop uses REF ${SUBMODULE_REF}
# (a variable, not a hex literal), so neither is matched by this pattern.
text, count = re.subn(
    r"REF\s+[0-9a-fA-F]{7,40}",
    f"REF {commit}",
    text,
    count=1,
)

if count == 0:
    sys.exit(
        f"No commit-SHA REF found in {portfile} -- expected the "
        "top-level vcpkg_from_github() block to already have a real "
        "commit SHA (not the <commit-sha> placeholder) before this "
        "script can find it to replace."
    )

text = re.sub(
    r"SHA512\s+[0-9a-fA-F]+",
    f"SHA512 {sha512}",
    text,
    count=1,
)

portfile.write_text(text, encoding="utf-8")

print("✓ Updated portfile.cmake")

# vcpkg.json

vcpkg_json = port_dir / "vcpkg.json"

data = json.loads(vcpkg_json.read_text(encoding="utf-8"))

data["version"] = version

vcpkg_json.write_text(
    json.dumps(data, indent=2) + "\n",
    encoding="utf-8",
)

print("✓ Updated vcpkg.json")