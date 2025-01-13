import os

PROC_FILE = "/proc/kernel_stats"

def read_kernel_stats():
    if not os.path.exists(PROC_FILE):
        print(f"{PROC_FILE} does not exist. Is the module loaded?")
        return

    with open(PROC_FILE, "r") as f:
        data = f.read()
        return data

def display_stats(data):
    print("=== Kernel Statistics ===")
    print(data)

if __name__ == "__main__":
    data = read_kernel_stats()
    if data:
        display_stats(data)
