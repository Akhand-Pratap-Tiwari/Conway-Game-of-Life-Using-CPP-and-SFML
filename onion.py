import argparse
import subprocess
import shutil
import os
import glob

output_exe_name = "main.exe"
workspace_folder = os.getcwd()
release_folder = os.path.join(workspace_folder, "release")
src_folder = os.path.join(workspace_folder, "src")
custom_headers_folder = os.path.join(src_folder, "custom_headers")
exec_folder = os.path.join(src_folder, "exec")
obj_folder = os.path.join(src_folder, "obj")
main_cpp_file_path = os.path.join(src_folder, "main.cpp")
custom_header_cpp_file_paths = glob.glob(os.path.join(custom_headers_folder, "*.cpp"))

def run_command(command):
    causes = [
        "Memory runtime error in C++.",
        "Null pointer error in C++.", 
        "Stack overflow error in C++.",
        "The required sfml dlls are missing. They should be in the same folder as main.exe.",
        "Not enough memory available for the program to run.",
        "Include file or some other path related error."
    ]
    try:
        result = subprocess.run(command, capture_output=True, text=True, check=True, shell=True)
        print(' '.join(command), "executed with code", result.returncode)
        print(result.stdout)
    except subprocess.CalledProcessError as e:
        if(e != None): print(f"Error Happened: {e}")
        if(e.stdout != None and e.stdout != ""): print(f"Output: {e.stdout}")
        if(e.stderr != None and e.stderr != ""): print(f"Error: {e.stderr}")
        print(f"Something went wrong, probable causes:")
        for cause in causes:
            print("\t"+cause)


def make_debug_build():
    custom_header_cpp_file_paths_str = " ".join(custom_header_cpp_file_paths)
    custom_header_cpp_file_paths_str = custom_header_cpp_file_paths_str.replace("\\", "/")
 
    main_cpp_file_path_formatted = main_cpp_file_path.replace("\\", "/")
    obj_folder_formatted = obj_folder.replace("\\", "/") + '/'
    exec_folder_formatted = exec_folder.replace("\\", "/") + '/'

    print("Making Debug Build...")
    command = [
        """cmd /c chcp 65001>nul""",
        """&& cd src """,
        f"""&& cl.exe /Zi /EHsc /nologo /ID:/ProgramsAndApps/SFML-2.6.1/include /Fo{obj_folder_formatted} {main_cpp_file_path_formatted} {custom_header_cpp_file_paths_str} /Fe{exec_folder_formatted} /Fd{exec_folder_formatted} /link /MACHINE:X86 /LIBPATH:"D:/ProgramsAndApps/SFML-2.6.1/lib" sfml-system.lib sfml-window.lib sfml-graphics.lib sfml-network.lib sfml-audio.lib"""
        ]
    run_command(command)

def copy_file(src_path, release_path, filename):
    try:
        shutil.copy(src_path, release_path)
        print(f"Copied {filename} to the release folder.")
    except FileNotFoundError as e:
        print(f"Error copying {filename}: {e}")
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
    src_exe_path = os.path.join(exec_folder, output_exe_name)
    release_exe_path = os.path.join(release_folder, output_exe_name)

    if os.path.isfile(release_exe_path):
        isOverwrite = ""
        while(isOverwrite != "y" and isOverwrite != "n"):
            print(f"{output_exe_name} already exists in release folder. Overwrite? (Y/N): ", end="")
            isOverwrite = input()
            isOverwrite.lower()
            if(isOverwrite == "y"):
                check_then_build_src_exe(src_exe_path, output_exe_name)
                print("Overwriting...")
                copy_file(src_exe_path, release_exe_path, output_exe_name)
                break
            elif(isOverwrite == "n"):
                print("Exiting...")
                break
            else:
                print("Invalid Option.")
    else:
        print(f"{output_exe_name} does not exist in release folder, checking src...")
        check_then_build_src_exe(src_exe_path, output_exe_name)
        print("Copying...")
        copy_file(src_exe_path, release_exe_path, output_exe_name)

def run_release_program():
    print("Running Release Build Program...")
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
    exe_filename = "main.exe"
    src_exe_path = os.path.join(exec_folder, exe_filename)
    command = [
        f"cmd /c chcp 65001>nul && {src_exe_path}"
    ]
    if not os.path.isfile(src_exe_path):
        print("Debug build not found. Building new debug...")
        make_debug_build()
    run_command(command)

def pack_to_zip(folder_path, output_zip):
    if not os.path.exists(folder_path):
        print(f"The folder {folder_path} does not exist.")
        return

    shutil.make_archive(output_zip, 'zip', folder_path)
    print(f"Folder '{folder_path}' has been compressed to '{output_zip}.zip'")

def pack_release():
    output_zip = 'release_pack'
    pack_to_zip(release_folder, output_zip)

def pack_debug():
    output_zip = 'src_pack'
    pack_to_zip(src_folder, output_zip)


def main():
    parser = argparse.ArgumentParser(description="Build and Run Utility")

    action_choices = ['make', 'run', 'pack']
    build_type_choices = ['debug', 'release']
    parser.add_argument("action", choices=action_choices,
                        help="Action: 'make', 'run' or 'pack'")
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
    elif args.action == 'pack':
        if args.build_type == 'debug':
            pack_debug()
        elif args.build_type == 'release':
            pack_release()

if __name__ == "__main__":
    main()
