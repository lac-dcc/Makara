import os
import subprocess
import time
from inject_code import injection_pipeline


def compilation_process(sources):
    errors = []

    # Clean non-compilable files
    for program in sources:
        output_name = program.replace(".c", ".out")
        print(f"Compiling {program}...")
        try:
            subprocess.run(
                [
                    "gcc",
                    "-O2",
                    "-w",
                    "-o",
                    f"bin/{output_name}",
                    f"ModifiedJotai/{program}",
                ],
                check=True,
            )
        except subprocess.CalledProcessError as e:
            print(f"Failed to compile {program}: {e}")
            print(f"Removing {program} from ModifiedJotai...")
            errors.append(program)
            os.remove(f"ModifiedJotai/{program}")
            continue

    print("Compilation finished.")
    print(f"Errors: {errors}")

    with open("results/errors.txt", "w") as f:
        for error in errors:
            f.write(error + "\n")


def main():

    start_time = time.time()
    # Collect CPU data
    os.makedirs("results", exist_ok=True)
    os.makedirs("bin", exist_ok=True)
    subprocess.run(["cat", "/proc/cpuinfo"], stdout=open("results/cpuinfo.txt", "w"))
    subprocess.run(["gcc", "--version"], stdout=open("results/gcc_version.txt", "w"))

    sources = os.listdir("ModifiedJotai")
    compilation_process(sources)

    sources = os.listdir("bin")
    for output_name in sources:
        print(f"Running {output_name}...")
        for _ in range(100):
            try:
                subprocess.run(
                    [f"./bin/{output_name}", "0"],
                    check=True,
                )
            except subprocess.CalledProcessError as e:
                print(f"Failed to run the {output_name} : {e}")
                continue
        
    with open("results/perf_results.csv", "r") as file:
        filedata = file.read()
    
    filedata = filedata.replace("newline", "\n")

    with open("results/perf_results.csv", "w") as file:
        file.write(filedata)

    subprocess.run(["zip", "-r", "results.zip", "results"], stdout=subprocess.DEVNULL)
    end_time = time.time()
    print(f"Total execution time: {end_time - start_time:.2f} seconds")


if __name__ == "__main__":
    injection_pipeline()
    main()
