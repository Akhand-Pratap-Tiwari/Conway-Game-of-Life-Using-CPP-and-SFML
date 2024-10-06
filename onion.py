import argparse
import subprocess
import shutil
import os

def run_command(command):
    causes = [
        "Maybe a memory runtime error in C++.",
        "Maybe a null pointer error in C++.", 
        "Maybe a stack overflow error in C++.",
        "Maybe the sfml dlls required for the program to run are missing. They should be in the same folder as main.exe.",
        "Maybe enough memory is not available for the program to run."
    ]
    try:
        subprocess.run(command, check=True, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        print(f"Something went wrong here is what might be the causes:")
        for cause in causes:
            print("\t"+cause)


def make_debug_build():
    print("Making Debug Build...")
    command = [
        """cmd /c chcp 65001>nul""",
        """&& cd src """,
        """&& cl.exe /Zi /EHsc /nologo /ID:/ProgramsAndApps/SFML-2.6.1/include E:\TEMP_TESTS\c++_sfml\src\main.cpp /link /MACHINE:X86 /LIBPATH:"D:/ProgramsAndApps/SFML-2.6.1/lib" sfml-system.lib sfml-window.lib sfml-graphics.lib sfml-network.lib sfml-audio.lib"""
    ]
    run_command(command)

def copy_file(src_exe_path, release_exe_path, exe_filename):
    try:
        shutil.copy(src_exe_path, release_exe_path)
        print(f"Copied {exe_filename} to the release folder.")
    except FileNotFoundError as e:
        print(f"Error copying {exe_filename}: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")

def check_then_build_src_exe(src_exe_path, exe_filename):
    if not os.path.isfile(src_exe_path):
        print(f"{exe_filename} does not exist in src folder, running build...")
        make_debug_build()
    else:
        print(f"Latest {exe_filename} found in src folder")

def make_release_build():
    print("Making Release Build...")
    src_folder = os.path.join(os.getcwd(), 'src')
    release_folder = os.path.join(os.getcwd(), 'release')
    exe_filename = "main.exe"
    src_exe_path = os.path.join(src_folder, exe_filename)
    release_exe_path = os.path.join(release_folder, exe_filename)

    if os.path.isfile(release_exe_path):
        isOverwrite = ""
        while(isOverwrite != "y" and isOverwrite != "n"):
            print(f"{exe_filename} already exists in release folder. Overwrite? (Y/N): ", end="")
            isOverwrite = input()
            isOverwrite.lower()
            if(isOverwrite == "y"):
                check_then_build_src_exe(src_exe_path, exe_filename)
                print("Overwriting...")
                copy_file(src_exe_path, release_exe_path, exe_filename)
                break
            elif(isOverwrite == "n"):
                print("Exiting...")
                break
            else:
                print("Invalid Option.")
    else:
        print(f"{exe_filename} does not exist in release folder, checking src...")
        check_then_build_src_exe(src_exe_path, exe_filename)
        print("Copying...")
        copy_file(src_exe_path, release_exe_path, exe_filename)

def run_release_program():
    print("Running Release Build Program...")
    release_folder = os.path.join(os.getcwd(), 'release')
    exe_filename = "main.exe"
    release_exe_path = os.path.join(release_folder, exe_filename)
    command = [
        "cmd /c chcp 65001>nul && cd release && main.exe"
    ]
    if not os.path.isfile(release_exe_path):
        print("Release build not found. Building new release...")
        make_release_build()
    run_command(command)


def run_debug_program():
    print("Running Debug Build Program...")
    src_folder = os.path.join(os.getcwd(), 'src')
    exe_filename = "main.exe"
    src_exe_path = os.path.join(src_folder, exe_filename)
    command = [
        "cmd /c chcp 65001>nul && cd src && main.exe"
    ]
    if not os.path.isfile(src_exe_path):
        print("Debug build not found. Building new debug...")
        make_debug_build()
    run_command(command)


def main():
    parser = argparse.ArgumentParser(description="Build and Run Utility")

    action_choices = ['make', 'run']
    build_type_choices = ['debug', 'release']
    parser.add_argument("action", choices=action_choices,
                        help="Action: 'make' or 'run'")
    parser.add_argument("build_type", choices=build_type_choices,
                        help="Build type: 'debug' or 'release'")

    args = parser.parse_args()

    if args.action == 'make':
        if args.build_type == 'debug':
            make_debug_build()
        elif args.build_type == 'release':
            make_release_build()
    elif args.action == 'run':
        if args.build_type == 'debug':
            run_debug_program()
        elif args.build_type == 'release':
            run_release_program()


if __name__ == "__main__":
    main()