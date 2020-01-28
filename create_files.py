import sys
import random

def main():
    if len(sys.argv) != 3:
        print("usage: create_files <file_name> <file_size>")
    arr = [0] * int(sys.argv[2])
    for i in range(0, int(sys.argv[2])):
        arr[i] = random.randint(0, int(sys.argv[2]))
    with open(sys.argv[1] + ".txt", "w") as fp:
        fp.write("\n".join(str(i) for i in arr))
    arr.sort()
    with open(sys.argv[1] + "_sorted.txt", "w") as fp:
        fp.write("\n".join(str(i) for i in arr))

if __name__ == "__main__":
    main()